#include "ros/ros.h"
#include "std_msgs/String.h"
#include <sensor_msgs/PointCloud2.h>

ros::Publisher pub;

void subscriberCallback(const sensor_msgs::PointCloud2ConstPtr& pc2)
{ 

  sensor_msgs::PointCloud2 output;
  output = *pc2;
  pub.publish(output);

}

int main(int argc, char **argv)
{

  ros::init(argc, argv, "left_velodyne2velodyne_points");
  ros::NodeHandle n;
  pub = n.advertise<sensor_msgs::PointCloud2>("velodyne_points", 20);
  ros::Subscriber sub = n.subscribe("left_velodyne/velodyne_points", 20, subscriberCallback);
  ros::spin();

  return 0;
}

