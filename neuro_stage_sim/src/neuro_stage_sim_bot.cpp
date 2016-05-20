#include "ros/ros.h"
#include "std_msgs/Bool.h"
#include "geometry_msgs/Pose.h"
#include "geometry_msgs/PoseWithCovarianceStamped.h"
#include "geometry_msgs/PoseStamped.h"

// Publisher and subscribers
ros::Publisher stage_pub;
ros::Publisher move_base_goal_pub;

// Uncomment when using real amcl localization
// ros::Publisher move_base_pose_pub;

void botCallback(const std_msgs::Bool new_round)
{
    if(new_round.data)
    {
        // Set some random points and push them into a vector of points
        geometry_msgs::Pose x_1, x_2, x_3, x_4, x_5, x_6;
        std::vector<geometry_msgs::Pose> poses;
        x_1.position.x = 2.06546545029;
        x_1.position.y = 4.28590583801;
        poses.push_back(x_1);

        x_2.position.x = 5.44342184067;
        x_2.position.y = 0.880249977112;
        poses.push_back(x_2);

        x_3.position.x = 1.84457874298;
        x_3.position.y = 6.83638143539;
        poses.push_back(x_3);

        x_4.position.x = 6.35693025589;
        x_4.position.y = 3.92254328728;
        poses.push_back(x_4);

        x_5.position.x = 7.59819555283;
        x_5.position.y = 3.7883439064;
        poses.push_back(x_5);

        x_6.position.x = 8.85293960571;
        x_6.position.y = 1.24048757553;
        poses.push_back(x_6);

        // Now randomly choose two points to use as start pose and goal pose and make sure they are different ones
        srand((unsigned int)time(NULL));
        unsigned long start = rand() % poses.size();
        unsigned long goal = rand() % poses.size();
        while (start == goal)
        {
            goal = rand() % poses.size();
        }

        // Send new position to stage
        geometry_msgs::Pose pose;
        pose.position = poses.at(start).position;
        pose.orientation.z = 1.0;
        stage_pub.publish(pose);

        // Uncomment when actually using amcl localization
        /*// Send new position to move_base
        geometry_msgs::PoseWithCovarianceStamped pose_with_co;
        pose_with_co.pose.pose.orientation.z = 1.0;
        pose_with_co.pose.pose.position = poses.at(start).position;
        pose_with_co.header.frame_id = "map";
        move_base_pose_pub.publish(pose_with_co);*/

        // Send new goal position to move_base
        geometry_msgs::PoseStamped goal_pose;
        goal_pose.pose.orientation.z = 1.0;
        goal_pose.pose.position = poses.at(goal).position;
        goal_pose.header.frame_id = "map";
        move_base_goal_pub.publish(goal_pose);
    }
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "neuro_stage_sim_bot");

    ros::NodeHandle n;

    ros::Subscriber sub = n.subscribe("/move_base/LocalPlannerWrapper/new_round", 1000, botCallback);
    stage_pub = n.advertise<geometry_msgs::Pose>("stage_ros_custom/set_pose", 1);
    move_base_goal_pub = n.advertise<geometry_msgs::PoseStamped>("move_base_simple/goal", 1);

    // Uncomment when using real amcl localization
    //move_base_pose_pub = n.advertise<geometry_msgs::PoseWithCovarianceStamped>("initialpose", 1);

    // Send an initial goal position to move_base
    geometry_msgs::PoseStamped goal_pose;
    goal_pose.pose.orientation.z = 1.0;
    goal_pose.pose.position.x = 6.35693025589;
    goal_pose.pose.position.y = 3.92254328728;
    goal_pose.header.frame_id = "map";
    move_base_goal_pub.publish(goal_pose);

    ros::spin();

    return 0;
}