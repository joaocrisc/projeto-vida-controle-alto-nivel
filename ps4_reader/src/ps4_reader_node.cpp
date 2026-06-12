#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/joy.hpp>

#include "ps4_reader/msg/car_command.hpp"

#include <memory>

class PS4Reader : public rclcpp::Node
{
public:
    PS4Reader()
        : Node("ps4_reader"),
          steering_(0.0f),
          throttle_(0.0f),
          emergency_stop_(false)
    {
        subscription_ =
            this->create_subscription<sensor_msgs::msg::Joy>(
                "/joy",
                10,
                std::bind(
                    &PS4Reader::joy_callback,
                    this,
                    std::placeholders::_1));

        publisher_ =
            this->create_publisher<
                ps4_reader::msg::CarCommand>(
                    "/car_command",
                    10);

        RCLCPP_INFO(
            this->get_logger(),
            "PS4 Reader iniciado");
    }

private:

    void joy_callback(
        const sensor_msgs::msg::Joy::SharedPtr msg)
    {
        if (msg->axes.size() < 6 || msg->buttons.empty())
        {
            return;
        }

        // Analógico esquerdo horizontal
        steering_ = msg->axes[0];

        // Gatilho RT
        throttle_ = msg->axes[5];

        // Botão X
        emergency_stop_ = msg->buttons[0];

        ps4_reader::msg::CarCommand command;

        command.steering = steering_;
        command.throttle = throttle_;
        command.brake = 0.0f;

        command.reverse = false;
        command.emergency_stop = emergency_stop_;

        command.headlights = false;
        command.horn = false;
        command.left_blinker = false;
        command.right_blinker = false;

        publisher_->publish(command);

        RCLCPP_INFO(
            this->get_logger(),
            "Steering: %.2f | Throttle: %.2f",
            steering_,
            throttle_);
    }

    float steering_;
    float throttle_;
    bool emergency_stop_;

    rclcpp::Subscription<
        sensor_msgs::msg::Joy>::SharedPtr subscription_;

    rclcpp::Publisher<
        ps4_reader::msg::CarCommand>::SharedPtr publisher_;
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);

    rclcpp::spin(
        std::make_shared<PS4Reader>());

    rclcpp::shutdown();

    return 0;
}