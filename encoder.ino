void EncoderInit()
{
    Serial.println("ENCODER INIT");
    pinMode(encoderPinA, INPUT);
    pinMode(encoderPinB, INPUT);
    // pinMode(buttonPin, INPUT_PULLUP);
    
    digitalWrite(encoderPinA, HIGH); // turn pullup resistor on
    digitalWrite(encoderPinB, HIGH); // turn pullup resistor on
    
    // call updateEncoder() when any high/low changed seen
    // on interrupt 0 (pin 2), or interrupt 1 (pin 3)
    // attachInterrupt(encoderPinA, updateEncoder, CHANGE);
    // attachInterrupt(encoderPinB, updateEncoder, CHANGE);
}
void updateEncoder()
{
    logoutTimerCnt = 0;
    gameTimerCnt = 0;

    int MSB = digitalRead(encoderPinA); // MSB = most significant bit
    int LSB = digitalRead(encoderPinB); // LSB = least significant bit
    
    int encoded = (MSB << 1) | LSB;         // converting the 2 pin value to single number
    int sum = (lastEncoded << 2) | encoded; // adding it to the previous encoded value
    
    if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderValue++;
    if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoderValue++;
    lastEncoded = encoded; // store this value for next time

}
