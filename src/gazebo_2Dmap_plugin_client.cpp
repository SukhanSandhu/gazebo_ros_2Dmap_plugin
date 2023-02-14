#include <fstream>
#include "gazebo_2Dmap_plugin.h"

using namespace std;

ofstream fout;

void processMap(const nav_msgs::OccupancyGrid::ConstPtr& msg) {
  ROS_INFO("processMap\n");
  static bool writeMeta = true;
  int h =  msg->info.height, w =  msg->info.width;
  float r = msg->info.resolution;
  if (writeMeta) {
    
    writeMeta = false;
    fout<<r<<","<<h<<","<<w<<"\n";
  }
  
  ROS_INFO("len = %d , width = %d\n",h,w);
  for (int i=h*w-1; i>0; i--)
  {
    fout<<(int)msg->data.at(i)*(int)msg->data.at(i)<<",";
  }
  fout<<(int)msg->data.at(0)*(int)msg->data.at(0);
  ROS_INFO("doneWriting\n");

  fout<<"\n";
}


int main(int argc, char **argv)
{
  ros::init(argc,argv,"gazebo_2d_map_client");

  ros::NodeHandle n;
  ros::Subscriber sub = n.subscribe("map2d", 10, processMap);
  ros::ServiceClient client = n.serviceClient<std_srvs::Empty>("gazebo_2Dmap_plugin/generate_map");
  std_srvs::Empty srv;
  bool run=true;
  fout =ofstream("/home/uav/DROID-SLAM/outdoor_world.txt");
  while(run)
  {
  run = client.call(srv);
  ros::spinOnce();
  }
  fout.close();

  return 1;

}
