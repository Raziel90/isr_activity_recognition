#!/usr/bin/env python  
import roslib
import rospy
import math
import tf
import geometry_msgs.msg
import numpy as np
import hcluster as mat
import pickle
import matplotlib.pyplot as plt
from sklearn import *
from scipy import ndimage, signal
import os.path
import time
from itertools import islice
import scipy.io as sio

# maximo e minimo das features de cada classe
max1= 0.22880338781876886 #1.4044994443754881  
min1= 0.036464403856246788 #-0.23638075121810423 
max2=  0.24128424184237723 #0.38884491081509731  
min2= 0.15672290621340512 #-0.11036939854711682  
max3=   0.19011263617768098 #1.9052726740152675  
min3= 0.056304114577363402  #-0.68748360554052268
max4=   0.27603304074611379 #1.4936609455445204  
min4= 0.15137403268663852 #-0.59018793340914055  
max5=   0.22307383818018914 #2.121425745276468   
min5= 0.068145957893900114 #-0.56362946630115474 
max6=  0.24705887874481483  #0.36081714863970898 
min6=0.063620456237165154 #-0.10913375515658537  
max7=    0.21876603113032589 #7.710816948667448  
min7=  0.018684949932898477 #-1.6530562541112834 
max8=   0.21642529501711671 #3.7482681637373534  
min8=  0.04328214815219332  #-1.0556700798150287
min_total = -1.6530562541112834
max_total = 7.710816948667448 

conta_class = np.zeros((8))
actividades =['andar', 'em pe', 'PC', 'telemovel','correr','pular','cair','sentando']

################################ funcao das features distancias convulucao

def feature_distancias_convolucao(entrada):

    frame_rate=1.0/30.0    # tempo decorrido entre cada frame em segundos
    window = 10.0        # janela de frames para aspectos temporais

    x = entrada[:,0::6]
    y = entrada[:,1::6]
    z = entrada[:,2::6]
    [m, n] = np.shape(entrada)

    distancias_convolucao=np.array([]);


    a = np.zeros((15,3));

    for frame in range(0,m):
    
        if frame == 0:
            anterior = frame;
        else:
            anterior = frame-1;
    
        actual = frame;
    
    
        for i in range(0,15):
            dx = x[actual,i]-x[anterior,i];
            dy = y[actual,i]-y[anterior,i];
            dz = z[actual,i]-z[anterior,i];
            a[i,:]=np.c_[dx, dy, dz];        
    
        c = signal.convolve(a,a[::-1,::-1])
    
        feat = np.reshape(c.T,(1,np.size(c)));
        distancias_convolucao = np.r_[distancias_convolucao, feat] if distancias_convolucao.size else feat
    return distancias_convolucao

#apply covariance on a matrix M and returns the elements of the upper triangular  in a row vector

def get_triu_cov(M): 

    covM  = np.cov( M ); 
  
    d = len(covM);
    n = d*(d+1)/2.0;
    V = np.zeros((n,1)); 

    true_mat = np.ones((d,d),dtype=np.int);
    in_triu  = np.triu(true_mat);

    V = covM[np.where(in_triu)].T
    return V 

################################ funcao que normaliza as features##############################  
def apply_norm(Mtr, Mte, lb_tr, lb_te):
         
    ANorm_tr = np.array([])
    ANorm_te = np.array([])
    A = np.array([])
    B = np.array([])
    l = np.unique(lb_tr)
       
    # Separate the different classes from the training or test set for
    # normalization per class
    for i in range(0,len(l)):
        #training norm
        A=np.array([])
        A = Mtr[lb_tr == l[i],:]
        maxval = A.max()
        minval = A.min()
        nMtr =  ( A - minval) / (maxval - minval) 
        ANorm_tr = np.concatenate((ANorm_tr, nMtr))
           
        #test norm           
        B=np.array([])
        B = Mte[lb_te == l[i], :]
        nMte = ( B - minval) / (maxval - minval) 
        ANorm_te = np.concatenate((ANorm_te, nMte))             
            
    return ANorm_tr, ANorm_te 

