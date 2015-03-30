/*
 * The purpose of this file is to define constants, which can be used anywhere in the code (as long as this file is #included first). This
 * makes it easy to quickly switch a value from true to false, or to change the value to which a motor runs. I would HIGHLY, HIGHLY recomend using a file like this.
 *
 * For instance, by declaring maxLiftClicks here, it was easy to mess around with until I got just the right value that would raise the lift to the top. I only had to
 * change it in one place, it wasn't spread throught the functions.
 *
 * I tried to put only the fundamental constants for the robot here, not values that are of little use. (rule of thumb, if you have trouble naming the variable, don't include it here)
 * If you want a simular effect for smaller variables (i.e. the amount of time the robot waits after backing up in a particular function), simply
 * declare a const at the begining of that function, and use that. I should have done more of that.
 */

static bool const runAutonomous = true;     // If you want to disable autonomous mode, set this to false. WORKS ONLY IN main2.c

static bool const liftStartsAtTop = false;  // If the lift starts at the top, set to true, if lift starts at bottom, set to false

static int const maxLiftClicks = 749;       // clicks from bottom to top of lift : FORMALLY 750!!

static int const partwayLiftClicks = 250;   // clicks from bottom to mid-level (used for lowering rings onto posts)

static int const clawOpenPos = -10;         // the position to which the servos should be opened to when claw is just open and down

static int const clawClosedPos = -80;       // the position to which the servos should be closed to when the claw is closed

static int const clawWideOpenPos = 60;      // the position to which the servos should be opened to when the claw is open fully, which only happens when claw is at or above partwayLiftClicks

static bool const goRight = true;           // determines whether the robot will turn right after picking up rings in autonomous mode,
                                            // as well as the direction the robot turns after depositing those rings, in an attempt to knock over the opposite color stack.
                                            // true for blue, false for red
