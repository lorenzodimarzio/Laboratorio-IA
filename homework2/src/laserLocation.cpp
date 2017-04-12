#include "ros/ros.h"
#include "sensor_msgs/LaserScan.h"
#include "tf/transform_listener.h"
//#include "tf2/ExtrapolationException.h"

tf::TransformListener* listener;

void callback(const sensor_msgs::LaserScan::ConstPtr& msg)
{	
	try {
		tf::StampedTransform transform;
		
		if (listener->waitForTransform(
				"/odom",
				"/base_laser_link",
				ros::Time::now(),
				ros::Duration(1))) {
		} else {
			ROS_INFO("Cannot transform");
		}
		
		listener->lookupTransform(
				"/odom",
				ros::Time::now(),
				"/base_laser_link",
				ros::Time::now(),
				"/base_laser_link",
				transform);	
		
		ROS_INFO("LASER %d.%d (%lf, %lf, %lf)",
				transform.stamp_.sec,
				transform.stamp_.nsec,
				transform.getOrigin().getX(),
				transform.getOrigin().getY(),
				transform.getRotation().getAngle());
				
	} catch(tf2::ExtrapolationException ex) {
		ROS_INFO("Exception!");
	}
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "laserLocation");
	ros::NodeHandle n;
	
	listener = new tf::TransformListener();
	
	ros::Subscriber sub = n.subscribe("base_scan", 1000, callback);
	
	ros::spin();
	
	return 0;
}
