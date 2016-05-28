#ifndef LOCAL_PLANNER_WRAPPER_LOCAL_PLANNER_WRAPPER_H_
#define LOCAL_PLANNER_WRAPPER_LOCAL_PLANNER_WRAPPER_H_

#include <tf/transform_listener.h>
#include <angles/angles.h>
#include <nav_msgs/Odometry.h>
#include <costmap_2d/costmap_2d_ros.h>
#include <nav_core/base_local_planner.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/Twist.h>
#include <nav_msgs/Path.h>
#include <std_msgs/Bool.h>
#include <pluginlib/class_loader.h>

#include <nav_msgs/OccupancyGrid.h>
#include <map_msgs/OccupancyGridUpdate.h>
#include <sensor_msgs/LaserScan.h>

#include <visualization_msgs/MarkerArray.h> // to_delete

#include <tf/tf.h>

// We use namespaces to keep things seperate under all the planners
namespace local_planner_wrapper
{
    class LocalPlannerWrapper : public nav_core::BaseLocalPlanner
    {
        public:

            // Constructor
            // --> Part of interface
            LocalPlannerWrapper();

            // Desctructor
            // --> Part of interface
            ~LocalPlannerWrapper();

            // Initialize the planner
            // --> Part of interface
            // name:                some string, not important
            // tf:                  this will tell the planner the robots location (i think)
            // costmap_ros:         the costmap
            // Return:              nothing
            void initialize(std::string name, tf::TransformListener* tf, costmap_2d::Costmap2DROS* costmap_ros);

            // Sets the plan
            // --> Part of interface
            // orig_global_plan:    this is the global plan we're supposed to follow (a vector of positions forms the
            //                      line)
            // Return:              True if plan was succesfully received...
            bool setPlan(const std::vector<geometry_msgs::PoseStamped>& orig_global_plan);

            // Compute the velocity commands
            // --> Part of interface
            // cmd_vel:             fill this vector with our velocity commands (the actual output we're producing)
            // Return:              True if we didn't fail
            bool computeVelocityCommands(geometry_msgs::Twist& cmd_vel);

            // Tell if goal was reached
            // --> Part of interface
            // Return:              True if goal pose was reached
            bool isGoalReached();

        private:

            // Get index for costmap update
            // x:
            // y:
            // Return:
            int getIndex(int x, int y);

            // Callback function for the subscriber to the local costmap update
            // costmap_update:      this is the costmap message
            // Return:              nothing
            void updateCostmap(map_msgs::OccupancyGridUpdate costmap_update);

            // Callback function for the subscriber to the local costmap
            // costmap:             this is the costmap message
            // Return:              nothing
            void filterCostmap(nav_msgs::OccupancyGrid costmap);

            // Callback function for the subscriber to laser scan
            // laser_scan:          this is the laser scan message
            // Return:              nothing
            void getLaserScanPoints(sensor_msgs::LaserScan laser_scan);

            // Listener to get our pose on the map
            tf::TransformListener* tf_;

            // For visualisation, publishers of global and local plan
            ros::Publisher g_plan_pub_, l_plan_pub_;

            // Visualize the update costmap
            ros::Publisher updated_costmap_pub_;

            // Subscribe to the costmap
            ros::Subscriber costmap_sub_;


            // Publisher to the stage_sim_bot
            ros::Publisher state_pub_;


            // Subscribe to the costmap update
            ros::Subscriber costmap_update_sub_;


            // Subscribe to laser scan topic
            ros::Subscriber laser_scan_sub_;

            // Publisher for customized costmap
            ros::Publisher customized_costmap_pub_;

            ros::Publisher marker_pub_; // to_delete


            // Our costmap ros interface
            costmap_2d::Costmap2DROS* costmap_ros_;

            // Our actual costmap
            costmap_2d::Costmap2D* costmap_;

            // The updated costmap
            nav_msgs::OccupancyGrid filtereded_costmap_;

            // Customized costmap as state representation of the robot base
            nav_msgs::OccupancyGrid customized_costmap_;

            // Customized costmap
            //costmap_2d::Costmap2D customized_costmap_2d_; // to_delete

            // Our current pose
            tf::Stamped<tf::Pose> current_pose_;

            // Our goal pose
            geometry_msgs::Pose goal_;


            // The current global plan in normal and costmap coordinates
            std::vector<geometry_msgs::PoseStamped> global_plan_;

            // Should we use the dwa planner to gather samples?
            // Then we need all of these variables...
            bool existing_plugin_;
            pluginlib::ClassLoader<nav_core::BaseLocalPlanner> blp_loader_;
            boost::shared_ptr<nav_core::BaseLocalPlanner> tc_;

            // Are we initialized?
            bool initialized_;
    };
};
#endif
