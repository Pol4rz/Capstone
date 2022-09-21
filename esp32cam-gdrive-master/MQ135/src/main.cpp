#include <Arduino.h>
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#include <MQUnifiedsensor.h>

#define REPORTING_PERIOD_MS 1000

#define placa "ESP-32"
#define Voltage_Resolution 3.3
#define pin 34
#define type "MQ-135"
#define ADC_Bit_Resolution 12
#define RatioMQ135CleanAir 3.6  
double CO2 = (0);  
MQUnifiedsensor MQ135(placa, Voltage_Resolution, ADC_Bit_Resolution, pin, type);


 
void setup() 
{ 	
    Serial.begin(115200);   
    if(Serial) Serial.println("Serial is open");  

    //Set math model to calculate the PPM concentration and the value of constants   
    MQ135.setRegressionMethod(1); //_PPM =  a*ratio^b   
    MQ135.setA(110.47); 
    MQ135.setB(-2.862); 
    // Configurate the ecuation values to get NH4 concentration    
    MQ135.init();    
    Serial.print("Calibrating please wait.");   
    float calcR0 = 0;   
    for(int i = 1; i<=10; i ++)   {     
        MQ135.update(); // Update data, the arduino will be read the voltage on the analog pin     
        calcR0 += MQ135.calibrate(RatioMQ135CleanAir);    
        Serial.print(".");   
    }   
    MQ135.setR0(calcR0/10);   
    Serial.println("  done!.");      
    if(isinf(calcR0)) { Serial.println("Warning: Conection issue founded, R0 is infite (Open circuit detected) please check your wiring and supply"); while(1);}   
    if(calcR0 == 0){Serial.println("Warning: Conection issue founded, R0 is zero (Analog pin with short circuit to ground) please check your wiring and supply"); while(1);}   
    /*****************************  MQ CAlibration **************************/                   
    MQ135.serialDebug(false); 
}
 
void loop() { 	
    MQ135.update(); // Update data, the arduino will be read the voltage on the analog pin   
    CO2 = MQ135.readSensor(); // Sensor will read CO2 concentration using the model and a and b values setted before or in the setup   
    Serial.print("CO2: ");   
    Serial.println(CO2);  	
    delay(5000); 
}