/* Initialize SD */
void InitSD ()
{
  Serial.print(F("Insert SD card."));
  while (!SD.begin()) {
    ; /* wait until SD card is mounted. */
  }
  Serial.println(F("SD card mounted."));

  //  /* Create a new directory */
  //  SD.mkdir("dir/");
}
