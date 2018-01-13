#include "ros/ros.h"
#include <sensor_msgs/PointCloud2.h>
#include <pcl_ros/point_cloud.h>
#include <message_filters/subscriber.h>
#include <message_filters/synchronizer.h>
#include <message_filters/sync_policies/approximate_time.h>
#include <pcl/point_types.h>

using namespace message_filters;
using namespace sensor_msgs;

PointCloud2 left;
PointCloud2 right;
ros::Publisher pub;

void merge(const PointCloud2ConstPtr& input_l, const PointCloud2ConstPtr& input_r)
{

  PointCloud2 output;
  pcl::concatenatePointCloud(*input_l, *input_r, output);

  pub.publish (output);
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "merge_two_pcls");

  ros::NodeHandle nh;

  pub = nh.advertise<PointCloud2>("/velodyne_points", 10);

  message_filters::Subscriber<PointCloud2> pcl_l_sub(nh, "/left_velodyne/transformed", 1);
  message_filters::Subscriber<PointCloud2> pcl_r_sub(nh, "/right_velodyne/transformed", 1);

  typedef sync_policies::ApproximateTime<PointCloud2, PointCloud2> MySyncPolicy;
  Synchronizer<MySyncPolicy> sync(MySyncPolicy(10), pcl_l_sub, pcl_r_sub);
  sync.registerCallback(boost::bind(&merge, _1, _2));  

  ros::spin();

  return 0;
}