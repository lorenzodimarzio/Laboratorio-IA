#include "ros/ros.h"
#include "sensor_msgs/LaserScan.h"
#include "std_msgs/String.h"
#include "cv_bridge/cv_bridge.h"
//OpenCV
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

float minDistance;

void baseScanCallback(const sensor_msgs::LaserScanConstPtr& msg) {
	
	int width = 600;
	int length = 600;
	int len = msg->ranges.size();
	int scale = (width < length ? width : length)/(2*msg->range_max);

	cv::Point center(width/2, length/2);
	cv::Point points[len];

	cv::Mat img(length, width, CV_8UC3, cv::Scalar(255,255,255));

	double angle = 0;

	float minDistance = msg->ranges[0];
	int minIndex = 0;
	for (int i=0; i<len; i++) {
		if (msg->ranges[i] < minDistance) {
			minDistance = msg->ranges[i];
			minIndex = i;
		}
		
		double deltax = msg->ranges[i]*scale*cos(angle);
		double deltay = msg->ranges[i]*scale*sin(angle);
		
		points[i] = cv::Point(center.x - deltax, center.y + deltay);
		
		angle += msg->angle_increment;
	}

	cv::line(img, center, points[len-1], cv::Scalar(0,0,0));
	for (int i=len-1; i>0; i--) {
		cv::line(img, points[i], points[i-1], cv::Scalar(0,0,0));
	}
	cv::line(img, points[0], center, cv::Scalar(0,0,0));
	
	cv::circle(img, points[minIndex], 5, cv::Scalar(0,0,255), 2);
	
	cv::imshow("NOME", img);
	cv::waitKey(30);
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "homework4_node");

	ros::NodeHandle n;
	
	ros::Subscriber basescan_sub = n.subscribe("base_scan", 1000, baseScanCallback);
	
	ros::spin();
	
	return 0;
}
