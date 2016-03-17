#base='/media/claudio/INTENSO/isr_social_behaviour'

#/home/claudio/catkin_ws/src/isr_activity_recognition


#cd $base

for action in {1..8};do
echo act_0$action
#cd "/media/claudio/INTENSO/isr_social_behaviour/act_"$(printf "%02d" $action)
	for session in {1..10};do
		cd "/media/claudio/INTENSO/isr_social_behaviour_dataset/act_"$(printf "%02d" $action)"/session_"$(printf "%02d" $session) || (rosnode kill /tf_listener && exit $?)

		for bagfile in $(ls | grep .bag);do
			rosrun learning_tf tf_user_listener _user:=1 __name:=tf_tracker_user_1 &
			rosrun learning_tf tf_user_listener _user:=2 __name:=tf_tracker_user_2 &
			echo "\nNext File:" $bagfile "\n"			
			rosbag play $bagfile						
			rosnode kill /tf_tracker_user_1 && rosnode kill tf_tracker_user_2
			for user in {1..2};do
				mv ./record_torso_usr$user.txt ~/LCAS/isr_social_behaviour_dataset/act_$(printf "%02d" $action)"/session_"$(printf "%02d" $session)"/"$(echo $bagfile | awk -F'.' '{print $1}')"_torso_usr"$user".txt"
				mv ./record_camera_usr$user.txt ~/LCAS/isr_social_behaviour_dataset/act_$(printf "%02d" $action)"/session_"$(printf "%02d" $session)"/"$(echo $bagfile | awk -F'.' '{print $1}')"_camera_usr"$user".txt"
			done
		done
	cd .. 
	done

done

