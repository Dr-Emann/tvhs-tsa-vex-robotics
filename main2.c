#pragma config(Sensor, in1,    lineFollower_L,      sensorLineFollower)
#pragma config(Sensor, in2,    lineFollower_R,      sensorLineFollower)
#pragma config(Sensor, dgtl1,  enc_lift,            sensorRotation)
#pragma config(Sensor, dgtl2,  bump_claw,           sensorTouch)
#pragma config(Motor,  port2,           motor_L,       tmotorNormal, openLoop)
#pragma config(Motor,  port3,           motor_R,       tmotorNormal, openLoop, reversed)
#pragma config(Motor,  port6,           servo_claw_L,  tmotorServoStandard, openLoop)
#pragma config(Motor,  port7,           servo_claw_R,  tmotorServoStandard, openLoop)
#pragma config(Motor,  port8,           motor_lift_L,  tmotorNormal, openLoop, reversed)
#pragma config(Motor,  port9,           motor_lift_R,  tmotorNormal, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "ChangableConstants.h"
#include "ZFunctions.h"

task main()
{
	liftENC = 0;
	openClaw();
	liftClicks = (liftStartsAtTop)?maxLiftClicks:0;
	if(runAutonMode)
  	{
  		StartTask(autonMode);
  		time1[T4] = 0;
  		while(time1[T4]<20000)
  		{
  			if(cont6D>0)
  			{
  				StopTask(autonMode);
  				break;
  			}
  		}
  		StopTask(autonMode);
	}

  int joy_x;            // will hold the X value of the analog stick (choices below)
  int joy_y;            // will hold the Y value of the analog stick (choices below)
  while(true)
  {
    joy_x = cont1;   // This is the RIGHT analog stick.  For LEFT, change 'Ch1' to 'Ch4'.
    joy_y = cont2;   // This is the RIGHT analog stick.  For LEFT, change 'Ch2' to 'Ch3'.


    setMovement(joy_x,joy_y);

    if(cont5U>0)
      StartTask(lift);
    else if(cont5D>0)
    	StartTask(lower);
    //kill switch
    if(cont7L>0)
    	openClaw();
    else if(cont7R>0)
    	closeClaw();
    if(cont3>120)
    {
    	liftMotors(50);
    }
    else if(cont3<-120)
    {
    	liftMotors(-50);
    }
    if(cont6D>0)
    {
      StopTask(lift);
      StopTask(lower);
      StopTask(openClaw);
      StopTask(closeClaw);
      stopAllMotors();
    }
  }
}
