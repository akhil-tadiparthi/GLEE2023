// LunaSat Class contains core lunasat functionality such as transmission and sensing
#include <Arduino.h>
#include <Wire.h>
#include "LunaSat.h"

//Constructor: Initializes lunasat and subsequent sensors
LunaSat::LunaSat(int _id, int _conf[5], bool _debug){                             
    // Set lunaSat info
    info.id = _id;
    
    for(int i = 0; i < 5; i++){ // Might need to replace this with more optimal refrencing
        info.conf[i] = _conf[i];
    }

    // Initialize sensors

    // Set private variables
    debug = _debug;

    if(info.conf[0]){
        tmp117 = new TMP117(1,_debug);
    }

    if(info.conf[1]){
        icm20602 = new ICM20602(2,_debug);
    }
    
    if(info.conf[2]){
        ak09940 = new AK09940(3,_debug);
    }
    

    // Set indicator LED pin modes
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);

    //TODO: Handle passing modes/configurations as arguments into sensor constructors
    //TODO: LunaSat class should have has its own begin function which begins transmission with other sensors)
}

void LunaSat::begin(int baudRate){
    // The begin function starts sierial communications at the given baud rate.
    Serial.begin(baudRate);
    if (debug){
        Serial.println("LunaSat has begun serial communications");
    }


    // Sensor specific begins and initializations
    if(info.conf[1]){
        icm20602->begin();
        icm20602->initialize();
    }
    

    // TODO: Implement sensor begin outside of constructor classes implement
    // tmp begin
    // mag begin 
}

lunaSat_sample_t LunaSat::getSample(void){
    lunaSat_sample_t sample;
    if(debug){
        // Debug test functonality passes appropriatley type values for testing library interfacing

        sample.timeStamp = millis();
        sample.temperature = tmp117->getTemperatureC_fuzzed();
        sample.acceleration = icm20602->getGAccel_fuzzed(AFS_2G);
        sample.magnetic = ak09940->getRawData_fuzzed();
    }else{
        sample.timeStamp = millis();
        sample.temperature = (info.conf[0] == 1) ? tmp117->getTemperatureC() : 0;
        
        if(info.conf[1]){
            sample.acceleration = icm20602->getGAccel(AFS_2G);
        }else{
            sample.acceleration.x = 0;
            sample.acceleration.y = 0;
            sample.acceleration.z = 0;
        }
        
        if(info.conf[2]){
            sample.magnetic = ak09940->getrawData();
        }else{
            sample.magnetic.x = 0;
            sample.magnetic.y = 0;
            sample.magnetic.z = 0;
        }
    }
    return sample;
}


void LunaSat::dispSample(lunaSat_sample_t sample){
    Serial.print(sample.timeStamp);
    Serial.print(',');
    if(info.conf[0]){
        Serial.print(sample.temperature);
        Serial.print(',');
    }
    if(info.conf[1]){
        Serial.print(sample.acceleration.x);
        Serial.print(',');
        Serial.print(sample.acceleration.y);
        Serial.print(',');
        Serial.print(sample.acceleration.z);
        Serial.print(',');
    }
    if(info.conf[2]){
        Serial.print(sample.magnetic.x);
        Serial.print(',');
        Serial.print(sample.magnetic.y);
        Serial.print(',');
        Serial.print(sample.magnetic.z);
    }
    Serial.println("");
}

void LunaSat::blink(int _LED, int _delay){
    int pin;
    
    if (_LED == 1){
        pin = LED1;
    }else if(_LED == 2){
        pin = LED2;
    }else{
        // Invalid Pin
    }

    digitalWrite(pin, HIGH);
    delay(_delay);
    digitalWrite(pin, LOW);
    delay(_delay);
}