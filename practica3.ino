// This #include statement was automatically added by the Particle IDE.
#include <PietteTech_DHT.h>
#include <Arduino.h>
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
//int pinBuzzer = D0;
//int pinVentilador = D1;
int anguloInicial = 10;
int anguloFinal = 150;
//int LEDr = D6;          //Advertencia
//int LEDy = D7;          //Lámpara
int humedad;
int temperatura;

//Constantes
int minBPM = 60;
int maxTemp = 30;
int maxHum = 75;
int minLum = 400;
int maxAireQ = 500;
int sLeds;

//Semáforo de LEDs
int LEDuno = D0;
int LEDdos = D1;
int LEDtres = D2;
int LEDcuatro = D3;
int LEDcinco = D5; 

//Rangos para la normalización
//Temperatura
int tRI = 20;
int tRS = 50;
//Humedad
int hRI = 10;
int hRS = 100;
//Luminosidad
int lRI = 0;
int lRS = 4096;
//BPM
int bpmRI = 0;
int bpmRS = 100;
//Calidad del aire
int airqRI = 0;
int airqRS = 4096;

//Salida difusa
float sDifusa;

int DB [104] [6] = {
     {1,     1,     1,     1,     1,     1},
     {1,     1,     2,     2,     2,     1},
     {1,     1,     3,     3,     3,     2},
     {1,     1,     4,     4,     4,     2},
     {1,     1,     5,     5,     5,     1},
     {1,     1,     1,     5,     1,     2},
     {1,     1,     2,     4,     2,     1},
     {1,     1,     3,     3,     3,     2},
     {1,     1,     4,     2,     4,     2},
     {1,     1,     5,     1,     5,     1},
     {1,     2,     1,     1,     1,     1},
     {1,     2,     2,     2,     2,     1},
     {1,     2,     3,     3,     3,     3},
     {1,     2,     4,     4,     4,     3},
     {1,     2,     5,     5,     5,     4},
     {1,     2,     1,     5,     1,     2},
     {1,     2,     2,     4,     2,     2},
     {1,     2,     3,     3,     3,     2},
     {1,     2,     4,     2,     4,     3},
     {1,     2,     5,     1,     5,     3},
     {1,     3,     1,     1,     1,     1},
     {2,     3,     2,     2,     2,     2},
     {2,     3,     3,     3,     3,     3},
     {2,     3,     4,     4,     4,     3},
     {2,     3,     5,     5,     5,     4},
     {2,     3,     1,     5,     1,     2},
     {2,     3,     2,     4,     2,     3},
     {2,     3,     3,     3,     3,     3},
     {2,     3,     4,     2,     4,     3},
     {2,     3,     5,     1,     5,     3},
     {2,     4,     1,     1,     1,     2},
     {2,     4,     2,     2,     2,     2},
     {2,     4,     3,     3,     3,     3},
     {2,     4,     4,     4,     4,     4},
     {2,     4,     5,     5,     5,     4},
     {2,     4,     1,     5,     1,     3},
     {2,     4,     2,     4,     2,     3},
     {2,     4,     3,     3,     3,     3},
     {2,     4,     4,     2,     4,     3},
     {2,     4,     5,     1,     5,     3},
     {2,     5,     1,     1,     1,     3},
     {3,     5,     2,     2,     2,     3},
     {3,     5,     3,     3,     3,     4},
     {3,     5,     4,     4,     4,     4},
     {3,     5,     5,     5,     5,     3},
     {3,     5,     1,     5,     1,     3},
     {3,     5,     2,     4,     2,     3},
     {3,     5,     3,     3,     3,     3},
     {3,     5,     4,     2,     4,     4},
     {3,     5,     5,     1,     5,     4},
     {3,     1,     1,     1,     1,     1},
     {3,     1,     2,     2,     2,     2},
     {3,     1,     3,     3,     3,     3},
     {3,     1,     4,     4,     4,     3},
     {3,     1,     5,     5,     5,     4},
     {3,     1,     1,     1,     1,     1},
     {3,     1,     2,     2,     2,     2},
     {3,     1,     3,     3,     3,     3},
     {3,     1,     4,     4,     4,     3},
     {3,     1,     5,     5,     5,     4},
     {3,     2,     1,     5,     1,     2},
     {4,     2,     2,     4,     2,     3},
     {4,     2,     3,     3,     3,     3},
     {4,     2,     4,     2,     4,     3},
     {4,     2,     5,     1,     5,     3},
     {4,     2,     1,     1,     1,     2},
     {4,     2,     2,     2,     2,     2},
     {4,     2,     3,     3,     3,     3},
     {4,     2,     4,     4,     4,     4},
     {4,     2,     5,     5,     5,     4},
     {4,     3,     1,     5,     1,     3},
     {4,     3,     2,     4,     2,     3},
     {4,     3,     3,     3,     3,     3},
     {4,     3,     4,     2,     4,     3},
     {4,     3,     5,     1,     5,     4},
     {4,     3,     1,     1,     1,     2},
     {4,     3,     2,     2,     2,     3},
     {4,     3,     3,     3,     3,     3},
     {4,     3,     4,     4,     4,     4},
     {4,     3,     5,     5,     5,     4},
     {4,     4,     1,     5,     1,     3},
     {5,     4,     2,     4,     2,     3},
     {5,     4,     3,     3,     3,     4},
     {5,     4,     4,     2,     4,     4},
     {5,     4,     5,     1,     5,     4},
     {5,     4,     1,     1,     1,     2},
     {5,     4,     2,     2,     2,     3},
     {5,     4,     3,     3,     3,     4},
     {5,     4,     4,     4,     4,     4},
     {5,     4,     5,     5,     5,     5},
     {5,     5,     1,     5,     1,     3},
     {5,     5,     2,     4,     2,     4},
     {5,     5,     3,     3,     3,     4},
     {5,     5,     4,     2,     4,     4},
     {5,     5,     5,     1,     5,     4},
     {5,     5,     1,     1,     1,     1},
     {5,     5,     2,     2,     2,     3},
     {5,     5,     3,     3,     3,     3},
     {5,     5,     4,     4,     4,     4},
     {5,     5,     5,     5,     5,     1},
     {5,     0,     0,     0,     0,     1},
     {0,     0,     0,     0,     5,     1},
     {0,     0,     0,     1,     0,     1},
     {0,     5,     0,     0,     0,     1}};

