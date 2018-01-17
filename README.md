# Usage

First, catkin_make the two workspaces and source in the same order as your making process. Be sure to download and compile the BLAM package properly. The modified files are LaserLoopClosure.cc, test_online.launch, and lidar_slam.rviz. You may want to replace them before compilation. 

Second, you may need to modify the path to **robot_description** in **test_online.launch**.

Third, filter out the **tf** topic from your recorded bag using:
```sh
$ rosbag filter data.bag data-no-tf.bag "topic!='/tf'"
```

Finally, you can test the code by running:
```sh
$ roslaunch blam_example test_online.launch
$ rosbag play data-no-tf.bag --clock
```

In our setting, we have two velodune lidars. By default, only the left lidar will be used. If you want to use the right lidar or both lidars at the same time, launch with:
```sh
$ roslaunch blam_example test_online.launch lidar:=right
$ roslaunch blam_example test_online.launch lidar:=both
```