#include "NachiSocket.h"

//ros
#include <ros/ros.h>
#include <ros/package.h>
#include <sensor_msgs/JointState.h>
#include <control_msgs/FollowJointTrajectoryAction.h>
#include <actionlib/server/action_server.h>
#include <actionlib/server/server_goal_handle.h>

#define PI 3.1415926

vector<double> JointPositions(6,0.0);
vector<double> JointVelocitys(6,0.0);
vector<double> JointEfforts(6,0.0);

vector<vector<double> > GoalPositions;
bool move_start = false;

void goalCB(actionlib::ServerGoalHandle<control_msgs::FollowJointTrajectoryAction> &&gh)
{
    gh.setAccepted();
    actionlib::ActionServer<control_msgs::FollowJointTrajectoryAction>::Goal goal = *gh.getGoal();
    if(goal.trajectory.joint_names.size()!=6)
    {
        gh.setAborted();
        ROS_ERROR("Received a goal with incorrect joint names size");
    }
    else
    {
        ROS_INFO("Received a Goal");
        move_start = true;
        int points_num = goal.trajectory.points.size();
        GoalPositions.clear();
        for(int i=0;i<points_num;i++)
        {
            GoalPositions.push_back(goal.trajectory.points[i].positions);
        }
        gh.setSucceeded();
    }
}

int main(int argc, char** argv)
{
    CNachiSocket m_nachiS;
    m_nachiS.init();
    ros::init(argc, argv, "nachi_driver_node");
    ros::NodeHandle nh;
    ros::Publisher pub_jointstates = nh.advertise<sensor_msgs::JointState>("joint_states", 1);
    actionlib::ActionServer<control_msgs::FollowJointTrajectoryAction> as(nh,"joint_trajectory_action",goalCB,false);
    float speed = 10.0;
    nh.getParam("/nachi_driver_node/speed", speed);
    as.start();
    vector<string> JointNames(6);
    JointNames[0] = "joint_1";
    JointNames[1] = "joint_2";
    JointNames[2] = "joint_3";
    JointNames[3] = "joint_4";
    JointNames[4] = "joint_5";
    JointNames[5] = "joint_6";

    ros::Rate loop_rate(100);
    while (ros::ok())
    {
        if(move_start == true)
        {
            m_nachiS.nachiHandHandle(0);
            move_start = false;
            m_nachiS.nachiSendMoveReady(GoalPositions.size());
            float input[6] = {0};
            for(int i=0;i<GoalPositions.size();i++)
            {
                for(int j=0;j<6;j++)
                {
                    input[j] = (float)GoalPositions[i][j]*180.0/PI;
                }
                m_nachiS.nachiSendSinglePoint(input, i+1);
            }
            m_nachiS.nachiBeginMove(speed);
            m_nachiS.nachiSendMoveReady(1);
            m_nachiS.nachiHandHandle(1);
        }
        else
        {
            m_nachiS.nachiPosState();
            JointPositions[0] = m_nachiS.m_curX*PI/180.0;
            JointPositions[1] = m_nachiS.m_curY*PI/180.0;
            JointPositions[2] = m_nachiS.m_curZ*PI/180.0;
            JointPositions[3] = m_nachiS.m_curA*PI/180.0;
            JointPositions[4] = m_nachiS.m_curB*PI/180.0;
            JointPositions[5] = m_nachiS.m_curC*PI/180.0;
        }
        sensor_msgs::JointState nachi_jointstates;
        nachi_jointstates.header.stamp = ros::Time::now();
        nachi_jointstates.name = JointNames;
        nachi_jointstates.position = JointPositions;
        nachi_jointstates.velocity = JointVelocitys;
        nachi_jointstates.effort = JointEfforts;
        pub_jointstates.publish(nachi_jointstates);
        ros::spinOnce();
        loop_rate.sleep();
    }
    ros::spin();
    return 0;
}
