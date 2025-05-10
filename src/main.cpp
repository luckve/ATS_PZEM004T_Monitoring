#include <PZEM004Tv30.h>

#define RX_PIN 17 // Change to the appropriate pin on ESP32
#define TX_PIN 16 // Change to the appropriate pin on ESP32
#define relay1 4
#define relay2 5
#define relay3 6
#define relay4 7

// Create a HardwareSerial object
HardwareSerial pzemSerial(1); // Use Serial1 (you can also use Serial2)

PZEM004Tv30 pzem(pzemSerial, RX_PIN, TX_PIN); // Pass the hardware serial and pins

void setup() {
  Serial.begin(115200);
  
  // Initialize the hardware serial
  pzemSerial.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);

  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);

  Serial.println("AC Power Monitoring with ESP32 and PZEM-004T");
}

void loop() {
    Serial.print("Custom Address:");
    Serial.println(pzem.readAddress(), HEX);

    // Read the data from the sensor
    float voltage = pzem.voltage();
    float current = pzem.current();
    float power = pzem.power();
    float energy = pzem.energy();
    float frequency = pzem.frequency();
    float pf = pzem.pf();

    // Check if the data is valid
    if (isnan(voltage)) {
        Serial.println("Error reading voltage");
    } else if (isnan(current)) {
        Serial.println("Error reading current");
    } else if (isnan(power)) {
        Serial.println("Error reading power");
    } else if (isnan(energy)) {
        Serial.println("Error reading energy");
    } else if (isnan(frequency)) {
        Serial.println("Error reading frequency");
    } else if (isnan(pf)) {
        Serial.println("Error reading power factor");
    } else {
        // Print the values to the Serial console
        Serial.print("Voltage: ");      Serial.print(voltage);      Serial.println("V");
        Serial.print("Current: ");      Serial.print(current);      Serial.println("A");
        Serial.print("Power: ");        Serial.print(power);        Serial.println("W");
        Serial.print("Energy: ");       Serial.print(energy, 3);   Serial.println("kWh");
        Serial.print("Frequency: ");    Serial.print(frequency, 1); Serial.println("Hz");
        Serial.print("Power Factor: "); Serial.println(pf);
    }

    Serial.println();
    delay(1000);

    // Logic to control relays based on certain conditions
    if (power < 100) {
        // Switch power source to battery
        digitalWrite(relay1, HIGH);  // Power from mains to relay 1
        digitalWrite(relay2, HIGH);   // GND from mains to relay 2
        digitalWrite(relay3, LOW);    // Power from battery to relay 3
        digitalWrite(relay4, LOW);     // GND from battery to relay 4
    } else {
        // Switch power source from battery to mains
        digitalWrite(relay1, LOW);    // Turn off relay 1
        digitalWrite(relay2, LOW);     // Turn off relay 2
        digitalWrite(relay3, HIGH);    // Power from battery to relay 3
        digitalWrite(relay4, HIGH);     // GND from battery to relay 4
    }

    delay(2000);  // Update every 2 seconds
}
