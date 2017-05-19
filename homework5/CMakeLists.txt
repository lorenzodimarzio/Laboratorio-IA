cmake_minimum_required(VERSION 2.8.3)
project(homework5)

find_package(catkin REQUIRED COMPONENTS roscpp actionlib actionlib_msgs nav_msgs)

find_package(Boost REQUIRED COMPONENTS system)

add_action_files(
  DIRECTORY action
  FILES Move.action
)

generate_messages(
  DEPENDENCIES actionlib_msgs std_msgs nav_msgs
)

catkin_package(
  CATKIN_DEPENDS actionlib_msgs
)

include_directories(include ${catkin_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})

add_executable(move_server src/move_server.cpp)

target_link_libraries(
  move_server
  ${catkin_LIBRARIES}
)

add_dependencies(
  move_server
  ${homework5_EXPORTED_TARGETS}
)

add_executable(move_client src/move_client.cpp)

target_link_libraries( 
  move_client
  ${catkin_LIBRARIES}
)

add_dependencies(
  move_client
  ${homework5_EXPORTED_TARGETS}
)