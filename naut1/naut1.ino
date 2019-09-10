#include <TheThingsNetwork.h>
#include "DHT.h"

#include "secrets.h"

const char *appEui = SECRET_TTN_APP_EUI;
const char *appKey = SECRET_TTN_APP_KEY;

#define DHTPIN 2
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

int a1VoltagePin   = A1;
float a1VoltageOut = 0.0;
float a1VoltageIn  = 0.0;
float a1Resitor1   = 30000.0;
float a1Resistor2  = 7500.0;
int a1VoltageRead  = 0;

#define loraSerial Serial1
#define debugSerial Serial
#define freqPlan TTN_FP_EU868

TheThingsNetwork ttn(loraSerial, debugSerial, freqPlan);

void setup()
{
  pinMode(analogInput, INPUT);

  loraSerial.begin(57600);
  debugSerial.begin(9600);
  dht.begin();

  // Wait a maximum of 10s for Serial Monitor
  while (!debugSerial && millis() < 10000)
    ;

  // debugSerial.println("-- STATUS");
  // ttn.showStatus();
  //
  // debugSerial.println("-- JOIN");
  // ttn.join(appEui, appKey);
}

void loop()
{
  debugSerial.println("-- LOOP");
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  a1VoltageRead = analogRead(a1VoltagePin);
  a1VoltageOut = (a1VoltageRead * 5.0) / 1024.0;
  a1VoltageIn = a1VoltageOut / (a1Resitor1 / (a1Resitor1 + a1Resistor2));

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C Voltage 1: "));
  Serial.println(a1VoltageIn, 2);

  // Prepare payload of 1 byte to indicate LED status
  byte payload[1];
  payload[0] = (digitalRead(LED_BUILTIN) == HIGH) ? 1 : 0;

  // Send it off
  // ttn.sendBytes(payload, sizeof(payload));


  delay(30000);
}
