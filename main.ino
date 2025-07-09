/**
 * @file main.ino
 * @authors Milton Hernández Morales (milherna@umag.cl) Cristian Flores Miranda (crisflor@umag.cl) Bruno Martinez Flores brumarti@umag.cl
 * @brief Este programa implementa un servidor web para controlar los pines de un Arduino
 * @version 0.1
 * @date 2025-09-07
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

// Clases para los piens
class Pin {
  public:
    int number;
    String name;
    String type;
    bool state;
    int associated_pin;

    Pin(int _number, String _name, int _associated_pin) {
      number = _number;
      name = _name;
      associated_pin = _associated_pin;
      state = false;
      type = "Standard";
    }

    virtual void setup_associated_pin() {
      pinMode(associated_pin, OUTPUT);
      digitalWrite(associated_pin, HIGH); // Apagado
    }

    virtual void check_pin_state() {
      if (state) {
        digitalWrite(associated_pin, LOW);  // Encender
      } else {
        digitalWrite(associated_pin, HIGH); // Apagar
      }
    }
};

class ProximityPin : public Pin {
  public:
    int trigPin;
    int echoPin;
    float distance = 40;

    ProximityPin(int _number, String _name, int _associated_pin, int _trigPin, int _echoPin)
      : Pin(_number, _name, _associated_pin) {
        trigPin = _trigPin;
        echoPin = _echoPin;
        distance = 0;
        type = "Proximity";
    }

    void setup_associated_pin() override {
      pinMode(trigPin, OUTPUT);
      pinMode(echoPin, INPUT);
      pinMode(associated_pin, OUTPUT);

      digitalWrite(associated_pin, HIGH); // Apagado
      digitalWrite(trigPin, LOW);
      delayMicroseconds(2);
    }

    void check_pin_state() override {
        if (!state) {
            digitalWrite(associated_pin, HIGH); // Apagar
            return;
        }

        digitalWrite(trigPin, LOW);
        delayMicroseconds(2);
        digitalWrite(trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin, LOW);

        long duration = pulseIn(echoPin, HIGH);
        distance = duration / 58.0;

        if (distance < 30) {
            digitalWrite(associated_pin, LOW); // Encender
        } else {
            digitalWrite(associated_pin, HIGH); // Apagar
        }

        Serial.print("Distancia: ");
        Serial.print(distance);
        Serial.println(" cm");
    }
};

// Definimos los pines
#define MAX_PINS 4
Pin* pins[MAX_PINS];

String readString = String(30); // Petición HTTP

void GET_pin_state(int pin_reference_number, EthernetClient client);
void GET_all_pins_state(EthernetClient client);
void POST_pin_state(int pin_reference_number, int new_state, EthernetClient client);

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

    // Imprime la dirección IP asignada
    Serial.print("Dirección IP: ");
    Serial.println(Ethernet.localIP());

    // Creamos los pines
    pins[0] = new Pin(0, "Sala de Estar", 2);
    pins[1] = new Pin(1, "Dormitorio Infantil", 3);
    pins[2] = new Pin(2, "Dormitorio Principal", 4);
    pins[3] = new ProximityPin(3, "Living", 5, 7, 8);

    // Seteo de los pines asociados a los leds
    for (int i=0;i<MAX_PINS;i++) pins[i]->setup_associated_pin();
}

/**
 * @brief  Función de bucle que se encarga de recibir las peticiones HTTP
 *
*/
void loop()
{
    String requestLine = "";
    EthernetClient client = server.available(); // Crear una conexión client
    int pinState;
    int pinNumber;
    if (client)
    {
        while (client.connected())         // Cliente conectado
        {
            if(client.available())
            {
                char c = client.read();
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
                        GET_pin_state(pinNumber, client);
                    } else if (requestLine.startsWith("POST /?P=")) { // Se cambia el estado de una lampara
                        pinNumber = requestLine.substring(9,10).toInt();
                        pinState = requestLine.substring(10,11).toInt();
                        POST_pin_state(pinNumber, pinState, client);
                    } else if (requestLine.startsWith("GET / ")) { // Obtiene el estado de todas las lamparas
                        GET_all_pins_state(client);
                    }


                    client.stop(); // Cierre de la conexión con el Cliente
                    readString = "";
                    requestLine = "";
                }
            }
        }
    }

    for (int i=0;i<MAX_PINS;i++) pins[i]->check_pin_state();

    delay(300);
}

