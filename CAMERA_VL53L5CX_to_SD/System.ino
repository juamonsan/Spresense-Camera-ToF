/* Idle state */
void SystemSleep()
{
  Serial.println("End.");
  digitalWrite(PwrEn, LOW); //Disable VL53L5CX
  while (1);
}

/* Board LEDs initialization */
void InitLEDs()
{
  pinMode(LED0, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
}

void delay_LEDs(unsigned int delay_ms)
{
  delay_ms = delay_ms >> 4; //divide by 4

  digitalWrite(LED0, HIGH);
  delay(delay_ms);
  digitalWrite(LED1, HIGH);
  delay(delay_ms);
  digitalWrite(LED2, HIGH);
  delay(delay_ms);
  digitalWrite(LED3, HIGH);
  delay(delay_ms);

  digitalWrite(LED0, LOW);
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
}
