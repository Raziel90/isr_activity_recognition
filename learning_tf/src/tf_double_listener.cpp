#include <ros/ros.h>
#include <tf/transform_listener.h>
#include <cstdio>
#include <iostream>
#include <cstring>
#include <string>
#include <fstream>
using namespace std;

int main(int argc, char** argv){



  //std::string f_str = std::to_string(tracked_user);
  //std::string.tostring(tracked_user);
  ros::init(argc, argv, "tf_listener_user");


  string FRAMES[] = {"head_", "neck_", "torso_", "left_shoulder_", "right_shoulder_", 
		"left_hand_", "right_hand_", "left_elbow_", "right_elbow_", "left_hip_",
	 	"right_hip_", "left_knee_", "right_knee_", "left_foot_", "right_foot_"};


  ros::NodeHandle node("~");
  int tracked_user=1;//Default is user 1
  node.getParam("user", tracked_user);
  ROS_INFO("Node tracking user %d", tracked_user);

  string MAIN_FRAME_camera = "openni_depth_frame";
  string MAIN_FRAME = "torso_"+tracked_user;

  tf::TransformListener listener;
    
  ros::Rate rate(30.0);
       
  ofstream record_torso_file_usr;
  ofstream record_camera_file_usr;

  
  if( remove("test_torso.txt") != 0 || remove("test_camera.txt") != 0)
    perror( "Error deleting file" );
  else
    puts( "File successfully deleted" );
  char last_char;
  
  while (node.ok()){
  int contador = 0;    

    while(listener.frameExists("torso_"+boost::lexical_cast<std::string>(tracked_user))==0){
      ros::Duration(0.1).sleep();
      ROS_INFO("Waiting user %d...",tracked_user);

    }
    string file_torso="record_torso_usr"+boost::lexical_cast<std::string>(tracked_user);
    string file_camera="record_camera_usr"+boost::lexical_cast<std::string>(tracked_user);
    record_torso_file_usr.open(file_torso.append(".txt").c_str(), ios::app);
    record_camera_file_usr.open(file_camera.append(".txt").c_str(), ios::app);



    vector <string> ids;
    listener.getFrameStrings(ids);  
    
    //while(contador==0){
      //ros::Duration(0.5).sleep();
      //listener.getFrameStrings(ids);
      /*for(vector<string>::iterator i = ids.begin(); i < ids.end(); i++){
        string elem = *i;
        if (strstr(elem.c_str(),"torso_") != NULL){
          contador++;
          cout << "Number of users: " << contador <<endl;
          last_char = elem[elem.length()-1];
          
        }  
        
      }*/

    MAIN_FRAME = "torso_"+boost::lexical_cast<std::string>(tracked_user); //+ boost::lexical_cast<std::string>(int(last_char)-'0');
    cout << "MAIN_FRAME: " << MAIN_FRAME <<endl;

    //FRAMES[i] = FRAMES[i] + boost::lexical_cast<std::string>(contador);

    //ros::Duration(5).sleep();  
    tf::StampedTransform tf_head;
 	tf::StampedTransform tf_neck;
 	tf::StampedTransform tf_torso;
 	tf::StampedTransform tf_left_shoulder;
 	tf::StampedTransform tf_right_shoulder;
 	tf::StampedTransform tf_left_hand;
 	tf::StampedTransform tf_right_hand;
 	tf::StampedTransform tf_left_elbow;
 	tf::StampedTransform tf_right_elbow;
 	tf::StampedTransform tf_left_hip;
 	tf::StampedTransform tf_right_hip;
 	tf::StampedTransform tf_left_knee;
 	tf::StampedTransform tf_right_knee;
 	tf::StampedTransform tf_left_foot;
 	tf::StampedTransform tf_right_foot;

    	try{
           //ros::Duration timeout(1.0 / 30);
           //listener.waitForTransform("torso_1", "openni_depth_frame", ros::Time(0), timeout );
		// head
		// torso
        listener.lookupTransform(MAIN_FRAME, FRAMES[0]+ boost::lexical_cast<std::string>(tracked_user), ros::Time(0), tf_head);

 		double x_head = tf_head.getOrigin().x();
 		double y_head = tf_head.getOrigin().y();
		double z_head = tf_head.getOrigin().z();

		double roll_head, pitch_head, yaw_head ;
		tf_head.getBasis().getRPY(roll_head, pitch_head, yaw_head);
		tf::Quaternion q_head = tf_head.getRotation();
		
		// camera
        listener.lookupTransform(MAIN_FRAME_camera, FRAMES[0]+ boost::lexical_cast<std::string>(tracked_user), ros::Time(0), tf_head);
		
 		double x_head_camera = tf_head.getOrigin().x();
 		double y_head_camera = tf_head.getOrigin().y();
		double z_head_camera = tf_head.getOrigin().z();

		double roll_head_camera, pitch_head_camera, yaw_head_camera;
		tf_head.getBasis().getRPY(roll_head_camera, pitch_head_camera, yaw_head_camera);
		tf::Quaternion q_head_camera = tf_head.getRotation();

		//////////////////// NECK ////////////////////////// 
		// torso
        listener.lookupTransform(MAIN_FRAME, FRAMES[1]+ boost::lexical_cast<std::string>(tracked_user), ros::Time(0), tf_neck);

 		double x_neck = tf_neck.getOrigin().x();
 		double y_neck = tf_neck.getOrigin().y();
		double z_neck = tf_neck.getOrigin().z();

		double roll_neck, pitch_neck, yaw_neck ;
		tf_neck.getBasis().getRPY(roll_neck, pitch_neck, yaw_neck);
		tf::Quaternion q_neck = tf_neck.getRotation();
		// camera
        listener.lookupTransform(MAIN_FRAME_camera, FRAMES[1]+ boost::lexical_cast<std::string>(tracked_user), ros::Time(0), tf_neck);

 		double x_neck_camera = tf_neck.getOrigin().x();
 		double y_neck_camera = tf_neck.getOrigin().y();
		double z_neck_camera = tf_neck.getOrigin().z();

		double roll_neck_camera, pitch_neck_camera, yaw_neck_camera;
		tf_neck.getBasis().getRPY(roll_neck_camera, pitch_neck_camera, yaw_neck_camera);
		tf::Quaternion q_neck_camera = tf_neck.getRotation();

		////////////////// TORSO ////////////////////////////
		// torso
        listener.lookupTransform(MAIN_FRAME, FRAMES[2]+ boost::lexical_cast<std::string>(tracked_user), ros::Time(0), tf_torso);

 		double x_torso = tf_torso.getOrigin().x();
 		double y_torso = tf_torso.getOrigin().y();
		double z_torso = tf_torso.getOrigin().z();

		double roll_torso, pitch_torso, yaw_torso ;
		tf_torso.getBasis().getRPY(roll_torso, pitch_torso, yaw_torso);
		tf::Quaternion q_torso = tf_torso.getRotation();
		
		// camera
        listener.lookupTransform(MAIN_FRAME_camera, FRAMES[2]+ boost::lexical_cast<std::string>(tracked_user), ros::Time(0), tf_torso);

 		double x_torso_camera = tf_torso.getOrigin().x();
 		double y_torso_camera = tf_torso.getOrigin().y();
		double z_torso_camera = tf_torso.getOrigin().z();

		double roll_torso_camera, pitch_torso_camera, yaw_torso_camera ;
		tf_torso.getBasis().getRPY(roll_torso_camera, pitch_torso_camera, yaw_torso_camera);
		tf::Quaternion q_torso_camera = tf_torso.getRotation();

 		//////////////////// left shoulder ///////////////////////
 		// torso
        listener.lookupTransform(MAIN_FRAME, FRAMES[3]+ boost::lexical_cast<std::string>(tracked_user), ros::Time(0), tf_left_shoulder);

		double x_left_shoulder = tf_left_shoulder.getOrigin().x();
		double y_left_shoulder = tf_left_shoulder.getOrigin().y();
		double z_left_shoulder = tf_left_shoulder.getOrigin().z();

		double roll_left_shoulder, pitch_left_shoulder, yaw_left_shoulder ;
		tf_left_shoulder.getBasis().getRPY(roll_left_shoulder, pitch_left_shoulder, yaw_left_shoulder);
		tf::Quaternion q_left_shoulder = tf_left_shoulder.getRotation();
		
		// camera
        listener.lookupTransform(MAIN_FRAME_camera, FRAMES[3]+ boost::lexical_cast<std::string>(tracked_user), ros::Time(0), tf_left_shoulder);

		double x_left_shoulder_camera = tf_left_shoulder.getOrigin().x();
		double y_left_shoulder_camera = tf_left_shoulder.getOrigin().y();
		double z_left_shoulder_camera = tf_left_shoulder.getOrigin().z();

		double roll_left_shoulder_camera, pitch_left_shoulder_camera, yaw_left_shoulder_camera ;
		tf_left_shoulder.getBasis().getRPY(roll_left_shoulder_camera, pitch_left_shoulder_camera, yaw_left_shoulder_camera);
		tf::Quaternion q_left_shoulder_camera = tf_left_shoulder.getRotation();

		////////////////////////// right shoulder ///////////////////////////
		// torso
        listener.lookupTransform(MAIN_FRAME, FRAMES[4]+ boost::lexical_cast<std::string>(tracked_user), ros::Time(0), tf_right_shoulder);

 		double x_right_shoulder = tf_right_shoulder.getOrigin().x();
 		double y_right_shoulder = tf_right_shoulder.getOrigin().y();
		double z_right_shoulder = tf_right_shoulder.getOrigin().z();

		double roll_right_shoulder, pitch_right_shoulder, yaw_right_shoulder ;
		tf_right_shoulder.getBasis().getRPY(roll_right_shoulder, pitch_right_shoulder, yaw_right_shoulder);
		tf::Quaternion q_right_shoulder = tf_right_shoulder.getRotation();
		
		// camera
        listener.lookupTransform(MAIN_FRAME_camera, FRAMES[4]+ boost::lexical_cast<std::string>(tracked_user), ros::Time(0), tf_right_shoulder);

 		double x_right_shoulder_camera = tf_right_shoulder.getOrigin().x();
 		double y_right_shoulder_camera = tf_right_shoulder.getOrigin().y();
		double z_right_shoulder_camera = tf_right_shoulder.getOrigin().z();

		double roll_right_shoulder_camera, pitch_right_shoulder_camera, yaw_right_shoulder_camera ;
		tf_right_shoulder.getBasis().getRPY(roll_right_shoulder_camera, pitch_right_shoulder_camera, yaw_right_shoulder_camera);
		tf::Quaternion q_right_shoulder_camera = tf_right_shoulder.getRotation();

 		/////////////////////////// left hand /////////////////////////////////
 		// torso
        listener.lookupTransform(MAIN_FRAME, FRAMES[5]+ boost::lexical_cast<std::string>(tracked_user), ros::Time(0), tf_left_hand);

		double x_left_hand = tf_left_hand.getOrigin().x();
		double y_left_hand = tf_left_hand.getOrigin().y();
		double z_left_hand = tf_left_hand.getOrigin().z();

		double roll_left_hand, pitch_left_hand, yaw_left_hand ;
		tf_left_hand.getBasis().getRPY(roll_left_hand, pitch_left_hand, yaw_left_hand);
		tf::Quaternion q_left_hand = tf_left_hand.getRotation();
		
		// camera
        listener.lookupTransform(MAIN_FRAME_camera, FRAMES[5]+ boost::lexical_cast<std::string>(tracked_user), ros::Time(0), tf_left_hand);

		double x_left_hand_camera = tf_left_hand.getOrigin().x();
		double y_left_hand_camera = tf_left_hand.getOrigin().y();
		double z_left_hand_camera = tf_left_hand.getOrigin().z();

		double roll_left_hand_camera, pitch_left_hand_camera, yaw_left_hand_camera ;
		tf_left_hand.getBasis().getRPY(roll_left_hand_camera, pitch_left_hand_camera, yaw_left_hand_camera);
		tf::Quaternion q_left_hand_camera = tf_left_hand.getRotation();

		////////////////////////// right hand //////////////////////////
		// torso
        listener.lookupTransform(MAIN_FRAME, FRAMES[6]+ boost::lexical_cast<std::string>(tracked_user), ros::Time(0), tf_right_hand);

		double x_right_hand = tf_right_hand.getOrigin().x();
		double y_right_hand = tf_right_hand.getOrigin().y();
		double z_right_hand = tf_right_hand.getOrigin().z();

		double roll_right_hand, pitch_right_hand, yaw_right_hand ;
		tf_right_hand.getBasis().getRPY(roll_right_hand, pitch_right_hand, yaw_right_hand);
		tf::Quaternion q_right_hand = tf_right_hand.getRotation();
		
		// camera
        listener.lookupTransform(MAIN_FRAME_camera, FRAMES[6]+ boost::lexical_cast<std::string>(tracked_user), ros::Time(0), tf_right_hand);

		double x_right_hand_camera = tf_right_hand.getOrigin().x();
		double y_right_hand_camera = tf_right_hand.getOrigin().y();
		double z_right_hand_camera = tf_right_hand.getOrigin().z();

		double roll_right_hand_camera, pitch_right_hand_camera, yaw_right_hand_camera ;
		tf_right_hand.getBasis().getRPY(roll_right_hand_camera, pitch_right_hand_camera, yaw_right_hand_camera);
		tf::Quaternion q_right_hand_camera = tf_right_hand.getRotation();

		/////////////////////////// left elbow /////////////////////////
		// torso
        listener.lookupTransform(MAIN_FRAME, FRAMES[7]+ boost::lexical_cast<std::string>(tracked_user), ros::Time(0), tf_left_elbow);

 		double x_left_elbow = tf_left_elbow.getOrigin().x();
 		double y_left_elbow = tf_left_elbow.getOrigin().y();
		double z_left_elbow = tf_left_elbow.getOrigin().z();

		double roll_left_elbow, pitch_left_elbow, yaw_left_elbow ;
		tf_left_elbow.getBasis().getRPY(roll_left_elbow, pitch_left_elbow, yaw_left_elbow);
		tf::Quaternion q_left_elbow = tf_left_elbow.getRotation();
		
		// camera
        listener.lookupTransform(MAIN_FRAME_camera, FRAMES[7]+ boost::lexical_cast<std::string>(tracked_user), ros::Time(0), tf_left_elbow);

 		double x_left_elbow_camera = tf_left_elbow.getOrigin().x();
 		double y_left_elbow_camera = tf_left_elbow.getOrigin().y();
		double z_left_elbow_camera = tf_left_elbow.getOrigin().z();

		double roll_left_elbow_camera, pitch_left_elbow_camera, yaw_left_elbow_camera ;
		tf_left_elbow.getBasis().getRPY(roll_left_elbow_camera, pitch_left_elbow_camera, yaw_left_elbow_camera);
		tf::Quaternion q_left_elbow_camera = tf_left_elbow.getRotation();

 		////////////////////////// right elbow //////////////////////////////
 		// torso
        listener.lookupTransform(MAIN_FRAME, FRAMES[8]+ boost::lexical_cast<std::string>(tracked_user), ros::Time(0), tf_right_elbow);

 		double x_right_elbow = tf_right_elbow.getOrigin().x();
 		double y_right_elbow = tf_right_elbow.getOrigin().y();
		double z_right_elbow = tf_right_elbow.getOrigin().z();

		double roll_right_elbow, pitch_right_elbow, yaw_right_elbow ;
		tf_right_elbow.getBasis().getRPY(roll_right_elbow, pitch_right_elbow, yaw_right_elbow);
		tf::Quaternion q_right_elbow = tf_right_elbow.getRotation();
		
		// camera
        listener.lookupTransform(MAIN_FRAME_camera, FRAMES[8]+ boost::lexical_cast<std::string>(tracked_user), ros::Time(0), tf_right_elbow);

 		double x_right_elbow_camera = tf_right_elbow.getOrigin().x();
 		double y_right_elbow_camera = tf_right_elbow.getOrigin().y();
		double z_right_elbow_camera = tf_right_elbow.getOrigin().z();

		double roll_right_elbow_camera, pitch_right_elbow_camera, yaw_right_elbow_camera ;
		tf_right_elbow.getBasis().getRPY(roll_right_elbow_camera, pitch_right_elbow_camera, yaw_right_elbow_camera);
		tf::Quaternion q_right_elbow_camera = tf_right_elbow.getRotation();

 		/////////////////////////// left hip ///////////////////////////
 		// torso
        listener.lookupTransform(MAIN_FRAME, FRAMES[9]+ boost::lexical_cast<std::string>(tracked_user), ros::Time(0), tf_left_hip);

 		double x_left_hip = tf_left_hip.getOrigin().x();
 		double y_left_hip = tf_left_hip.getOrigin().y();
		double z_left_hip = tf_left_hip.getOrigin().z();

		double roll_left_hip, pitch_left_hip, yaw_left_hip ;
		tf_left_hip.getBasis().getRPY(roll_left_hip, pitch_left_hip, yaw_left_hip);
		tf::Quaternion q_left_hip = tf_left_hip.getRotation();
		
		// camera
        listener.lookupTransform(MAIN_FRAME_camera, FRAMES[9]+ boost::lexical_cast<std::string>(tracked_user), ros::Time(0), tf_left_hip);

 		double x_left_hip_camera = tf_left_hip.getOrigin().x();
 		double y_left_hip_camera = tf_left_hip.getOrigin().y();
		double z_left_hip_camera = tf_left_hip.getOrigin().z();

		double roll_left_hip_camera, pitch_left_hip_camera, yaw_left_hip_camera ;
		tf_left_hip.getBasis().getRPY(roll_left_hip_camera, pitch_left_hip_camera, yaw_left_hip_camera);
		tf::Quaternion q_left_hip_camera = tf_left_hip.getRotation();

 		////////////////////////// right hip //////////////////////////////
 		// torso
        listener.lookupTransform(MAIN_FRAME, FRAMES[10]+ boost::lexical_cast<std::string>(tracked_user), ros::Time(0), tf_right_hip);

 		double x_right_hip = tf_right_hip.getOrigin().x();
 		double y_right_hip = tf_right_hip.getOrigin().y();
		double z_right_hip = tf_right_hip.getOrigin().z();

		double roll_right_hip, pitch_right_hip, yaw_right_hip ;
		tf_right_hip.getBasis().getRPY(roll_right_hip, pitch_right_hip, yaw_right_hip);
		tf::Quaternion q_right_hip = tf_right_hip.getRotation();
		
		// camera
        listener.lookupTransform(MAIN_FRAME_camera, FRAMES[10]+ boost::lexical_cast<std::string>(tracked_user), ros::Time(0), tf_right_hip);

 		double x_right_hip_camera = tf_right_hip.getOrigin().x();
 		double y_right_hip_camera = tf_right_hip.getOrigin().y();
		double z_right_hip_camera = tf_right_hip.getOrigin().z();

		double roll_right_hip_camera, pitch_right_hip_camera, yaw_right_hip_camera ;
		tf_right_hip.getBasis().getRPY(roll_right_hip_camera, pitch_right_hip_camera, yaw_right_hip_camera);
		tf::Quaternion q_right_hip_camera = tf_right_hip.getRotation();

 		///////////////////////////// left knee ///////////////////////////
 		// torso
        listener.lookupTransform(MAIN_FRAME, FRAMES[11]+ boost::lexical_cast<std::string>(tracked_user), ros::Time(0), tf_left_knee);

 		double x_left_knee = tf_left_knee.getOrigin().x();
 		double y_left_knee = tf_left_knee.getOrigin().y();
		double z_left_knee = tf_left_knee.getOrigin().z();

		double roll_left_knee, pitch_left_knee, yaw_left_knee ;
		tf_left_knee.getBasis().getRPY(roll_left_knee, pitch_left_knee, yaw_left_knee);
		tf::Quaternion q_left_knee = tf_left_knee.getRotation();
		
		// camera
        listener.lookupTransform(MAIN_FRAME_camera, FRAMES[11]+ boost::lexical_cast<std::string>(tracked_user), ros::Time(0), tf_left_knee);

 		double x_left_knee_camera = tf_left_knee.getOrigin().x();
 		double y_left_knee_camera = tf_left_knee.getOrigin().y();
		double z_left_knee_camera = tf_left_knee.getOrigin().z();

		double roll_left_knee_camera, pitch_left_knee_camera, yaw_left_knee_camera ;
		tf_left_knee.getBasis().getRPY(roll_left_knee_camera, pitch_left_knee_camera, yaw_left_knee_camera);
		tf::Quaternion q_left_knee_camera = tf_left_knee.getRotation();

 		/////////////////////////// right knee ///////////////////////////////
 		// torso
        listener.lookupTransform(MAIN_FRAME, FRAMES[12]+ boost::lexical_cast<std::string>(tracked_user), ros::Time(0), tf_right_knee);

 		double x_right_knee = tf_right_knee.getOrigin().x();
 		double y_right_knee = tf_right_knee.getOrigin().y();
		double z_right_knee = tf_right_knee.getOrigin().z();

		double roll_right_knee, pitch_right_knee, yaw_right_knee ;
		tf_right_knee.getBasis().getRPY(roll_right_knee, pitch_right_knee, yaw_right_knee);
		tf::Quaternion q_right_knee = tf_right_knee.getRotation();
		
		// camera
        listener.lookupTransform(MAIN_FRAME_camera, FRAMES[12]+ boost::lexical_cast<std::string>(tracked_user), ros::Time(0), tf_right_knee);

 		double x_right_knee_camera = tf_right_knee.getOrigin().x();
 		double y_right_knee_camera = tf_right_knee.getOrigin().y();
		double z_right_knee_camera = tf_right_knee.getOrigin().z();

		double roll_right_knee_camera, pitch_right_knee_camera, yaw_right_knee_camera ;
		tf_right_knee.getBasis().getRPY(roll_right_knee_camera, pitch_right_knee_camera, yaw_right_knee_camera);
		tf::Quaternion q_right_knee_camera = tf_right_knee.getRotation();

 		////////////////////// left foot //////////////////////////
 		// torso
        listener.lookupTransform(MAIN_FRAME, FRAMES[13]+ boost::lexical_cast<std::string>(tracked_user), ros::Time(0), tf_left_foot);

 		double x_left_foot = tf_left_foot.getOrigin().x();
 		double y_left_foot = tf_left_foot.getOrigin().y();
		double z_left_foot = tf_left_foot.getOrigin().z();

		double roll_left_foot, pitch_left_foot, yaw_left_foot ;
		tf_left_foot.getBasis().getRPY(roll_left_foot, pitch_left_foot, yaw_left_foot);
		tf::Quaternion q_left_foot = tf_left_foot.getRotation();

        // camera
        listener.lookupTransform(MAIN_FRAME_camera, FRAMES[13]+ boost::lexical_cast<std::string>(tracked_user), ros::Time(0), tf_left_foot);

 		double x_left_foot_camera = tf_left_foot.getOrigin().x();
 		double y_left_foot_camera = tf_left_foot.getOrigin().y();
		double z_left_foot_camera = tf_left_foot.getOrigin().z();

		double roll_left_foot_camera, pitch_left_foot_camera, yaw_left_foot_camera ;
		tf_left_foot.getBasis().getRPY(roll_left_foot_camera, pitch_left_foot_camera, yaw_left_foot_camera);
		tf::Quaternion q_left_foot_camera = tf_left_foot.getRotation();

 		///////////////////////////// right foot ////////////////////////
 		// torso
        listener.lookupTransform(MAIN_FRAME, FRAMES[14]+ boost::lexical_cast<std::string>(tracked_user), ros::Time(0), tf_right_foot);

 		double x_right_foot = tf_right_foot.getOrigin().x();
 		double y_right_foot = tf_right_foot.getOrigin().y();
		double z_right_foot = tf_right_foot.getOrigin().z();

		double roll_right_foot, pitch_right_foot, yaw_right_foot ;
		tf_right_foot.getBasis().getRPY(roll_right_foot, pitch_right_foot, yaw_right_foot);
		tf::Quaternion q_right_foot = tf_right_foot.getRotation();
		
		// camera
        listener.lookupTransform(MAIN_FRAME_camera, FRAMES[14]+ boost::lexical_cast<std::string>(tracked_user), ros::Time(0), tf_right_foot);

 		double x_right_foot_camera = tf_right_foot.getOrigin().x();
 		double y_right_foot_camera = tf_right_foot.getOrigin().y();
		double z_right_foot_camera = tf_right_foot.getOrigin().z();

		double roll_right_foot_camera, pitch_right_foot_camera, yaw_right_foot_camera ;
		tf_right_foot.getBasis().getRPY(roll_right_foot_camera, pitch_right_foot_camera, yaw_right_foot_camera);
		tf::Quaternion q_right_foot_camera = tf_right_foot.getRotation();
		// End of body transforms

        // file em relacao ao torso 
            if (record_torso_file_usr.is_open()){
                record_torso_file_usr << x_head<<" "<<y_head<<" "<<z_head<<" "<<roll_head<<" "<<pitch_head<<" "<<yaw_head<<" ";
                record_torso_file_usr << x_neck<<" "<<y_neck<<" "<<z_neck<<" "<<roll_neck<<" "<<pitch_neck<<" "<<yaw_neck<<" ";
                record_torso_file_usr << x_torso<<" "<<y_torso<<" "<<z_torso<<" "<<roll_torso<<" "<<pitch_torso<<" "<<yaw_torso<<" ";
                record_torso_file_usr << x_left_shoulder<<" "<<y_left_shoulder<<" "<<z_left_shoulder<<" "<<roll_left_shoulder<<" " <<pitch_left_shoulder<<" "<<yaw_left_shoulder<<" ";
                record_torso_file_usr << x_left_elbow<<" "<<y_left_elbow<<" "<<z_left_elbow<<" "<<roll_left_elbow<<" "<<pitch_left_elbow<<" " <<yaw_left_elbow<<" ";
                record_torso_file_usr << x_right_shoulder<<" "<<y_right_shoulder<<" "<<z_right_shoulder<<" "<<roll_right_shoulder<<" " <<pitch_right_shoulder<<" "<<yaw_right_shoulder<<" ";
                record_torso_file_usr << x_right_elbow<<" "<<y_right_elbow<<" "<<z_right_elbow<<" "<<roll_right_elbow<<" "<<pitch_right_elbow<<" " <<yaw_right_elbow<<" ";
                record_torso_file_usr << x_left_hip<<" "<<y_left_hip<<" "<<z_left_hip<<" "<<roll_left_hip<<" "<<pitch_left_hip<<" " <<yaw_left_hip<<" ";
                record_torso_file_usr << x_left_knee<<" "<<y_left_knee<<" "<<z_left_knee<<" "<<roll_left_knee<<" "<<pitch_left_knee<<" " <<yaw_left_knee<<" ";
                record_torso_file_usr << x_right_hip<<" "<<y_right_hip<<" "<<z_right_hip<<" "<<roll_right_hip<<" "<<pitch_right_hip<<" " <<yaw_right_hip<<" ";
                record_torso_file_usr << x_right_knee<<" "<<y_right_knee<<" "<<z_right_knee<<" "<<roll_right_knee<<" "<<pitch_right_knee<<" " <<yaw_right_knee<<" ";
                record_torso_file_usr << x_left_hand<<" "<<y_left_hand<<" "<<z_left_hand<<" "<<roll_left_hand<<" "<<pitch_left_hand<<" " <<yaw_left_hand<<" ";
                record_torso_file_usr << x_right_hand<<" "<<y_right_hand<<" "<<z_right_hand<<" "<<roll_right_hand<<" "<<pitch_right_hand<<" " <<yaw_right_hand<<" ";
                record_torso_file_usr << x_left_foot<<" "<<y_left_foot<<" "<<z_left_foot<<" "<<roll_left_foot<<" "<<pitch_left_foot<<" " <<yaw_left_foot<<" ";
                record_torso_file_usr << x_right_foot<<" "<<y_right_foot<<" "<<z_right_foot<<" "<<roll_right_foot<<" "<<pitch_right_foot<<" "<<yaw_right_foot<< endl;
   		} 
   		
   		// file em relacao a camera 
            if (record_camera_file_usr.is_open()){
                record_camera_file_usr << x_head_camera<<" "<<y_head_camera<<" "<<z_head_camera<<" "<<roll_head_camera<<" "<<pitch_head_camera<<" "<<yaw_head_camera<<" ";
                record_camera_file_usr << x_neck_camera<<" "<<y_neck_camera<<" "<<z_neck_camera<<" "<<roll_neck_camera<<" "<<pitch_neck_camera<<" "<<yaw_neck_camera<<" ";
                record_camera_file_usr << x_torso_camera<<" "<<y_torso_camera<<" "<<z_torso_camera<<" "<<roll_torso_camera<<" "<<pitch_torso_camera<<" "<<yaw_torso_camera<<" ";
                record_camera_file_usr << x_left_shoulder_camera<<" "<<y_left_shoulder_camera<<" "<<z_left_shoulder_camera<<" "<<roll_left_shoulder_camera<<" " <<pitch_left_shoulder_camera<<" "<<yaw_left_shoulder_camera<<" ";
                record_camera_file_usr << x_left_elbow_camera<<" "<<y_left_elbow_camera<<" "<<z_left_elbow_camera<<" "<<roll_left_elbow_camera<<" "<<pitch_left_elbow_camera<<" " <<yaw_left_elbow_camera<<" ";
                record_camera_file_usr << x_right_shoulder_camera<<" "<<y_right_shoulder_camera<<" "<<z_right_shoulder_camera<<" "<<roll_right_shoulder_camera<<" " <<pitch_right_shoulder_camera<<" "<<yaw_right_shoulder_camera<<" ";
                record_camera_file_usr << x_right_elbow_camera<<" "<<y_right_elbow_camera<<" "<<z_right_elbow_camera<<" "<<roll_right_elbow_camera<<" "<<pitch_right_elbow_camera<<" " <<yaw_right_elbow_camera<<" ";
                record_camera_file_usr << x_left_hip_camera<<" "<<y_left_hip_camera<<" "<<z_left_hip_camera<<" "<<roll_left_hip_camera<<" "<<pitch_left_hip_camera<<" " <<yaw_left_hip_camera<<" ";
                record_camera_file_usr << x_left_knee_camera<<" "<<y_left_knee_camera<<" "<<z_left_knee_camera<<" "<<roll_left_knee_camera<<" "<<pitch_left_knee_camera<<" " <<yaw_left_knee_camera<<" ";
                record_camera_file_usr << x_right_hip_camera<<" "<<y_right_hip_camera<<" "<<z_right_hip_camera<<" "<<roll_right_hip_camera<<" "<<pitch_right_hip_camera<<" " <<yaw_right_hip_camera<<" ";
                record_camera_file_usr << x_right_knee_camera<<" "<<y_right_knee_camera<<" "<<z_right_knee_camera<<" "<<roll_right_knee_camera<<" "<<pitch_right_knee_camera<<" " <<yaw_right_knee_camera<<" ";
                record_camera_file_usr << x_left_hand_camera<<" "<<y_left_hand_camera<<" "<<z_left_hand_camera<<" "<<roll_left_hand_camera<<" "<<pitch_left_hand_camera<<" " <<yaw_left_hand_camera<<" ";
                record_camera_file_usr << x_right_hand_camera<<" "<<y_right_hand_camera<<" "<<z_right_hand_camera<<" "<<roll_right_hand_camera<<" "<<pitch_right_hand_camera<<" " <<yaw_right_hand_camera<<" ";
                record_camera_file_usr << x_left_foot_camera<<" "<<y_left_foot_camera<<" "<<z_left_foot_camera<<" "<<roll_left_foot_camera<<" "<<pitch_left_foot_camera<<" " <<yaw_left_foot_camera<<" ";
                record_camera_file_usr << x_right_foot_camera<<" "<<y_right_foot_camera<<" "<<z_right_foot_camera<<" "<<roll_right_foot_camera<<" "<<pitch_right_foot_camera<<" "<<yaw_right_foot_camera<< endl;
   		} 

    }
    catch (tf::TransformException &ex) {
      ROS_ERROR("%s",ex.what());
      //ros::Duration(1.0).sleep();
      cout << "Failure at "<< ros::Time::now() << endl;
      cout << "Exception thrown:" << ex.what()<< endl;
      cout << "The current list of frames is:" << endl;
      cout << listener.allFramesAsString()<< endl;
      
    }

    record_torso_file_usr.close();
    record_camera_file_usr.close();
    rate.sleep();

  }
  //record_torso_file_usr1.close();
  return 0;
};
