#ifndef Z_CONTROLLER_H
#define Z_CONTROLLER_H 1

void mainLoop()
{
	setMovement(cont1,cont2);

	// lift control
	if(cont8U>0)
		StartTask(lift);
	else if(cont8D>0)
		StartTask(lower);
	else if(cont8R>0)
		StartTask(liftToPartway);
	// claw control
	if(cont7L>0)
		openClaw();
	else if(cont7R>0)
		closeClaw();
	// manual override for lift
	if(cont3>120)
	{
		liftMotors(50);
	}
	else if(cont3<-120)
	{
		liftMotors(-50);
	}
	//kill switch
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
