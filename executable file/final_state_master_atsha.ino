//master at com4

// P= HIGH PASSIVE LED Q=LOW PASSIVE LED
// G= ACTIVATE GAS SENSOR  H=GAS LED LOW 
// A=HIGH ACTIVE LED  B=HIGH ACTIVE LED   
//M=MOTION SENSOR LED HIGH    N=MOTION SENSOR LED LOW


#include <Password.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
#include <Wire.h>
#include <sha204_library.h>
#include <Keypad.h>
#include <EEPROM.h>
 int check=0;
int addr = 0;
uint8_t serial_Number[9];
uint8_t value;
uint8_t status_flag;
const int sha204Pin = 13;
atsha204Class sha204(sha204Pin);

uint8_t exp_response[4];
uint8_t exp_mac[MAC_RSP_SIZE];


SoftwareSerial esp8266(2, 3);
#define DEBUG true
int calibrationTime = 30;
char incomingByte;
int minSecsBetweenEmails = 20;

long unsigned int lowIn;


long unsigned int pause = 5000;  //5000ms= 5 sec

boolean lockLow = true;
boolean takeLowTime;

//int interval = 30; //
long lastSend = -minSecsBetweenEmails * 1000l;


int pirPin = 12;

int passwd_pos = 10 ; // the postition of the password input

Password password = Password( "1234" );
const byte ROWS = 4; // Four rows
const byte COLS = 4;
byte rowPins[ROWS] = {A1,A2,A3,5};     //connect to the row pinouts of the keypad
byte colPins[COLS] = {4, 3, 2, 1};     //connect to the column pinouts of the keypad a1,a2

char keys[ROWS][COLS] = { // Define the Keymap
  {
    '1', '2', '3', 'A'
  }
  ,
  {
    '4', '5', '6', 'B'
  }
  ,
  {
    '7', '8', '9', 'C'
  }
  ,
  {
    '*', '0', '#', 'D'
  }
};
// Create the Keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(11, 10, 9, 8, 7, 6); // Assignign arduino pins to LCD display module

int alarmStatus = 0;
int alarmActive = 0;

char Key;




void setup() {



//default response
exp_response[0] = 4;
exp_response[1] = 17;
exp_response[2] = 51;
exp_response[3] = 67;



  //esp8266.begin(115200);
  Serial.begin(115200);
  pinMode(pirPin, INPUT);
  digitalWrite(pirPin, LOW);


  // your esp's baud rate might be different
  /*Serial.println("connecting to internet via esp8266");
    sendData("AT\r\n",5000,DEBUG);
    Serial.println("RESPONSE is");
    sendData("AT+RST\r\n",4000,DEBUG);// reset module
    Serial.println("VERSION IS");
    sendData("AT+GMR\r\n",4000,DEBUG); // reset module
    Serial.println("ACCESS POINTS ARE");
    sendData("AT+CWLAP\r\n",4000,DEBUG);

    sendData("AT+CWJAP=\"VOLSBB\",\"\"\r\n",3000,DEBUG);
      Serial.println(" GOT CONNECTED TO VIT WIFI ");
    delay(5000);*/
  Serial.println("HOME SECURITY SYSTEM");
  delay(2000);
 Serial.println("Sending a Wakeup Command.");
 delay_sec();
 wakeup();
 Serial.println("calibrating sensor ");
 for (int i = 0; i < calibrationTime; i++) {
   Serial.print(".");
  delay(1000);  }
  Serial.println(" done");
  Serial.println("SENSORS ARE ACTIVE");
  delay(500);
  Serial.println("Validating the hardware identity");
  delay_sec();
  check_sensors();
  
  lcd.begin(16, 2);
  displayCodeEntryScreen();
  keypad.addEventListener(keypadEvent);//function keypadEvent
 }


void loop()
{ Key=keypad.getKey();


 //call_passive();
 //call_active()
 }

// funtion-1
void keypadEvent(KeypadEvent Key)
{ //1
  switch (keypad.getState())
  {
    case PRESSED:
      if (passwd_pos - 10 >= 5) {
        return ;
      }
      lcd.setCursor((passwd_pos++), 0);

      switch (Key)
      {
        case '#':                 //# is to validate password
          passwd_pos  = 10;
          checkPassword();
          break;
        case '*':                 //* is to reset password attempt
          password.reset();
          passwd_pos = 10;
          // TODO: clear the screen output
          break;
        default:
          password.append(Key);
          lcd.print("*");
      }

  }
}



