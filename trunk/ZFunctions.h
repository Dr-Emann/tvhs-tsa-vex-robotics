#ifndef Z_FUNCTIONS_H
#define Z_FUNCTIONS_H
#include "ZHeader.h"

static int lightVal;
static int liftClicks = 0;
void stopAllMotors()
{
	leftMotor=0;
	rightMotor=0;
	liftMotors(0);
	clawL = 0;
	clawR = 0;
}
void forward(const int nForward)
{
	leftMotor = nForward;
	rightMotor = nForward;
}
void forward(const float fForward){ forward((int)(fForward*maxMotor)); }
void turnInPlace(const int nTurn)
{
	leftMotor = -nTurn;
	rightMotor = nTurn;
}
void turnInPlace(const float fTurn)	{ turnInPlace((int)(fTurn*maxMotor)); }
void backward(const int nBackward)	{ forward(-nBackward); }
void backward(const float fBackward){ forward((int)(-maxMotor*fBackward)); }

bool clawOpen() {	return clawL == -10; }
void openClaw()
{
	const int openAmount = 10;
	if(useSlowClawMotion)
	{
		while(abs(clawL-openAmount)>5 && abs(clawR+openAmount)>5)
		{
			clawL -= abs(clawL-openAmount)/2;
			clawR += abs(clawR+openAmount)/2;
		}
	}
	clawL = -openAmount; clawR = openAmount;

}
void closeClaw(){	clawL = clawClosedPos; clawR = -clawClosedPos; }

task lower();
task lowerPartway();
task lift()
{
	const int RAISE_AMOUNT = maxMotor;
	StopTask(lower);
	StopTask(lowerPartway);
	int clicksStart = liftClicks;

	liftENC = 0;

	liftMotors(RAISE_AMOUNT);
	while(liftClicks<maxLiftClicks)
	{
		liftClicks = clicksStart + liftENC;
	}
	liftMotors(0);

}
task lower()
{
	const int RAISE_AMOUNT = -3*maxMotor/4;
	StopTask(lift);
	StopTask(lowerPartway);
	int clicksStart = liftClicks;

	liftENC = 0;
	liftMotors(RAISE_AMOUNT);
	while(liftClicks>0)
	{
		liftClicks = clicksStart - liftENC;
	}
	liftMotors(0);
}
task lowerPartway()
{
	const int RAISE_AMOUNT = -3*maxMotor/4;
	StopTask(lift);
	StopTask(lower);
	int clicksStart = liftClicks;

	liftENC = 0;
	liftMotors(RAISE_AMOUNT);
	while(liftClicks>partwayLiftClicks)
	{
		liftClicks = clicksStart - liftENC;
	}
	liftMotors(0);
}

void setMovement(int x, int y)
{
	int left = y+x;
	int right = y-x;
	if(abs(left)>maxMotor)
		right -= (left-maxMotor);
	else if (abs(right)>maxMotor)
		left -= (right-maxMotor);
	leftMotor = left;
	rightMotor = right;
	/*
	 *Keeping in case previus code does not work
	 **		leftMotor = y/2 + x/2;		//y-axis value: forward/backward motion
	 **		rightMotor = y/2 - x/2;		//x-axis value: left/right steering
	 */
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
	return whiteOnLeft()|| whiteOnRight();
}
void pickUpRing()
{
	closeClaw();
	wait1Msec(200);
	StartTask(lift);
}
void putRingOnPost()
{
	StartTask(lowerPartway);
	wait1Msec(1000);
	openClaw();
	wait1Msec(100);
	backward(maxMotor);
	wait1Msec(2000);
	closeClaw();
	StartTask(lower);
}
bool wiggle(bool const goRight, int const nMSec = shakeTime)
{
	time1[T2] = 0;
	while(!seeingWhite() && time1[T2]<nMSec)
	{
		if(goRight)
			setMovement(maxMotor*sin(2*PI/nMSec*time1[T2]),3*maxMotor/4);
		else
			setMovement(-maxMotor*sin(2*PI/nMSec*time1[T2]),3*maxMotor/4);
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
}
task autonMode()
{
	bool const goRight = true;
	int const maxTime = 8000;
	time1[T2] = 0;
	forward(maxMotor);
	while(!seeingWhite() && time1[T2]<maxTime);// wait untill white is seen or time is up
	if(!seeingWhite()) return; // if no line found in time, exit to prevent robot damage

	if(goRight)
		setMovement(maxMotor/4, 3*maxMotor/4);
	else
		setMovement(-maxMotor/4, 3*maxMotor/4);
	wait1Msec(turnAmount);// turn for turn amount

	if(!wiggle(goRight,shakeTime)) return; // if no line found, exit to prevent robot damage

	followLineUntilBump();

	pickUpRing();
	wait1Msec(150); // Give some time for lift to start, hopefuly avoid collision

	followLineUntilBump();

	putRingOnPost();
}
#endif
