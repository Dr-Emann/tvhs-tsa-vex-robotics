void setDriveMotors(const int x, const int y){
	const int DEAD_ZONE = 12;
	if(abs(x)<DEAD_ZONE){
		motor[motor_L] = y;
		motor[motor_R] = y;
	} else if(abs(y)<DEAD_ZONE) {
		motor[motor_L] = x;
		motor[motor_R] = -x;
	} else {
		motor[motor_L] = (y+x)/2;
		motor[motor_R] = (y-x)/2;
	}
}
