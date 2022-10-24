
#ifndef _LEAPROS_H
#define _LEAPROS_H

#include "ros/ros.h"
#include "Leap.h"
#include "geometry_msgs/Pose.h"
#include "geometry_msgs/PoseArray.h"

class RosListener:public Leap::Listener{
  public:
    
    //RosListener();
    virtual void onInit(const Leap::Controller& );
    virtual void onConnect(const Leap::Controller& );
    virtual void onDisconnect(const Leap::Controller& );
    virtual void onExit(const Leap::Controller& );
    virtual void onFrame(const Leap::Controller& );

    geometry_msgs::Pose pose2RA;   //设置给机械臂的消息6 * 1，前三个为掌心位置，后三个为掌心法向量
    geometry_msgs::Pose p; //push入PoseArray的中间量
    geometry_msgs::PoseArray poses2Gloves; //设置传递给手套的消息，16 * 6，第一个为空，后十五个分别对应拇指三个手指（从掌骨到指尖），食指四个手指（从掌骨到指尖），中指，无名指依次类推
};

#endif