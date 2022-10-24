#include"Leapros.h"
#include "ros/ros.h"
#include<iostream>
#include <math.h>
#include <iomanip>

void RosListener::onInit(const Leap::Controller& controller){
    std::cout << "Initialized" <<std::endl;
    
    //初始化，这里我不用w，position.x/y/z表示掌心坐标，orientation.x/y/z表示手掌法向量orientation.w弃用
    pose2RA.position.x = 0;
    pose2RA.position.y = 0;
    pose2RA.position.z = 0;
    pose2RA.orientation.x = 0;
    pose2RA.orientation.y = 0;
    pose2RA.orientation.z = 0;
    pose2RA.orientation.w = 0;
}

void RosListener::onConnect(const Leap::Controller& controller){
    std::cout<< "Connected" <<std::endl;
    controller.enableGesture(Leap::Gesture::TYPE_CIRCLE);	//启用画圆手势
}

void RosListener::onDisconnect(const Leap::Controller& controller){
    std::cout<< "Disconnected" <<std:: endl;
}

void RosListener::onExit(const Leap::Controller& controller){
    std::cout<< "Exited" <<std::endl;
}

void RosListener::onFrame(const Leap::Controller& controller){	//获取数据
    const Leap::Frame frame = controller.frame();

    //获取掌心和法向量的信息
    Leap::HandList hands = frame.hands();   //获取手对象的列表
    Leap::HandList::const_iterator hl = hands.begin(); // 获得第一只手，此处传感器范围内只能有一只手
    const Leap::Hand hand = *hl;
    
    //掌心位置
    pose2RA.position.x = hand.palmPosition().z; //坐标转换            
    pose2RA.position.y = hand.palmPosition().x;
    pose2RA.position.z = hand.palmPosition().y;

    //掌心法向量，这个地方roll/pitch/yaw对应的轴与xyz轴的关系还有疑问，是按照维基百科roll pitch yaw那夹角来给的
    pose2RA.orientation.x = hand.palmNormal().yaw();
    pose2RA.orientation.y = hand.palmNormal().roll();
    pose2RA.orientation.z = hand.palmNormal().pitch();


/****************************************************************************************************/                                
    // 获取指骨方向
    const Leap::FingerList fingers = hand.fingers();
    poses2Gloves.poses.clear();
    for (Leap::FingerList::const_iterator fl = fingers.begin(); fl != fingers.end(); ++fl) {
        const Leap::Finger finger = *fl;
        if(finger.type() == 4){
            continue;           
        }
        Leap::Bone bone;
        Leap::Bone::Type boneType;
        for(int b = 0; b < 4; b++)
        {
            if(finger.type() == 0 && b == 0) continue;//拇指只有三根骨头，https://developer-archive.leapmotion.com/documentation/cpp/api/gen-cpp/class_leap_1_1_bone.html#details
            boneType = static_cast<Leap::Bone::Type>(b);
            bone = (*fl).bone(boneType);
            p.position.x = bone.direction()[2]; //坐标转换
            p.position.y = bone.direction()[0];
            p.position.z = bone.direction()[1];
            poses2Gloves.poses.push_back(p);
        }
    }  

}