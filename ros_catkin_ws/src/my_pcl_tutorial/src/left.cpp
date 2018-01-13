#include <ros/ros.h>
#include <sensor_msgs/PointCloud2.h>
#include <pcl_ros/point_cloud.h>
#include <pcl_ros/transforms.h>
#include <pcl/point_types.h>
#include <tf/tf.h>
#include <iostream>

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
  ros::init (argc, argv, "my_pcl_tutorial_left");

  target_frame = "base_link";

  // transform.setOrigin( tf::Vector3(-0.23, 0.091, 0.528) );
  // q.setRPY(0.806, 0.855, -2.723);

  transform.setOrigin( tf::Vector3(-0.17, 0.18, 0.528) );
  q.setRPY(0.803, 0.857, 3.112);  
  transform.setRotation(q);

  ros::NodeHandle nh;

  // Create a ROS subscriber for the input point cloud
  ros::Subscriber sub = nh.subscribe<sensor_msgs::PointCloud2>("/left_velodyne/velodyne_points", 10, cloud_cb);
  // Create a ROS publisher for the output model coefficients
  pub = nh.advertise<sensor_msgs::PointCloud2>("/left_velodyne/transformed", 10);

  // Spin
  ros::spin ();
}
