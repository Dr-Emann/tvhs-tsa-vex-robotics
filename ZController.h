#ifndef Z_CONTROLLER_H
#define Z_CONTROLLER_H 1
static bool override = false;
void mainLoop()
{
	// drive motor control
	setMovement(cont1,cont2);

	// lift control
	if(cont8U>0)
		StartTask(lift);
	else if(cont8D>0)
		StartTask(lower);
	else if(cont8R>0)
		StartTask(liftToPartway);
	else if(cont8L>0)
		StartTask(dropRingOnPost);

	// claw control
	if(cont7L>0)
	{
		if(liftClicks>=partwayLiftClicks) // if the claw is at partwayLiftClicks or higher, it is past the body, and therefore can open further
			wideOpenClaw();
		else // if the claw is below partwayLiftClicks, it is not past the body, and cannot open as far
			openClaw();
	}
	else if(cont7R>0)
		closeClaw();
	// manual override for lift
	if(cont3>120)
	{
		liftMotors(80);
		override=true;
	}
	else if(cont3<-120)
	{
		liftMotors(-80);
		override = true;
	}
	else if(cont3<=120 && cont3>=-120 && override == true)
	{
		liftMotors(0);
		override = false;	// override variable prevents lift from stopping constantly when joystick is in the middle,
							// it should only stop the lift once, when override was just true, and after stopping it, it will
							// reset override to false, so that it will not run again
	}
	// kill switch
	if(cont6D>0)
	{
		StopTask(lift);
		StopTask(lower);
		StopTask(openClaw);
		StopTask(closeClaw);
		stopAllMotors();
	}
}
#endif
