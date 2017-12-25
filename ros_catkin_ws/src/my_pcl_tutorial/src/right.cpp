#include <ros/ros.h>
// PCL specific includes
#include <sensor_msgs/PointCloud2.h>
#include <pcl_ros/point_cloud.h>
#include <pcl_ros/transforms.h>
#include <pcl/point_types.h>
#include <tf/tf.h>
#include <iostream>

//TODO: 1. Use easier transform 2. can we not convert to pointcloud? 3. change reference frame

ros::Publisher pub;
std::string target_frame;
tf::Transform transform;
tf::Quaternion q;

void cloud_cb (const sensor_msgs::PointCloud2ConstPtr& input)
{
  sensor_msgs::PointCloud2 output;
  pcl_ros::transformPointCloud(target_frame, transform, *input, output);
  pub.publish (output);
}

int main (int argc, char** argv)
{
  // Initialize ROS
  ros::init (argc, argv, "my_pcl_tutorial_right");

  target_frame = "base_link";

  transform.setOrigin( tf::Vector3(-0.279, -0.234, -0.454) );
  q.setRPY(-0.97, 0.613, -0.668);
  transform.setRotation(q);

  ros::NodeHandle nh;

  // Create a ROS subscriber for the input point cloud
  ros::Subscriber sub = nh.subscribe<sensor_msgs::PointCloud2>("/right_velodyne/velodyne_points", 10, cloud_cb);
  // Create a ROS publisher for the output model coefficients
  pub = nh.advertise<sensor_msgs::PointCloud2>("/velodyne_points", 10);

  // Spin
  ros::spin ();
}
