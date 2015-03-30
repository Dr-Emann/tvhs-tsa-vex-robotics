/*
 * The purpose of this file is to hold the high level functions. These functions only call other functions, they do not access the robot directly.
 * These functions are also responsible for dealing with all user input.
 *
 * I seperated this file from the others so that I could seperate the low level functions (those that direcly change motor values, etc) from the higher level functions.
 */

// This variable is used to keep track of whether the lift is being overriden by the joystick.
static bool override = false;
void mainLoop()
{
    // Set the movement speed based on the value of the x and y position of the controller joystick.
    setMovement(cont1,cont2);

    // Buttons that control the lift by telling it where to go
    if(cont8U>0)
        StartTask(lift);
    else if(cont8D>0)
        StartTask(lower);
    else if(cont8R>0)
        StartTask(liftToPartway);
    else if(cont8L>0)
        StartTask(dropRingOnPost);

    // If the button is pressed to open the claw, check if the claw can open wide, or only part way
    // (this is dependant apon the lift's height, if it's too low, the claw cannot physicly open all the way, so don't try)
    if(cont7L>0)
    {
        if(liftClicks>=partwayLiftClicks) // if the claw is at partwayLiftClicks or higher, it is past the body, and therefore can open further
            wideOpenClaw();
        else // if the claw is below partwayLiftClicks, it is not past the body, and cannot open as far
            openClaw();
    }

    // If the close claw button is pressed, close the claw.
    if(cont7R>0)
        closeClaw();

    // If the joystick is almost all the way up, force the lift up, and remember that we are now overriding.
    if(cont3>120)
    {
        liftMotors(80);
        override=true;
    }
    // If the joystick is not almost all the way up, but it is all the way down, force the lift down
    else if(cont3<-120)
    {
        liftMotors(-80);
        override = true;
    }
    // Joystick must be neither up or down, if we were just overriding it last time, stop the motors
    // (otherwise they'd just keep going, and so that it won't be stopped agian next time, remember we're not overriding anymore.
    else if(override == true)
    {
        liftMotors(0);
        override = false;   // override variable prevents lift from stopping constantly when joystick is in the middle,
                            // it should only stop the lift once, when override was just true, and after stopping it, it will
                            // reset override to false, so that it will not run again
    }

    // If killSwitch button is pressed, stop all tasks that even COULD be running, then stop all motors. This should leave the robot
    // able to be controlled again afterward, but will stop any current movement.
    if(cont6D>0)
    {
        StopTask(lift);
        StopTask(lower);
        StopTask(openClaw);
        StopTask(closeClaw);
        stopAllMotors();
    }
}

task autonomous()
{
    /*
     * Open the claw and move forward, timed to pick up rings
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
     * Picks up rings, and waits for lift to reach the top.
     * this is nessisary because pickUpRing only starts the task named lift, the lift has therefore only just
     * started going up after pickUpRing ends.
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
    forward(0); // This stops the turning, because it sets both drive motors to 0.

    /*
     * go forward slowly until one of the claw bump sensors are hit
     */
    forward(1*maxMotor/3);
    while(!clawBump)// Do nothing while the bump switches aren't touching anything, the drive motors will continue going forward.
    {

  }
    forward(0);// Stop moving when one of the bump switches are touched.

    /*
     * It is assumed that only one claw sensor is being hit currently,
     * so which ever one is being hit, it trys to move forward in an arc that will try to get the other
     * sensor to hit also
     *
     * i.e if only the left sensor is touching, it will act as if you put the joystick to the left and up.
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
    while(!(clawBumpL && clawBumpR) && time1[T3] < 2000) // while both bumps are not hit, and the timer is less than 2000 ms
    {
      // if it's not hitting both yet, add one to the speed of both motors.
      // NOTE: this could also be done by saying leftMotor = leftMotor + 1;
        leftMotor +=1;
        rightMotor += 1;
    }
    // Back up for a little bit of time
    backward(maxMotor);
    wait1Msec(260);

    // Stop and wait for momentum to stop
    backward(0);
    wait1Msec(150);

    // Put rings on the post, and wait for the lift to lower
    putRingOnPost();
    wait1Msec(1000);

    /*
     * Move backward, with a slight turn, in order to knock over the stack of opponent rings.
     * Direction of turning is dependant on the direction we had to turn the first time.
     */
    const int RING_TURN_AMT = 73;
    if(goRight)
        setMovement(-RING_TURN_AMT, -maxMotor);
    else
        setMovement(RING_TURN_AMT,-maxMotor);
    wait1Msec(3000);

    /*
     * After knocking over opponent's rings, move forward and turn back toward playing field, then lower the lift all the way
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
