#include "ros/ros.h"
#include "std_msgs/String.h"
#include "geometry_msgs/PoseStamped.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <net/if.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

#include <linux/can.h>
#include <linux/can/raw.h>

#include <math.h>
#include <time.h>
double x,y,z;
//浮点数转16进制函数
//void FloatToByte(float floatNum,unsigned char*byteArry)
//{
//    char*pchar=(char*)&floatNum;
//    for(int i=0; i<sizeof(float); i++) {
//        *byteArry=*pchar;
//        pchar++;
//        byteArry++;
//    }
//}

void chatterCallback(const geometry_msgs::PoseStamped::ConstPtr& msg)
{	
  
//  double x,y,z;
//  ROS_INFO("ok");
  x = msg->pose.position.x;
  y = msg->pose.position.y;
  z = msg->pose.position.z;
  ROS_INFO("\nx : %f\ny : %f\nz : %f\nsuccessed!\n",x,y,z);
}

int main(int argc, char **argv)
{
 
  	ros::init(argc, argv, "listener");
  	ros::NodeHandle n;
  	ros::Subscriber sub = n.subscribe("tripod/pose", 1000, chatterCallback); 

	int s;					//！是socket返回的套接字描述符//
    int nbytes_write,nbytes_read;
    struct sockaddr_can addr;  //！这个结构体是用来存套接字的地址，例如32,33//
    struct can_frame frame;
    struct ifreq ifr;    //！ifreq是一个配置接口（例如can0），IP地址的等等//

    const char *ifname = "can0";

    if((s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {   //!socket函数定义，返回-1,就是发生错误//
        perror("Error while opening socket");
        return -1;
    }

    strcpy(ifr.ifr_name, ifname);
    ioctl(s, SIOCGIFINDEX, &ifr);        //！一般ioctl是用来管理i/o口的，例如波特率，//

    addr.can_family  = AF_CAN;			//！地址族//
    addr.can_ifindex = ifr.ifr_ifindex;		//！协议地址//

	if(bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) 
	{   //!就是bind函数，把本地和套接字连接//
        perror("Error in socket bind");
        return -2;
    }
    
    
    

	
 ros::Rate rate(10);
 while(ros::ok()&&(x*x+y*y > 0.4))
 {
  	ros::spinOnce();
  	
 	frame.can_id  = 0x104A0072;
    if(!(frame.can_id & CAN_ERR_FLAG))    
    	frame.can_id |= CAN_EFF_FLAG;	  
	frame.can_dlc = 2;
  	frame.data[0] = 0x01;
    frame.data[1] = 0x00;
    nbytes_write = write(s, &frame, sizeof(struct can_frame));
    printf("Wrote %d bytes\n", nbytes_write);
       		
    frame.can_id  = 0x104A0076;
    if(!(frame.can_id & CAN_ERR_FLAG))    
    	frame.can_id |= CAN_EFF_FLAG;	  
	frame.can_dlc = 2;
  	frame.data[0] = 0x01;
    frame.data[1] = 0x00;
    nbytes_write = write(s, &frame, sizeof(struct can_frame));
    printf("Wrote %d bytes\n", nbytes_write);
//    frame.can_id  = 0x104A005C;
//    frame.can_dlc = 4;
//    if(!(frame.can_id & CAN_ERR_FLAG))    
//    	frame.can_id |= CAN_EFF_FLAG;
//    frame.can_dlc = 4;
//    frame.data[0] = 0x00;
//    frame.data[1] = 0x00;
//    frame.data[2] = 0x80;
//    frame.data[3] = 0x3F;
//    nbytes = write(s, &frame, sizeof(struct can_frame));
//    printf("Wrote %d bytes\n", nbytes);
  	
    if(nbytes_write != sizeof(frame)) 
    {
   		printf("Send failed, Wrote %d bytes\n", nbytes_write);
    } 
   	else 
    {
      	printf("Send successfully ! \n");
    }
  	
  	
    
    
  	rate.sleep();
 }
 
 frame.can_id  = 0x104A0072;
    if(!(frame.can_id & CAN_ERR_FLAG))    
    	frame.can_id |= CAN_EFF_FLAG;	  
	frame.can_dlc = 2;
  	frame.data[0] = 0x00;
    frame.data[1] = 0x00;
    nbytes_write = write(s, &frame, sizeof(struct can_frame));
    printf("Wrote %d bytes\n", nbytes_write);
       		
    frame.can_id  = 0x104A0076;
    if(!(frame.can_id & CAN_ERR_FLAG))    
    	frame.can_id |= CAN_EFF_FLAG;	  
	frame.can_dlc = 2;
  	frame.data[0] = 0x00;
    frame.data[1] = 0x00;
    nbytes_write = write(s, &frame, sizeof(struct can_frame));
    printf("Wrote %d bytes\n", nbytes_write);
//    frame.can_id  = 0x104A005C;
//    frame.can_dlc = 4;
//    if(!(frame.can_id & CAN_ERR_FLAG))    
//    	frame.can_id |= CAN_EFF_FLAG;
//    frame.can_dlc = 4;
//    frame.data[0] = 0x00;
//    frame.data[1] = 0x00;
//    frame.data[2] = 0x80;
//    frame.data[3] = 0x3F;
//    nbytes = write(s, &frame, sizeof(struct can_frame));
//    printf("Wrote %d bytes\n", nbytes);
  	
    if(nbytes_write != sizeof(frame)) 
    {
   		printf("Send failed, Wrote %d bytes\n", nbytes_write);
    } 
   	else 
    {
      	printf("Send successfully ! \n");
    }
    
  return 0;
}
