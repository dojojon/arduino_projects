// Dispense Sensor Pins
const int DISPENSE_TRIG_PIN = 3;
const int DISPENSE_ECHO_PIN = 4;
const int DISPENSE_ADJUST_PIN = A2;
const int DISPENSE_DURATION_ADJUST_PIN = A1;
const int DISPENSE_MOTOR_PWM = 100;

//Dispense Motor Pi
const int DISPENSE_MOTOR_PIN = 9;

// Distance in CM to start dispensing
const int DISPENSE_DISTANCE_MAX = 60;
long dispenseDistance = DISPENSE_DISTANCE_MAX / 2;

// How long in milliseconds to dispense paper. e.g.  3000 is 3 seconds
const int DISPENSE_DURATION_MAX = 3000;
long dispenseDuration = DISPENSE_DURATION_MAX / 2;

// State of motor, HIGH its running, LOW its off
int dispenseMotorState = LOW;

// Fill Level Pins
const int FILL_TRIG_PIN = 5;
const int FILL_ECHO_PIN = 6;
const int FILL_ADJUST_PIN = A0;

// Low Paper Pin
const int FILL_LED_PIN = 8;

// Distance in CM to indicate low paper
const int FILL_LEVEL_DISTANCE_MAX = 60;
long fillDistance = FILL_LEVEL_DISTANCE_MAX / 2;

// Time dispense started
long dispenseStartTime = 0;

// Distance for each sensor
long cmDispense, cmFill;

void setup()
{
  //Serial Port begin
  Serial.begin(115200);
  Serial.println("Setup start");

  //Define inputs and outputs
  pinMode(DISPENSE_ADJUST_PIN, INPUT);
  pinMode(DISPENSE_DURATION_ADJUST_PIN, INPUT);
  pinMode(DISPENSE_TRIG_PIN, OUTPUT);
  pinMode(DISPENSE_ECHO_PIN, INPUT);
  pinMode(DISPENSE_MOTOR_PIN, OUTPUT);

  pinMode(FILL_ADJUST_PIN, INPUT);
  pinMode(FILL_TRIG_PIN, OUTPUT);
  pinMode(FILL_ECHO_PIN, INPUT);
  pinMode(FILL_LED_PIN, OUTPUT);

  Serial.println("Setup complete");
}


int c = 0;
void loop()
{

  c++;
  Serial.println(c);

  // Recalculate the dispense, duration and fill levels
  // using the analog pins
  updateLevels();

  // Check the hand sensor and motor states
  upateDispensorState();

  // Check the paper levels
  updateFillState();

//  testOutputPins();

  delay(250);
}


// Check for hand and control the motor pin
void upateDispensorState()
{
  Serial.println("------------");
  // Serial.print("dispenseMotorState ");
  // Serial.println(dispenseMotorState);

  cmDispense = getDistance(DISPENSE_TRIG_PIN, DISPENSE_ECHO_PIN);
  Serial.print("cmDispense ");
  Serial.println(cmDispense);

  Serial.print("dispenseDistance ");
  Serial.println(dispenseDistance);

  // If the motor is off, check the sensor
  if (dispenseMotorState == LOW)
  {

    if (dispenseDistance > cmDispense)
    {
      Serial.println("Hand detected");

      // Update the start dispense time
      dispenseStartTime = millis();

      // Turn the motor on
      dispenseMotorState = HIGH;
      analogWrite(DISPENSE_MOTOR_PIN, DISPENSE_MOTOR_PWM);
      
    }
  }
  else
  {
    Serial.println("Motor running");
    Serial.print("dispenseStartTime ");
    Serial.println(dispenseStartTime);
    Serial.print("dispenseDuration ");
    Serial.println(dispenseDuration);
    Serial.print("millis ");
    Serial.println(millis());

    // Motor is running, check the start + duration < now to see if we need to stop it
    if (dispenseStartTime + dispenseDuration < millis())
    {

      Serial.println("Switch off the motor");

      // Turn the motor on
      dispenseMotorState = LOW;
      digitalWrite(DISPENSE_MOTOR_PIN, dispenseMotorState);
    }
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
  long cm = (duration / 2) /  29.1; // Divide by 29.1 or multiply by 0.0343

  return cm;
}

// Calculate the levels based on the potentiometers
void updateLevels()
{

  // Calculate the levels
  int dispenseAdjust = analogRead(DISPENSE_ADJUST_PIN);
  int dispenseDurationAdjust = analogRead(DISPENSE_DURATION_ADJUST_PIN);
  int fillAdjust = analogRead(FILL_ADJUST_PIN);

  dispenseDistance = map(dispenseAdjust, 0, 1023, 1, DISPENSE_DISTANCE_MAX);
  dispenseDuration = map(dispenseDurationAdjust, 0, 1023, 100, DISPENSE_DURATION_MAX);
  fillDistance = map(fillAdjust, 0, 1023, 1, FILL_LEVEL_DISTANCE_MAX);


  Serial.println("------------------------------------");

  Serial.println();
  Serial.print("dispenseAdjust ");
  Serial.println(dispenseAdjust);

  Serial.print("dispenseDurationAdjust ");
  Serial.println(dispenseDurationAdjust);

  Serial.print("fillAdjust     ");
  Serial.println(fillAdjust);

  Serial.println("");
  Serial.print("dispenseDistance ");
  Serial.println(dispenseDistance);

  Serial.print("dispenseDuration ");
  Serial.println(dispenseDuration);

  Serial.print("fillDistance ");
  Serial.println(fillDistance);

}


// Check the paper level and turn on led if low
void updateFillState()
{

  Serial.println("------------");

  // Get the fill level distance
  cmFill = getDistance(FILL_TRIG_PIN, FILL_ECHO_PIN);
  Serial.print("cmFill ");
  Serial.print(cmFill);
  Serial.println();

  Serial.print("fillDistance ");
  Serial.println(fillDistance);

  // If the distance is bigger than required level, turn on warning led
  if (cmFill > fillDistance)
  {
    Serial.println("Low paper");
    digitalWrite(FILL_LED_PIN, HIGH);
  }
  else
  {
    Serial.println("Paper above level");
    digitalWrite(FILL_LED_PIN, LOW);
  }
}


//Test of motor and fill level pins
void testOutputPins() {

  // Turn on motor
  analogWrite(DISPENSE_MOTOR_PIN, DISPENSE_MOTOR_PWM);
  
  // Wait some milliseconds
  delay(500);

  // Turn off motor
  analogWrite(DISPENSE_MOTOR_PIN, LOW);

  // Turn on light
  digitalWrite(FILL_LED_PIN, HIGH);

  // Wait 5 seconds
  delay(5000);

  // Turn off light
  digitalWrite(FILL_LED_PIN, LOW);

}
