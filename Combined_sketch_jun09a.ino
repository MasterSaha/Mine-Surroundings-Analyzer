
int cycle = -10;
#include "DHT.h"           //DHT11
#define DHTPIN 7           //DHT11
#define DHTTYPE DHT11      //DHT11
DHT dht(DHTPIN, DHTTYPE);  //DHT11
float temper,t2,t3,t4,t5,t6,t7,t8,t9,t10;
boolean fl_temp=LOW;
boolean fl_air=LOW;
boolean fl_inf=LOW;
boolean fl_pres=LOW;
boolean fl_humd=LOW;
int fl_alert=0;
int sensorValue;   //mq135
int digitalValue;  //mq135

#include <SFE_BMP180.h>    //BMP180
#include <Wire.h>          //BMP180
SFE_BMP180 pressure;       //BMP180
#define ALTITUDE 1655.0    //BMP180

float sumis;
float pres;
double T,P,p0,a;                   //BMP-180

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
Serial.println(F("Test Begin or Restart............................!!!!!!"));   //Test Begin
  dht.begin();             //DHT11
  
pinMode(50,OUTPUT);
pressure.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(2000);
  float h = dht.readHumidity();             //DHT11
  float f = dht.readTemperature(true);      //DHT11
float hif = dht.computeHeatIndex(f, h);
sensorValue = analogRead(0);                //mq135  // read analog input pin 0
digitalValue = digitalRead(22);             //mq135

 float sensor_volt;                                    //mq9
 float RS_air;                                         //mq9     //  Rs in clean air 
 float R0;                                             //mq9     // R0 in 1000 ppm LPG 
 float sensorValue_mq;                                 //mq9
 float ratio;                                          //mq9
//Average  MQ-9                                        //mq9
 sensorValue_mq = analogRead(A7);                      //mq9
 sensor_volt = ((float)sensorValue_mq/1024)*5.0;       //mq9
 RS_air = (5.0-sensor_volt)/sensor_volt;               //mq9 // Depend on RL on yor module 
 R0 = 0.23;                                            //mq9 // According to MQ9 Calculated test Value 
 ratio = RS_air/R0;                                    //mq9 //ratio = RS/R0 
//Average_end MQ-9                                     //mq9

Serial.print("\n\nNew Cycle : "); //check
Serial.println(cycle);
  pressure.startPressure(3);         //BMP-180
  pressure.startTemperature();
  pressure.getTemperature(T); 
  pressure.getPressure(P,T);         //BMP-180
  float temp = dht.readTemperature();   //DHT11
  temp = temp * 1.8 + 32;               // convert Celsius to Farenheit
  //delay(2000);
  Serial.println("\n");
  Serial.println("DHT-11");
  Serial.print("Temperature: "); //DHT11
  Serial.print(temp);               //DHT11
  //Serial.print(hif);             //DHT11
  Serial.println(F("°F"));       //DHT11
  Serial.println("DHT-11 END \n\n");
  Serial.println("MQ-135");
  Serial.print("Air Quality :  ");
  Serial.println(sensorValue, DEC);     //mq135   // prints the value read
  Serial.println("MQ-135   END \n\n");
  Serial.println("MQ-9");
  Serial.print("sensor_volt = ");                       //mq9
  Serial.print(sensor_volt);                            //mq9
  Serial.println("V");                                  //mq9
  Serial.print("RS_air/R0 Ratio = ");                   //mq9
  Serial.println(ratio);                                //mq9
  Serial.println("MQ-9 END \n\n");
  Serial.println("BMP-180");
  Serial.print(P*0.0295333727,2);
  Serial.println(" inHg");
  Serial.println("BMP-180 END\n\n");

  
 // delay(5000);                                                      //Delay
 

  //t2=temper;
  //Serial.println(t2);              Used while testing...
  t10=t9;
  //Serial.println(t10);
  t9=t8;
  t8=t7;
  t7=t6;
  //Serial.println(t7);
  t6=t5;
  t5=t4;
  //Serial.println(t6);
  t4=t3;
  t3=t2;
  temper = dht.readTemperature();
  temper = temper * 1.8 + 32;
  Serial.print("Temper  ");
  Serial.println(temper);
  t2=temper;
  
  sumis=temper+t2+t3+t4+t5+t6+t7+t8+t9+t10;
  float humd = dht.readHumidity();
  float air = sensorValue;
  float sum = sumis;
  float pres = P*0.0295333727;

float mean;
mean = sumis/10;           // Count of values for calculating mean is 10
float bug;
bug = temp-mean;
float zscore;
zscore = bug/7;            //Standard Deviation to be 7
//////////////////////////////Values and flag check - Part 1
float ub=0.10;             //Upper Bound for Z-Score

if(cycle>=0){
if(ub<zscore){
  fl_temp=HIGH;
  }
}
if(air>700){
    fl_air=HIGH;
    }
    if(ratio<5){
      fl_inf=HIGH;
      }
      if(humd<50){
        fl_humd=HIGH;
        }
        if(pres>40){
          fl_pres=HIGH;
          }
fl_alert=fl_temp+fl_air+fl_inf+fl_humd+fl_pres;
if(fl_alert>0){
digitalWrite(50,HIGH);
}
/////////////////////////////////End of Values check - part 1
Serial.print("Z-Score  ");
Serial.println(zscore);
Serial.print("Mean ");
Serial.println(mean);
Serial.print("Flag Temp  ");
Serial.println(fl_temp);
Serial.print("Flag Air  ");
Serial.println(fl_air);
Serial.print("Flag Inf  ");
Serial.println(fl_inf);
Serial.print("Flag Pres  ");
Serial.println(fl_pres);
Serial.print("Flag Humd  ");
Serial.println(fl_humd);
Serial.print("Flag Alert  ");
Serial.println(fl_alert);
cycle= cycle+1;           //Cycle Increment
//delay(5000);                                                      //Delay
}
