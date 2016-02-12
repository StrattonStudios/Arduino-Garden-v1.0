#define ph_minus 10 //6v peristallic Pin down
#define ph_plus 11  // 6v peristallic Pin up

const int num_readings = 10;
int readings[num_readings];
int ph_setpoint = 0;
int inputPin = A0;
int index = 0;                  // the index of the current reading
int total = 0;                  // the running total
int average = 0;
float phplus = 0;
float phminus = 0;
float ph = 0.00;

void phsetup() {
  // put your setup code here, to run once:

for (int thisReading = 0; thisReading < num_readings; thisReading++)
    readings[thisReading] = 0;

    pinMode(ph_minus, OUTPUT);
    pinMode(ph_plus, OUTPUT);
    Serial.begin(9600);
}

void phloop() {
  // put your main code here, to run repeatedly:

Serial.println("Enter value for ph_setpoint, Press ENTER");

  while (Serial.available() == 0)
  {
    ph_setpoint = Serial.read() - '0';
    break;
  }
  
Serial.println(ph_setpoint);
Serial.print("Setpoint = ");
Serial.println(ph_setpoint);     
Serial.print("pH = ");
Serial.println(average);


readings[index] = analogRead(inputPin);
  total = total + readings[index];         
  index = index + 1;                    
  if (index >= num_readings)              
    index = 0;      
                           
  average = total / num_readings;
  ph = (average * .02);
  Serial.println(average);   
  delay(1); 

//////Calibration of ph steps///////

if (average > ph_setpoint)
{
    phminus = (ph - ph_setpoint);

}
else if (average < ph_setpoint)
{
    phplus = (ph_setpoint - ph);
}
else (average = ph_setpoint);
{
    Serial.println("ph is set properly");
}

/////ph dosing alpha/////

      
 while (average =! ph_setpoint)
{
          if (phplus >= ph_setpoint)
          {
            digitalWrite (ph_minus, LOW);
            digitalWrite (ph_plus, LOW);
          }
         
          else if (phplus < phminus)
          {
            digitalWrite (ph_plus, HIGH);
            digitalWrite (ph_minus, LOW);
          }
          else
          {
          Serial.println('Ph is calibrated');
          }
    break;
}
}


