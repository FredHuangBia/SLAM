# Usage

First, catkin_make the two workspaces and source in the same order as your making process.

Second, you may need to modify the path to **robot_description** in **test_online.launch**.

Third, filter out the **tf** topic from your recorded bag using:
```sh
$ rosbag filter data.bag data-no-tf.bag "topic!='tf'
```

Finally, you can test the code by running:
```sh
$ roslaunch blam_example test_online.launch
$ rosbag play data-no-tf.bag --clock
```