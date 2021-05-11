#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include <sensor_msgs/Image.h>
#include <iostream>

using namespace std;

// Define a global client that can request services
ros::ServiceClient client;

// This function calls the command_robot service to drive the robot in the specified direction
void drive_robot(float lin_x, float ang_z)
{
    // TODO: Request a service and pass the velocities to it to drive the robot
    ROS_INFO_STREAM("Driving the robot towards the white ball");
    ball_chaser::DriveToTarget srv;
    srv.request.linear_x = lin_x;
    srv.request.angular_z = ang_z;
    if(!client.call(srv)){
      ROS_ERROR("Failed to execute drive command");
}
}

// This callback function continuously executes and reads the image data
void process_image_callback(const sensor_msgs::Image img)
{

    cout << "Image width:::" << img.width << endl;
    cout << "Image height:::" << img.height << endl;
    cout << "Image step:::" << img.step << endl;
    int white_pixel = 255;

    // TODO: Loop through each pixel in the image and check if there's a bright white one
    int left = img.step/3;
    int right = 2*(img.step/3);
    bool is_white = false;
    int img_dim = img.height*img.step;
    int pos;
    for(int i=0; i<img_dim; i=i+3){
      if(img.data[i] == white_pixel && img.data[i+1] == white_pixel && img.data[i+2] == white_pixel){
        is_white = true;
        cout << "Value of i:::" << i << endl;
        pos = i%img.step;
        cout << "Value of pos:::" << pos << endl;
}
}
    // Then, identify if this pixel falls in the left, mid, or right side of the image
    // Depending on the white ball position, call the drive_bot function and pass velocities to it
    if (pos <= left){
      cout << "Moving Left" << endl;
      drive_robot(0.0, 0.5);
}
    else if (pos >= right){
      cout << "Moving Right" << endl;
      drive_robot(0.0, -0.5);
}
    else {
      cout << "Moving Center" << endl;
      drive_robot(0.5, 0.0);
}
    // Request a stop when there's no white ball seen by the camera
    if (is_white == false){
      drive_robot(0.0, 0.0);
}
}

int main(int argc, char** argv)
{
    // Initialize the process_image node and create a handle to it
    ros::init(argc, argv, "process_image");
    ros::NodeHandle n;

    // Define a client service capable of requesting services from command_robot
    client = n.serviceClient<ball_chaser::DriveToTarget>("/ball_chaser/command_robot");

    // Subscribe to /camera/rgb/image_raw topic to read the image data inside the process_image_callback function
    ros::Subscriber sub1 = n.subscribe("/camera/rgb/image_raw", 10, process_image_callback);

    // Handle ROS communication events
    ros::spin();

    return 0;
}
