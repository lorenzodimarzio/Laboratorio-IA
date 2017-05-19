#include <ros/ros.h>
#include <actionlib/server/simple_action_server.h>
#include <homework5/MoveAction.h>
#include <geometry_msgs/Twist.h>
#include <nav_msgs/Odometry.h>

ros::Publisher pub;
nav_msgs::Odometry::ConstPtr lastKnownOdom;

void odomCB(const nav_msgs::Odometry::ConstPtr& msg) {
  lastKnownOdom = msg;
  //ROS_INFO("lastKnownOdomX=%f", lastKnownOdom->pose.pose.position.x);
}


class MoveAction
{
protected:

  ros::NodeHandle nh_;
  actionlib::SimpleActionServer<homework5::MoveAction> as_; // NodeHandle instance must be created before this line. Otherwise strange error occurs.
  std::string action_name_;
  homework5::MoveFeedback feedback_;
  homework5::MoveResult result_;

public:

  MoveAction(std::string name) : as_(nh_, name, boost::bind(&MoveAction::executeCB, this, _1), false), action_name_(name) {
    as_.start();
  }

  ~MoveAction(void) {}


  void executeCB(const homework5::MoveGoalConstPtr &goal) {

    // helper variables
    bool success = true;

    // create the message
    geometry_msgs::Twist msg;
    msg.linear.x = goal->desired_speed;
    msg.linear.y = 0;
    msg.linear.z = 0;
    msg.angular.x = 0;
    msg.angular.y = 0;
    msg.angular.z = 0;

    // publish info to the console for the user
    ROS_INFO("%s: Executing, moving at %f speed for %f distance", action_name_.c_str(), goal->desired_speed, goal->distance);

    if (as_.isPreemptRequested() || !ros::ok())
    {
      ROS_INFO("%s: Preempted", action_name_.c_str());
        // set the action state to preempted
      as_.setPreempted();
      success = false;
    }

    // set the current time and distance
    ros::Time t0 = ros::Time::now();
    float current_distance = 0;

    // loop to move the robot until distance is reached
    while (ros::ok() && current_distance < goal->distance) {  
      pub.publish(msg);
      ros::Time t1 = ros::Time::now();
      printf("Secondi tempo t1: %f\n", t1.toSec());
      printf("current_distance: %f\n", current_distance);
      current_distance = (goal->desired_speed)*(t1.toSec()-t0.toSec());
    }

    // stop the robot when distance is reached
    msg.linear.x = 0;
    pub.publish(msg);
    
    if(success) {
      result_.odom_pose = *lastKnownOdom;
      ROS_INFO("%s: Succeeded", action_name_.c_str());
      // set the action state to succeeded
      as_.setSucceeded(result_);
    }

  }
};

  int main(int argc, char** argv)
  {
    ros::init(argc, argv, "Move");
    ros::NodeHandle n;

    MoveAction Move("Move");

    pub = n.advertise<geometry_msgs::Twist>("cmd_vel", 1000);
    ros::Subscriber sub = n.subscribe("odom", 1000, odomCB);
    ros::spin();

    return 0;
  }