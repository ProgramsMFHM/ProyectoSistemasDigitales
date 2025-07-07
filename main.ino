/**
 * @file main.ino
 * @authors Milton Hernández Morales (milherna@umag.cl) Cristian Flores Miranda (crisflor@umag.cl) Bruno Martinez Flores brumarti@umag.cl
 * @brief Este programa implementa un servidor web para controlar los pines de un Arduino
 * @version 0.1
 * @date 2025-07-02
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <SPI.h>
#include <Ethernet.h>

// Se definen los parámetros de la red para el arduino
byte mac[] = {0x44,0x6D,0x57,0x33,0xA5,0x9F};
byte ip[] = {10,9,0,111};
EthernetServer server(80);

// Estructura para los pines
typedef struct {
	int number;
	bool state;
} PIN;
#define MAX_PINS 4
PIN pines[MAX_PINS];

String readString = String(30); // Petición HTTP

/**
 * @brief Función de configuración del programa
 *
*/
void setup()
{
  Serial.begin(9600);
  while (!Serial);
 
	Ethernet.begin(mac, ip);    // Comienzo Ethernet
	server.begin();             // Iniciar datos servidor

	for (int i=0;i<MAX_PINS;i++)
	{
		pines[i].number=i+2;
		pinMode(pines[i].number, OUTPUT);    // Definir pines como salida
		digitalWrite(pines[i].number, HIGH); // Se define el pin en alto
		pines[i].state=false;
	}
}

/**
 * @brief  Función de bucle que se encarga de recibir las peticiones HTTP
 *
*/
void loop()
{
	EthernetClient cliente = server.available(); // Crear una conexión cliente
	if (cliente)
	{
		while (cliente.connected())         // Cliente conectado
		{
			if(cliente.available())
			{
				char c = cliente.read();
				if(readString.length() < 30)    // Petición HTTP Caracteres
					readString.concat(c);
				if (c == '\n') // Si la petición finalizó
				{
					int S=readString.indexOf("P=");
					int pin_state = readString.substring(S+2,S+3).toInt();
					int pin_number = readString.substring(S+3,S+4).toInt();

					if (pin_number >= 0 && pin_number < MAX_PINS)
						if (pin_state == 1)
						{
							pines[pin_number].state=true;
							digitalWrite(pines[pin_number].number, LOW);
						}
						else
						{
							pines[pin_number].state=false;
							digitalWrite(pines[pin_number].number, HIGH);
						}

						// Cabecera HTTP estándar
						cliente.println("HTTP/1.1 200 OK");
						cliente.println("Content-Type: text/html");
						cliente.println("Connection: close");
						cliente.println(); // Línea vacía antes del contenido HTML

						// Página Web en HTML
						cliente.print("<html><head><title>LAMPARA ON/OFF</title></head>");
						cliente.print("<body width=100% height=100%><center><h1>LAMPARAS ON/OFF</h1><br><br>");

						for (int i=0;i<MAX_PINS;i++)
						{
							cliente.print("Estado de la lampara " + String(i+1) + ": ");
							pines[i].state ? cliente.print("ON ") : cliente.print("OFF ");
							cliente.print("<input type=submit value=ON style=width:50px;height:25px onClick=location.href='./?P=1"+String(i)+"\'>");
							cliente.print("<input type=submit value=OFF style=width:50px;height:25px onClick=location.href='./?P=0"+String(i)+"\'><br><br>");
						}
						cliente.print("</center></body></html>");
						cliente.stop(); // Cierre de la conexión con el cliente
						readString="";
				}
			}
		}
	}
}
