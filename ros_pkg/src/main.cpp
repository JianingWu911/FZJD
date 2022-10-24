#include "ros/ros.h"
#include "Leapros.h"
#include "Leap.h"
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/PoseArray.h>
#include <fstream>

int main(int argc, char **argv){

    // 输出数据留作测试
    std::ofstream palmOutput("./palmOutput.txt"); //直接新建文件，并与palmOutput关联
    std::ofstream fingerOutput("./fingerOutput.txt");
    if (palmOutput.is_open()) {
        std::cout << "能正常打开palmOutput文件\n";
    }
    if (fingerOutput.is_open()) {
        std::cout << "能正常打开fingerOutput文件\n";
    }

    //节点初始化
    ros::init(argc, argv, "hand_pose_publisher");//定义节点hand_pose_publisher
    ros::NodeHandle n;
    ros::Publisher toAR;//设置掌心位置
    ros::Publisher toGloves;//设置指骨方向
    toAR = n.advertise<geometry_msgs::Pose>("palm_Position",1);//设置topic发布名为palm_Position(这个名字表现在可视化工具里面)    
    toGloves = n.advertise<geometry_msgs::PoseArray>("finger_bones_directon",1);//设置topic为finger_bones_direction 

    //创建对象
    RosListener listener;  //创建监听对象
    Leap::Controller controller;//创建对象
    controller.addListener(listener);//Listener对象注册到Controller对象
    geometry_msgs::Pose palmPositionP;//手掌中心数据类型
    geometry_msgs::PoseArray fingerBonesDirectionsP;//指骨方向数据类型

    //开始发布
    std::cout <<"start publish "<<std::endl;   
    ros::Rate loop_rate(10); //设置发布频率10HZ
    int k = 0;
    while (ros::ok())
    {
        ++k;
        palmPositionP = listener.pose2RA;
        fingerBonesDirectionsP = listener.poses2Gloves;
        if (palmPositionP.position.x == 0 && palmPositionP.position.y == 0 && palmPositionP.position.z == 0) continue;
        else {
            if (k % 10 == 0) {
                k = 0;
                toAR.publish(palmPositionP);//掌心位姿
                std::cout << "掌心位姿" << std::endl; // 发布日志
                std::cout << palmPositionP << std::endl;
                palmOutput << palmPositionP << std::endl;
            }
            toGloves.publish(fingerBonesDirectionsP);//指骨方向
            std::cout << "指骨方向" << std::endl;
            int i = 0, j = 0;
            for (auto f : fingerBonesDirectionsP.poses) {
            std::cout << "#" << ++i << std::endl;
            fingerOutput << "#" << ++j << std::endl;
            std::cout << f.position << std::endl;
            fingerOutput << f.position << std::endl;
            }
        }
        ros::spinOnce();//回调函数
        loop_rate.sleep();
    }

    //结束
    std::cout <<"Stop ..."<<std::endl;
    controller.removeListener(listener);
    palmOutput.close();
    fingerOutput.close();
    std::cout << "已经写入文件"<< std::endl;
    return 0;
}
