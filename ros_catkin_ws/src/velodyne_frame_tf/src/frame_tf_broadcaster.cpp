#include "ros/ros.h"
#include "tf/transform_broadcaster.h"

int main(int argc, char** argv){
  ros::init(argc, argv, "velodyne_frame_tf_broadcaster");
  ros::NodeHandle node;

  tf::TransformBroadcaster br;
  tf::Transform transform;

  ros::Rate rate(60.0);
  while (node.ok()){
    transform.setOrigin( tf::Vector3(0, 0, 0) );
    transform.setRotation( tf::Quaternion(0, 0, 0, 1) );
    br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "base", "base_link"));
    rate.sleep();
  }
  return 0;

};