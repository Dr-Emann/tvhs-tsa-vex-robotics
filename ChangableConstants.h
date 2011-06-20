#ifndef CHANGEABLE_CONSTANTS_H
#define CHANGEABLE_CONSTANTS_H

static bool const runAutonMode = false;	// If you want to disable autonomous mode, set this to false

static bool const liftStartsAtTop = false;// If the lift starts at the top, set to true, if lift starts at bottom, set to false

static int const whiteLevel = 500;			// level where line is recognized as white.

static int const maxLiftClicks = 750;	// clicks from bottom to top of lift

static int const partwayLiftClicks = 250; // clicks from bottom to mid-level (used for lowering rings onto posts)

static int const turnAmount = 1200;			// determines how far robot will turn in autonomous mode after finding the line.
																				//		try to get the robot to turn about 90 degrees

static int const shakeTime = 7000;			// determines the number of milliseconds the robot will shake after turning, trying to find the line again. (1000 ms = 1 sec)

static int const clawClosedPos = 55;

static int const clawOpenPos = -10;

static bool useSlowClawMotion = false;
#endif
