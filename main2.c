#pragma config(Sensor, in1,    lineFollower_L,      sensorLineFollower)
#pragma config(Sensor, in2,    lineFollower_R,      sensorLineFollower)
#pragma config(Sensor, dgtl1,  enc_lift,            sensorRotation)
#pragma config(Sensor, dgtl2,  bump_claw_1,         sensorTouch)
#pragma config(Sensor, dgtl3,  bump_claw_2,    sensorTouch)
#pragma config(Motor,  port2,           motor_L,       tmotorNormal, openLoop)
#pragma config(Motor,  port3,           motor_R,       tmotorNormal, openLoop, reversed)
#pragma config(Motor,  port6,           servo_claw_L,  tmotorServoStandard, openLoop)
#pragma config(Motor,  port7,           servo_claw_R,  tmotorServoStandard, openLoop)
#pragma config(Motor,  port8,           motor_lift_L,  tmotorNormal, openLoop, reversed)
#pragma config(Motor,  port9,           motor_lift_R,  tmotorNormal, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "ChangableConstants.h"
#include "ZFunctions.h"
#include "ZController.h"

task main()
{
    liftENC = 0;
    closeClaw();
    if(liftStartsAtTop)
        liftClicks = maxLiftClicks;
    else
        liftClicks = 0;

    // If runAutonomous is set to true in ChangeableConstants.h
    if(runAutonomous)
    {
        StartTask(autonomous);
        time1[T4] = 0;// reset timer 4
        while(time1[T4]<20000)// Keep checking for the killswitch durring autonomous mode.
        {
            if(cont6D>0)// If the killswitch is pressed durring autonomus, kill autonomous, and break out of the while loop.
            {
                StopTask(autonomous);
                break;// Break cancels a loop.
            }
        }
        StopTask(autonomous);// After 20 seconds on the timer, stop the task.
    }

    // After maybe running autonomous, loop through user control.
    while(true)
    {
        mainLoop();
    }
}
