import rospy
from std_msgs.msg import Float64MultiArray
from std_msgs.msg import Float64
from geometry_msgs.msg import WrenchStamped
import numpy as np
import time


def publish_callback(self):
    msg = Float64
    msg.data = 2
    self.pub_publish.publish(msg)

matrix = np.zeros((1508,106))
class NN:
    def __init__(self):
        print('Controller: initializing node')
        self.sub_array = rospy.Subscriber("array", Float64MultiArray, self.array_callback)
        self.pub_publish = rospy.Publisher("output_NN", Float64, queue_size=1)

    def array_callback(self, msg):
        self.array = msg.data
        self.array_data = np.array([self.array])

   
######  
       
        for i in range(1507):
            matrix[i,:] = matrix [i+1,:]
        
        matrix[1507,:] = self.array
        

        ###RETE

        publish_callback(self)
        print'Matrix of data %s' %matrix
        

   

            
if __name__ == '__main__':
    print('starting listen topic.py')
    rospy.init_node('NN')
    try:
        node = NN()
        rospy.spin()
    except rospy.ROSInterruptException:
        print('caught exception')
    print('exiting')

        
   