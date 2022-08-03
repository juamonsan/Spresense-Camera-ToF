/*
******************************************************************************
  Log 8x8 distance measures with ST's VL53L5CX-SATEL on a Sony's Spresense Board

  This code for the Sony Spresense Board (Main Board plus Extension Board) logs
  lectures from a VL53L5CX 8x8 ToF sensor into a file on a SD card.
  SparkFun's VL53L5CX library is used (

  Juan M. Montes - jmontes@atc.us.es
  ORCID: 0000-0002-0983-2386
  GitHub: https://github.com/juamonsan

******************************************************************************
*/

#include <Arduino.h>
#include <SDHCI.h>
#include <File.h>
#include <stdio.h>

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
  Serial.print("Insert SD card.");
  while (!SD.begin()) {
    ; /* wait until SD card is mounted. */
  }
  Serial.println("SD card mounted.");

  //  /* Create a new directory */
  //  SD.mkdir("dir/");


  /* Initialize VL53L5CX */
  myImager.setWireMaxPacketSize(128); // Increase default from 32 bytes to 128 - not supported on all platforms

  Serial.println("Initializing VL53L5CX.");
  while (myImager.begin() == false)
  {
    Serial.println(F("Connection to VL53L5CX failed. Retrying."));
    delay (1000);
  }

  myImager.setResolution(8 * 8); // Enable all 64 pads

  imageResolution = myImager.getResolution(); // Query sensor for current resolution - either 4x4 or 8x8
  imageWidth = sqrt(imageResolution);         // Calculate printing width

  // Using 4x4, min frequency is 1Hz and max is 60Hz
  // Using 8x8, min frequency is 1Hz and max is 15Hz
  myImager.setRangingFrequency(15);

  myImager.startRanging();

}

void loop()
{
  // Poll sensor for new data
  if (lecture_number < TOTAL_MEASUREMENTS)
  {

    if (myImager.isDataReady() == true)
    {
      if (myImager.getRangingData(&measurementData)) // Read distance data into array
      {


        sprintf(filename, "ToF%03d.csv", lecture_number);
        Serial.print("Saving lecture as ");
        Serial.print(filename);
        Serial.println("");

        /* Remove the old file with the same file name as new created file,
           and create new file.
        */

        SD.remove(filename);
        File myFile = SD.open(filename, FILE_WRITE);



        // The ST library returns the data transposed from zone mapping shown in datasheet
        // Pretty-print data with increasing y, decreasing x to reflect reality
        for (int y = 0; y <= imageWidth * (imageWidth - 1); y += imageWidth)
        {
          for (int x = imageWidth - 1; x >= 0; x--)
          {
            myFile.print(measurementData.distance_mm[x + y]); //Print to file
            Serial.print(measurementData.distance_mm[x + y]); //Serial print
            myFile.print(";"); //Comma for CSV
            Serial.print(","); //for serial display
          }
          myFile.println(); //New row in CSV
          Serial.println(); //for serial display
        }
        myFile.close();
        
      }
    }
  }
  else
  {
    Serial.println("End.");
    digitalWrite(PwrEn, LOW); //Disable VL53L5CX
    while (1);
  }

  lecture_number++;
  delay(1000); // 1s delay between polling

}
