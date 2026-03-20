void MotorInit()
{
    //Linear Motor Init
    pinMode(BOXSWITCH_PIN,INPUT_PULLUP);
    pinMode(MOTOR_INA1_PIN, OUTPUT);
    pinMode(MOTOR_INA2_PIN, OUTPUT);
    ledcAttach(MOTOR_PWMA_PIN, MotorFreq, MotorResolution);
    ledcWrite(MOTOR_PWMA_PIN, 0);
}
void EngineSpeeed(int enginePwm)
{
    // Serial.println("Engine On");
    ledcWrite(MOTOR_PWMA_PIN, enginePwm);
    digitalWrite(MOTOR_INA1_PIN, LOW);
    digitalWrite(MOTOR_INA2_PIN, HIGH);
}

void EngineStop()
{
    Serial.println("Engine Stop");
    digitalWrite(MOTOR_INA1_PIN, LOW);
    digitalWrite(MOTOR_INA2_PIN, LOW);
}
