/*//udp server
var dgram = require('dgram');
var server = dgram.createSocket('udp4');
server.on('message', function(msg, rinfo) {
    console.log('server got: ' + msg + ' from ' + rinfo.address + ':' + rinfo.port);

    //regresar mensaje
	setInterval(()=>{

    var sebdata = "Hola perro";<

    server.send(sebdata, 0, sebdata.length, rinfo.port, rinfo.address, function(err, bytes) {
        if (err) throw err;
        console.log('UDP message sent to ' + rinfo.address + ':' + rinfo.port);
    });

	},3000);
});


server.on('listening', function() {
    var address = server.address();
    console.log('server listening on ' + address.address + ':' + address.port);
});


server.bind(50);
*/

//
var bytesReceived = 0;
var bytesSend = 0;
var identificador = "Rt-Ser|";

var clientes = [];
var Admins = [];

const { Console } = require('console');
const net = require('net');
const { parse } = require('path');

// Crea un nuevo servidor TCP
const server = net.createServer((socket) => {
  // Evento 'connection': se ejecuta cuando un cliente se conecta al servidor
  console.log('Cliente conectado');

  //enviar datos al cliente
  socket.write("Server connect success\n");

  // Evento 'data': se ejecuta cuando el servidor recibe datos del cliente
  socket.on('data', (data) => {

    console.log(`Datos recibidos del cliente: ${data}`);

    //procesamos comandos de rt-thread
    var dataSepare = data.toString().split("|");

    var rt = dataSepare[0];
    var comando = dataSepare[1];
    var device = dataSepare[2];

    console.log("rt: "+rt);
    console.log("comando: "+comando);
    console.log("device: "+parseInt(device));

    if(rt.search("rt-thread") > -1){

      console.log("Comando Client: "+comando);

      if(comando.search("register") > -1){
      
        socket.write(identificador+"Client register success");
        socket.write(identificador+":" + socket.remoteAddress + ":" + socket.remotePort);
        
        clientes.push({
          socket: socket,
          socketId: socket.remoteAddress + ":" + socket.remotePort,
          device: device,
          socketStatus: false,
          setInterval: null
        });
  
      }else if(comando.search("SocketInt") > -1){
  
        //BUSCAMOS device si existe
        var existe = false;
        var index = 0;
  
        for(var i = 0; i < clientes.length; i++){
          if(clientes[i].device == device){
  
            clientes[i].socketStatus = true;
            existe = true;
            index = i;
          }
        }
        
        if(existe){
  
          var a = 0;
  
          /*
          clientes[index].setInterval = setInterval(()=>{
            socket.write(identificador+ device + "|"+ a + "\n");
            a++;
  
          },1000);
          */
    
        }
  
      }
    }else if(rt.search("Rt-Admin") > -1){

      if(comando){

        console.log("Comando Admin: "+comando);
        //enviamos datos al cliente
        for(var i = 0; i < clientes.length; i++){
          //if(clientes[i].device == device){
          //  clientes[i].socket.write(identificador+"|"+comando + "\n");
          //}

          clientes[i].socket.write(identificador+"|"+comando + "\n");
          
        }

        socket.write("success\n");
    
        
      }

    }

    
  });

  // Evento 'end': se ejecuta cuando el cliente cierra la conexión
  //disconnect
  socket.on('end', () => {

    //buscamos el device por su ip y puerto
    for(var i = 0; i < clientes.length; i++){
      if(clientes[i].socketId == socket.remoteAddress + ":" + socket.remotePort){

        //detenemos el intervalo
        clearInterval(clientes[i].setInterval);

        //eliminamos el cliente
        clientes.splice(i,1);
      }
    }

    
    console.log('Cliente desconectado');
    

  });


});

// Inicia el servidor en un puerto específico
const port = 30;
server.listen(port, () => {
  console.log(`Servidor TCP iniciado en el puerto ${port}`);
});
