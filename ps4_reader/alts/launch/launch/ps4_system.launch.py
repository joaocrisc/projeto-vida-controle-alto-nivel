from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():

    joy_node = Node(
        package='joy',
        executable='joy_node',
        name='joy_node'
    )

    ps4_reader_node = Node(
        package='ps4_reader',
        executable='ps4_reader_node',
        name='ps4_reader_node'
    )

    serial_sender_node = Node(
        package='ps4_reader',
        executable='serial_sender_node',
        name='serial_sender_node'
    )

    return LaunchDescription([
        joy_node,
        ps4_reader_node,
        serial_sender_node
    ])