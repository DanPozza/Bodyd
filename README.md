# Bodyd

1. Install OpenCV library from the official site http://opencv.org/

2. Inside Bodydetection.cpp you have to modify the path of haar classifier

3. Default setting use local camera not ip camera, so if you want to use external camera,in the function vcap.open(0)     you have to insert inside brackets videoStreamAddress instead of 0


At first go inside the folder Bodyd using :

 cd Bodyd

than write these commands in the console:

  cmake .
 
  make
 
  ./Bodyd

in this way you can run the Body detection software part.
