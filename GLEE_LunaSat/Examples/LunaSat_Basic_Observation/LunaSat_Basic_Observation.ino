#include <Arduino.h>

// The GLEE team is developing library architecture to enable easy programming of the lunasat 
#include <LunaSat.h>    

// Set lunasat configuration (1's equates to default mode)
int lunaSatConfig[5] = {1,1,0,1,1}; // {TMP117, ICM20602, AK09940, TIPS1385, SX1272} Need to add cap

// LunaSat object initialization is used for declaration of parameters such as ID and debugging mode
// Can be easily expanded to account for different axes of LunaSat configuration
LunaSat lunaSat(0, lunaSatConfig, false);

// Custom datatypes allow for sample specialization, user can craft their own ideal sample
lunaSat_sample_t sample;  

void setup() {
    // The GLEE library architecture enables easy, interpretable and familiar programming of the lunasat
    lunaSat.begin(9600);    // Direct serial communications with computer
    delay(5000);
}

void loop() {
    // Simple fetching of sensor observation using lunasat class 
    // Later versions will allow for dynamic sampling based on user defined config
    sample = lunaSat.getSample(); 

    // Simple examples of interacting with an observation sample
    lunaSat.dispSample(sample); // Observation samples can be directly displayed via serial
    
    //lunaSat.transmitSample(sample);  // Observation samples can be just as easily transmitted to a reciever
    
    //lunaSat.dispAccel(sample.acceleration);
    
    delay(100);
}