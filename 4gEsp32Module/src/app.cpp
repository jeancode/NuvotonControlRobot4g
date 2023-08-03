

#include "app.h"

using namespace std;


namespace app{
    
    int buffer[30];

    info info;

    SoftwareSerial* pSerial; 

    String fechaHora = "";

    bool internet = false;

    bool socketSend = false;

    bool sockectAvailable (){
        return socketSend;
    }

    bool availableInternet(){

        return internet;

    }

    //setsenal
    void setSenal(String data){

        info.senal = data;

    }

    void setFechaHora(String data){

        info.fechaHora = data;

    }

    //return buffer
    int* getBuffer(){

        return buffer;

    }

    void setBuffer(int index, int data){

        buffer[index] = data;
    
    }

    void setBuffer(String data){

        int lenData = data.length()-1;

        for(int i = 0; i <= lenData-1; i++)
        {
            buffer[i] = data[i];
        }
        
    }

    //setStateConnection
    void setStateConnection(bool data){

        info.conect = data;

    }



    void init(){
        Serial.println("Incializando Systema");
    }

    void restartModule4g(){
        
        
        String data = "AT+CPOF";

        for(int i = 0; i < data.length(); i++)
        {
            pSerial->write(data[i]);
            delay(10);

        }
        pSerial->println("");

        delay(300);
    }

    struct info getInfo(){

        return info;

    }

    void Module4gInit(){

        Module4gClose();

        //pSerial->println("AT+NETOPEN");
        delay(300);
        
        String data = "AT+NETOPEN";

        for(int i = 0; i < data.length(); i++)
        {
            pSerial->write(data[i]);
            delay(10);

        }
        pSerial->println("");

        delay(300);

    }

    void clearBuffer(int size){

        //
        int sizeBuffer = 30;

        for(int i = 0; i < sizeBuffer-1; i++)
        {
            buffer[i] = 0;
        }
        
    }

    

    void Module4g(SoftwareSerial* pSerialPtr){

        pSerial = pSerialPtr;

        pSerial->begin(115200);

        //AT+IPR=?
        pSerial->println("AT+IPR=9600");
        delay(1000);

        pSerial->begin(9600);

        Serial.println("Iniciando modulo 4g");

        pSerial->println("AT");
        delay(1000);

        //intensidad de la señal
        pSerial->println("AT+CSQ");
        delay(1000);

        //fecha y hora
        pSerial->println("AT+CCLK?");
        delay(1000);

        //ip
        pSerial->println("AT+CGPADDR=1");
        delay(1000);


        //AT+NETOPEN
        pSerial->println("AT+NETOPEN");
        delay(2000);

        //pSerial->println("AT+CIPOPEN=0,\"TCP\",\"47.87.134.53\",30,0");

        //

    }

