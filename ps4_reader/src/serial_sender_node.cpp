#include <rclcpp/rclcpp.hpp>
#include <serial/serial.h>

#include "ps4_reader/msg/car_command.hpp"

#include <memory>
#include <sstream>

class SerialSender : public rclcpp::Node
{
public:
    SerialSender()
        : Node("serial_sender")
    {
        try
        {
            serial_port_.setPort("/dev/ttyUSB0");
            serial_port_.setBaudrate(115200);

            serial::Timeout timeout =
                serial::Timeout::simpleTimeout(100);

            serial_port_.setTimeout(timeout);

            serial_port_.open();

            if (serial_port_.isOpen())
            {
                RCLCPP_INFO(
                    this->get_logger(),
                    "Porta serial aberta com sucesso");
            }
        }
        catch (serial::IOException &e)
        {
            RCLCPP_ERROR(
                this->get_logger(),
                "Nao foi possivel abrir a porta serial");
        }

        subscription_ =
            this->create_subscription<
                ps4_reader::msg::CarCommand>(
                    "/car_command",
                    10,
                    std::bind(
                        &SerialSender::command_callback,
                        this,
                        std::placeholders::_1));

        RCLCPP_INFO(
            this->get_logger(),
            "Serial Sender iniciado");
    }

    ~SerialSender()
    {
        if (serial_port_.isOpen())
        {
            serial_port_.close();
        }
    }

private:

    void command_callback(
        const ps4_reader::msg::CarCommand::SharedPtr msg)
    {
        if (!serial_port_.isOpen())
        {
            return;
        }

        std::stringstream ss;

        ss
            << msg->steering << ","
            << msg->throttle << ","
            << msg->brake << ","
            << msg->reverse << ","
            << msg->emergency_stop << ","
            << msg->headlights << ","
            << msg->horn << ","
            << msg->left_blinker << ","
            << msg->right_blinker
            << "\n";

        serial_port_.write(ss.str());

        RCLCPP_INFO(
            this->get_logger(),
            "Enviado: %s",
            ss.str().c_str());
    }

    serial::Serial serial_port_;

    rclcpp::Subscription<
        ps4_reader::msg::CarCommand>::SharedPtr subscription_;
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);

    rclcpp::spin(
        std::make_shared<SerialSender>());

    rclcpp::shutdown();

    return 0;
}