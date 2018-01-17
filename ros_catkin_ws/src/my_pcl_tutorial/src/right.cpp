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
  ros::init (argc, argv, "my_pcl_tutorial_right");

  target_frame = "base_link";

  transform.setOrigin( tf::Vector3(-0.22, -0.09, 0.528) );
  q.setRPY(0.768, -0.873, -0.035);
  transform.setRotation(q);

  ros::NodeHandle nh;

  // Create a ROS subscriber for the input point cloud
  ros::Subscriber sub = nh.subscribe<sensor_msgs::PointCloud2>("/right_velodyne/velodyne_points", 10, cloud_cb);
  // Create a ROS publisher for the output model coefficients
  if(strcmp(argv[1], "both") == 0){
    pub = nh.advertise<sensor_msgs::PointCloud2>("/right_velodyne/transformed", 10);
  }
  else{
    pub = nh.advertise<sensor_msgs::PointCloud2>("/velodyne_points", 10);
  }

  // Spin
  ros::spin ();
}
