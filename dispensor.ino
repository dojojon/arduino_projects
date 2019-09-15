
// Dispense Sensor Pins
const int DISPENSE_TRIG_PIN = 10;
const int DISPENSE_ECHO_PIN = 11;

//Dispense Motor Pin
const int DISPENSE_MOTOR_PIN = 4;

// Fill Level Pins
const int FILL_TRIG_PIN = 12;
const int FILL_ECHO_PIN = 13;

// Low Paper Pin
const int FILL_LED_PIN = 6;

// Distance in CM to start dispensing
const int DISPENSE_DISTANCE = 20;

// Distance in CM to indicate low paper
const int FILL_LEVEL_LOW_DISTANCE = 25;

// Time in milliseconds dispense started
unsigned long dispenseStartTime = 0;

// How long in milliseconds to dispense paper. e.g.  2000 is 2 seconds
unsigned long DISPENSE_DURATION = 2000;

// State of motor, HIGH its running, LOW its off
int dispenseMotorState = LOW;

// Current time in milliseconds
unsigned long now = 0;

// Distance for each sensor
unsigned long cmDispense, cmFill;

void setup()
{
  //Serial Port begin
  Serial.begin(9600);

  //Define inputs and outputs
  pinMode(DISPENSE_TRIG_PIN, OUTPUT);
  pinMode(DISPENSE_MOTOR_PIN, OUTPUT);
  pinMode(DISPENSE_ECHO_PIN, OUTPUT);

  pinMode(FILL_TRIG_PIN, OUTPUT);
  pinMode(FILL_ECHO_PIN, INPUT);
  pinMode(FILL_LED_PIN, OUTPUT);
}

void loop()
{

  // Get the current time
  now = millis();

  // Get the sensor distances
  cmDispense = getDistance(DISPENSE_TRIG_PIN, DISPENSE_ECHO_PIN);

  Serial.print(cmDispense);
  Serial.print(" cm Dispense");
  Serial.println();

  Serial.print("Motor State:");
  Serial.println(dispenseMotorState);

  // If the motor is not running and a tigger is in range
  if (dispenseMotorState == LOW && cmDispense < DISPENSE_DISTANCE)
  {
    Serial.println("Start dispense");
    dispenseStartTime = now;
    dispenseMotorState = HIGH;
    digitalWrite(DISPENSE_MOTOR_PIN, dispenseMotorState);
  }

  // If the motor is running, check to see if we need to turn it off
  if (dispenseMotorState == HIGH)
  {
    // Calculate how long the motor has been running for
    unsigned long ellapsedTime = now - dispenseStartTime;

    // Check to if we have been running for longer than our duration
    if (ellapsedTime > DISPENSE_DURATION)
    {

      Serial.println("Stop dispense");
      dispenseMotorState = LOW;
      digitalWrite(DISPENSE_MOTOR_PIN, dispenseMotorState);
    }
  }

  // Get the fill level distance
  cmFill = getDistance(FILL_TRIG_PIN, FILL_ECHO_PIN);
  Serial.print(cmFill);
  Serial.print("cmFill");
  Serial.println();

  // If the distance is lower than required level, turn on warning led
  if (cmFill < FILL_LEVEL_LOW_DISTANCE)
  {
    digitalWrite(FILL_LED_PIN, HIGH);
  }
  else
  {
    digitalWrite(FILL_LED_PIN, LOW);
  }
}

// Get the distance in cm
long getDistance(int trigPin, int echoPin)
{

  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  long duration = pulseIn(echoPin, HIGH);

  // Convert the time into a distance
  long cm = (duration / 2) / 29.1; // Divide by 29.1 or multiply by 0.0343

  return cm;
}