    void Module4gCommand(String data, callback cb){

        int len = data.length()-1;

    
        //Serial.println(data);

        if(data.indexOf("+CSQ:") > -1)//intensidad de la señal
        {
            String result = "";
            int index = data.indexOf("+CSQ:");

            for(int i = index+6; i < len; i++)
            {
                result += data[i];
            }
            
            setSenal(result);
            //Serial.println(result);
            
 
        }else if(data.indexOf("+CCLK:") > -1){//fecha y hora

            String result = "";
            int index = data.indexOf("+CCLK:");

            for(int i = index+6; i <= len; i++)
            {
                result += data[i];
            }
            fechaHora = result;

            setFechaHora(result);
            //Serial.println(result);
            
        }else if(data.indexOf("+CGATT:") > -1){

            String result = "";
            int index = data.indexOf("+CGATT:");

            for(int i = index+7; i <= len; i++)
            {
                result += data[i];
            }

            Serial.println("Estado de la red: " + result);

            

        }else if(data.indexOf("+CGPADDR:") > -1){

            String result = "";
            int index = data.indexOf("+CGPADDR:");

            for(int i = index+9; i <= len; i++)
            {
                result += data[i];
            }

            Serial.println("IP: " + result);

        }else if(data.indexOf("+NETOPEN:") > -1){

            String result = "";
            int index = data.indexOf("+NETOPEN:");

            for(int i = index+9; i <= len; i++)
            {
                result += data[i];
            }

           
            
            //convert result to in
            int res = result.toInt();
            internet = res;

            //Serial.println(internet);

        }else if(data.indexOf("Rt-Ser|") > -1){

            setStateConnection(1);

            String result = "";

            int index = data.indexOf("Rt-Ser|");

            for(int i = index+8; i <= len; i++)
            {
                result += data[i];
            }

            socketSend = true;
            //Recv
            Serial.println(result + "\n");
            
            cb(result);

            //se envia losd atos al bufer durante 10ms            
            //setBuffer("Soc:"+result);
            //delay(10);
            //clearBuffer(30);

        
        }else if(data.indexOf("+CIPOPEN: 0,0") > -1){

            Serial.println("Conectado al servidor");

        }else if(data.indexOf("+CIPOPEN: 0,4") > -1){

            Serial.println("Error al conectar al servidor");

        }else if(data.indexOf("+CIPCLOSE: 0,0") > -1){

            Serial.println("Se ha cerrado la conexion al servidor");
            setStateConnection(0);
            
        }
        
        
    }


    //AT+CIPOPEN=0,"TCP","
    void Module4gConnect(){
        
        String data = "AT+CIPOPEN=0,\"TCP\",\"47.87.134.53\",30,0";
        for(int i = 0; i < data.length(); i++)
        {
            pSerial->write(data[i]);
            delay(10);

        }

        pSerial->println("");

    }



    void i2cCommand(String data){

        if(data.indexOf("Modem") > -1)
        {
            Serial.println("Comando Modem Aceptado");
            Module4gInit();
        }
        else if(data.indexOf("conect") > -1){
            Serial.println("Comando Conectar Aceptado");
            Module4gConnect();
        }else if(data.indexOf("register") > -1){
            Serial.println("Comando register Aceptado");
            Module4gRegister(data);
        }else if(data.indexOf("socket") > -1){
            Serial.println("Comando socket Aceptado");
            Module4gStartSocket();
            socketSend = true;
        }else if(data.indexOf("close") >-1){
            Serial.println("Comando close Aceptado");
            socketSend = false;
            Module4gClose();
        }else{
            Serial.println("Comando no reconocido");
        }
    }
    


    void Module4gRegister(String dataIn){

        String data = "AT+CIPSEND=0,26";

        for(int i = 0; i < data.length(); i++)
        {
            pSerial->write(data[i]);
            delay(10);

        }
        pSerial->println("");

        delay(300);



        data = "rt-thread|register|000100";


        for(int i = 0; i < data.length(); i++)
        {
            pSerial->write(data[i]);
            delay(20);
        }
        pSerial->println("");
        delay(100);
        //Module4gClose();
    
    }
    void Module4gStartSocket(){
        
        String data = "AT+CIPSEND=0,27";

        for(int i = 0; i < data.length(); i++)
        {
            pSerial->write(data[i]);
            delay(10);

        }
        pSerial->println("");

        delay(300);

        data = "rt-thread|SocketInt|000100";//comando inicializa el socket


        for(int i = 0; i < data.length(); i++)
        {
            pSerial->write(data[i]);
            delay(10);
        }
        pSerial->println("");

    
    }
    void Module4gClose(){
            

            
        String data = "AT+CIPCLOSE=0";

        for(int i = 0; i < data.length(); i++)
        {
            pSerial->write(data[i]);
            delay(20);

        }
        pSerial->println("");
        delay(30);
        
    }

    void Module4gSignal(){
        pSerial->println("AT+CSQ");
    }

    void Module4gDateTime(){
        pSerial->println("AT+CCLK?");
    }

    
}

