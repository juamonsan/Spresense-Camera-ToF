/* Camera related functions */



/* Camera Error handler */
void printError(enum CamErr err)
{
  Serial.print("Error: ");
  switch (err)
  {
    case CAM_ERR_NO_DEVICE:
      Serial.println("No Device");
      break;
    case CAM_ERR_ILLEGAL_DEVERR:
      Serial.println("Illegal device error");
      break;
    case CAM_ERR_ALREADY_INITIALIZED:
      Serial.println("Already initialized");
      break;
    case CAM_ERR_NOT_INITIALIZED:
      Serial.println("Not initialized");
      break;
    case CAM_ERR_NOT_STILL_INITIALIZED:
      Serial.println("Still picture not initialized");
      break;
    case CAM_ERR_CANT_CREATE_THREAD:
      Serial.println("Failed to create thread");
      break;
    case CAM_ERR_INVALID_PARAM:
      Serial.println("Invalid parameter");
      break;
    case CAM_ERR_NO_MEMORY:
      Serial.println("No memory");
      break;
    case CAM_ERR_USR_INUSED:
      Serial.println("Buffer already in use");
      break;
    case CAM_ERR_NOT_PERMITTED:
      Serial.println("Operation not permitted");
      break;
    default:
      break;
  }
}


/* Initialize camera */
void InitCamera()
{
  CamErr err;
  /* begin() without parameters means that
      number of buffers = 1, 30FPS, QVGA, YUV 4:2:2 format */
  Serial.println("Preparing camera...");
  err = theCamera.begin();
  if (err != CAM_ERR_SUCCESS)
  {
    printError(err);
    SystemSleep();
  }

  Serial.println("Set Auto white balance parameter");
  err = theCamera.setAutoWhiteBalanceMode(CAM_WHITE_BALANCE_DAYLIGHT);
  if (err != CAM_ERR_SUCCESS)
  {
    printError(err);
    SystemSleep();
  }

  /* Set parameters about still picture.
     In the following case, QUADVGA and JPEG.
  */

  Serial.println("Set still picture format");
  err = theCamera.setStillPictureImageFormat(
          CAM_IMGSIZE_VGA_H,
          CAM_IMGSIZE_VGA_V,
          CAM_IMAGE_PIX_FMT_JPG);
  if (err != CAM_ERR_SUCCESS)
  {
    printError(err);
    SystemSleep();
  }

}

/* Takes a picture and saves it to the SD */
void getPicture()
{
  /* Take still picture.
    Unlike video stream(startStreaming) , this API wait to receive image data
     from camera device.
  */

  Serial.println("call takePicture()");
  CamImage img = theCamera.takePicture();

  /* Check availability of the img instance. */
  /* If any errors occur, the img is not available. */

  if (img.isAvailable())
  {
    /* Create file name */

    char filename[16] = {0};
    sprintf(filename, "PICT%03d.JPG", lecture_number);

    Serial.print("Saving picture as ");
    Serial.print(filename);
    Serial.println("");

    /* Remove the old file with the same file name as new created file,
       and create new file.
    */

    SD.remove(filename);
    File myFile = SD.open(filename, FILE_WRITE);
    myFile.write(img.getImgBuff(), img.getImgSize());
    myFile.close();
  }
  else
  {
    /* The size of a picture may exceed the allocated memory size.
       Then, allocate the larger memory size and/or decrease the size of a picture.
       [How to allocate the larger memory]
       - Decrease jpgbufsize_divisor specified by setStillPictureImageFormat()
       - Increase the Memory size from Arduino IDE tools Menu
       [How to decrease the size of a picture]
       - Decrease the JPEG quality by setJPEGQuality()
    */

    Serial.println("Failed to take picture");
  }
}
