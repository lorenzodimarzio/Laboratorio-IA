#include "ros/ros.h"
#include "tf2_msgs/TFMessage.h"

void tfCallback(const tf2_msgs::TFMessage::ConstPtr& msg)
{	
	for (int i=0; i<msg->transforms.size(); i++) {
		if (msg->transforms[i].header.frame_id == "odom") {
			ROS_INFO("LASER (%f,%f,%f)",
				-(msg->transforms[i].transform.translation.x),
				-(msg->transforms[i].transform.translation.y),
				-(atan(msg->transforms[i].transform.rotation.w/msg->transforms[i].transform.rotation.z))
			);
			
		}
	}
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "laserLocation");
	ros::NodeHandle n;
	
	ros::Subscriber sub = n.subscribe("tf", 1000, tfCallback);
	
	ros::spin();
	
	return 0;
}
