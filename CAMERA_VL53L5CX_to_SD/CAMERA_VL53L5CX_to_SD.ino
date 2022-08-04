/*
******************************************************************************
  Captures an image with Spresense Camera Board and at the same time logs
  8x8 distance measures with ST's VL53L5CX-SATEL on a Sony's Spresense Board

  This code for the Sony Spresense Board (Main Board plus Extension Board) takes
  a picture with the attached Camera Board and logs lectures from a VL53L5CX
  8x8 ToF sensor each second, and stores them into separate files on a SD card.
  The idea is to combine those 2 files into one with latter processing on a computer.

  Sony's Spresense libraries are used ( https://github.com/sonydevworld/spresense-arduino-compatible )
  SparkFun's VL53L5CX library is used ( http://librarymanager/All#SparkFun_VL53L5CX )

  NOTE: Memory needs to be increased to 1024KB at Arduino IDE Tools menu to run this code.

  Juan M. Montes - jmontes@atc.us.es
  ORCID: 0000-0002-0983-2386
  GitHub: https://github.com/juamonsan

******************************************************************************
*/

#include <Arduino.h>
#include <SDHCI.h>
#include <File.h>
#include <stdio.h>

#include <Camera.h>

#include <Wire.h>
#include <SparkFun_VL53L5CX_Library.h> //http://librarymanager/All#SparkFun_VL53L5CX

#define TOTAL_MEASUREMENTS (10)

const int PwrEn = 13; //Needed for ST's VL53L5CX-SATEL board. PwrEn pin needs to go here.
const int LPn = 12; //Needed for ST's VL53L5CX-SATEL board. LPn pin needs to go here.

SDClass SD;  /**< SDClass object */
File myFile; /**< File object */

SparkFun_VL53L5CX myImager;
VL53L5CX_ResultsData measurementData; // Result data class structure, 1356 byes of RAM



int imageResolution = 0; // Used to pretty print output
int imageWidth = 0;      // Used to pretty print output

int lecture_number = 0; //Used as counter and for file naming
char filename[16] = {0}; //For file naming





/* Configurations */
void setup()
{
  /* Initialize digital pins */
  pinMode(PwrEn, OUTPUT);
  pinMode(LPn, OUTPUT);
  digitalWrite(PwrEn, HIGH); //Enable VL53L5CX
  digitalWrite(LPn, HIGH); //Disable VL53L5CX Low Power mode

  /* Initialize Serial */
  Serial.begin(115200);
  while (!Serial) {
    ; /* wait for serial port to connect. Needed for native USB port only */
  }
  //delay(1000);
  Serial.println("Booting...");


  /* Initialize I2C */
  Wire.begin(); // This resets I2C bus to 100kHz
  Wire.setClock(1000000); //Sensor has max I2C freq of 1MHz

  /* Initialize SD */
  InitSD();

  /* Initialize camera */
  InitCamera();


  /* Initialize VL53L5CX */
  InitToF();

}

/* Data capture done inside loop */
void loop()
{
  // Poll sensor for new data
  if (lecture_number < TOTAL_MEASUREMENTS)
  {
    getPicture();
    getToFsample();

  }
  else
  {
    SystemSleep();
  }

  lecture_number++;
  delay(1000); // 1s delay between polling

}


void SystemSleep()
{
  Serial.println("End.");
  digitalWrite(PwrEn, LOW); //Disable VL53L5CX
  while (1);
}
