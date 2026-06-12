#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/joy.hpp"

class PS4Reader : public rclcpp::Node
{
public:
    PS4Reader()
    : Node("ps4_reader")
    {
        subscription_ =
            this->create_subscription<
                sensor_msgs::msg::Joy>(
                    "/joy",
                    10,
                    std::bind(
                        &PS4Reader::joy_callback,
                        this,
                        std::placeholders::_1));

        RCLCPP_INFO(
            this->get_logger(),
            "PS4 Reader iniciado");
    }

private:

    void joy_callback(
        const sensor_msgs::msg::Joy::SharedPtr msg)
    {
        // Analógico esquerdo horizontal
        float steering = msg->axes[0];

        // Gatilho RT
        float throttle = msg->axes[5];

        RCLCPP_INFO(
            this->get_logger(),
            "Steering: %.2f | Throttle: %.2f",
            steering,
            throttle);
    }

    rclcpp::Subscription<
        sensor_msgs::msg::Joy>::SharedPtr subscription_;
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);

    rclcpp::spin(
        std::make_shared<PS4Reader>());

    rclcpp::shutdown();

    return 0;
}