//Entradas normalizadas
double nTemp;
double nHum;
double nBPM;
double nLuz;
double nAirQ;


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
// pinMode(LEDy, OUTPUT);
// pinMode(LEDr, OUTPUT);
// pinMode(pinBuzzer, OUTPUT);
// pinMode(pinVentilador, OUTPUT);
 myServo.attach(A5);
 myServo.write(anguloInicial); ////////????????????
//Semáforo de LEDs
 pinMode(LEDuno, OUTPUT);
 pinMode(LEDdos, OUTPUT);
 pinMode(LEDtres, OUTPUT);
 pinMode(LEDcuatro, OUTPUT);
 pinMode(LEDcinco, OUTPUT);
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

//Normalización de entradas
nTemp = (temperatura-tRI)/tRS;
constrain(nTemp,0,1); //Mantenemos el valor normalizado dentro del rango.

nHum = (humedad-hRI)/hRS;
constrain(nHum,0,1); //Mantenemos el valor normalizado dentro del rango.

nLuz = (fotorresistor-lRI)/lRS;
constrain(nLuz,0,1); //Mantenemos el valor normalizado dentro del rango.

nAirQ = (aireQ-airqRI)/airqRS;
constrain(nAirQ,0,1); //Mantenemos el valor normalizado dentro del rango.

nBPM = (BPM-bpmRI)/bpmRS;
constrain(nBPM,0,1);  //Mantenemos el valor normalizado dentro del rango.

double inSensores[5] = {nTemp, nHum, nLuz, nAirQ, nBPM};

sDifusa = FuzzySysT1(inSensores, DB); //Salida del sistema difuso.

sLeds = round(sDifusa * 255);
    Blynk.virtualWrite(V13, sDifusa);
