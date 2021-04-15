# Axat Shah CS207_Fit2Go

It has almost been a year and half since the COVID-19 outbreak and people are still very much uncomfortablevisiting hospital for regular check-ups. I have made an arduino based medical device named **Fit2Go** to help those people overcome their fear. Wirth this devoce, they can easily measure their body temprature along with their pulse anytime. This project can help a lot of lderly people and can save a lot of money. 

## Repository Contents:
* .ino: The Code for this project can be found here.  
* Images: 
* * Circuit - Picture of the circuit diagram can be found here.
* * Fit2Go.bmp Logo - Bitmap Picture of the logo can be found here.

## Materials Required:
* Arduino Uno.
* Breadboard.
* Jumper wires.
* Red LED.
* 220Ohm resistor.
* 128x64 OLD display.
* Temprature Sensor.
* MAX30102 Heart rate and oximeter sensor.

## Building Instructions:

Building the circuit isn't that hard. This project is more based on software instead of hardware. Before powering the arduino Include all the necessary libraries like Adafruit_GFX.h, Adafruit_SSD1306.h, MAX30105.h and heartRate.h. Connect all the VCC and GND to the temprature sensor, MAX30102 and 128x64 OLED display. After that connect SDA and CLK to analog pins A5 and A4 respectively. Just be carefull while doing so, I ruined my MAX30102 sensor. After that connect temprature sensor to A1 and power up the arduino. Dont't forget to attach the LED with digital pin of your choice. Here's the picture of my circuit:![circuit](https://user-images.githubusercontent.com/79736196/114800803-a1f78180-9d57-11eb-8552-01ea8b9c3c65.jpg)

## Team:

* Axat Shah - For this project, I was the one handling everything from Designing, Assembling, coding to documentation.  

## Credit: 

* https://create.arduino.cc/projecthub/SurtrTech/measure-heart-rate-and-spo2-with-max30102-c2b4d8?ref=tag&ref_id=health&offset=1
* https://randomnerdtutorials.com/guide-for-oled-display-with-arduino/
* https://www.arduino.cc/
