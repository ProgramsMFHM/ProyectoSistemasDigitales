/**
 * @file main.ino
 * @authors Milton Hernández Morales (milherna@umag.cl) Cristian Flores Miranda (crisflor@umag.cl) Bruno Martinez Flores brumarti@umag.cl
 * @brief Este programa implementa un servidor web para controlar los pins de un Arduino
 * @version 0.1
 * @date 2025-07-07
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <SPI.h>
#include <Ethernet.h>

// Se definen los parámetros de la red para el Arduino
byte mac[] = {0x44,0x6D,0x57,0x33,0xA5,0x9F};
byte ip[] = {10,9,11,111};
EthernetServer server(80);

// Estructura para los pines
typedef struct {
    int reference_number; // Numero de referencia del pin en el servidor
	int number; // Numero del pin en el arduino
	bool state; // Estado del pin: 1 = ON, 0 = OFF
    String name; // Nombre del pin
} PIN;
#define MAX_PINS 4
PIN pins[MAX_PINS];

// Nombre de cada una de las lámparas
String pin_names[MAX_PINS] = {"Lámpara Sala de Estar", "Lámpara Dormitorio Infantil", "Lámpara Dormitorio Principal", "Lámpara Living"};

String readString = String(30); // Petición HTTP

void GET_pin_state(int pin_reference_number, EthernetClient customer);
void GET_all_pins_state(EthernetClient customer);
void POST_pin_state(int pin_reference_number, int new_state, EthernetClient customer);

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
	    pins[i].number=i+2;
        pins[i].reference_number = i;
		pinMode(pins[i].number, OUTPUT);    // Definir pines como salida
		digitalWrite(pins[i].number, HIGH); // Se define el pin en alto
		pins[i].state=false;
        pins[i].name = pin_names[i];
	}
}

/**
 * @brief  Función de bucle que se encarga de recibir las peticiones HTTP
 *
*/
void loop()
{
    String requestLine = "";
	EthernetClient customer = server.available(); // Crear una conexión customer
    int pinState;
    int pinNumber;
	if (customer)
	{
		while (customer.connected())         // Cliente conectado
		{
			if(customer.available())
			{
				char c = customer.read();
				if(readString.length() < 30)    // Petición HTTP Caracteres
					readString.concat(c);
				if (c == '\n') // Si la petición finalizó
				{
                    if (requestLine.length() == 0) {
                        requestLine = readString; // Guarda la primera línea
                        Serial.println("Primera línea: " + requestLine);
                    }

                    // Endpoints
                    if (requestLine.startsWith("GET /?P=")) { // Obtiene el estado de una lampara
                        pinNumber = requestLine.substring(8,9).toInt();
                        GET_pin_state(pinNumber, customer);
                    } else if (requestLine.startsWith("POST /?P=")) { // Se cambia el estado de una lampara
                        pinNumber = requestLine.substring(9,10).toInt();
                        pinState = requestLine.substring(10,11).toInt();
                        POST_pin_state(pinNumber, pinState, customer);
                    } else if (requestLine.startsWith("GET / ")) { // Obtiene el estado de todas las lamparas
                        GET_all_pins_state(customer);
                    }


                    customer.stop(); // Cierre de la conexión con el Cliente
                    readString = "";
                    requestLine = "";
				}
			}
		}
	}
}

/**
 * @brief Envia el estado de una lampara a la interfaz web como objeto JSON
 *
 * @param pin Referencia a la estructura PIN que se desea imprimir
 * @param customer Referencia a la conexión Cliente
 */
void GET_pin_state(int pin_reference_number, EthernetClient customer)
{
    if (pin_reference_number >= 0 && pin_reference_number < MAX_PINS) {
        // Inicio de la respuesta
        customer.println("HTTP/1.1 200 OK");
        customer.println("Content-Type: application/json");
        customer.println("Access-Control-Allow-Origin: *"); // CORS
        customer.println("Connection: close");
        customer.println();

        // Información JSON
        customer.print("{\"pins\":[");
        customer.print("{\"number\": ");
        customer.print(pin_reference_number);
        customer.print(",\"state\": ");
        customer.print(pins[pin_reference_number].state ? "1" : "0");
        customer.print(",\"name\": \"");
        customer.print(pins[pin_reference_number].name);
        customer.print("\"}");
        customer.println("]}");

        /* Ejemplo de JSON {pins:[{number: 1, state: 1}]} */
    } else {
        // Error: pin inválido
        customer.println("HTTP/1.1 400 Bad Request");
        customer.println("Content-Type: application/json");
        customer.println("Access-Control-Allow-Origin: *"); // CORS
        customer.println("Connection: close");
        customer.println();
        customer.println("{\"error\":\"Número de pin inválido\"}");
    }
}

/**
 * @brief Envia el estado de todas las lamparas a la interfaz web como objeto JSON
 *
 * @param customer Referencia a la conexión Cliente
 */
void GET_all_pins_state(EthernetClient customer)
{
    // Inicio de la respuesta
    customer.println("HTTP/1.1 200 OK");
    customer.println("Content-Type: application/json");
    customer.println("Access-Control-Allow-Origin: *"); // CORS
    customer.println("Connection: close");
    customer.println();

    // Información JSON
    customer.print("{\"pins\":[");
    for (int i=0;i<MAX_PINS;i++)
    {
        customer.print("{\"number\": ");
        customer.print(pins[i].reference_number);
        customer.print(",\"state\": ");
        customer.print(pins[i].state ? "1" : "0");
        customer.print(",\"name\": \"");
        customer.print(pins[i].name);
        customer.print("\"}");
        if (i < MAX_PINS-1) customer.print(",");
    }
    customer.println("]}");

    /* Ejemplo de JSON {pins:[{number: 1, state: 1}, {number: 2, state: 0}, {number: 3, state: 1}]} */
}

/**
 * @brief Cambia el estado de un pin en base a una petición HTTP POST
 *
 * @param pin Referencia a la estructura PIN que se desea cambiar
 * @param new_state Nuevo estado del pin
 * @param customer Referencia a la conexión Cliente
 */
void POST_pin_state(int pin_reference_number, int new_state, EthernetClient customer)
{
    if (pin_reference_number >= 0 && pin_reference_number < MAX_PINS) {
        if (new_state == 1) {
            digitalWrite(pins[pin_reference_number].number, LOW);  // Enciende
            pins[pin_reference_number].state = true;
        } else {
            digitalWrite(pins[pin_reference_number].number, HIGH); // Apaga
            pins[pin_reference_number].state = false;
        }

        // Responder con JSON confirmando el cambio
        customer.println("HTTP/1.1 200 OK");
        customer.println("Content-Type: application/json");
        customer.println("Access-Control-Allow-Origin: *"); // CORS
        customer.println("Connection: close");
        customer.println();

        customer.print("{\"mensaje\":\"estado actualizado\",\"pin\":");
        customer.print(pin_reference_number);
        customer.print(",\"new_state\":");
        customer.print(pins[pin_reference_number].state ? "1" : "0");
        customer.print(",\"name\": \"");
        customer.print(pins[pin_reference_number].name);
        customer.print("\"}");
    } else {
        // Error: pin inválido
        customer.println("HTTP/1.1 400 Bad Request");
        customer.println("Content-Type: application/json");
        customer.println("Access-Control-Allow-Origin: *"); // CORS
        customer.println("Connection: close");
        customer.println();
        customer.println("{\"error\":\"Número de pin inválido\"}");
    }
}