//Condiciones se encienden para valor de sLeds
//Las salidas a pines virtuales son del pin V8 en adelante
if((0<sLeds) && (sLeds<51)){
    digitalWrite(LEDuno, HIGH);
    Blynk.virtualWrite(V8, 255);
    
    digitalWrite(LEDdos, LOW);
    Blynk.virtualWrite(V9, 0);
    
    digitalWrite(LEDtres, LOW);
    Blynk.virtualWrite(V10, 0);
    
    digitalWrite(LEDcuatro, LOW);
    Blynk.virtualWrite(V11, 0);
    
    digitalWrite(LEDcinco, LOW);
    Blynk.virtualWrite(V12, 0);
}
else if((51<sLeds) && (sLeds<102)){
    digitalWrite(LEDuno, LOW);
    Blynk.virtualWrite(V8, 0);
    
    digitalWrite(LEDdos, HIGH);
    Blynk.virtualWrite(V9, 255);
    
    digitalWrite(LEDtres, LOW);
    Blynk.virtualWrite(V10, 0);
    
    digitalWrite(LEDcuatro, LOW);
    Blynk.virtualWrite(V11, 0);
    
    digitalWrite(LEDcinco, LOW);
    Blynk.virtualWrite(V12, 0); 
}
else if((102<sLeds) && (sLeds<153)){
    digitalWrite(LEDuno, LOW);
    Blynk.virtualWrite(V8, 0);
    
    digitalWrite(LEDdos, LOW);
    Blynk.virtualWrite(V9, 0);
    
    digitalWrite(LEDtres, HIGH);
    Blynk.virtualWrite(V10, 255);
    
    digitalWrite(LEDcuatro, LOW);
    Blynk.virtualWrite(V11, 0);
    
    digitalWrite(LEDcinco, LOW);
    Blynk.virtualWrite(V12, 0);    
}
else if((153<sLeds) && (sLeds<204)){
    digitalWrite(LEDuno, LOW);
    Blynk.virtualWrite(V8, 0);
    
    digitalWrite(LEDdos, LOW);
    Blynk.virtualWrite(V9, 0);
    
    digitalWrite(LEDtres, LOW);
    Blynk.virtualWrite(V10, 0);
    
    digitalWrite(LEDcuatro, HIGH);
    Blynk.virtualWrite(V11, 255);
    
    digitalWrite(LEDcinco, LOW);
    Blynk.virtualWrite(V12, 0);  
}
else if(204<sLeds){
    digitalWrite(LEDuno, LOW);
    Blynk.virtualWrite(V8, 0);
    
    digitalWrite(LEDdos, LOW);
    Blynk.virtualWrite(V9, 0);
    
    digitalWrite(LEDtres, LOW);
    Blynk.virtualWrite(V10, 0);
    
    digitalWrite(LEDcuatro, LOW);
    Blynk.virtualWrite(V11, 0);
    
    digitalWrite(LEDcinco, HIGH);
    Blynk.virtualWrite(V12, 255);     
}

}

//Sistema difuso
int reglas=104;
int entradas = 5;//NTI=NTV-1;
double trapezoidmf(double x,double a,double b,double c,double d) {
double mf=max(min(min((x-a)/(b-a+0.000001),1),(d-x)/(d-c+0.0000001)),0);
return mf;
}
double trianglemf(double x,double a,double b,double c) {
double mf=max(min((x-a)/(b-a+0.000001),(c-x)/(c-b+0.000001)),0);
return mf;
}
double Type1FS(double x,int n,double V[]) {
double a=V[0];
double b=V[1];
double c=V[2];
double mf;
if (n== 1){
mf=trapezoidmf(x,a-1.0001,a,b,c);
return mf;
}
if (n==2){
mf=trianglemf(x,a,b,c);
return mf;
}
if (n==3){
mf=trianglemf(x,a,b,c);
return mf;
}
if (n==4){
mf=trianglemf(x,a,b,c);
return mf;
}
if (n==5){
mf=trapezoidmf(x,a,b,c,c+1);
return mf;
}
if (n==0){
mf=1;
return mf;
}
}
double FuzzySysT1(double X[],int DB[][6])
{
double PARAM[5][3]={{-1.0, -0.6666, -0.3333}
,{-0.6666, -0.3333, 0}
,{-0.3333, 0, 0.3333}
,{0, 0.3333, 0.6666}
,{0.3333, 0.6666, 1}};
double V[3];
double AC[5]={0,0.25,0.5,0.75,1};
double Fo[104];
for(int r=0;r<=(reglas-1);r++)
{
double sumin=1;
int n;
for(int i=0;i<=(entradas-1);i++)
{
n=DB[r][i]-1;
if(n>-1){
V[0]=PARAM[n][0];
V[1]=PARAM[n][1];
V[2]=PARAM[n][2];
}
double mf=Type1FS(X[i],(n+1),V);
sumin=min(sumin,mf);
}
Fo[r] = sumin;
}
double sum1=0; double sum2=0.00000001;
for(int r=0;r<=(reglas);r++)
{
sum1=(sum1+(Fo[r]*AC[DB[r][2]-1]));
sum2=(sum2+Fo[r]);
}
double y=sum1/sum2;
return y;
}
