/* Initialize SD */
void InitSD ()
{
  Serial.print("Insert SD card.");
  while (!SD.begin()) {
    ; /* wait until SD card is mounted. */
  }
  Serial.println("SD card mounted.");

  //  /* Create a new directory */
  //  SD.mkdir("dir/");
}
