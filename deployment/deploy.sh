#!/bin/bash

rm -rf resources sources LFRController.zip modelDescription.xml main.c

mv LFRController.fmu LFRController.zip
unzip LFRController.zip


leftval=`grep 'name="lfLeftVal"' modelDescription.xml | awk -F \" '{print $4}'`
forwardrotate=`grep 'name="forwardRotate"' modelDescription.xml | awk -F \" '{print $4}'`
servoleft=`grep 'name="servoLeftVal"' modelDescription.xml | awk -F \" '{print $4}'`
servoright=`grep 'name="servoRightVal"' modelDescription.xml | awk -F \" '{print $4}'`
forwardspeed=`grep 'name="forwardSpeed"' modelDescription.xml | awk -F \" '{print $4}'`
rightval=`grep 'name="lfRightVal"' modelDescription.xml | awk -F \" '{print $4}'`
backwardrotate=`grep 'name="backwardRotate"' modelDescription.xml | awk -F \" '{print $4}'`

cp main-template.c main.c

sed -i "s/XX_leftval_XX/$leftval/g" main.c
sed -i "s/XX_forwardrotate_XX/$forwardrotate/g" main.c
sed -i "s/XX_servoleft_XX/$servoleft/g" main.c
sed -i "s/XX_servoright_XX/$servoright/g" main.c
sed -i "s/XX_forwardspeed_XX/$forwardspeed/g" main.c
sed -i "s/XX_rightval_XX/$rightval/g" main.c
sed -i "s/XX_backwardrotate_XX/$backwardrotate/g" main.c