//function 2 (called from function 1)
void checkPassword() {                 // To check if PIN is corrected, if not, retry!
  if (password.evaluate())
  {
    if (alarmActive == 0 && alarmStatus == 0)
    { lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("ACTIVE!");
        Wire.begin();
  Wire.beginTransmission(2);
  Wire.write('Q');
  Wire.endTransmission();
  delay(5000);
      alarmActive = 1;
      password.reset();
      delay(2000);
       
      call_active();
      //
    }
    else if ( alarmActive == 1 || alarmStatus == 1) {
      alarmStatus = 0;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("PASSIVE!");
      //PASSIVE
      alarmActive = 0;
      password.reset();
      delay(5000);
        Wire.begin();
  Wire.beginTransmission(2);
  Wire.write('B');
  Wire.endTransmission();
      call_passive();
    }
  
  else {
    password.reset();
    lcd.clear();
    delay(5000);
    lcd.setCursor(0, 0);
    lcd.print("INVALID CODE!");
    delay(5000);
    lcd.setCursor(1, 1);
    lcd.print("TRY AGAIN!");
    delay(5000);
    displayCodeEntryScreen();
  }
}
}


// function 3
void displayCodeEntryScreen()    // Dispalying start screen for users to enter PIN
{
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Enter PIN:");
  lcd.setCursor(0, 1);
  lcd.print("Security System");
}

void displayCodeEntryScreens()    // Dispalying start screen for users to enter PIN
{
  lcd.setCursor(0, 0);
  lcd.print("Enter PIN:");
  lcd.setCursor(0, 1);
  lcd.print("Security System");
}

void call_passive() //runs for 20 loops
{//1
     
  Serial.println("PASSIVE STATE");
  //turn status led on
  Wire.begin();
  Wire.beginTransmission(2);
  Wire.write('P');
  Wire.endTransmission();
  delay(5000);
   lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("PASSIVE!");
  for (int l = 1; l <=10; l++)
  { //2 
     lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("PASSIVE!");
    check=l;
    motion_sensor_passive();
    delay(1000);
    gas_detection_passive();
    Wire.begin();
    Wire.beginTransmission(2);
    Wire.write('H');
    Wire.endTransmission();
      if(check%2==0)
   {//3
    Serial.println("CHECKING FOR STATE CHANGE");
      delay(5000);
      displayCodeEntryScreens();
      for(int l=0;l<5;l++)
        {//5
  keypad.getKey();
  keypad.getKey();
  keypad.getKey();
  keypad.getKey();
  keypad.getKey();
  delay(1000);
  keypad.getKey();
  keypad.getKey();
  keypad.getKey();
  keypad.getKey();
  keypad.getKey();
  delay(1000);
  keypad.getKey();
  keypad.getKey();
  keypad.getKey();
  keypad.getKey();
  keypad.getKey();
  delay(1000);
  keypad.getKey();
  keypad.getKey();
  keypad.getKey();
  keypad.getKey();
  keypad.getKey();
  delay(1000);
  keypad.getKey();
  keypad.getKey();
  keypad.getKey();
  keypad.getKey();
  keypad.getKey();
  delay(1000);
  keypad.getKey();
  keypad.getKey();
  keypad.getKey();
  keypad.getKey();
  keypad.getKey();
  delay(1000);
  keypad.getKey();
  keypad.getKey();
  keypad.getKey();
  keypad.getKey();
  keypad.getKey();
  delay(1000);
  keypad.getKey();
  keypad.getKey();
  keypad.getKey();
  keypad.getKey();
  keypad.getKey();
  delay(1000);
  keypad.getKey();
  keypad.getKey();
  keypad.getKey();
  keypad.getKey();
  keypad.getKey();

}//5
      
    }//3
    else
    
    { lcd.setCursor(0, 0);
      lcd.print("PASSIVE STATE!");
      
   }
 
  }//2
  
  Wire.begin();
  Wire.beginTransmission(2);
  Wire.write('Q');
  Wire.endTransmission();
  delay(5000);
  displayCodeEntryScreen();

}

