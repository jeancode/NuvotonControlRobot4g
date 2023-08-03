
// Incluir la biblioteca para el control del motor de paso
#include <Arduino.h>
#include <AccelStepper.h>
#include <softwareSerial.h>
#include <vector>
#include "app.h"

using namespace std;

//Rt-Admin|m1 s-1000 t1000 m2 s-100 t5000

// Definir los pines del driver A4988 conectados al Arduino
#define STEP_PIN 5
#define DIR_PIN 23

#define STEP_PIN2 21
#define DIR_PIN2 22

#define RX 18
#define TX 19


SoftwareSerial Serial4g(RX,TX);// RX, TX


// Crear un objeto de la clase AccelStepper
AccelStepper stepper(AccelStepper::DRIVER, STEP_PIN, DIR_PIN);

AccelStepper stepper2(AccelStepper::DRIVER, STEP_PIN2, DIR_PIN2);

bool ejecute1 = true;
bool ejecute2 = true;

struct MotorInfo{
    int time = 0;
    int speed = 0;
    bool process = false;
    void clear(){
        time = 0;
        speed = 0;
    }
};


struct MotorsInfo
{
    MotorInfo motor1;
    MotorInfo motor2;
};



MotorsInfo motorsInfo;


//Motor1Control
void Motor1Control(void *pvParameters){
    while(1){
        
        stepper.runSpeed();
        vTaskDelay(1);

        
    }
}

void Motor1Contro2(void *pvParameters){
    while(1){

      
        stepper2.runSpeed();
        vTaskDelay(1);
        
    }
}

void Motor1Drive(void *pvParameters){
    while(1){

        if(motorsInfo.motor1.process){
          
            stepper.setSpeed(motorsInfo.motor1.speed);
            vTaskDelay(motorsInfo.motor1.time);
            
            stepper.setSpeed(0);
            

            motorsInfo.motor1.clear();
            motorsInfo.motor1.process = false;

            Serial.println("Motor1 time: " + String(motorsInfo.motor1.time));
            Serial.println("Motor1 speed: " + String(motorsInfo.motor1.speed));
        }else{
            

            vTaskDelay(1);
            
       
        }

        
    }
}

void Motor2Drive(void *pvParameters){
    while(1){


        if(motorsInfo.motor2.process){
            
            
            stepper2.setSpeed(motorsInfo.motor2.speed);
            vTaskDelay(motorsInfo.motor2.time);
            stepper2.setSpeed(0);
            //stop
            motorsInfo.motor2.clear();
            motorsInfo.motor2.process = false;

        }else{
            
            vTaskDelay(1);
            //vTaskDelay microsegundos
            

    
            
        }

        

    }
}

void setup() {

    Serial.begin(115200);

    // Establecer el máximo voltaje para el driver (valor en mV, consulta la hoja de datos del driver)
    stepper.setMaxSpeed(5000); // Velocidad máxima en pasos por segundo
    stepper.setAcceleration(5000); // Aceleración en pasos por segundo por segundo

    stepper2.setMaxSpeed(5000); // Velocidad máxima en pasos por segundo
    stepper2.setAcceleration(10000); // Aceleración en pasos por segundo por segundo


    xTaskCreatePinnedToCore(Motor1Control,"Motor1Control",1000,NULL,1,NULL,1); //Motor1Control,
    xTaskCreatePinnedToCore(Motor1Contro2,"Motor1Contro2",1000,NULL,1,NULL,1);

    //crear otro hilo para el control de los motores individuales
    xTaskCreatePinnedToCore(Motor1Drive,"Motor1Drive",1000,NULL,1,NULL,0); //Motor1Control,
    xTaskCreatePinnedToCore(Motor2Drive,"Motor2Drive",1000,NULL,1,NULL,0); //Motor1Control,

    
    app::init();
    SoftwareSerial* pSerial4g = &Serial4g;
    //variable que contenga app::Module4g();
    app::Module4g(pSerial4g);

    
}

void procesComand(String data){

    //crear variable sque contendran el indice incial y final para  m1 y m2
    int m1Index = data.indexOf("m1");
    int m2Index = data.indexOf("m2");

    String m1Data = "";
    String m2Data = "";


    int m1Time = 0;
    int m2Time = 0;
    int m1Spped = 0;
    int m2Spped = 0;

    if(m1Index > -1){
        m1Data = data.substring(m1Index+2,m2Index);

        Serial.println("m1Data: " + m1Data);

        //buscar t100
        int tIndex = m1Data.indexOf("t");
        int sIndex = m1Data.indexOf("s");

        //solo si tindex es menor que sindex
        if(sIndex > tIndex){
            String tData = m1Data.substring(tIndex+1,sIndex);
            String sData = m1Data.substring(sIndex+1,m1Data.length());

            m1Time = tData.toInt();
            m1Spped = sData.toInt();
            
        }else{

            String sData = m1Data.substring(sIndex+1,tIndex);
            String tData = m1Data.substring(tIndex+1,m1Data.length());

            m1Time = tData.toInt();
            m1Spped = sData.toInt();
            
        }



    }

    if(m2Index > -1){
        m2Data = data.substring(m2Index+2,data.length());

        int m2tIndex = m2Data.indexOf("t");
        int m2sIndex = m2Data.indexOf("s");

        if(m2sIndex > m2tIndex){
            String m2tData = m2Data.substring(m2tIndex+1,m2sIndex);
            String m2sData = m2Data.substring(m2sIndex+1,m2Data.length());

            m2Time = m2tData.toInt();
            m2Spped = m2sData.toInt();

        }else{
                
            String m2sData = m2Data.substring(m2sIndex+1,m2tIndex);
            String  m2tData = m2Data.substring(m2tIndex+1,m2Data.length());

            m2Time = m2tData.toInt();
            m2Spped = m2sData.toInt();

        }


    }

    
    motorsInfo.motor1.time = m1Time;
    motorsInfo.motor1.speed = m1Spped;

    
    motorsInfo.motor2.time = m2Time;
    motorsInfo.motor2.speed = m2Spped;


    motorsInfo.motor2.process = true;
    motorsInfo.motor1.process = true;

 
}

void loop() {

    

    if(Serial4g.available())
    {
        
        Serial4g.setTimeout(20);
        String data2 = Serial4g.readStringUntil('\n');

        Serial.println(data2);
        app::Module4gCommand(data2,[](String data){
            
            procesComand(data);
        });

    }

    if(Serial.available())
    {
        Serial.setTimeout(50);
        String data = Serial.readStringUntil('\n');

        procesComand(data);

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