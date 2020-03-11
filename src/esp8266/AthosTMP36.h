#ifdef ATH_TMP36

//Try to avoid having anything in global scope
int TMP36_sensorPin = 0;
    
void TMP_Setup()
{
}

float TMP36_readTemperature() {
    int reading = analogRead(TMP36_sensorPin);
    // measure the 3.3v with a meter for an accurate value
    //In particular if your Arduino is USB powered
    float voltage = reading * 3.3;
    voltage /= 1024.0;

    // now print out the temperature
    return (voltage - 0.5) * 100;
}

void TMP_Loop()
{

    Serial.print(TMP36_readTemperature());
    Serial.println(" degrees C");
}

#endif
