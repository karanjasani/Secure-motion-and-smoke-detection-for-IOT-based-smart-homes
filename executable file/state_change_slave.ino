//i2c Slave Code at com3


// P= HIGH PASSIVE LED Q=LOW PASSIVE LED
// G= ACTIVATE GAS SENSOR  H=GAS LED LOW 
// A=HIGH ACTIVE LED  B=HIGH ACTIVE LED   
//M=MOTION SENSOR LED HIGH    N=MOTION SENSOR LED LOW


#include <Wire.h>

int gas_sensor=A0; 
int gas_value=0;
int gas_led=6;
  
int ledPin = 5;//device 4

int passiveLed=7;//device 2
int activeLed=8;//device 3
void setup()

{  Wire.begin();
  //esp8266.begin(115200);
  Serial.begin(115200);
  
  pinMode(passiveLed,OUTPUT);
  digitalWrite(passiveLed,LOW);
  pinMode(activeLed,OUTPUT);
  digitalWrite(activeLed,LOW);
  pinMode(ledPin,OUTPUT);
  digitalWrite(ledPin,LOW);
  pinMode(gas_led,OUTPUT);
  digitalWrite(gas_led,LOW);

}

void loop()
{  Wire.begin(2);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
 
    }
    
void receiveEvent(int howMany)
{
  while(Wire.available())
  {
    char c = Wire.read();
    
    if(c == 'P')                                       
    {//Serial.println(c);
      digitalWrite(passiveLed,HIGH);
      //delay(3000);
    }
     if(c == 'Q')
    {//Serial.println(c);
      digitalWrite(passiveLed,LOW);
      //delay(3000);
    }

     if(c == 'A')                                     
    {//Serial.println(c);
      digitalWrite(activeLed,HIGH);
      //delay(3000);
    }   
     if(c == 'B')
    {//Serial.println(c);
      digitalWrite(activeLed,LOW);
      }
      
      if(c == 'M')                                  
    {
       
      digitalWrite(ledPin,HIGH);
   }
      
    if(c == 'N')                                      
    {digitalWrite(ledPin,LOW);}

     if(c == 'G')                                       
    {
  gas_value=analogRead(gas_sensor);
   
  if(gas_value>500)
  
  {digitalWrite(gas_led, HIGH);
     delay(4000);
     delay(4000);
     delay(4000);}
  
  else 
  {
   digitalWrite(gas_led, LOW);
  }
 

}
if(c == 'H')                                 
    {digitalWrite(gas_led,LOW);}
}
}

void requestEvent()
{gas_value=analogRead(gas_sensor);
   Wire.write(gas_value); 
}



