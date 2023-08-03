#include <iostream>
#include <Arduino.h>
#include <softwareSerial.h>

struct info{
    String fechaHora;
    String senal;
    bool conect =  false;
};

//function callback
typedef void (*callback)(String);

namespace app{
    //buffer int8_t

    //return buffer
    int* getBuffer();
    void setBuffer(int index, int data);
    void setBuffer(String data);
    void clearBuffer(int size);

    void init();
    void Module4g(SoftwareSerial *Serial4g);
    void Module4gCommand(String data, callback cb);

    void i2cCommand(String data);


    bool sockectAvailable ();
    void socketSendInfo(String data);
    void Module4gInit();
    void restartModule4g();
    void Module4gConnect();
    void Module4gRegister(String data);
    void Module4gStartSocket();
    void Module4gSignal(); 
    void Module4gDateTime();
    void Module4gClose();

    void Module4gSendData(String data);



    //setSenal
    void setSenal(String data);
    void setFechaHora(String data);
    void setStateConnection(bool data);
    struct info getInfo();



    bool availableInternet();


    
    
}
