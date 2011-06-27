#ifndef Z_FUNCTIONS_H
#define Z_FUNCTIONS_H 1
#include "ZHeader.h"

static int liftClicks = 0; // keeps track of the position of the lift. 0 means bottom of lift, maxLiftClicks means the top.

void forward(const int nForward)// const just says that you wont change nForward. It is not nessisary, just good coding practice
								// since it makes sense to not be able to change the amount you are telling it to move forward while you're telling it to move forward,
								// this will warn you if you try, i.e, if you said
								// 		leftMotor = nForward;
								//		nForward = 6;
								//		rightMotor = nForward;
								// it would complain because you are trying to change a constant value, and rightly so.
{
	leftMotor = nForward;
	rightMotor = nForward;
}
void backward(const int nBackward) { forward(-nBackward); }
void turnInPlace(const int nTurn)
{
	leftMotor = -nTurn;
	rightMotor = nTurn;
}

bool clawIsOpen() {	return clawL > clawClosedPos; } // if clawL is greater than (not equal to) clawClosedPos, will return true, else will return false
bool clawIsWideOpen() { return clawL > clawOpenPos; }

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

task lower(); 			// have to pre-declare these tasks, because lift uses them, and it will complain if it doesn't know they exist yet. They will be defined fully
task liftToPartway();	// later in the code
task lift()
{
	const int RAISE_AMOUNT = maxMotor; // deturmines the speed of the motors while lifting. Set here to maximum power.
	StopTask(lower);
	StopTask(liftToPartway);

	if(liftValue<=0) // if the lift is not already going up (liftValue is the velocity of the lift)
	{
		clicksStart = liftClicks;

		liftENC = 0;

		liftMotors(RAISE_AMOUNT);
		while(liftClicks<maxLiftClicks)
		{
			liftClicks = clicksStart + liftENC;
		}
		liftMotors(20); // set the lift to keep lifting with a power of 20, to keep it from drifting down.
	}
	else 	// need this, because the lift task could be called multiple times, and it should not reset the encoder each time.
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

	if(clawIsWideOpen()) // check if the claw is too far open for the
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

void setMovement(int x, int y)
{
	leftMotor = y/2 + x/2;		//y-axis value: forward/backward motion
	rightMotor = y/2 - x/2;		//x-axis value: left/right steering
}

void stopAllMotors()
{
	leftMotor=0;
	rightMotor=0;
	liftMotors(0);
	closeClaw();
}

bool whiteOnLeft()
{
	return lineL<whiteLevel;
}
bool whiteOnRight()
{
	return lineR<whiteLevel;
}
bool seeingWhite()
{
	return whiteOnLeft()|| whiteOnRight(); // returns true if either sensor reads a white line
}
void pickUpRing()
{
	closeClaw();
	wait1Msec(200); // let servo reach specified position
	StartTask(lift);
}
task dropRingOnPost()
{
	if(abs(liftClicks-maxLiftClicks) < 4) 	// if lift is already pretty much at the top, put ring on, otherwise do nothing.
											// cannot put a ring on a post if the lift is not over the post.
	{
		StartTask(liftToPartway);
		wait1Msec(800);
		wideOpenClaw();
		wait1Msec(750);
	}
}
void putRingOnPost() 	// lowers all the way before opening claw, must only be used if there is garenteed to be no rings on a post.
						// otherwise use dropRingOnPost
{
	StartTask(liftToPartway);
	wait1Msec(1000);
	wideOpenClaw();
}
bool wiggle(bool const goRight, int const nMSec = shakeTime) 	// can define a default value for a parameter. i.e, wiggle can be called as:
																//		wiggle(true);
																//		wiggle(true, shakeTime);
																//		wiggle(false, 700);
																// the first two of those being equivielent statements
{
	time1[T2] = 0;
	while(!seeingWhite() && time1[T2]<nMSec)
	{
		if(goRight)
			setMovement(maxMotor/4*sin(32*PI/nMSec*time1[T2]),3*maxMotor/4);
		else
			setMovement(-maxMotor/4*sin(32*PI/nMSec*time1[T2]),3*maxMotor/4);
	}
	forward(0);
	return seeingWhite();
}
/**
*	pre-condition:	white line between two light sensors
*	post-coindtion:	white line between sensors,
*/
void followLineUntilBump()
{
	int const speed = maxMotor;
	while(!clawBump)
	{
		while( (!seeingWhite()) && !clawBump )
		{
			forward(speed);
		}
		while( (whiteOnLeft()) && !clawBump )
		{
			setMovement(-maxMotor/4,3*maxMotor/4);
		}
		while( (whiteOnRight()) && !clawBump )
		{
			setMovement(maxMotor/4, 3*maxMotor/4);
		}
	}
	forward(0);
}
task autonomous()
{
	/*
	 * open the claw and move forward, timed to pick up rings
	 * slows down when close to avoid knocking rings over
	 * waits for a short time after stoping to allow for momentum to stop
	 */
	openClaw();
	forward(maxMotor/3);
	wait1Msec(1000);
	forward(maxMotor/5);
	wait1Msec(400);
	forward(0);
	wait1Msec(150);

	/*
	 * picks up rings, and waits for lift to reach the top
	 */
	pickUpRing();
	wait1Msec(1600);

	/*
	 * turns toward post, going right or left, depending on team color,
	 * if on blue team, will go right, red will go left
	 */
	if(goRight)
		turnInPlace(-maxMotor/2);
	else
		turnInPlace(maxMotor/2);
	wait1Msec(480);
	forward(0);

	/*
	 * go forward slowly until one of the claw bump sensors are hit
	 */
	forward(1*maxMotor/3);
	while(!clawBump);
	forward(0);

	/*
	 * it is assumed that only one claw sensor is being hit currently,
	 * so which ever one is being hit, it trys to move forward in an arc that will try to get the other
	 * sensor to hit also
	 */
	if(clawBumpL)
		setMovement(-maxMotor/3,maxMotor/2);
	else if(clawBumpR)
		setMovement(maxMotor/3,maxMotor/2);

	/*
	 * the robot goes forward with incresing power until both sensors are hit or until time runs out.
	 * it then backs up, just enough to line the rings up with the post, since the switches are mounted behind the claw
	 */
	time1[T3] = 0;
	while(!(clawBumpL && clawBumpR) && time1[T3] < 2000)
	{
		leftMotor +=1;
		rightMotor += 1;
	}
	backward(maxMotor);
	wait1Msec(260);
	backward(0);
	wait1Msec(150);

	/*
	 * puts ring on post, and waits for lift to lower and such
	 */
	putRingOnPost();
	wait1Msec(1000);

	/*
	 * moves backward, with a slight turn, in order to knock over the stack of opponent rings
	 * direction of turning is dependant on team color
	 */
	const int RING_TURN_AMT = 73;
	if(goRight)
		setMovement(-RING_TURN_AMT, -maxMotor);
	else
		setMovement(RING_TURN_AMT,-maxMotor);
	wait1Msec(3000);

	/*
	 * after knocking over opponent's rings, moves forward and turns back toward playing field, then lowers the lift all the way,
	 * to prepare for user control mode
	 */
	forward(maxMotor);
	wait1Msec(400);
	forward(0);
	if(goRight)
		turnInPlace(-maxMotor/2);
	else
		turnInPlace(maxMotor/2);
	wait1Msec(1000);
	forward(0);
	StartTask(lower);
}
#endif
