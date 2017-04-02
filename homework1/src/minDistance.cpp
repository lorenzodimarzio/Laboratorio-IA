#include "ros/ros.h"
#include "sensor_msgs/LaserScan.h"
#include "std_msgs/String.h"
 
float _minDistance;
ros::Publisher pub;

void baseScanCallback(const sensor_msgs::LaserScan::ConstPtr& msg)
{
	int size = msg->ranges.size();
	float _minDistance = msg->ranges[0];
	for (int i=1; i<size; i++) {
	  if (msg->ranges[i] < _minDistance) _minDistance = msg->ranges[i];
	}
	ROS_INFO("Ho misurato %f", _minDistance);
	
	std_msgs::String msgg;
	
	std::stringstream ss;
	ss << "Min distance is " << _minDistance;
	msgg.data = ss.str();
	
	pub.publish(msgg);
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "minDistance");
	ros::NodeHandle n;
	
	ros::Subscriber sub = n.subscribe("base_scan", 1000, baseScanCallback);
	
	pub = n.advertise<std_msgs::String>("minDistance_topic", 1000);
	
	ros::spin();
	
	return 0;
}
