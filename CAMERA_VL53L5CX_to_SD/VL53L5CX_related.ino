/* Initialize VL53L5CX */
void InitToF()
{
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

/* Takes a sample and saves it to the SD */
void getToFsample()
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
