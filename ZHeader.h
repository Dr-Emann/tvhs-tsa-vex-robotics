/*
 * The purpose of this file is to give easy names to motors and controller buttons.
 * i.e. instead of typing if(vexRT[Btn5U]), I can just type if(cont5U) instead. Thsi file is mostly unnessisarry,
 * you could simply replace anything with its equivilent on the right.
 */

// syntax: #define name realMeaning
// i.e. #define plus +
// You then could type 5 plus 5, and would get 10. But that would be a pointless use of defines.
#define leftMotor motor[motor_L]
#define rightMotor motor[motor_R]
#define liftMotor1 motor[motor_lift_L]
#define liftMotor2 motor[motor_lift_R]

/*
 * For simple functions, it may be better to write a define. This, for example,
 * takes x, and sets both left and right lift motor to the same value.
 * i.e, you could type liftMotors(50), which would mean the same as motor[motor_lift_L]=motor[motor_lift_R]=50, which sets both motors to 50.
 * notice that x needs to be surrounded by parentheses on both sides of the equation.
 *
 * You could also write this as a standard function like this:
 *
 * void liftMotors(int x)
 * {
 *    motor[motor_lift_L] = x;
 *    motor[motor_lift_R] = x;
 * }
 *
 * If I had chosen to it that way, i would have placed that function into ZFunctions.h
 */
#define liftMotors(x) motor[motor_lift_L]=motor[motor_lift_R]=(x)
// liftValue just tells the speed at which the lift is going, up is positive, down is negitive.
#define liftValue motor[motor_lift_L]

#define clawL motor[servo_claw_L]
#define clawR motor[servo_claw_R]

// Controller axises 1-4 are the same on new and old controllers
#define cont1 vexRT[Ch1]
#define cont2 vexRT[Ch2]
#define cont3 vexRT[Ch3]
#define cont4 vexRT[Ch4]

// Note: #ifndef means if undefined
// Uncomment the next line to use the old controller
// #define usingOldController 1
#ifndef usingOldController // add a #define usingOldController to use old controller, else use the new controler mappings
    #define cont5U vexRT[Btn5U]
    #define cont5D vexRT[Btn5D]
    #define cont6U vexRT[Btn6U]
    #define cont6D vexRT[Btn6D]
    #define cont7L vexRT[Btn7L]
    #define cont7R vexRT[Btn7R]
    #define cont7U vexRT[Btn7U]
    #define cont7D vexRT[Btn7D]
    #define cont8U vexRT[Btn8U]
    #define cont8D vexRT[Btn8D]
    #define cont8R vexRT[Btn8R]
    #define cont8L vexRT[Btn8L]
// Notice I never got around to adding equivilents for the 8 buttons. This is because there are less buttons on the old controller.
#else
    #define cont5U ((vexRT[Ch5]>0)?127:0)
    #define cont5D ((vexRT[Ch5]<0)?127:0)
    #define cont6U ((vexRT[Ch6]>0)?127:0)
    #define cont6D ((vexRT[Ch6]<0)?127:0)
    #define cont7L ((vexRT[Ch4]>0)?127:0)
    #define cont7R ((vexRT[Ch4]<0)?127:0)
#endif

#define liftENC SensorValue[enc_lift]
#define clawBumpL SensorBoolean[bump_claw_1]
#define clawBumpR SensorBoolean[bump_claw_2]
#define lineL SensorValue(lineFollower_L)
#define lineR SensorValue(lineFollower_R)

/*
 * This way you can easily do:
 *
 * if(clawBump), because clawBump will be true if either bump switch is pressed.
 *
 * This could also be writen as a function:
 *    bool clawBump()
 *    {
 *      return (SensorBoolean[bump_claw_1] || SensorBoolean[bump_claw_2]);
 *    }
 * However, this would have to be used like this: if(clawBump()), which looks less pretty than if(clawBump).
 */
#define clawBump (SensorBoolean[bump_claw_1] || SensorBoolean[bump_claw_2]) //will be true if either bump sensor is hit

const static int maxMotor = 127;
