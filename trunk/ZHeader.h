#ifndef Z_HEADER_H
#define Z_HEADER_H

#define until(x) while(!(x))

#define leftMotor motor[motor_L]
#define rightMotor motor[motor_R]
#define liftMotor1 motor[motor_lift_L]
#define liftMotor2 motor[motor_lift_R]
#define liftMotors(x) motor[motor_lift_L]=motor[motor_lift_R]=(x)
#define liftValue motor[motor_lift_L]
#define clawL motor[servo_claw_L]
#define clawR motor[servo_claw_R]

#define cont1 vexRT[Ch1]
#define cont2 vexRT[Ch2]
#define cont3 vexRT[Ch3]
#define cont4 vexRT[Ch4]

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


/*
 * uncomment for old controler
#define cont5U ((vexRT[Ch5]>0)?127:0)
#define cont5D ((vexRT[Ch5]<0)?127:0)
#define cont6U ((vexRT[Ch6]>0)?127:0)
#define cont6D ((vexRT[Ch6]<0)?127:0)
#define cont7L ((vexRT[Ch4]>0)?127:0)
#define cont7R ((vexRT[Ch4]<0)?127:0)
*/

#define liftENC SensorValue[enc_lift]
#define clawBump SensorBoolean[bump_claw]
#define lineL SensorValue(lineFollower_L)
#define lineR SensorValue(lineFollower_R)

const static int maxMotor = 127;
#endif
