/*
Name: Axat Shah
SID: 200436771
CS207 Project FIT2GO

Inspiration from: https://www.arduino.cc/
                  https://create.arduino.cc/projecthub/SurtrTech/measure-heart-rate-and-spo2-with-max30102-c2b4d8?ref=tag&ref_id=health&offset=1

*/
#include <Adafruit_GFX.h>  //oled display libraries
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include "MAX30105.h"      //heart rate sensor library
#include "heartRate.h"

MAX30105 particleSensor;

#define SCREEN_WIDTH 128 //display width
#define SCREEN_HEIGHT 64 //display height

// Display Declaration
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

byte rates[4]; //Array for heart rates
byte rateSpot = 0;
long lastBeat = 0; // last beat occurrence
int Pname = 9;  //LED pin
float beatsPerMinute;
const int tempPin = A1; //temprature analog pin


// Function Prototype
float AnalogToVolts(int reading);


//bitmap array for printing the company logo
const unsigned char myBitmap [] PROGMEM = {
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x7f, 0xfc, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x07, 0x80, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0xfc, 0x00, 0x00, 0x7e, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xc0, 0x1f, 0xff, 0xe0, 0x0f, 0xff, 0xf0, 0x07, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xfc, 0x7f, 0xff, 0xfc, 0x03, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xfe, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xfe, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xfc, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x7f, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xf8, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x3f, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xf8, 0x1f, 0xff, 0xf0, 0x7f, 0xff, 0xff, 0xff, 0xf0, 0x3f, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xf8, 0x3f, 0xff, 0xc0, 0x3f, 0xff, 0xff, 0xff, 0xf0, 0x3f, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xf8, 0x3f, 0xff, 0x80, 0x1f, 0xff, 0xff, 0xff, 0xf8, 0x3f, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xf8, 0x3f, 0xfe, 0x00, 0x1f, 0xff, 0x81, 0xff, 0xf8, 0x3f, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xe0, 0x1f, 0xf0, 0x00, 0x0f, 0xff, 0x00, 0x07, 0xe0, 0x0f, 0xff, 0xff, 0xff,
  0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x1c, 0x07, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff,
  0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x3e, 0x07, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff,
  0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0xff, 0x03, 0xf0, 0x1f, 0x80, 0x00, 0x00, 0x0f, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xfe, 0x07, 0xff, 0xff, 0x01, 0xe0, 0x3f, 0xff, 0xc0, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xfe, 0x03, 0xff, 0xff, 0x81, 0x80, 0xff, 0xff, 0x80, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0x01, 0xff, 0xff, 0xc0, 0x00, 0xff, 0xff, 0x01, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0x80, 0xff, 0xff, 0xc0, 0x03, 0xff, 0xfe, 0x03, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xc0, 0x7f, 0xff, 0xe0, 0x07, 0xff, 0xfc, 0x07, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xe0, 0x3f, 0xff, 0xf0, 0x0f, 0xff, 0xf8, 0x0f, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xf0, 0x1f, 0xff, 0xf0, 0x1f, 0xff, 0xe0, 0x3f, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xfc, 0x07, 0xff, 0xfc, 0x7f, 0xff, 0xc0, 0x7f, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xfe, 0x01, 0xff, 0xff, 0xff, 0xff, 0x80, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xfe, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x3f, 0xff, 0xff, 0xf8, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x0f, 0xff, 0xff, 0xe0, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x07, 0xff, 0xff, 0xc0, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x01, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x7f, 0xfc, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x1f, 0xe0, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x07, 0x80, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfd, 0xff, 0xff,
  0xf8, 0x00, 0x03, 0x81, 0xc0, 0x00, 0x03, 0xff, 0xff, 0xfe, 0x00, 0x0f, 0xff, 0x00, 0x07, 0xff,
  0xfc, 0x00, 0x03, 0x81, 0xc0, 0x00, 0x03, 0xff, 0xff, 0xf0, 0x00, 0x03, 0xfc, 0x00, 0x01, 0xff,
  0xfc, 0x3f, 0xff, 0x81, 0xff, 0xc3, 0xff, 0xc0, 0x3f, 0xe0, 0x7f, 0x03, 0xf8, 0x1f, 0xc0, 0xff,
  0xfc, 0x3f, 0xff, 0x81, 0xff, 0x83, 0xfc, 0x00, 0x07, 0xc1, 0xff, 0xcf, 0xe0, 0x7f, 0xf0, 0x7f,
  0xfc, 0x3f, 0xff, 0x81, 0xff, 0x83, 0xfc, 0x0e, 0x03, 0x83, 0xff, 0xff, 0xe0, 0xff, 0xf8, 0x3f,
  0xfc, 0x00, 0x1f, 0x81, 0xff, 0x83, 0xff, 0x7f, 0x83, 0x83, 0xff, 0xff, 0xc1, 0xff, 0xf8, 0x3f,
  0xfc, 0x00, 0x0f, 0x81, 0xff, 0x83, 0xff, 0xff, 0x07, 0x83, 0xfe, 0x01, 0xc1, 0xff, 0xf8, 0x3f,
  0xfc, 0x00, 0x0f, 0x81, 0xff, 0x83, 0xff, 0xfc, 0x0f, 0x83, 0xfc, 0x00, 0xc1, 0xff, 0xf8, 0x3f,
  0xfc, 0x3f, 0xff, 0x81, 0xff, 0x83, 0xff, 0xc0, 0x3f, 0x83, 0xff, 0xe0, 0xe0, 0xff, 0xf8, 0x3f,
  0xfc, 0x3f, 0xff, 0x81, 0xff, 0x83, 0xff, 0x01, 0xff, 0xc1, 0xff, 0xe0, 0xe0, 0x7f, 0xf0, 0x7f,
  0xfc, 0x3f, 0xff, 0x81, 0xff, 0x83, 0xfc, 0x0f, 0xff, 0xe0, 0x7f, 0x80, 0xf8, 0x3f, 0xc0, 0xff,
  0xfc, 0x3f, 0xff, 0x81, 0xff, 0x83, 0xfc, 0x00, 0x03, 0xf0, 0x00, 0x00, 0xf8, 0x00, 0x01, 0xff,
  0xf8, 0x1f, 0xff, 0x81, 0xff, 0x81, 0xf8, 0x00, 0x03, 0xfe, 0x00, 0x30, 0xff, 0x00, 0x07, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xe7, 0xfe, 0x7f, 0xff, 0xff, 0xe3, 0xfb, 0xff, 0xf8, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};


