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
float pesos[10][5]=	{{0.44435 -2.293 1.3138 -1.4886 -1.375}
     {-0.033387 5.7369 -0.62415 -0.29212 3.8119}
     {1.863 5.1253 1.0754 6.6832 -2.5477}
     {0.13805 9.4413 1.9507 4.7973 -2.066}
     {-2.7207 -6.1352 -5.5332 -0.73735 -0.44178}
     {1.7368 0.21042 3.2907 0.12012 -3.9488}
     {-5.5105 -2.3035 -5.8404 0.90441 -4.8049}
     {5.5815 2.6916 1.3844 1.0309 -0.24739}
     {3.9305 0.76659 1.9925 0.41152 -5.0029}
     {-0.2725 1.8422 -3.2891 -0.21919 -0.076324}}
float entradas[5];  
float sum_salidas=0;
float normy,y; 
float bias2=1.619;
float sum[10]; 
float xw[10][5]; 
float ofx[10]; 
float fx[10]; 
float normx[5]; 


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
entradas[0]=100;
entradas[1]=20;
entradas[2]=20;
entradas[3]=70;
entradas[4]=90;

//se normalizan entradas
normx[0]=(entradas[0]-0)*(1-(-1))/(100-0)+(-1);
normx[1]=(entradas[1]-20)*(1-(-1))/(70-20)+(-1);
normx[2]=(entradas[2]-20)*(1-(-1))/(70-20)+(-1);
normx[3]=(entradas[3]-0)*(1-(-1))/(118-0)+(-1);
normx[4]=(entradas[4]-9)*(1-(-1))/(180-9)+(-1);   
//INICIA RED NEURONAL
   for(int i=0;i<10;i++){ 
     for(int j=0;j<5;j++){
          xw[i][j]=pesos[i][j]*normx[j];
          sum[i]=sum[i]+xw[i][j];
      }
      fx[i]=1/(1+exp(-1*(sum[i]+wbias[i])));
      ofx[i]=fx[i]*o[i]; 
      sum_salidas=ofx[i]+sum_salidas; 
    } 
     normy=1/(1+exp((sum_salidas+bias2)*-1)); //Activación
     y=(((normy-0)*2)/1)+1;  

        for(int v=0;v<10;v++){
		sum[v]=0;
	}
	sum_salidas=0;

}
