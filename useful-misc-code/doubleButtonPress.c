#pragma config(Sensor, dgtl2,  bump_claw_1,         sensorTouch)
#pragma config(Sensor, dgtl3,  bump_claw_2,    sensorTouch)

#define clawBumpL SensorBoolean[bump_claw_1]
#define clawBumpR SensorBoolean[bump_claw_2]
#define cont5U vexRT[Btn5U]
static bool wasUpLastTime = true;
static int numberOfButtonPresses = 0;
///////////////////////////////////////////////////////
//// IMPORTANT:
////	This code is set up to use timer 4 (time1[T4]). Do _NOT_ use this timer anywhere else in your code to time anything, or it will be reset by this code.
//// Other Information:
////	Timers will automatically keep track of time, after being set. All you have to worry about is checking their value, and resetting them to zero when nessisary.
////	
///////////////////////////////////////////////////////
void mainLoop()
{
	const int MAX_DOUBLE_PRESS_TIME = 200; // This code sets the maximum amount of time between button presses to be registered, in milliseconds.
	if(time1[T4] > MAX_DOUBLE_PRESS_TIME)
	{
		numberOfButtonPresses = 0; // If the timer expires, reset the number of buttons presses to zero.
	}
	if(cont5U) // Replace this with whatever button you want. You can replace it with clawBumpL, for example, and it will count the number of times that sensor is pressed.
									// If the button is pressed this time, check if it was up before (if it was up before, it was just pressed this time, so add one to the number of presses)
	{								//		this is nessisary because this code will get run several times while the button is down, and without checking if the button was up before, the number of button presses
									//		will increase by a large amount each time the button is pressed, becuase the code may run 50 times for the 1 second the button is held down.
		if(wasUpLastTime)
		{
			time1[T4] = 0;
			numberOfButtonPresses++; // Add one to the recorded number of button presses
		}
	}
	else
	{
		wasUpLastTime = true;		// This is important. If the button is not being pressed this time, reset wasUpLastTime to true, that way the next time the button is pressed, the 
									//		it will know that it was up last time, so it can add one to the number of button presses.
	}

	if(numberOfButtonPresses == 2)
	{
		// Run the code that's supposed to be run when that button is double pressed.
		// For Example, you could set the robot to reverse itself (see reverseableDriving.c) by doing:

		isNotReversed = !isNotReversed; // which will set isNotReversed to the opposite of itself (i.e. it will be false if it was true, and true if it was false
	}
}