######################### Feature extraction ##########################################
def feature_extraction(entrada):

    frame_rate=1.0/30.0    # tempo decorrido entre cada frame em segundos
    window = 10.0        # janela de frames para aspectos temporais

    x = entrada[:,0::6]
    y = entrada[:,1::6]
    z = entrada[:,2::6]
    [m, n] = np.shape(entrada)

    ## distancias

    distancias = np.zeros((14,14))
    distancias_total=np.array([])

    for frame in range(0,m):
        for i in range(0,14):
            for j in range(0,14):
                distancias[i,j]= mat.pdist([[x[frame,i], y[frame,i], z[frame,i]], [x[frame,j], y[frame,j], z[frame,j]]])
       
        distlower = np.tril(distancias)
        distupper = np.triu(distancias)
        distancias_final = distlower[1:, :] + distupper[0:-1,:] # eleminacao da diagonal de zeros
        
        #cov_distancias = np.cov(distancias_final)
        #print np.shape(cov_distancias)
        #cov_distancias_final = np.triu(cov_distancias)
        #aux = np.reshape(cov_distancias_final.T,1,15*15)
        #aux[aux==0]=[]
        #distancias_total=concatenate((distancias_total, aux))
        aux = np.array([get_triu_cov(distancias_final.T)])
        distancias_total = np.concatenate([distancias_total, aux]) if distancias_total.size else aux
        #print np.shape(distancias_total) 
    # velocidades absolutas

    velocidades=np.zeros((np.floor(m/window),14))
    velocidades_total = np.array([])

    for frame in range(0,int(np.floor(m/window))):
            
        actual = frame*window
        anterior = frame*(window-9)

        for i in range(0,14): 
            velocidades[frame,i]= mat.pdist([[x[actual,i], y[actual,i], z[actual,i]], [x[anterior,i], y[anterior,i], z[anterior,i]]])/(frame_rate*window)

        if frame==int(np.floor(m/window)):
            velocidades_total = np.concatenate((velocidades_total, np.tile(velocidades[frame,:],(m-(window*frame)+window,1))))
        else:
            velocidades_total = np.concatenate((velocidades_total, np.tile(velocidades[frame,:],(window,1)))) if velocidades_total.size else np.tile(velocidades[frame,:],(window,1))
 
    # velocidades e direcoes relativamente a cada eixo

    vx = np.zeros((np.floor(m/window),14))
    vy = np.zeros((np.floor(m/window),14))
    vz = np.zeros((np.floor(m/window),14))
    dx = np.zeros((np.floor(m/window),14))
    dy = np.zeros((np.floor(m/window),14))
    dz = np.zeros((np.floor(m/window),14))
    direcao_xyz = np.array([])
    velocidade_xyz = np.array([])

    for frame in range(0,int(np.floor(m/window))):
            
        actual = frame*window
        anterior = frame*window-9
        
        for i in range(0,14):
            dx[frame,i] = x[actual,i]-x[anterior,i]
            dy[frame,i] = y[actual,i]-y[anterior,i]
            dz[frame,i] = z[actual,i]-z[anterior,i]
            vx[frame,i] = dx[frame,i]/(frame_rate*window)
            vy[frame,i] = dy[frame,i]/(frame_rate*window)
            vz[frame,i] = dz[frame,i]/(frame_rate*window)

        if frame==np.floor(m/window):
            aux_v = np.c_[np.tile(vx[frame,:],((m-(window*frame)+window,1))), np.tile(vy[frame,:],((m-(window*frame)+window,1))), np.tile(vz[frame,:],((m-(window*frame)+window,1)))]
            
            velocidade_xyz = np.concatenate([velocidade_xyz, aux_v]) if velocidade_xyz.size else aux_v
            
            aux_d = np.array([np.c_[np.tile(dx[frame,:],(m-(window*frame)+window,1)), np.tile(dy[frame,:],(m-(window*frame)+window,1)), np.tile(dz[frame,:],((m-(window*frame)+window,1)))]])
            
            direcao_xyz = np.concatenate([direcao_xyz, aux_d]) if direcao_xyz.size else aux_d
        else:
            aux_v = np.c_[np.tile(vx[frame,:],(window,1)), np.tile(vy[frame,:],(window,1)), np.tile(vz[frame,:],(window,1))]
            
            velocidade_xyz = np.concatenate([velocidade_xyz, aux_v]) if velocidade_xyz.size else aux_v
            
            aux_d = np.c_[np.tile(dx[frame,:],(window,1)), np.tile(dy[frame,:],(window,1)), np.tile(dz[frame,:],(window,1))]
            
            direcao_xyz = np.concatenate([direcao_xyz, aux_d]) if direcao_xyz.size else aux_d
            
                        
    return [distancias_total, velocidades_total, velocidade_xyz, direcao_xyz]             
        
