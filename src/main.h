#ifndef AVOIDER_H_
#define AVOIDER_H_

#define LEFT_FORWARD 2
#define LEFT_BACKWARD 3
#define LEFT_SPEED 5
#define RIGHT_FORWARD 4
#define RIGHT_BACKWARD 7
#define RIGHT_SPEED 6

#define PROXIMITY_ECHO 13
#define PROXIMITY_TRIG 12

#define PROXIMITY_SERVO 11
#define PROXIMITY_SERVO_MIN 10
#define PROXIMITY_SERVO_MAX 170
#define PROXIMITY_SERVO_MID 90

#define LEFT_LINE_DETECTOR 8
#define CENTER_LINE_DETECTOR 9
#define RIGHT_LINE_DETECTOR 10

#define DEFAULT_SPEED 255

void setup();
void loop();
void avoid();
bool is_line_detected();
int get_farest_direction();
void decrease_proximity_heading_to(int direction);
int degree_to_turn_time(int degree);

#endif
