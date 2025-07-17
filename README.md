# Proyecto de Iluminación Domótica 💡🤖

**Universidad de Magallanes - Curso de Sistemas Digitales**  
**Ingeniería en Computación e Informática**  
**Autores: Milton Hernández Morales, Cristian Flores Miranda y Bruno Martínez Flores**  
**Profesor guía: Eduardo Peña Jaramillo**  
**Basado en el proyecto original de Juan Pablo Arancibia**

---

## Descripción

Este proyecto consiste en un **servidor web embebido en un Arduino**, que permite controlar la iluminación del hogar mediante una red local **por conexión Ethernet**. El sistema incluye una interfaz web desarrollada en HTML, CSS y JavaScript, que utiliza la **API `fetch()` para realizar solicitudes HTTP tipo REST** al servidor del Arduino.

Estas solicitudes permiten activar o desactivar los pines digitales del microcontrolador, encendiendo o apagando luces de forma remota desde cualquier navegador moderno.

Además del control manual desde la interfaz web, el sistema **ya integra sensores de proximidad y luminosidad**, permitiendo el encendido inteligente de luces en función del entorno físico (por ejemplo, encender luces al detectar presencia o baja luminosidad).

Este trabajo forma parte del curso de **Sistemas Digitales** de la **Universidad de Magallanes**, y tiene como objetivo aplicar conocimientos de microcontroladores, electrónica digital y tecnologías web en un proyecto funcional de automatización doméstica.

---

## Características actuales

- ✅ **Servidor web embebido** en Arduino accesible vía red Ethernet.
- ✅ Control de luces mediante **solicitudes HTTP tipo REST** utilizando `fetch()` en JavaScript.
- ✅ Interfaz web accesible desde cualquier navegador.
- ✅ **Diseño web responsive**, adaptable a celulares, tablets y PC.
- ✅ Control individual de pines digitales para encendido/apagado.
- ✅ **Soporte para sensores de proximidad (HC-SR04)** y **luminosidad (TSL2561)** para activar luces automáticamente.
- ✅ **Arquitectura orientada a objetos**, lo que permite una **alta escalabilidad**.

---

## Escalabilidad del sistema

El sistema implementa una estructura de **programación orientada a objetos (POO)** en el código Arduino. Se define una **clase base que controla LEDs estándar**, mientras que los **LEDs activados por sensores** (proximidad y luminosidad) **heredan de esta clase**.

Gracias a esta arquitectura, la integración de nuevos sensores o dispositivos inteligentes puede realizarse fácilmente con **mínimos cambios al código**, facilitando la evolución futura del sistema domótico.

---

## Librerías y encabezados utilizados

El código de Arduino utiliza los siguientes encabezados:

- SPI.h  
- Ethernet.h  
- Wire.h  
- Adafruit_Sensor.h  
- Adafruit_TSL2561_U.h

### Recomendaciones de instalación:

- Las librerías **Adafruit_TSL2561_U** y **Adafruit_Sensor** no vienen incluidas por defecto en el Arduino IDE.
  - Para instalarlas:
    1. Abrir el **Arduino IDE**.
    2. Ir a **Herramientas > Administrar bibliotecas...**
    3. Buscar e instalar:
       - `Adafruit Unified Sensor`
       - `Adafruit TSL2561`
- Las librerías **SPI.h**, **Ethernet.h** y **Wire.h** vienen preinstaladas por defecto y no requieren instalación adicional.

---

## Requisitos de hardware

Para emular y probar el proyecto se requieren los siguientes componentes:

- 🧠 **1 Arduino UNO** (placa base)
- 🌐 **1 Módulo Ethernet** compatible (por ejemplo, W5100)
- 💡 **4 LEDs** (luces de salida)
- 🌞 **1 Sensor de luminosidad** TSL2561 / GY2561
- 🚶 **1 Sensor de proximidad** HC-SR04
- 🔌 **1 Módulo relé de 4 canales**
- ⚡ **1 regleta con transformador** para alimentar el sistema
- 🧰 **Protoboard**
- 🌐 **Cable de red Ethernet**
- 🧷 **Cables de conexión jumper**
- 💻 **PC con Arduino IDE instalado** para cargar el código a la placa
- 🌐 **Navegador web moderno** (Chrome, Firefox, Edge, etc.) para controlar el sistema

---

## Tecnologías utilizadas

- Arduino UNO + Módulo Ethernet  
- Lenguaje C/C++ (Arduino IDE)  
- HTML y CSS (interfaz responsive)  
- JavaScript con **API `fetch()`** para la comunicación tipo REST  
- Protocolo HTTP entre cliente y servidor  

---

## Estructura del proyecto

El proyecto está organizado de la siguiente forma:

- `main.ino`: Script principal que se sube al Arduino, contiene:
  - Lógica del servidor web embebido.
  - Definición de clases orientadas a objetos para manejo de LEDs.
  - Control de sensores y automatización.
- Carpeta `HTML/`: Contiene los archivos del cliente web:
  - `index.html`: Página principal con botones de control.
  - `script.js`: Código JavaScript que usa `fetch()` para enviar comandos al servidor.
  - `style.css`: Estilos de la interfaz con diseño responsive.

---

## Cómo usar

1. Conectar el Arduino UNO a la red local mediante cable Ethernet.
2. Subir el código `main.ino` desde el IDE de Arduino a la placa.
3. Abrir el archivo `index.html` en un navegador web moderno.
4. Utilizar la interfaz gráfica para interactuar con el sistema:
   - 🔘 **Encender o apagar manualmente** las luces controladas directamente por botones.
   - 📡 **Habilitar o deshabilitar los sensores** (proximidad y luminosidad) para que enciendan o apaguen luces automáticamente según condiciones del entorno.
