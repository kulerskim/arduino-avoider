#include <L298N.h>
#include <NewPing.h>
#include <Servo.h>
#include <SoftwareSerial.h>
#include "main.h"

NewPing proximity(PROXIMITY_TRIG, PROXIMITY_ECHO, 200);
Servo proximity_heading;
L298N wheels(LEFT_SPEED, LEFT_FORWARD, LEFT_BACKWARD, RIGHT_FORWARD, RIGHT_BACKWARD, RIGHT_SPEED);

void setup() {
  proximity_heading.attach(PROXIMITY_SERVO);
  proximity_heading.write(PROXIMITY_SERVO_MID);
}

void loop() {
  if (is_line_detected()) {
    wheels.backward(DEFAULT_SPEED, 1500);
    wheels.turn_left(DEFAULT_SPEED, 2000);
  }else if (proximity.ping_cm() < 50 && proximity.ping_cm() > 0) {
   avoid();
  } else {
   wheels.forward(DEFAULT_SPEED, 50);
  }
}

bool is_line_detected() {
  return digitalRead(LEFT_LINE_DETECTOR) == HIGH || digitalRead(CENTER_LINE_DETECTOR) == HIGH || digitalRead(RIGHT_LINE_DETECTOR) == HIGH;
}

void avoid() {
  wheels.full_stop(0);
  int farest_direction = get_farest_direction();
  if(farest_direction == -1){
    wheels.backward(DEFAULT_SPEED, 1500);
  }else if(farest_direction < 90){
    wheels.turn_right(DEFAULT_SPEED, degree_to_turn_time(90 - farest_direction));
  }else{
    wheels.turn_left(DEFAULT_SPEED, degree_to_turn_time(farest_direction - 90));
  }
}

int get_farest_direction() {
  int max_distance = 0;
  int farest_direction = -1;
  int segment_size = 5;

  decrease_proximity_heading_to(PROXIMITY_SERVO_MIN);
  for(int i=PROXIMITY_SERVO_MIN; i < PROXIMITY_SERVO_MAX; i = i+segment_size){
    int segment_sum = 0;
    for(int segment_index=0; segment_index < segment_size; segment_index++){
      proximity_heading.write(i+segment_index);
      delay(10);
      segment_sum += proximity.ping_cm();
    }
    int avg_distance = segment_sum / segment_size;
    if(avg_distance > max_distance){
      max_distance = avg_distance;
      farest_direction = i+(segment_size/2);
    }
  }
  decrease_proximity_heading_to(PROXIMITY_SERVO_MID);

  return farest_direction;
}

void decrease_proximity_heading_to(int direction){
  int currentDirection = proximity_heading.read();
  for(int i=currentDirection; i > direction; i--){ proximity_heading.write(i); delay(5); }
}

int degree_to_turn_time(int degree){
  return 500+(degree*1000)/90;
}
