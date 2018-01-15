#include "ros/ros.h"
#include <sensor_msgs/PointCloud2.h>
#include <pcl_ros/point_cloud.h>
#include <message_filters/subscriber.h>
#include <message_filters/synchronizer.h>
#include <message_filters/sync_policies/approximate_time.h>
#include <pcl/point_types.h>
#include <pcl_ros/transforms.h>
#include <tf/tf.h>
#include <iostream>
#include <vector>
#include <pcl/registration/gicp.h>
#include <thread>
typedef pcl::PointCloud<pcl::PointXYZ> PointCloud;

using namespace std;
using namespace message_filters;


//TODO 1. subscribe two loop closure scan  2. wait for keyboard command to perform visualization 3. use multi-thread to deal with it.
ros::Publisher pub1;
ros::Publisher pub2;

//New data form to represent the loop closure.
struct loop_closure{
  PointCloud A;
  PointCloud B;
};
// Using vector to store all received data.
vector<loop_closure> loop_closures;

// Every time we subscribe a pair of loop closure, push it to the vector.
bool addtoset(const PointCloud::ConstPtr& source, const PointCloud::ConstPtr& target){
  if (source==NULL || target==NULL){
    return false;
  }

  loop_closure instance;
  instance.A=*source;
  instance.B=*target;
  loop_closures.push_back(instance);
  ROS_INFO_STREAM(loop_closures.size() << "received");
  return true;
}

// Waiting for command to visualize icp
int visualize_helper(const PointCloud::Ptr& source, const PointCloud::Ptr& target){
  if(source==NULL || target==NULL){
    ROS_INFO_STREAM("NULL pointers!");
    return 0;
  }
  pcl::GeneralizedIterativeClosestPoint<pcl::PointXYZ, pcl::PointXYZ> icp;//initialize icp
  icp.setTransformationEpsilon(0.0000000001);
  icp.setMaxCorrespondenceDistance(2.0);
  icp.setMaximumIterations(1);// set iteration to 1 so that we can visualize the process in a loop
  icp.setRANSACIterations(0);
  icp.setInputTarget(target);
  icp.setInputSource(source);
  PointCloud::Ptr result=source;
  pub1.publish(*target);// the output of each iteration.

  for(int i=1;i<=10;i++){
    pub2.publish(*result);
    cout<<"Press Enter to start iteration "<< i <<endl;
    cin.get();

    icp.setInputSource(result);
    icp.align (*result);// the transformed source point cloud??
    cout<<"Reach Convergence? "<<icp.hasConverged()<<endl;
  }
}

int visualize(){
  while(true){
    cout<<"Enter corresponding index to visualize closure or -1 to quite"<<endl;
    int index;
    cin>>index;//waiting for which loop closure to visualize
    if(index<=0){
      return 0;
    }
    if(index<=loop_closures.size()){
      //make boost pointer
      cout<<"Start to visualize loop closure "<<index<<endl;
      cin.get();
      PointCloud::Ptr source=boost::make_shared<PointCloud> (loop_closures[index-1].A);
      PointCloud::Ptr target=boost::make_shared<PointCloud> (loop_closures[index-1].B);
      visualize_helper(source,target);
    }
    else{
      cout<<"Index out of range!"<<endl;
    }
  }
  return 0;
}




// multi thread for subscribe callback and visualization
int main(int argc, char **argv) {

  ros::init (argc, argv, "visualize_icp");

  thread handle_interact(&visualize);

  ros::NodeHandle nh;

  pub1 = nh.advertise<PointCloud>("/visualize_icp_target", 10);
  pub2 = nh.advertise<PointCloud>("/visualize_icp_result", 10);
  ros::Rate r(10);
  //get pairs of loop closure.
  message_filters::Subscriber<PointCloud> source(nh, "/blam/blam_slam/loop_closure_scan1", 10);
  message_filters::Subscriber<PointCloud> target(nh, "/blam/blam_slam/loop_closure_scan2", 10);

  typedef sync_policies::ApproximateTime<PointCloud, PointCloud> MySyncPolicy;
  Synchronizer<MySyncPolicy> sync(MySyncPolicy(10), source, target);
  sync.registerCallback(boost::bind(&addtoset, _1, _2));

  while (nh.ok())
  {
    ros::spinOnce();
    r.sleep();
  }

  handle_interact.join();

  return 0;
}
