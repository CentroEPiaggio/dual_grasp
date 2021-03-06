#include "ros/ros.h"
#include "Eigen/Dense"
#include <eigen_conversions/eigen_msg.h>


// ROS Service and Message Includes
#include "std_msgs/Float64.h"
#include <iostream>
#include <vector>
#include "std_msgs/Bool.h"
#include "std_srvs/SetBool.h"
#include "panda_softhand_control/set_object.h"
#include "panda_softhand_control/complex_grasp.h"
#include "geometry_msgs/Pose.h"
#include <controller_manager_msgs/SwitchController.h>
#include <franka_msgs/FrankaState.h>
#include <franka_msgs/ErrorRecoveryActionGoal.h>

// Parsing includes
#include <XmlRpcValue.h>
#include <XmlRpcException.h>

// MoveIt
#include <moveit/robot_model_loader/robot_model_loader.h>
#include <moveit/robot_model/robot_model.h>
#include <moveit/robot_state/robot_state.h>

// Custom Includes
#include "panda_softhand_control/PandaSoftHandClient_2.h"



#include "qb_class_2.h"

// Defines
#define     DEBUG   1       // Prints out additional stuff
#define     VISUAL          // Publishes visual info on RViz

class ReGrasp {
    /// public variables and functions ------------------------------------------------------------
    public:
        ReGrasp(ros::NodeHandle& nh_);

        ~ReGrasp();

        // Parameters parsing
        bool parse_task_params();

        geometry_msgs::Pose convert_vector_to_pose(std::vector<double> input_vec);

        void get_object_pose(const geometry_msgs::Pose::ConstPtr &msg);

        // Callback for franka state subscriber
        void get_franka_state(const franka_msgs::FrankaState::ConstPtr &msg);

       // Callback for simple grasp task service
       bool call_simple_grasp_task_2(std_srvs::SetBool::Request &req, std_srvs::SetBool::Response &res);

       bool performIK(geometry_msgs::Pose pose_in, double timeout, std::vector<double>& joints_out);

    /// private variables -------------------------------------------------------------------------
    private:
       ros::NodeHandle nh;

     // Subscriber to object pose and the pose
       
        geometry_msgs::Pose object_pose_T;

        // Subscriber to franka_states for getting tau_ext on joints and other info and Publisher of its norm
   
        franka_msgs::FrankaState latest_franka_state;
        bool franka_ok = true;
   
        
        // Open and Close pub

        ros::Publisher handRef_pub;
        std::vector<float> v1{19000};
        std::vector<float> v2{0};
        
        // The Panda SoftHand Client
        PandaSoftHandClient_2 panda_softhand_client_2;
   
   // Service names
        std::string grasp_task_service_name_2;
        std::string franka_state_topic_name_2 = "/franka_state_controller_2/franka_states";
        //ros::ServiceServer grasp_task_server;
       // Parsed task sequence variables
       std::string robot_name_2;                     // Name of the robot (namespace)
       std::string robot_joints_name_2;              // Name of the robot joints (without the number of the joints)
       std::string pos_controller_2;                 // Name of position controller
       std::string imp_controller_2;                 // Name of impedance controller
       std::string object_topic_name_2;              // Name of the topic where the object pose is published
       std::vector<double> home_joints_2;
       std::vector<double> grasp_transform_2;
       geometry_msgs::Pose grasp_T;
       std::vector<double> pre_grasp_transform_2;
       geometry_msgs::Pose pre_grasp_T;
       std::vector<double> post_grasp_transform_2;
       geometry_msgs::Pose post_grasp_T;
       std::vector<double> home_grasp_transform_2;
       geometry_msgs::Pose home_grasp_T;
       // MoveIt stuff and functions for FK and IK
       std::string group_name;
       std::string end_effector_name;
       std::shared_ptr<robot_model_loader::RobotModelLoader> robot_model_loader_ptr;
       robot_model::RobotModelPtr kinematic_model;
       robot_state::RobotStatePtr kinematic_state;
    
       //Generic
       trajectory_msgs::JointTrajectory tmp_traj;
       ros::Duration waiting_time;
       ros::Duration waiting_time2;
       std::vector<double> null_joints;                            // null joints in order to make joint plan from present joints
       ros::ServiceServer grasp_task_server_2;
};