/**
 * @brief Envia el estado de una lampara a la interfaz web como objeto JSON
 *
 * @param pin Referencia a la estructura PIN que se desea imprimir
 * @param client Referencia a la conexión Cliente
 */
void GET_pin_state(int pin_reference_number, EthernetClient client)
{
    if (pin_reference_number >= 0 && pin_reference_number < MAX_PINS) {
        // Inicio de la respuesta
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: application/json");
        client.println("Access-Control-Allow-Origin: *"); // CORS
        client.println("Connection: close");
        client.println();

        // Información JSON
        client.print("{\"pins\":[");
        client.print("{\"number\": ");
        client.print(pin_reference_number);
        client.print(",\"state\": ");
        client.print(pins[pin_reference_number]->state ? "1" : "0");
        client.print(",\"name\": \"");
        client.print(pins[pin_reference_number]->name);
        client.print("\",\"type\": \"");
        client.print(pins[pin_reference_number]->type);
        client.print("\"}");
        client.println("]}");

        /* Ejemplo de JSON {pins:[{number: 1, state: 1}]} */
    } else {
        // Error: pin inválido
        client.println("HTTP/1.1 400 Bad Request");
        client.println("Content-Type: application/json");
        client.println("Access-Control-Allow-Origin: *"); // CORS
        client.println("Connection: close");
        client.println();
        client.println("{\"error\":\"Número de pin inválido\"}");
    }
}

/**
 * @brief Envia el estado de todas las lamparas a la interfaz web como objeto JSON
 *
 * @param client Referencia a la conexión Cliente
 */
void GET_all_pins_state(EthernetClient client)
{
    // Inicio de la respuesta
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: application/json");
    client.println("Access-Control-Allow-Origin: *"); // CORS
    client.println("Connection: close");
    client.println();

    // Información JSON
    client.print("{\"pins\":[");
    for (int i=0;i<MAX_PINS;i++)
    {
        client.print("{\"number\": ");
        client.print(pins[i]->number);
        client.print(",\"state\": ");
        client.print(pins[i]->state ? "1" : "0");
        client.print(",\"name\": \"");
        client.print(pins[i]->name);
        client.print("\",\"type\": \"");
        client.print(pins[i]->type);
        client.print("\"}");
        if (i < MAX_PINS-1) client.print(",");
    }
    client.println("]}");

    /* Ejemplo de JSON {pins:[{number: 1, state: 1}, {number: 2, state: 0}, {number: 3, state: 1}]} */
}

/**
 * @brief Cambia el estado de un pin en base a una petición HTTP POST
 *
 * @param pin Referencia a la estructura PIN que se desea cambiar
 * @param new_state Nuevo estado del pin
 * @param client Referencia a la conexión Cliente
 */
void POST_pin_state(int pin_reference_number, int new_state, EthernetClient client)
{
    if (pin_reference_number >= 0 && pin_reference_number < MAX_PINS) {
        if (new_state == 1) {
            pins[pin_reference_number]->state = true;
        } else {
            pins[pin_reference_number]->state = false;
        }

        // Responder con JSON confirmando el cambio
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: application/json");
        client.println("Access-Control-Allow-Origin: *"); // CORS
        client.println("Connection: close");
        client.println();

        client.print("{\"mensaje\":\"estado actualizado\",\"pin\":");
        client.print(pin_reference_number);
        client.print(",\"new_state\":");
        client.print(pins[pin_reference_number]->state ? "1" : "0");
        client.print(",\"name\": \"");
        client.print(pins[pin_reference_number]->name);
        client.print("\",\"type\": \"");
        client.print(pins[pin_reference_number]->type);
        client.print("\"}");
    } else {
        // Error: pin inválido
        client.println("HTTP/1.1 400 Bad Request");
        client.println("Content-Type: application/json");
        client.println("Access-Control-Allow-Origin: *"); // CORS
        client.println("Connection: close");
        client.println();
        client.println("{\"error\":\"Número de pin inválido\"}");
    }
}
