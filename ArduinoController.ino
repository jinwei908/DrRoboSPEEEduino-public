// Include the Servo library 
#include <WiFi.h>
#include <Servo.h> 
#include <SPEEEduino_LowLevel_Library.h>
// Declare the Servo pin 
int servoPin1 = 5; 
int servoPin2 = 6; 
int servoPin3 = 7; 

// Create a servo object 
char ssid[] = "FABLAB14";      //  your network SSID (name)
char pass[] = "fabpass123";   // your network password

String responseString = "";
String iValue = "";
// Initialize the Wifi client library
WiFiClient client;

//controllers
int close = 0;
int open = 180;

bool startCapture = false;
int status = WL_IDLE_STATUS;

SPEEEduino_LowLevel device = SPEEEduino_LowLevel();

String urlRequest = String("GET /php/asean100/getinteraction.php HTTP/1.1\r\n");

const char* payload = 
"GET /php/asean100/getinteraction.php HTTP/1.1\r\n"
"User-Agent: Arduino-Wifi/1.1\r\n" 
"Host: asean100.ap-southeast-1.elasticbeanstalk.com\r\n\r\n";

Servo Servo1; 
Servo Servo2; 
Servo Servo3; 

void setup() { 
  // We need to attach the servo to the used pin number 
  

  Serial.begin(9600);
  //Serial.println("PROGRAM START");
  device.slowOpenLink();
  device.setDHCPEnabled(true);
  device.setConnectionAmount(SINGLE);
  device.setWiFiMode(STATION);

  device.listVersion(); //print device version

  // Join network
  Serial.println("JOINING AP");
  Serial.println(device.joinAP("FABLAB14","fabpass123"));
  Serial.println("JOINED AP");
}
void loop(){ 
  // Make servo go to 0 degrees 
  
  delay(5000);
  
  String httpResponse = httpRequest();
  if(httpResponse == "0"){
     Servo1.attach(servoPin1);
     delay(1000);
    //Serial.println("Starting Servo 1");
    Servo1.write(close);
    Servo1.write(open);
    delay(1000);
    Servo1.write(close);
    Servo1.detach();
    delay(1000);
  } else if(httpResponse == "1"){
    Servo2.attach(servoPin2); 
    delay(1000);
   // Serial.println("Starting Servo 2");
    Servo2.write(close);
    Servo2.write(open);
    delay(1000);
    Servo2.write(close);
    delay(1000);
    Servo2.detach();
  } else if(httpResponse == "2"){
    Servo3.attach(servoPin3); 
    delay(1000);
    //Serial.println("Starting Servo 3");
    Servo3.write(close);
    Servo3.write(open);
    delay(1000);
    Servo3.write(close);
    Servo3.detach();
    delay(1000);
  } else {
  }
}


String httpRequest() {
  // Begin connection
  
  device.beginSingleConnection(TCP, F("asean100.ap-southeast-1.elasticbeanstalk.com"), F("80"));
  String realPayload = payload; // Make a copy of the original template
  device.sendDataSingleConnection(realPayload);
  Serial.println("Printing received data:");
  Serial.println("Data: ");
  String responseString2 = device.receiveData(SINGLE).content; 
  Serial.println(responseString2);
  //check if it's error

  int interactionValueIndex = responseString2.indexOf("interactionValue");
 int errorIndex = responseString2.indexOf("error");
  
  if(errorIndex >= 0){
      Serial.println("Error found: ");
      Serial.println(errorIndex);
      return "-1";
  } else if(interactionValueIndex >= 0){
    //found value take the value
    int lastQuoteIndex = responseString2.indexOf('"', interactionValueIndex + 17);
    String ibeforeValue = responseString2.substring(lastQuoteIndex+1);
    int lastQuoteIndex2 = ibeforeValue.indexOf('"');
    Serial.println("Before: " + ibeforeValue);
    iValue = ibeforeValue.substring(0, lastQuoteIndex2).toInt();
    Serial.println("Value: " + iValue);
    return iValue;
  }
  
  //Serial.println("Completed printing: ");
  
   
}