void setup() {
  Serial.begin(115200);
  pinMode(9, OUTPUT);

  //for display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println(F("Display not found"));
    for (;;); //loop forever
  }

  // for sensor
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST))
  {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
  }

  particleSensor.setup();

  display.clearDisplay();
  display.drawBitmap(4, 4, myBitmap, 128, 128, WHITE);
  display.display();
  delay(3000);

  //printing welcome
  display.clearDisplay();
  display.setTextSize(2); //font size
  display.setTextColor(WHITE);  //text color
  display.setCursor(10, 30); // text position on the oled display
  display.print("Welcome!");
  display.display();
  delay(2000);

  //printing instructions
  display.clearDisplay();
  display.setTextSize(1.75); //font size
  display.setTextColor(WHITE);  //text color
  display.setCursor(0, 25); // text position on the oled display
  display.print("Please place your finger on the sensor!");
  display.display();

  delay(2000);
  display.clearDisplay();
}

void loop()
{

  int reading;
  float volts, temperature_in_celsius;

  long infraValue = particleSensor.getIR(); //getting the infrareid value from the sensor
  if (checkForBeat(infraValue) == true)
  {
    //We sensed a beat!
    long delta = millis() - lastBeat;
    lastBeat = millis();

    beatsPerMinute = 60 / (delta / 1000.0);

    if (beatsPerMinute < 255 && beatsPerMinute > 20) //it wont store the reading if beatsperminute is greater than 255 and less than 20
    {
      rates[rateSpot++] = (byte)beatsPerMinute; //Storing the reading
      rateSpot %= 4;
    }
  }


  reading = analogRead(tempPin);  //storing the analog readings in the integer variable reading

  volts = AnalogToVolts(reading);  //Function call
  Serial.println(volts);
  temperature_in_celsius = volts * 100 - 50; //converting the tremprature in celsius

  Serial.print("TMP:");                       //here if the temprature goes above 37*C which is equivalent to around 100*F the RED LED will turn ON indicating the user to visit a doctor.
  Serial.println(temperature_in_celsius);
  Serial.print("BPM=");
  Serial.print(beatsPerMinute);

  if (temperature_in_celsius >= 25)
  {
    digitalWrite(Pname, HIGH);   // turn the LED on if the temp is higher than 37
  }
  else
  {
    digitalWrite(Pname, LOW);
  }


  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.println("Hello, user!");
  
  display.setTextSize(2.5); //font size
  display.setTextColor(WHITE);  //text color
  display.setCursor(5, 30); // text position on the oled display
  display.print("TMP:");
  display.print(temperature_in_celsius); //printing the temprature
  display.println("C");
  display.display();
  delay(4000);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.println("Hello, user!");

  display.setTextSize(2.5); //font size
  display.setTextColor(WHITE);  //text color
  display.setCursor(5, 30); // text position on the oled display
  display.print("BPM:");
  display.print(beatsPerMinute); //printing the heart rate
  display.display();
  delay(4000);
  display.clearDisplay();


}
float AnalogToVolts(int reading)    //a function to convert the analog input into volts
{
  float volts;
  volts = reading / 1023.0 * 5.0; //Performing conversion to volts
  return volts; //Returning the result
}
