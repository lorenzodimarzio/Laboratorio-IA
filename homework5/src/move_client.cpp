#include <ros/ros.h>
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>
#include <homework5/MoveAction.h>

int main (int argc, char **argv)
{
  if (argc < 3)
  {
    printf("Usage: move_client [desired_speed] [distance]\n");
    return -1;
  }
  if (atof(argv[2]) <= 0) {
    printf("Distance must be positive");
    return -1;
  } 
  ros::init(argc, argv, "test_Move");

  // create the action client
  // true causes the client to spin its own thread
  actionlib::SimpleActionClient<homework5::MoveAction> ac("Move", true);

  ROS_INFO("Waiting for action server to start.");
  // wait for the action server to start
  ac.waitForServer(); //will wait for infinite time

  ROS_INFO("Action server started, sending goal.");
  // send a goal to the action
  homework5::MoveGoal goal;
  goal.desired_speed = atof(argv[1]);
  goal.distance = atof(argv[2]);
  ac.sendGoal(goal);

  //wait for the action to return
  bool finished_before_timeout = ac.waitForResult(ros::Duration(100000));

  if (finished_before_timeout)
  {
    actionlib::SimpleClientGoalState state = ac.getState();
    ROS_INFO("Action finished: %s",state.toString().c_str());
  }
  else
    ROS_INFO("Action did not finish before the time out.");

  //exit
  return 0;
}