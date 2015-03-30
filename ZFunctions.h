/*
 * The purpose of this file is to provide the functions and tasks that the robot uses.
 * I tried to make a function for everything I wanted the robot to be able to do, even if they were redundant
 * (i.e. because backward is just going forward with a negitive speed, i could have not used backward, only forward. However, having a seperate backwards function made the code much more readable)
 *
 * IMPORTANT NOTE: Notice that the functions go from most simple to most complex. This is nessisary, because RobotC works from top to bottom. You cannot use a variable or function unless it is
 *  declared above. For example, I could not have put the backward function before the forward function, because backward uses
 *
 * I seperated this file because it contains the basic functions and tasks which other functions (i.e. mainLoop) can use.
 */

#include "ZHeader.h"

static int liftClicks = 0; // keeps track of the position of the lift. 0 means bottom of lift, maxLiftClicks means the top.

void forward(const int nForward)// const just says that you wont change nForward. It is not nessisary, just good coding practice
                                // since it makes sense to not be able to change the amount you are telling it to move forward while you're telling it to move forward,
                                // this will warn you if you try, i.e, if you said
                                //      leftMotor = nForward;
                                //      nForward = 6;
                                //      rightMotor = nForward;
                                // it would complain because you are trying to change a constant value, and rightly so.
{
    leftMotor = nForward;
    rightMotor = nForward;
}
void backward(const int nBackward)
{
  forward(-nBackward);
}
void turnInPlace(const int nTurn)
{
    leftMotor = -nTurn;
    rightMotor = nTurn;
}

bool clawIsOpen() { return clawL > clawClosedPos; } // if clawL is greater than (not equal to) clawClosedPos, will return true, else will return false
bool clawIsWideOpen() { return clawL > clawOpenPos; } // if clawL is further open than just open, it must be wide open.

void openClaw() // was not able to reverse a servo, so had to reverse all changes to one side of the claw
{
    clawL = clawOpenPos;
    clawR = -clawOpenPos;
}
void closeClaw()
{
    clawL = clawClosedPos;
    clawR = -clawClosedPos;
}
void wideOpenClaw()
{
    clawL = clawWideOpenPos;
    clawR = -clawWideOpenPos;
}

static int clicksStart; // used to store the start position of the lift at the begining of one of the lift-related tasks: lift, lower, liftToPartway

task lower();           // have to pre-declare these tasks, because lift uses them, and it will complain if it doesn't know they exist yet. They will be defined fully
task liftToPartway();   // later in the code
task lift()
{
  // NOTE: Usually, I name constant variables, that will not, and CANNOT change with all caps, and undescores if it is more than one word.
  //      a non constant variable, I name with a lowercase first letter, and an upper case letter for the begining of each subsequent word:
  //          i.e.  const int THIS_VARIABLE_IS_CONSTANT;
  //                int thisVariableIsNotConstant;
    const int RAISE_AMOUNT = maxMotor; // deturmines the speed of the motors while lifting. Set here to maximum power.

    // If we're trying to lift, we don't want to be lowering or only lifting partway.
    StopTask(lower);
    StopTask(liftToPartway);

    if(liftValue<=0) // if the lift is not already going up (liftValue is the velocity of the lift)
    {
        clicksStart = liftClicks; // it starts at the current position (liftClicks)

        liftENC = 0;// reset the value of the encoder back to zero at the begining of the lifting operation.

        liftMotors(RAISE_AMOUNT); // set the motors to start lifting at RAISE_AMOUNT
        // while the current position(liftClicks) is still less than the target (maxLiftClicks, the top), set the current position to the start position plus the amount of clicks
        // the encoder has recorded since the task was started (we reset it to zero at the begining)
        // i.e. if we started at the bottom, clicks start will equal zero. Until we get to the top, it will update the current position to be the start plus how many it's moved since
        // the start.
        // if we started at 50, and we've gone 10 clicks since the start, liftClicks will be set to 60, and will keep increasing because the motor is still running.
        while(liftClicks<maxLiftClicks)
        {
            liftClicks = clicksStart + liftENC;
        }
        liftMotors(20); // set the lift to keep lifting with a power of 20, to keep it from drifting down.
    }
    else    // need this, because the lift task could be called multiple times, and it should not reset the encoder each time.
            // also, each time the lift task is called, it kills the earlier lift task first, so if the new task doesn't continue to check if the lift is
            // high enough, the motors will simply continue forever
    {
        while(liftClicks<maxLiftClicks)
        {
            liftClicks = clicksStart + liftENC;
        }
        liftMotors(20);
    }

}
task lower()
{
    const int RAISE_AMOUNT = -3*maxMotor/4; // lifting with negitive power is lowering
    StopTask(lift);
    StopTask(liftToPartway);

    if(clawIsWideOpen()) // check if the claw is too far open, if it is, set it to only partly open, instead of wide open.
        openClaw();

    if(liftValue>=0)
    {
        clicksStart = liftClicks;

        liftENC = 0;
        liftMotors(RAISE_AMOUNT);
        while(liftClicks>30) // lift was lowering too far, this will only wait until the lift reaches a position of 30, then it will say that is 0
        {
            liftClicks = clicksStart - liftENC;
        }
        liftMotors(0);
        liftClicks= 0;
    }
    else
    {
        while(liftClicks>30)
        {
            liftClicks = clicksStart - liftENC;
        }
        liftMotors(0);
        liftClicks = 0;
    }
}
/**
 * Either lifts or lowers the lift to the point just above the peg.
 */