void call_active()
{ Serial.println("ACTIVE STATE");

  //TURN ON RED LED
  Wire.begin();
  Wire.beginTransmission(2);
  Wire.write('A');
  Wire.endTransmission();
  delay(3000);
  
   
  for (int j = 1; j <=10; j++)// run for 50 loops
  {
    lcd.clear();
    lcd.setCursor(0, 0);
      lcd.print("ACTIVE!");

    check=j;
    motion_sensor_active();
    delay(1000);
    gas_detection_active();
    
   if(check%2==0)
   {
    Serial.println("CHECKING FOR STATE CHANGE");
      delay(5000);
      displayCodeEntryScreens();
      for(int l=0;l<5;l++)
        {
     
keypad.getKey();
  keypad.getKey();
  keypad.getKey();
  keypad.getKey();
  keypad.getKey();
  delay(1000);
  keypad.getKey();
  keypad.getKey();
  keypad.getKey();
  keypad.getKey();
  keypad.getKey();
  delay(1000);
  keypad.getKey();
  keypad.getKey();
  keypad.getKey();
  keypad.getKey();
  keypad.getKey();
  delay(1000);
  keypad.getKey();
  keypad.getKey();
  keypad.getKey();
  keypad.getKey();
  keypad.getKey();
  delay(1000);
  keypad.getKey();
  keypad.getKey();
  keypad.getKey();
  keypad.getKey();
  keypad.getKey();
  delay(1000);
  keypad.getKey();
  keypad.getKey();
  keypad.getKey();
  keypad.getKey();
  keypad.getKey();
  delay(1000);
  keypad.getKey();
  keypad.getKey();
  keypad.getKey();
  keypad.getKey();
  keypad.getKey();
  delay(1000);
  keypad.getKey();
  keypad.getKey();
  keypad.getKey();
  keypad.getKey();
  keypad.getKey();
  delay(1000);
  keypad.getKey();
  keypad.getKey();
  keypad.getKey();
  keypad.getKey();
  keypad.getKey();

}
      }
    
    else
    { lcd.setCursor(0, 0);
      lcd.print("ACTIVE!");
   }
  
  }
  
  Wire.begin();
  Wire.beginTransmission(2);
  Wire.write('B');
  Wire.endTransmission();
  displayCodeEntryScreen();

}

void motion_sensor_passive()
{ //Serial.println("1");
  for (int m = 0; m < 10; m++)
  { //Serial.println("2");

    if (digitalRead(pirPin) == HIGH)
    {
      Serial.println("MOTION SENSOR ACTIVE");

      //Serial.println("3");
      Wire.begin();
      Wire.beginTransmission(2); //
      Wire.write('M');
      Wire.endTransmission();
      delay(5000);
      delay(5000);
      delay(5000);
      delay(5000);
      delay(5000);//
      //for(int m=0;m<2;m++);
      //{ delay(5000); }
      if (lockLow) {

        lockLow = false;
        Serial.println("---");
        Serial.print("Motion detected at ");
        Serial.print(millis() / 1000);
        Serial.println(" sec");
        delay(50);
      }
      takeLowTime = true;
    }
    delay(2000);
    if (digitalRead(pirPin) == LOW)
    {
      Wire.begin();
      Wire.beginTransmission(2);
      Wire.write('N');
      Wire.endTransmission();

      if (takeLowTime) {
        lowIn = millis();
        takeLowTime = false;
      }

      if (!lockLow && millis() - lowIn > pause) {

        lockLow = true;
        Serial.print("Motion ended at ");
        Serial.print((millis() - pause) / 1000);
        Serial.println(" sec");
        delay(50);
      }
    }
  }

}

void motion_sensor_active() {
  for (int k = 0; k < 5; k++)
  {

    long now = millis();


    if (digitalRead(pirPin) == HIGH)
    { Serial.println("movement");
      delay(5000);
      digitalWrite(pirPin, LOW);
    }
    digitalWrite(pirPin, LOW);
    //digitalWrite(ledPin, LOW);
    delay(5000);
  }

}



