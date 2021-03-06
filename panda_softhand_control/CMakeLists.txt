cmake_minimum_required(VERSION 2.8.3)
project(panda_softhand_control)

add_definitions(-std=c++11)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  std_srvs
  rviz
  rviz_visual_tools
  message_generation
  actionlib_msgs
  geometry_msgs
  actionlib
  sensor_msgs
  tf
  tf_conversions
  moveit_msgs
  moveit_ros_planning 
  moveit_ros_planning_interface
  moveit_visual_tools
  interactive_markers 
  visualization_msgs
  moveit_core
  franka_msgs
  franka_control
  qb_interface
)

add_service_files(FILES
  hand_plan.srv
  hand_wait.srv
  hand_control.srv
  arm_wait.srv
  arm_control.srv
  slerp_plan.srv
  pose_plan.srv
  joint_plan.srv
  set_object.srv
  complex_grasp.srv
)

generate_messages(DEPENDENCIES
  std_msgs
  tf
  geometry_msgs
  trajectory_msgs
)

find_package(SDFormat REQUIRED)

catkin_package(
  INCLUDE_DIRS include
  CATKIN_DEPENDS actionlib_msgs moveit_ros_planning_interface message_runtime std_msgs std_srvs qb_interface
  LIBRARIES ${PROJECT_NAME}_lib
)

set(OBJECTS_SOURCE_FILES
  src/utils/parsing_utilities.cpp
  src/HandPlan_1.cpp
  src/HandControl_1.cpp
  src/ArmControl_1.cpp
  src/SlerpPlan_1.cpp 
  src/PosePlan_1.cpp
  src/JointPlan_1.cpp
  src/PandaSoftHandClient_1.cpp
  src/HandPlan_2.cpp
  src/HandControl_2.cpp
  src/ArmControl_2.cpp
  src/SlerpPlan_2.cpp 
  src/PosePlan_2.cpp
  src/JointPlan_2.cpp
  src/PandaSoftHandClient_2.cpp
  src/GraspFailure.cpp
  src/ReGrasp.cpp
)

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
  /usr/include/eigen3
  ${SDFormat_INCLUDE_DIRS}
)

link_directories(${SDFormat_LIBRARY_DIRS})

## Declare a C++ library
set(LIB_SOURCE_FILES
    src/PandaSoftHandClient_1.cpp
    src/PandaSoftHandClient_2.cpp
)
add_library(${PROJECT_NAME}_lib
  ${LIB_SOURCE_FILES}
)
target_link_libraries(${PROJECT_NAME}_lib ${catkin_LIBRARIES} ${ROS_LIBRARIES})

# NODE: Lower level position controls
add_executable(data src/data.cpp ${OBJECTS_SOURCE_FILES})
add_dependencies(data ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(data ${catkin_LIBRARIES} ${ROS_LIBRARIES})


# NODE: Lower level position controls
add_executable(control_server_1 src/control_server_1.cpp ${OBJECTS_SOURCE_FILES})
add_dependencies(control_server_1 ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(control_server_1 ${catkin_LIBRARIES} ${ROS_LIBRARIES})


# NODE: Lower level position controls
add_executable(control_server_2 src/control_server_2.cpp ${OBJECTS_SOURCE_FILES})
add_dependencies(control_server_2 ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(control_server_2 ${catkin_LIBRARIES} ${ROS_LIBRARIES})


# NODE: Grasp
add_executable(grasp_robot_1 src/grasp_robot_1.cpp ${OBJECTS_SOURCE_FILES})
add_dependencies(grasp_robot_1 ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(grasp_robot_1 ${catkin_LIBRARIES} ${ROS_LIBRARIES})

# NODE: Regrasp
add_executable(grasp_robot_2 src/grasp_robot_2.cpp ${OBJECTS_SOURCE_FILES})
add_dependencies(grasp_robot_2 ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(grasp_robot_2 ${catkin_LIBRARIES} ${ROS_LIBRARIES})

catkin_install_python(PROGRAMS src/listner.py 
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
  )


# NODE: Utility to publish collision objects to planning scene from world file
add_executable(world_to_planning_scene_node src/SdfToPlanningScene.cpp)
target_link_libraries(world_to_planning_scene_node ${catkin_LIBRARIES} ${ROS_LIBRARIES} ${SDFormat_LIBRARIES})



# Mark cpp header files for installation
install(TARGETS ${PROJECT_NAME}_lib
        ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
        LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
        RUNTIME DESTINATION ${CATKIN_GLOBAL_BIN_DESTINATION}
)

install(DIRECTORY include/${PROJECT_NAME}/
  DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
  FILES_MATCHING PATTERN "*.h"
  PATTERN ".svn" EXCLUDE
)