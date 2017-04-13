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
  if (proximity.ping_cm() < 50 && proximity.ping_cm() > 0) {
   avoid();
  } else {
   wheels.forward(DEFAULT_SPEED, 50);
  }
}

void avoid() {
  wheels.full_stop(0);
  int farest_direction = get_farest_direction();
  if(farest_direction == -1){
    wheels.backward(DEFAULT_SPEED, 1500);
  }else if(farest_direction < 90){
    wheels.turn_right(DEFAULT_SPEED, 500+((90-farest_direction)*STRAIGHT_TURN_TIME)/90);
  }else{
    wheels.turn_left(DEFAULT_SPEED, 500+((farest_direction-90)*STRAIGHT_TURN_TIME)/90);
  }
}

int get_farest_direction() {
  int max_distance = 0;
  int farest_direction = -1;

  decrease_proximity_heading_to(PROXIMITY_SERVO_MIN);
  for(int i=PROXIMITY_SERVO_MIN; i < PROXIMITY_SERVO_MAX; i = i+6){
    int distance_sum = 0;
    for(int probe=0; probe < 5; probe++){
      proximity_heading.write(i+probe);
      delay(10);
      distance_sum += proximity.ping_cm();
    }
    int avg_distance = distance_sum / 6;
    if(avg_distance > max_distance){
      max_distance = avg_distance;
      farest_direction = i+3;
    }
  }
  decrease_proximity_heading_to(PROXIMITY_SERVO_MID);

  return farest_direction;
}

void decrease_proximity_heading_to(int direction){
  int currentDirection = proximity_heading.read();
  for(int i=currentDirection; i > direction; i--){ proximity_heading.write(i); delay(5); }
}
