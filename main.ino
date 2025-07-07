/* Juan Pablo Arancibia 2013*/

#include <SPI.h>
#include <Ethernet.h>

// Se definen los parametros de la red para el arduino
byte mac[] = {0x44,0x6D,0x57,0x33,0xA5,0x9F};
byte ip[] = {10,9,0,111};
EthernetServer server(80);

// Variables para los pines
const int ledPin0 = 0;
const int ledPin1 = 1;
const int ledPin2 = 2;
const int ledPin3 = 3;

// Textos
String readString = String(30); // Petición HTTP
String state0 = String(3);
String state1 = String(3);
String state2 = String(3);
String state3 = String(3);

void setup()
{
  Ethernet.begin(mac, ip);    // comienzo Ethernet
  server.begin();             // iniciar datos servidor
  pinMode(ledPin0,OUTPUT);    // definir pines como salida
  digitalWrite(ledPin0,HIGH); // Se define el pin en alto
  state0="OFF";               // Estado del pin APAGADO
  pinMode(ledPin1,OUTPUT);
  digitalWrite(ledPin1,HIGH);
  state1="OFF";
  pinMode(ledPin2,OUTPUT);
  digitalWrite(ledPin2,HIGH);
  state2="OFF";
  pinMode(ledPin3,OUTPUT);
  digitalWrite(ledPin3,HIGH);
  state3="OFF";
}

void loop()
{
 EthernetClient cliente = server.available(); //crear una conexion cliente
 if (cliente)
 {
  while (cliente.connected())         //cliente conectado
  {
    if(cliente.available())
    {
      char c = cliente.read();
      if(readString.length() < 30)    //peticion HTTP Caracteres
        readString.concat(c);
      if (c == '\n') //si la peticion finalizo
      {
        int S=readString.indexOf("P=");
          if(readString.substring(S,S+5)=="P=111")
          {
            digitalWrite(ledPin0,LOW);
            state0="ON";
          }else if(readString.substring(S,S+5)=="P=110")
          {
            digitalWrite(ledPin0,HIGH);
            state0="OFF";
          }else if(readString.substring(S,S+5)=="P=121")
          {
            digitalWrite(ledPin1,LOW);
            state1="ON";
          }else if(readString.substring(S,S+5)=="P=120")
          {
            digitalWrite(ledPin1,HIGH);
            state1="OFF";
          }else if(readString.substring(S,S+5)=="P=131")
          {
            digitalWrite(ledPin2,LOW);
            state2="ON";
          }else if(readString.substring(S,S+5)=="P=130")
          {
            digitalWrite(ledPin2,HIGH);
            state2="OFF";
          }else if(readString.substring(S,S+5)=="P=141")
          {
            digitalWrite(ledPin3,LOW);
            state3="ON";
          }else if(readString.substring(S,S+5)=="P=140")
          {
            digitalWrite(ledPin3,HIGH);
            state3="OFF";
          }

          //Cabecera HTTP estandar
          cliente.print("HTTP/1.1 200 OK");
          cliente.print("Content-Type: text/html");
          //Pagina Web en HTML
          cliente.print("<html><head><title>LAMPARA ON/OFF</title></head>");
          cliente.print("<body width=100% height=100%><center><h1>LAMPARAS ON/OFF</h1><br><br>");
	        cliente.print("Estado de la lampara 0: ");
          cliente.print(state0);
          cliente.print("<input type=submit value=ON style=width:50px;height:25px onClick=location.href='./?P=111\'>");
          cliente.print("<input type=submit value=OFF style=width:50px;height:25px onClick=location.href='./?P=110\'><br><br>");
          cliente.print("Estado de la lampara 1: ");
          cliente.print(state1);
          cliente.print("<input type=submit value=ON style=width:50px;height:25px onClick=location.href='./?P=121\'>");
          cliente.print("<input type=submit value=OFF style=width:50px;height:25px onClick=location.href='./?P=120\'><br><br>");
          cliente.print("Estado de la lampara 2: ");
          cliente.print(state2);
          cliente.print("<input type=submit value=ON style=width:50px;height:25px onClick=location.href='./?P=131\'>");
          cliente.print("<input type=submit value=OFF style=width:50px;height:25px onClick=location.href='./?P=130\'><br><br>");
          cliente.print("Estado de la lampara 3: ");
          cliente.print("</center></body></html>");
          cliente.stop();//Cierro conexion con el cliente
          readString="";
      }
    }
  }
}
}
