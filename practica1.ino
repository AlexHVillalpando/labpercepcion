// This #include statement was automatically added by the Particle IDE.
#include <PietteTech_DHT.h>

#include <blynk.h>
// system defines
#define DHTTYPE  DHT22              // Sensor type DHT11/21/22/AM2301/AM2302
#define DHTPIN   4         	    // Digital pin for communications
#define DHT_SAMPLE_INTERVAL   5000  // Sample every minute


//declaration
void dht_wrapper(); // must be declared before the lib initialization

Servo myServo;//<---------------------------------------------------------
// Lib instantiate
PietteTech_DHT DHT(DHTPIN, DHTTYPE, dht_wrapper);

// globals
unsigned int DHTnextSampleTime;	    // Next time we want to start sample
bool bDHTstarted;		    // flag to indicate we started acquisition
int n;                              // counter

char resultstr[64]; //String to store the sensor data

char auth[] = "4d6f3e793f7b44afa3ba2155aaeb6529";

char VERSION[64] = "0.04";

#define READ_INTERVAL 5000
//Entradas
int pinfoto = A1;
int fotorresistor;
int pinBPM = A2;
int BPMr;
int BPM;
int pinAire = A0;
int aireQ;
//Salidas
int pinBuzzer = D0;
int pinVentilador = D1;
int anguloInicial = 10;
int anguloFinal = 150;
int LEDr = D6;          //Advertencia
int LEDy = D7;          //Lámpara
int humedad;
int temperatura;
//Constantes
int minBPM = 60;
int maxTemp = 30;
int maxHum = 75;
int minLum = 400;
int maxAireQ = 500;


void setup()
{

  Blynk.begin(auth);
 
 DHTnextSampleTime = 0;  // Start the first sample immediately
 Particle.variable("result", resultstr, STRING);

 Particle.publish("DHT22 - firmware version", VERSION, 60, PRIVATE);
 //Entradas
 pinMode(fotorresistor, INPUT);
 pinMode(pinAire, INPUT);
 pinMode(pinBPM,INPUT);
  //Salidas
 pinMode(LEDy, OUTPUT);
 pinMode(LEDr, OUTPUT);
 pinMode(pinBuzzer, OUTPUT);
 pinMode(pinVentilador, OUTPUT);
 myServo.attach(A5);
 myServo.write(anguloInicial); ////////????????????

}

void dht_wrapper() {
    DHT.isrCallback();
}

void loop()
{
  ////////////////////////////////////////////////////
Blynk.run(); // all the Blynk magic happens here

//ADQUISICIÓN DE DATOS
//Luminosidad
fotorresistor = analogRead(pinfoto);
//Visualización en Blynk App
Blynk.virtualWrite(V4, fotorresistor);

//Ritmo cardíaco
BPMr = analogRead(pinBPM);
Blynk.virtualWrite(V6, BPMr);
if (2500 <= BPMr && BPMr <= 3500){
BPM=random(75,90);

}
else {
    BPM = 0;
}
//Visualización en Blynk App
Blynk.virtualWrite(V5, BPM);

//Calidad del aire
aireQ = analogRead(pinAire);
//Visualización en Blynk App
Blynk.virtualWrite(V7, aireQ);

// Revisa si se necesita hacer el próximo muestreo
 if (millis() > DHTnextSampleTime) {
      
	if (!bDHTstarted) {		// comienza el muestreo
	    DHT.acquire();
	    bDHTstarted = true;
	}

 if (!DHT.acquiring()) {		// Revisa si se termino el muestreo

  float temp = (float)DHT.getCelsius();
  int temp1 = (temp - (int)temp) * 100;
  temperatura = temp1;///////////////////////////////
  char tempInChar[32];
  sprintf(tempInChar,"%0d.%d", (int)temp, temp1);
  Particle.publish("La temperatura del dht22 es:", tempInChar, 60, PRIVATE);

  //V1 es la temperatura
  Blynk.virtualWrite(V1, tempInChar);

  //google docs can get this variable
  sprintf(resultstr, "{\"t\":%s}", tempInChar);

  float humid = (float)DHT.getHumidity();
  int humid1 = (humid - (int)humid) * 100;
  humedad = humid1;////////////////////////////////
  sprintf(tempInChar,"%0d.%d", (int)humid, humid1);
  Particle.publish("La humedad del dht22 es:", tempInChar, 60, PRIVATE);
  
  //VP2 es la humedad
  Blynk.virtualWrite(V2, tempInChar);

  n++;  // incrementa el contador
  bDHTstarted = false;  // reset the sample flag so we can take another
  DHTnextSampleTime = millis() + DHT_SAMPLE_INTERVAL;  // set the time for next sample
 }
}
}
