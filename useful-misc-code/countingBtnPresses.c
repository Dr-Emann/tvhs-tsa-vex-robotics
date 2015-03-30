#pragma config(Sensor, dgtl2,  bump_claw_1,         sensorTouch)
#pragma config(Sensor, dgtl3,  bump_claw_2,    sensorTouch)

#define clawBumpL SensorBoolean[bump_claw_1]
#define clawBumpR SensorBoolean[bump_claw_2]
#define cont5U vexRT[Btn5U]
static bool wasUpLastTime = true;
static int numberOfButtonPresses = 0;
void mainLoop()
{
	if(cont5U) // Replace this with whatever button you want. You can replace it with clawBumpL, for example, and it will count the number of times that sensor is pressed.
									// If the button is pressed this time, check if it was up before (if it was up before, it was just pressed this time, so add one to the number of presses)
	{								//		this is nessisary because this code will get run several times while the button is down, and without checking if the button was up before, the number of button presses
									//		will increase by a large amount each time the button is pressed, becuase the code may run 50 times for the 1 second the button is held down.
		if(wasUpLastTime)
		{
			numberOfButtonPresses++; // Add one to the recorded number of button presses
		}
	}
	else
	{
		wasUpLastTime = true;		// This is important. If the button is not being pressed this time, reset wasUpLastTime to true, that way the next time the button is pressed, the 
									//		it will know that it was up last time, so it can add one to the number of button presses.
	}

	if(numberOfButtonPresses > 5)
	{
		// Run your code that is dependant on the number of button presses here.
		// For example, maybe after a bump switch is hit 5 times, stop all the motors. I don't know.
	}
}