########################### FIM DAS FUNCOES ###########################################

rospy.init_node('tf_listener_rospy')

listener = tf.TransformListener()

MAIN_FRAME = 'torso_'
FRAMES = [
        'head_',
        'neck_',
        'torso_',
        'left_shoulder_',
        'left_elbow_',
        'left_hand_',
        'left_hip_',
        'left_knee_',
        'left_foot_',
        'right_shoulder_',
        'right_elbow_',
        'right_hand_',
        'right_hip_',
        'right_knee_',
        'right_foot_',
        ]
rate = rospy.Rate(30.0)
dados = np.array([])
M = np.array([])
segundos = 0

while not rospy.is_shutdown():
    counter = 0
    frame_list = listener.getFrameStrings()    # lista de frames da tf a cada instante

    for frame in frame_list:            # conta o numero de users detetados pelo openni_tracker 
        if MAIN_FRAME in frame:
            counter+=1    

    for i in range(1,counter+1):        # para cada user guardar as coordenadas de cada junta em relacao ao torso
        f = open('test_py_'+str(i)+'.txt', 'a+')
        try:            
            # head
            (tf_head_trans, tf_head_rot) = listener.lookupTransform(MAIN_FRAME+str(i), FRAMES[0]+str(i), rospy.Time(0))
        
            x_head = tf_head_trans[0]
            y_head = tf_head_trans[1]
            z_head = tf_head_trans[2]
            roll_head = tf_head_rot[0]
            pitch_head = tf_head_rot[1]
            yaw_head = tf_head_rot[2]

            # neck
            (tf_neck_trans, tf_neck_rot) = listener.lookupTransform(MAIN_FRAME+str(i), FRAMES[1]+str(i), rospy.Time(0))
            
            x_neck = tf_neck_trans[0]
            y_neck = tf_neck_trans[1]
            z_neck = tf_neck_trans[2]
            roll_neck = tf_neck_rot[0]
            pitch_neck = tf_neck_rot[1]
            yaw_neck = tf_neck_rot[2]

            # torso
            (tf_torso_trans, tf_torso_rot) = listener.lookupTransform(MAIN_FRAME+str(i), FRAMES[2]+str(i), rospy.Time(0))
            
            x_torso = tf_torso_trans[0]
            y_torso = tf_torso_trans[1]
            z_torso = tf_torso_trans[2]
            roll_torso = tf_torso_rot[0]
            pitch_torso = tf_torso_rot[1]
            yaw_torso = tf_torso_rot[2]

            # left shoulder
            
            (tf_left_shoulder_trans, tf_left_shoulder_rot) = listener.lookupTransform(MAIN_FRAME+str(i), FRAMES[3]+str(i), rospy.Time(0))
            
            x_left_shoulder = tf_left_shoulder_trans[0]
            y_left_shoulder = tf_left_shoulder_trans[1]
            z_left_shoulder = tf_left_shoulder_trans[2]
            roll_left_shoulder = tf_left_shoulder_rot[0]
            pitch_left_shoulder = tf_left_shoulder_rot[1]
            yaw_left_shoulder = tf_left_shoulder_rot[2]

            # right shoulder
            (tf_right_shoulder_trans, tf_right_shoulder_rot) = listener.lookupTransform(MAIN_FRAME+str(i), FRAMES[4]+str(i), rospy.Time(0))

            x_right_shoulder = tf_right_shoulder_trans[0]
            y_right_shoulder = tf_right_shoulder_trans[1]
            z_right_shoulder = tf_right_shoulder_trans[2]
            roll_right_shoulder = tf_right_shoulder_rot[0]
            pitch_right_shoulder = tf_right_shoulder_rot[1]
            yaw_right_shoulder = tf_right_shoulder_rot[2]

            # left hand
            (tf_left_hand_trans, tf_left_hand_rot) = listener.lookupTransform(MAIN_FRAME+str(i), FRAMES[5]+str(i), rospy.Time(0))

            x_left_hand = tf_left_hand_trans[0]
            y_left_hand = tf_left_hand_trans[1]
            z_left_hand = tf_left_hand_trans[2]
            roll_left_hand = tf_left_hand_rot[0]
            pitch_left_hand = tf_left_hand_rot[1]
            yaw_left_hand = tf_left_hand_rot[2]

            # right hand
            (tf_right_hand_trans, tf_right_hand_rot) = listener.lookupTransform(MAIN_FRAME+str(i), FRAMES[6]+str(i), rospy.Time(0))

            x_right_hand = tf_right_hand_trans[0]
            y_right_hand = tf_right_hand_trans[1]
            z_right_hand = tf_right_hand_trans[2]
            roll_right_hand = tf_right_hand_rot[0]
            pitch_right_hand = tf_right_hand_rot[1]
            yaw_right_hand = tf_right_hand_rot[2]

            # left elbow
            (tf_left_elbow_trans, tf_left_elbow_rot) = listener.lookupTransform(MAIN_FRAME+str(i), FRAMES[7]+str(i), rospy.Time(0))

            x_left_elbow = tf_left_elbow_trans[0]
            y_left_elbow = tf_left_elbow_trans[1]
            z_left_elbow = tf_left_elbow_trans[2]
            roll_left_elbow = tf_left_elbow_rot[0]
            pitch_left_elbow = tf_left_elbow_rot[1]
            yaw_left_elbow = tf_left_elbow_rot[2]

            # right elbow
            (tf_right_elbow_trans, tf_right_elbow_rot) = listener.lookupTransform(MAIN_FRAME+str(i), FRAMES[8]+str(i), rospy.Time(0))

            x_right_elbow = tf_right_elbow_trans[0]
            y_right_elbow = tf_right_elbow_trans[1]
            z_right_elbow = tf_right_elbow_trans[2]
            roll_right_elbow = tf_right_elbow_rot[0]
            pitch_right_elbow = tf_right_elbow_rot[1]
            yaw_right_elbow = tf_right_elbow_rot[2]

            # left hip
            (tf_left_hip_trans, tf_left_hip_rot) = listener.lookupTransform(MAIN_FRAME+str(i), FRAMES[9]+str(i), rospy.Time(0))

            x_left_hip = tf_left_hip_trans[0]
            y_left_hip = tf_left_hip_trans[1]
            z_left_hip = tf_left_hip_trans[2]
            roll_left_hip = tf_left_hip_rot[0]
            pitch_left_hip = tf_left_hip_rot[1]
            yaw_left_hip = tf_left_hip_rot[2]

            # right hip
            (tf_right_hip_trans, tf_right_hip_rot) = listener.lookupTransform(MAIN_FRAME+str(i), FRAMES[10]+str(i), rospy.Time(0))

            x_right_hip = tf_right_hip_trans[0]
            y_right_hip = tf_right_hip_trans[1]
            z_right_hip = tf_right_hip_trans[2]
            roll_right_hip = tf_right_hip_rot[0]
            pitch_right_hip = tf_right_hip_rot[1]
            yaw_right_hip = tf_right_hip_rot[2]

            # left knee
            (tf_left_knee_trans, tf_left_knee_rot) = listener.lookupTransform(MAIN_FRAME+str(i), FRAMES[11]+str(i), rospy.Time(0))

            x_left_knee = tf_left_knee_trans[0]
            y_left_knee = tf_left_knee_trans[1]
            z_left_knee = tf_left_knee_trans[2]
            roll_left_knee = tf_left_knee_rot[0]
            pitch_left_knee = tf_left_knee_rot[1]
            yaw_left_knee = tf_left_knee_rot[2]

            # right knee
            (tf_right_knee_trans, tf_right_knee_rot) = listener.lookupTransform(MAIN_FRAME+str(i), FRAMES[12]+str(i), rospy.Time(0))

            x_right_knee = tf_right_knee_trans[0]
            y_right_knee = tf_right_knee_trans[1]
            z_right_knee = tf_right_knee_trans[2]
            roll_right_knee = tf_right_knee_rot[0]
            pitch_right_knee = tf_right_knee_rot[1]
            yaw_right_knee = tf_right_knee_rot[2]

            # left foot
            (tf_left_foot_trans, tf_left_foot_rot) = listener.lookupTransform(MAIN_FRAME+str(i), FRAMES[13]+str(i), rospy.Time(0))

            x_left_foot = tf_left_foot_trans[0]
            y_left_foot = tf_left_foot_trans[1]
            z_left_foot = tf_left_foot_trans[2]
            roll_left_foot = tf_left_foot_rot[0]
            pitch_left_foot = tf_left_foot_rot[1]
            yaw_left_foot = tf_left_foot_rot[2]

            # right foot
            (tf_right_foot_trans, tf_right_foot_rot) = listener.lookupTransform(MAIN_FRAME+str(i), FRAMES[14]+str(i), rospy.Time(0))

            x_right_foot = tf_right_foot_trans[0]
            y_right_foot = tf_right_foot_trans[1]
            z_right_foot = tf_right_foot_trans[2]
            roll_right_foot = tf_right_foot_rot[0]
            pitch_right_foot = tf_right_foot_rot[1]
            yaw_right_foot = tf_right_foot_rot[2]
        
            # End of body transforms

            f.write(str(x_head)+" "+str(y_head)+" "+str(z_head)+" "+str(roll_head)+" "+str(pitch_head)+" "+str(yaw_head)+" ")
            f.write(str(x_neck)+" "+str(y_neck)+" "+str(z_neck)+" "+str(roll_neck)+" "+str(pitch_neck)+" "+str(yaw_neck)+" ")
            f.write(str(x_torso)+" "+str(y_torso)+" "+str(z_torso)+" "+str(roll_torso)+" "+str(pitch_torso)+" "+str(yaw_torso)+" ")
            f.write(str(x_left_shoulder)+" "+str(y_left_shoulder)+" "+str(z_left_shoulder)+" "+str(roll_left_shoulder)+" " +str(pitch_left_shoulder)+" "+str(yaw_left_shoulder)+" ")
            f.write(str(x_left_elbow)+" "+str(y_left_elbow)+" "+str(z_left_elbow)+" "+str(roll_left_elbow)+" "+str(pitch_left_elbow)+" " +str(yaw_left_elbow)+" ")
            f.write(str(x_right_shoulder)+" "+str(y_right_shoulder)+" "+str(z_right_shoulder)+" "+str(roll_right_shoulder)+" " +str(pitch_right_shoulder)+" "+str(yaw_right_shoulder)+" ")
            f.write(str(x_right_elbow)+" "+str(y_right_elbow)+" "+str(z_right_elbow)+" "+str(roll_right_elbow)+" "+str(pitch_right_elbow)+" " +str(yaw_right_elbow)+" ")
            f.write(str(x_left_hip)+" "+str(y_left_hip)+" "+str(z_left_hip)+" "+str(roll_left_hip)+" "+str(pitch_left_hip)+" " +str(yaw_left_hip)+" ")
            f.write(str(x_left_knee)+" "+str(y_left_knee)+" "+str(z_left_knee)+" "+str(roll_left_knee)+" "+str(pitch_left_knee)+" " +str(yaw_left_knee)+" ")
            f.write(str(x_right_hip)+" "+str(y_right_hip)+" "+str(z_right_hip)+" "+str(roll_right_hip)+" "+str(pitch_right_hip)+" " +str(yaw_right_hip)+" ")
            f.write(str(x_right_knee)+" "+str(y_right_knee)+" "+str(z_right_knee)+" "+str(roll_right_knee)+" "+str(pitch_right_knee)+" " +str(yaw_right_knee)+" ")
            f.write(str(x_left_hand)+" "+str(y_left_hand)+" "+str(z_left_hand)+" "+str(roll_left_hand)+" "+str(pitch_left_hand)+" " +str(yaw_left_hand)+" ")
            f.write(str(x_right_hand)+" "+str(y_right_hand)+" "+str(z_right_hand)+" "+str(roll_right_hand)+" "+str(pitch_right_hand)+" " +str(yaw_right_hand)+" ")
            f.write(str(x_left_foot)+" "+str(y_left_foot)+" "+str(z_left_foot)+" "+str(roll_left_foot)+" "+str(pitch_left_foot)+" " +str(yaw_left_foot)+" ")
            f.write(str(x_right_foot)+" "+str(y_right_foot)+" "+str(z_right_foot)+" "+str(roll_right_foot)+" "+str(pitch_right_foot)+" "+str(yaw_right_foot)+"\n")

            # criacao do array com todas as coordenadas a cada instante
            frame = np.array([[x_head, y_head, z_head, roll_head, pitch_head, yaw_head, x_neck, y_neck, z_neck, roll_neck, pitch_neck, yaw_neck, x_torso, y_torso, z_torso, roll_torso, pitch_torso, yaw_torso, x_left_shoulder, y_left_shoulder, z_left_shoulder, roll_left_shoulder, pitch_left_shoulder, yaw_left_shoulder, x_left_elbow, y_left_elbow, z_left_elbow, roll_left_elbow, pitch_left_elbow, yaw_left_elbow, x_right_shoulder, y_right_shoulder, z_right_shoulder, roll_right_shoulder, pitch_right_shoulder, yaw_right_shoulder, x_right_elbow, y_right_elbow, z_right_elbow, roll_right_elbow, pitch_right_elbow, yaw_right_elbow, x_left_hip, y_left_hip, z_left_hip, roll_left_hip, pitch_left_hip, yaw_left_hip, x_left_knee, y_left_knee, z_left_knee, roll_left_knee, pitch_left_knee, yaw_left_knee, x_right_hip, y_right_hip, z_right_hip, roll_right_hip, pitch_right_hip, yaw_right_hip, x_right_knee, y_right_knee, z_right_knee, roll_right_knee, pitch_right_knee, yaw_right_knee, x_left_hand, y_left_hand, z_left_hand, roll_left_hand, pitch_left_hand, yaw_left_hand, x_right_hand, y_right_hand, z_right_hand, roll_right_hand, pitch_right_hand, yaw_right_hand, x_left_foot, y_left_foot, z_left_foot, roll_left_foot, pitch_left_foot, yaw_left_foot, x_right_foot, y_right_foot, z_right_foot, roll_right_foot, pitch_right_foot, yaw_right_foot]])

            dados = np.concatenate([dados, frame]) if dados.size else frame # array com todas as coordenadas de todos os instantes
            print np.shape(dados)
            '''    
    while not os.path.exists('test.txt'):
        print "Waiting..."
        time.sleep(3)

    
    if os.path.isfile('test.txt'):
        # read file
        time.sleep(2)
        segundos+=3
        with open("test.txt") as f:
            dados = np.loadtxt(f)
        print np.shape(dados)  
    else:
        raise ValueError("%s isn't a file!" % file_path)                                            
    
    #test = feature_distancias_convolucao(dados)
    #test = test[1::10,:]
    #print np.shape(test)
    test = np.delete(dados,[12,13,14,15,16,17],1)
    #(distancias_total, velocidades_total, velocidade_xyz, direcao_xyz)=feature_extraction(dados)
    #distancias_total = distancias_total[0:len(velocidades_total),:] 
    #direcao_xyz = direcao_xyz[:,[4,19,34,6,21,36,11,26,41,12,27,42,13,28,43,14,29,44]]
    #velocidades_total = velocidades_total[:,[0,1,3,4,5,6,7,8,9,10,11,12,13,14]]                
    
    #test = np.c_[distancias_total, direcao_xyz, velocidades_total]
'''      
'''          
    test1 =  ( test - min1) / (max1 - min1)
    test2 =  ( test - min2) / (max2 - min2)
    test3 =  ( test - min3) / (max3 - min3)
    test4 =  ( test - min4) / (max4 - min4)
    test5 =  ( test - min5) / (max5 - min5)
    test6 =  ( test - min6) / (max6 - min6)
    test7 =  ( test - min7) / (max7 - min7)
    test8 =  ( test - min8) / (max8 - min8)'''
    '''
    #test = (test1+test2+test3+test4+test5+test6+test7+test8)/8
    #test = ( test - min_total) / (max_total - min_total) 
         
    #test = np.c_[dados[0:len(direcao_xyz),:], direcao_xyz]     
    clfNB2 = externals.joblib.load('Linear_SVM_torso/lin_svm_clf.pkl')
    predicao_NB = clfNB2.predict(test)
    predicao_NB = np.ndarray.tolist(predicao_NB)
    #proba_predict = clfNB2.predict_proba(test)
    proba_predict=0
    for i in range(1,9):
        c = predicao_NB.count(i)
        conta_class[i-1]=c
    #print conta_class 
    print conta_class/sum(conta_class)
    #print proba_predict.mean(axis=0)'''
    '''
                predicao_NB = clfNB2.predict(test2)
                predicao_NB = np.ndarray.tolist(predicao_NB)
                for i in range(1,9):
                    c = predicao_NB.count(i)
                    conta_class[i-1]=c
                print conta_class 
                print conta_class/sum(conta_class)
                
                predicao_NB = clfNB2.predict(test3)
                predicao_NB = np.ndarray.tolist(predicao_NB)
                for i in range(1,9):
                    c = predicao_NB.count(i)
                    conta_class[i-1]=c
                print conta_class 
                print conta_class/sum(conta_class)
                
                predicao_NB = clfNB2.predict(test4)
                predicao_NB = np.ndarray.tolist(predicao_NB)
                for i in range(1,9):
                    c = predicao_NB.count(i)
                    conta_class[i-1]=c
                print conta_class 
                print conta_class/sum(conta_class)
                
                predicao_NB = clfNB2.predict(test5)
                predicao_NB = np.ndarray.tolist(predicao_NB)
                for i in range(1,9):
                    c = predicao_NB.count(i)
                    conta_class[i-1]=c
                print conta_class 
                print conta_class/sum(conta_class)
                
                predicao_NB = clfNB2.predict(test6)
                predicao_NB = np.ndarray.tolist(predicao_NB)
                for i in range(1,9):
                    c = predicao_NB.count(i)
                    conta_class[i-1]=c
                print conta_class 
                print conta_class/sum(conta_class)
                
                predicao_NB = clfNB2.predict(test7)
                predicao_NB = np.ndarray.tolist(predicao_NB)
                for i in range(1,9):
                    c = predicao_NB.count(i)
                    conta_class[i-1]=c
                print conta_class 
                print conta_class/sum(conta_class)
                
                predicao_NB = clfNB2.predict(test8)
                predicao_NB = np.ndarray.tolist(predicao_NB)
                for i in range(1,9):
                    c = predicao_NB.count(i)
                    conta_class[i-1]=c
                print conta_class 
                print conta_class/sum(conta_class)
    '''            '''
    if segundos==12:
        print np.shape(conta_class)
        #sio.savemat('pe2_SVC.mat', {'pe2_SVC':proba_predict})
        conta_class=np.ndarray.tolist(conta_class)
        print "Activity: %s" % actividades[conta_class.index(max(conta_class))]
        conta_class = np.zeros((8)) 
        time.sleep(10)
        #except(tf.LookupException, tf.ConnectivityException, tf.ExtrapolationException):
         #   continue

        #f.close()
    rate.sleep()
