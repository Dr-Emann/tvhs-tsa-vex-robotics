#pragma config(Motor,  port2,           motor_L,       tmotorNormal, openLoop)
#pragma config(Motor,  port3,           motor_R,       tmotorNormal, openLoop, reversed)

#define leftMotor = motor[motor_L]
#define rightMotor = motor[motor_R]

static bool isNotReversed = true;	// Notice that this is declared as static (it can be accessed from anywhere), but it is not constant. This is because we want to be able to reverse
									// the robot in the code.

void forward(const int nForward)
{
	if(isNotReversed)
	{
		leftMotor = nForward;
		rightMotor = nForward;
	}
	else
	{
		leftMotor = -nForward;
		rightMotor = -nForward;
	}
}
void backward(const int nBackward)
{
  forward(-nBackward);
}
void turnInPlace(const int nTurn)
{
	if(isNotReversed)
	{
		leftMotor = -nTurn;
		rightMotor = nTurn;
	}
	else
	{
		leftMotor = nTurn;
		rightMotor = -nTurn;
	}
}
// sets the movement of the drive motors based on a joystick's x and y coorinates.
void setMovement(const int x, const int y)
{
	if(isNotReversed)
	{
		leftMotor = y/2 + x/2;		//y-axis value: forward/backward motion
		rightMotor = y/2 - x/2;		//x-axis value: left/right steering
	}
	else
	{
		leftMotor = -y/2 - x/2;		//Just negated the motor values from above
		rightMotor = x/2 - y/2;
	}
}