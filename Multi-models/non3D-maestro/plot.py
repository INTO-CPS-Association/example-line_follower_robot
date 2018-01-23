#!/usr/bin/env python

import os
import sys
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt


outputsPath = sys.argv[1]

df = pd.read_csv(outputsPath)

dfNoZc= pd.read_csv('Variable/R_2018_01_23-19_10_36_337/outputs.csv')

resDir=os.path.dirname(outputsPath)

fig,ax = plt.subplots()

#time,step-size,{bodyFMU}.body.robot_theta,{bodyFMU}.body.robot_x,{bodyFMU}.body.robot_y,{bodyFMU}.body.robot_z,{bodyFMU}.body.total_energy_used,{controllerFMU}.controller.servoLeftVal,{controllerFMU}.controller.servoRightVal,
#{sensor1FMU}.sensor1.lf_1_sensor_reading,
#{sensor2FMU}.sensor2.lf_1_sensor_reading

ax.set_title("left")
s='{sensor1FMU}.sensor1.lf_1_sensor_reading'
df[['time',s]].plot(x='time',y=s,ax=ax, label=s+' zc')

s='{sensor1FMU}.sensor1.lf_1_sensor_reading'
dfNoZc[['time',s]].plot(x='time',y=s,ax=ax)

#s='{controller}.i.lagError'
#df[['time',s]].plot(x='time',y=s,ax=ax)
#s='{load}.i.phi'
#df[['time',s]].plot(x='time',y=s,ax=ax)
fig.savefig(os.path.join(resDir,'left.pdf'))

#fig,ax = plt.subplots()
#ax.set_title("controller output")
#s='{controller}.i.output'
#df[['time',s]].plot(x='time',y=s,ax=ax)
#fig.savefig(os.path.join(resDir,'controller_output.pdf'))

#fig,ax = plt.subplots()
#ax.set_title("temperatures")
#s='{th}.i.CoilTemperature'
#df[['time',s]].plot(x='time',y=s,ax=ax)
#s='{th}.i.HousingTemperature'
#df[['time',s]].plot(x='time',y=s,ax=ax)
#fig.savefig(os.path.join(resDir,'temperatures.pdf'))


#fig,ax = plt.subplots(nrows=4)
#ax.set_title("others")
#s='{controller}.i.output'
#df[['time',s]].plot(x='time',y=s,ax=ax[0])
#s='{pwm}.i.output'
#df[['time',s]].plot(x='time',y=s,ax=ax[1])
#s='{motor}.i.ElectricCurrent'
#df[['time',s]].plot(x='time',y=s,ax=ax[2])
#s='{load}.i.p.omega'
#df[['time',s]].plot(x='time',y=s,ax=ax[3])
#fig.savefig(os.path.join(resDir,'others.pdf'))

plt.show()
