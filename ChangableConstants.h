#ifndef CHANGEABLE_CONSTANTS_H
#define CHANGEABLE_CONSTANTS_H

static bool const runAutonomous = true;		// If you want to disable autonomous mode, set this to false

static bool const liftStartsAtTop = false;	// If the lift starts at the top, set to true, if lift starts at bottom, set to false

static int const whiteLevel = 1900;			// level where line is recognized as white.

static int const blackLevel = 1000;			// level where line is recognized as black

static int const maxLiftClicks = 749;		// clicks from bottom to top of lift : FORMALLY 750!!

static int const partwayLiftClicks = 250; 	// clicks from bottom to mid-level (used for lowering rings onto posts)

static int const turnAmount = 1050;			// determines how far robot will turn in autonomous mode after finding the line.

static int const shakeTime = 7000;			// determines the number of milliseconds the robot will shake after turning, trying to find the line again. (1000 ms = 1 sec)

static int const clawOpenPos = -10;			// the position to which the servos should be opened to when claw is just open and down

static int const clawClosedPos = -80;		// the position to which the servos should be closed to when the claw is closed

static int const clawWideOpenPos = 60;		// the position to which the servos should be opened to when the claw is open fully, which only happens when claw is at or above partwayLiftClicks

static bool const useSlowClawMotion = false;// if true, will slow down the claw open and close motion. DOES NOT WORK.

static bool const goRight = true; 			// determines whiether the robot will turn right after picking up rings in autonomous mode,
											// as well as the direction the robot turns after depositing those rings, in an attempt to knock over the opposite color stack
											// true for blue, false for red
#endif
