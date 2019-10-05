

int irSensorDigital = 0;

void setup()
{
    Serial.begin(115200);
    pinMode(5, INPUT);
    pinMode(3, OUTPUT);
}

void loop()
{
    Serial.println("SENSOR");
    irSensorDigital = digitalRead(5);
    Serial.println(irSensorDigital);

     if (irSensorDigital == 0)
     {
        digitalWrite(3, HIGH);
     }
     else
     {
         digitalWrite(3, LOW);
     }
}
