//code i2c esclavo y serve e

#include <Arduino.h>
#include <Wire.h>
#include <softwareSerial.h>
#include <Metro.h>
//esp library

//Module 4g
//SoftwareSerial Serial4g(5, 23);// RX, TX
SoftwareSerial Serial4g(21,22);// RX, TX

Metro rMetro = Metro(5000); 
Metro rInternet = Metro(1000);


#include "app.h"

void receiveEvent(int howMany)
{

  String data = "";

  //Serial.println("receiveEvent");

  while(1 < Wire.available()) // loop through all but the last
  {
    char c = Wire.read(); // receive byte as a character

    if (c == '\n')
    {
      app::i2cCommand(data);

      Serial.println(data);

      data = "";

    }else{
      data += c;
    }
    
  }

}

//se ejecuta cada vez que el cliente solicita informacion
void requestEvent()
{

  int * buffer = app::getBuffer();


  String concade = "";



  for(int i = 0; i < 30; i++)
  { 
    if(buffer[i] == 0)
    {
      break;
    }
    char caracterAscii = char(buffer[i]);    
    concade += caracterAscii;
  }

  concade += "\n";



  //create array to send
  Wire.write(concade.c_str());  

  //Wire.write("hello 123456789\n");  
}



void setup()
{
  //espesificar pines de i2c

  Wire.begin(4);  

  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);

  Serial.begin(115200);    

  //app::setBuffer(0, 36);
  //app::setBuffer(1, 36);
  //app::setBuffer(2, 36);
  //app::setBuffer("hola mundo");
  //app::clearBuffer(30);
  //Serial4g.begin(9600);

  app::init();
  SoftwareSerial* pSerial4g = &Serial4g;
  //variable que contenga app::Module4g();
  app::Module4g(pSerial4g);

}

void loop()
{

  if(Serial4g.available())
  {
    
    Serial4g.setTimeout(20);
    String data2 = Serial4g.readStringUntil('\n');

    Serial.println(data2);
    app::Module4gCommand(data2);

  }

  if(Serial.available())
  {
    Serial.setTimeout(50);
    String data = Serial.readStringUntil('\n');

    //Serial.println(data);
    
    
    if(data.indexOf("Modem") >= 0){

      Serial.println("Modem Internet Open");
      app::Module4gInit();
      

    }else if(data.indexOf("restart") >= 0){

      app::restartModule4g();

      //esp32 restart
      //ESP.restart();

    }else if(data.indexOf("conect") >= 0){

      app::Module4gClose();
      delay(300);
      app::Module4gConnect(); 

    }else if(data.indexOf("register") >= 0){

      app::Module4gRegister("Register User Ser");

    }else if (data.indexOf("signal") >= 0)
    {
      app::Module4gSignal();
      Serial.println(app::getInfo().senal);

    }
    else if(data.indexOf("close") >= 0)
    {
      app::Module4gClose();  

    }else if(data.indexOf("socket") >= 0)
    {
      app::Module4gStartSocket();
    }
    



  } 




}