task liftToPartway()
{
    StopTask(lift);
    StopTask(lower);
    if(liftClicks > partwayLiftClicks) // if above partwayLiftClicks
    {
        const int RAISE_AMOUNT = -3*maxMotor/4;
        if(liftValue>=0) // if above and not already going down
        {
            clicksStart = liftClicks;
            liftENC = 0;
            liftMotors(RAISE_AMOUNT);
            while(liftClicks>partwayLiftClicks)
            {
                liftClicks = clicksStart - liftENC;
            }
            liftMotors(0);
        }
        else // if above and already going down
        {
            while(liftClicks>partwayLiftClicks)
            {
                liftClicks = clicksStart - liftENC;
            }
            liftMotors(0);
        }

    }
    else if(liftClicks<partwayLiftClicks) // if below partwayLiftClicks
    {
        const int RAISE_AMOUNT = 3*maxMotor/4;
        if(liftValue>=0) // if below and not already going up
        {
            clicksStart = liftClicks;
            liftENC = 0;
            liftMotors(RAISE_AMOUNT);
            while(liftClicks<partwayLiftClicks)
            {
                liftClicks = clicksStart + liftENC;
            }
            liftMotors(0);
        }
        else // if below and already going up
        {
            while(liftClicks<partwayLiftClicks)
            {
                liftClicks = clicksStart + liftENC;
            }
            liftMotors(0);
        }
    }
}

// sets the movement of the drive motors based on a joystick's x and y coorinates.
void setMovement(int x, int y)
{
    leftMotor = y/2 + x/2;      //y-axis value: forward/backward motion
    rightMotor = y/2 - x/2;     //x-axis value: left/right steering
}

// Sets all motors to stop, and closes the claw (safest position)
void stopAllMotors()
{
    leftMotor=0;
    rightMotor=0;
    liftMotors(0);
    closeClaw();
}

void pickUpRing()
{
    closeClaw();
    wait1Msec(200); // let servo reach specified position, even though the position can be set immediately, it takes a few milliseconds for the claw to actually close.
    StartTask(lift);
}
task dropRingOnPost()
{
    if(abs(liftClicks-maxLiftClicks) < 4)   // if lift is already pretty much at the top, put ring on, otherwise do nothing.
                                            // cannot put a ring on a post if the lift is not over the post.
    {
        StartTask(liftToPartway);
        wait1Msec(800);
        wideOpenClaw();
        wait1Msec(750);
    }
}
void putRingOnPost()    // lowers all the way before opening claw, must only be used if there is garenteed to be no rings on a post.
                        // otherwise use dropRingOnPost
{
    StartTask(liftToPartway);
    wait1Msec(1000);
    wideOpenClaw();
}