void gas_detection_active()
{

  Serial.println("GAS SENSOR IS WORKING ");
  Serial.print("Value of gas sensor is ");

  Wire.begin();
  Wire.requestFrom(2, 1);    // request 1 bytes from slave device #2
  while (Wire.available())
  { // slave may send less than requested
    int c = Wire.read(); // receive a byte as character
    Serial.println(c);
    if (c > 500)
    {
      Serial.println("Smoke detected");
    }
  }

}

void gas_detection_passive()
{

  Serial.println("GAS SENSOR IS WORKING ");
  Serial.print("Value of gas sensor is ");
  Wire.begin();
  Wire.beginTransmission(2); // transmit to device #5
  Wire.write('G');              // sends x
  Wire.endTransmission();
  delay(4000);
  delay(4000);
  delay(4000);
  Wire.begin();
  Wire.requestFrom(2, 1);    // request 1 bytes from slave device #2
  while (Wire.available())
  { // slave may send less than requested
    int c = Wire.read(); // receive a byte as character
    Serial.println(c);

  }

}


String sendData(String command, const int timeout, boolean debug)
{
  String response = "";

  esp8266.print(command); // send the read character to the esp8266

  long int time = millis();

  while ( (time + timeout) > millis())
  {
    while (esp8266.available())
    {


      char c = esp8266.read();
      //delay(1000);
      response += c;
      //delay(1000);
    }
  }

  if (debug)
  {
    Serial.println(response);
  }

  return response;
}
byte wakeup()
{int i;
  uint8_t response[SHA204_RSP_SIZE_MIN];
  byte returnValue;

  returnValue = sha204.sha204c_wakeup(&response[0]);
  for ( i = 0; i < SHA204_RSP_SIZE_MIN; i++)
  {       
     if (response[i] != exp_response[i])
      {   
        Serial.println("Device not awake! Wait for sometime!");
                       delay(5000);
                       break;
      }
      
    }
  
   
    if (i ==4)
   
    {Serial.println("ATSHA AWAKE!!!");
    return returnValue;
    }
    else
    {Serial.println("Trying again");
      setup();
       delay_sec();}
    
  
}

byte check_sensors()
{
  
  for(int j=0;j<=8;j++)
{
  serial_Number[j] = EEPROM.read(addr);
  addr++;
  if(addr==EEPROM.length())
  {addr=0;}
  delay(100);

}
  
  int i;
  uint8_t serialNumber[9];
  uint8_t j;
  byte returnValue;
  returnValue = sha204.getSerialNumber(serialNumber);

  for ( i = 0; i <=8; i++)
  {   
    
    if (serialNumber[i] != serial_Number[i])
    { 
      Serial.println("VALIDATION FAILED");
      Serial.println("TRY AGAIN");
      delay_sec();
   setup();
   }

  }
i--;
  if (i == 8)
  { 
    j = macChallenge();
    
    if (j == 1)// if failed
    {Serial.println("VALIDATION FAILED");
      
   Serial.println("TRY AGAIN");
   //return returnValue;
   delay_sec();
   setup();
 
    }
    
  else if (j == 0)// if successful
  { Serial.println("VALIDATION SUCESSFUL");
      return returnValue;
}
}

}
byte macChallenge()
{int j;
    for(j=0;j<=34;j++)
{
   
  exp_mac[j] = EEPROM.read(addr);

  addr=addr+1;
 
 if(addr==EEPROM.length())
{addr=0;}
  delay(100);

}

 Serial.println(j);
  uint8_t command[MAC_COUNT_LONG];
  uint8_t response[MAC_RSP_SIZE];
  int i;

  const uint8_t challenge[MAC_CHALLENGE_SIZE] = {
    0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
    0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF,
    0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
    0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF
  };

  uint8_t ret_code = sha204.sha204m_execute(SHA204_MAC, 0, 0, MAC_CHALLENGE_SIZE,
                     (uint8_t *) challenge, 0, NULL, 0, NULL, sizeof(command), &command[0],
                     sizeof(response), &response[0]);

  for ( i = 0; i < SHA204_RSP_SIZE_MAX; i++)
  {
    

    if (response[i] != exp_mac[i])
    { 
      ret_code = 0x01;
       return ret_code;
       break;
    }
  }
 i--;
  if (i ==34 )
  {ret_code=0x00;
    return ret_code;
  }

}
void delay_sec()
{ for(int i=0;i<30;i++)
{ delay(1000);}
}
void entry ()
{ 
}
  
