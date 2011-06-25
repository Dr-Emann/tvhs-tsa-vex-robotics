#ifndef Z_FUNCTIONS_H
#define Z_FUNCTIONS_H 1
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

bool clawIsOpen() {	return clawL > clawClosedPos; }
bool clawIsWideOpen() { return clawL > clawOpenPos; }
void openClaw()
{
	if(useSlowClawMotion)
	{
		while(abs(clawL-clawClosedPos)>5 && abs(clawR+clawClosedPos)>5)
		{
			clawL -= abs(clawL-clawClosedPos)/2;
			clawR += abs(clawR+clawClosedPos)/2;
		}
	}
	clawL = clawOpenPos; clawR = -clawOpenPos;
}
void closeClaw()
{
	if(useSlowClawMotion)
	{
		while(abs(clawL-clawOpenPos)>5 && abs(clawR+clawOpenPos)>5)
		{
			clawL -= abs(clawL-clawOpenPos)/2;
			clawR += abs(clawR+clawOpenPos)/2;
		}
	}
	clawL = clawClosedPos; clawR = -clawClosedPos;
}
void wideOpenClaw()
{
	if(useSlowClawMotion)
	{
		while(abs(clawL-clawWideOpenPos)>5 && abs(clawR+clawWideOpenPos)>5)
		{
			clawL -= abs(clawL-clawWideOpenPos)/2;
			clawR += abs(clawR+clawWideOpenPos)/2;
		}
	}
	clawL = clawWideOpenPos; clawR = -clawWideOpenPos;
}

static int clicksStart;
task lower();
task liftToPartway();
task lift()
{
	const int RAISE_AMOUNT = maxMotor;
	StopTask(lower);
	StopTask(liftToPartway);


	if(liftValue<=0)
	{
		clicksStart = liftClicks;

		liftENC = 0;

		liftMotors(RAISE_AMOUNT);
		while(liftClicks<maxLiftClicks)
		{
			liftClicks = clicksStart + liftENC;
		}
		liftMotors(20);
	}
	else
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
	const int RAISE_AMOUNT = -3*maxMotor/4;
	StopTask(lift);
	StopTask(liftToPartway);

	if(clawIsWideOpen())
		openClaw();

	if(liftValue>=0)
	{
		clicksStart = liftClicks;

		liftENC = 0;
		liftMotors(RAISE_AMOUNT);
		while(liftClicks>30)
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
	if(liftClicks > partwayLiftClicks)
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
		else
		{
			while(liftClicks>partwayLiftClicks)
			{
				liftClicks = clicksStart - liftENC;
			}
			liftMotors(0);
		}

	}
	else if(liftClicks<partwayLiftClicks)
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
		else
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
	/*
	int left = y+x;
	int right = y-x;
	if(abs(left)>maxMotor)
		right -= (left-maxMotor);
	else if (abs(right)>maxMotor)
		left -= (right-maxMotor);
	leftMotor = left;
	rightMotor = right;
	*/

	//Keeping in case previus code does not work
	leftMotor = y/2 + x/2;		//y-axis value: forward/backward motion
	rightMotor = y/2 - x/2;		//x-axis value: left/right steering

}
bool whiteOnLeft()
{
	if(darkOnWhite)
		return lineL>whiteLevel;
	else
		return lineL<whiteLevel;
}
bool whiteOnRight()
{
	if(darkOnWhite)
		return lineR>whiteLevel;
	else
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
task dropRingOnPost()
{
	if(abs(liftClicks-maxLiftClicks) < 4)
	{
		StartTask(liftToPartway);
		wait1Msec(800);
		wideOpenClaw();
		wait1Msec(750);
	}
}
void putRingOnPost()
{
	StartTask(liftToPartway);
	wait1Msec(1000);
	wideOpenClaw();
}
bool wiggle(bool const goRight, int const nMSec = shakeTime)
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
	/* old autonomous code
	StartTask(lift);
	wait1Msec(1500);
	int const maxTime = 5000;
	time1[T2] = 0;
	forward(maxMotor);
	while(seeingWhite()&& time1[T2]<maxTime);
	while(!seeingWhite() && time1[T2]<maxTime);// wait untill white is seen or time is up
	if(!seeingWhite()) return; // if no line found in time, exit to prevent robot damage

	if(goRight)
		turnInPlace(-maxMotor/2);
	else
		turnInPlace(maxMotor/2);
	wait1Msec(turnAmount);// turn for turn amount
	forward(0);
	if(!wiggle(goRight,shakeTime)) return; // if no line found, exit to prevent robot damage

	followLineUntilBump();
	if(clawBumpL&& !clawBumpR)
	{

	}
	else if(clawBumpR && !clawBumpL)
	{

	}
	backward(maxMotor/2);
	wait1Msec(400);
	backward(0);
	putRingOnPost();
	wait1Msec(200);
	backward(maxMotor);
	wait1Msec(700);
	backward(0);
	StartTask(lower);
	*/


	/* Goodish Code
	StartTask(lift);
	wait1Msec(2000);
	backward(maxMotor);
	wait1Msec(800);
	forward(1*maxMotor/4);
	while(!clawBump);
	forward(0);
	if(clawBumpL)
		turnInPlace(maxMotor/4);
	else if(clawBumpR)
		turnInPlace(-maxMotor/4);
	wait1Msec(175);
	backward(maxMotor);
	wait1Msec(220);
	backward(0);

	putRingOnPost();
	wait1Msec(1000);
	backward(maxMotor);
	wait1Msec(500);
	backward(0);
	StartTask(lower);
	*/
	/* GOOD CODE
	openClaw();
	forward(maxMotor/3);
	wait1Msec(1110);
	forward(0);
	wait1Msec(150);
	closeClaw();
	StartTask(lift);
	wait1Msec(1800);
	turnInPlace((goRight)?(maxMotor/2):(-maxMotor/2));
	wait1Msec(398);
	forward(0);

	forward(1*maxMotor/3);
	while(!clawBump);
	forward(0);
	if(clawBumpL)
		turnInPlace(maxMotor/4);
	else if(clawBumpR)
		turnInPlace(-maxMotor/4);
	wait1Msec(175);
	backward(maxMotor);
	wait1Msec(138);
	backward(0);
	wait1Msec(150);
	putRingOnPost();
	wait1Msec(1000);
	backward(maxMotor);
	wait1Msec(600);
	backward(0);
	StartTask(lower);
	*/
	openClaw();
	forward(maxMotor/3);
	wait1Msec(1000);
	forward(maxMotor/5);
	wait1Msec(400);
	forward(0);
	wait1Msec(150);
	closeClaw();
	wait1Msec(150);
	StartTask(lift);
	wait1Msec(1600);
	turnInPlace((goRight)?(-maxMotor/2):(maxMotor/2));
	wait1Msec(480);
	forward(0);

	forward(1*maxMotor/3);
	while(!clawBump);
	forward(0);
	if(clawBumpL)
		setMovement(-maxMotor/3,maxMotor/2);
	else if(clawBumpR)
		setMovement(maxMotor/3,maxMotor/2);

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
	putRingOnPost();
	wait1Msec(1000);
	const int RING_TURN_AMT = 73;
	if(goRight)
		setMovement(-RING_TURN_AMT, -maxMotor);
	else
		setMovement(RING_TURN_AMT,-maxMotor);
	wait1Msec(3000);
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
