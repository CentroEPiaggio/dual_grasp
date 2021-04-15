   #include "ros/ros.h"
   #include "qb_class_imu.h"

   #include "std_msgs/Float64.h"
   #include <vector>   
   #include "std_msgs/Float64MultiArray.h"
   #include "geometry_msgs/WrenchStamped.h"

   #include <sstream>
comm_settings qb_comm_;
char *port_s=(char*)"/dev/ttyUSB0";
int id_imu_ = 1;
uint8_t* ids_;
uint8_t* imu_table_;
uint8_t* mag_cal_;
uint8_t num_mag_cal_params_ = 0;
uint16_t num_of_params_;
// Imu values is a 3 sensors x 3 axes x n_max_imu
float* imu_values_;
uint16_t tstep = 0;

//NUOVO
int count=0;

// ================================================================================= global code variables
char x_;
char g_;
int n_imu_;

int sizeBuffer_;
uint8_t* dataBuffer_; 
char cwd[256]; 


std_msgs::Float64MultiArray acc_data, gyro_data;
std_msgs::Float64 hand_data;
geometry_msgs::WrenchStamped F_ext_data;
std::vector<double> vect;



    void gyroCallback(const std_msgs::Float64MultiArray::ConstPtr &msg)
   {
     gyro_data=*msg;
     ROS_INFO("I heard gyro");
   }
    void accCallback(const std_msgs::Float64MultiArray::ConstPtr &msg)
   {
     acc_data=*msg;
     ROS_INFO("I heard acc");
   }

    void handCallback(const std_msgs::Float64::ConstPtr &msg)
   {
     hand_data=*msg;
     ROS_INFO("I heard hand");
   }

    void F_extCallback(const geometry_msgs::WrenchStamped::ConstPtr &msg)
   {
     F_ext_data=*msg;
     ROS_INFO("I heard F_ext");
   }



   int main(int argc, char **argv)
   { 

     ros::init(argc, argv, "talker");

     ros::NodeHandle n;


     ros::Subscriber gyro_sub = n.subscribe("qb_class_imu/gyro_vett", 1, gyroCallback);
     ros::Subscriber acc_sub = n.subscribe("qb_class_imu/acc_vett", 1, accCallback);
     ros::Subscriber hand_sub = n.subscribe("/qb_class/hand_measurement/grasp_failure", 1, handCallback);
     ros::Subscriber F_ext_sub = n.subscribe("/panda_arm_1/franka_state_controller_1/F_ext", 1, F_extCallback);


      ros::Publisher F_ext_pub = n.advertise<geometry_msgs::WrenchStamped>("F_ext", 1);
      ros::Publisher hand_pub = n.advertise<std_msgs::Float64>("hand_opening", 1);
      ros::Publisher gyro_pub = n.advertise<std_msgs::Float64MultiArray>("gyro",1);
      ros::Publisher acc_pub = n.advertise<std_msgs::Float64MultiArray>("acc", 1);
      ros::Publisher vector_pub = n.advertise<std_msgs::Float64MultiArray>("vector", 1);

   
     ros::Rate rate(50);

  
     /**
      * A count of how many messages we have sent. This is used to create
      * a unique string for each message.
      */
     int count = 0;
     while (ros::ok())
     {
       /**
       * This is a message object. You stuff it with data, and then publish it.
        */
      std::vector<double> vect{0, 1, 2, 3, 4};
      std_msgs::Float64MultiArray vett;
      vett.data=vect;     
       
      gyro_pub.publish(gyro_data);
      acc_pub.publish(acc_data);
      hand_pub.publish(hand_data);
      F_ext_pub.publish(F_ext_data);
      vector_pub.publish(vett);


  
      ros::spinOnce();
  
      rate.sleep();
      ++count;
   }
  
  
    return 0;
 }