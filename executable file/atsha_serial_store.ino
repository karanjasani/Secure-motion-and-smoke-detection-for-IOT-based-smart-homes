#include <sha204_library.h>

#include <EEPROM.h>

int addr = 0;
uint8_t serial_Number[9];
uint8_t exp_mac[MAC_RSP_SIZE];
uint8_t value;
void setup()
{ digitalWrite(13,LOW);
  Serial.begin(9600);
  for (int i = 0 ; i < EEPROM.length() ; i++)
  {
    EEPROM.write(i, 0);
    
  }
//atsha serial number in master uno
serial_Number[0] = 1;
serial_Number[1] = 35;
serial_Number[2] = 54;
serial_Number[3] = 16;
serial_Number[4] = 76;
serial_Number[5] = 234;
serial_Number[6] = 211;
serial_Number[7] = 219;
serial_Number[8] = 238;

//hmac
exp_mac[0] = 0x23;
exp_mac[1] = 0x06;
exp_mac[2] = 0x67;
exp_mac[3] = 0x00;
exp_mac[4] = 0x4F;
exp_mac[5] = 0x28;
exp_mac[6] = 0x4D;
exp_mac[7] = 0x6E;
exp_mac[8] = 0x98;
exp_mac[9] = 0x62;
exp_mac[10] = 0x04;
exp_mac[11] = 0xF4;
exp_mac[12] = 0x60;
exp_mac[13] = 0xA3;
exp_mac[14] = 0xE8;
exp_mac[15] = 0x75;
exp_mac[16] = 0x8A;
exp_mac[17] = 0x59;
exp_mac[18] = 0x85;
exp_mac[19] = 0xA6;
exp_mac[20] = 0x79;
exp_mac[21] = 0x96;
exp_mac[22] = 0xC4;
exp_mac[23] = 0x8A;
exp_mac[24] = 0x88;
exp_mac[25] = 0x46;
exp_mac[26] = 0x43;
exp_mac[27] = 0x4E;
exp_mac[28] = 0xB3;
exp_mac[29] = 0xDB;
exp_mac[30] = 0x58;
exp_mac[31] = 0xA4;
exp_mac[32] = 0xFB;
exp_mac[33] = 0xE5;
exp_mac[34] = 0x73;

for(int j=0;j<=8;j++)
{ EEPROM.write(addr,serial_Number[j]);
  value = EEPROM.read(addr);
   Serial.print(addr);
  Serial.print("\t");
  Serial.print(value);
  Serial.println();
  addr++;
  
  if(addr==EEPROM.length())
  {addr=0;}
  
 delay(100);

}

for(int k=0;k<=34;k++)
{ EEPROM.write(addr,exp_mac[k]);
  value = EEPROM.read(addr);
   Serial.print(addr);
  Serial.print("\t");
  Serial.print(value);
  Serial.println();
  addr++;
  
  if(addr==EEPROM.length())
  {addr=0;}
  
 delay(100);

}

digitalWrite(13,HIGH);
delay(5000);
digitalWrite(13,LOW);


}

void loop() {

 
}
