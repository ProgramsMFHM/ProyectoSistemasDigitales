

# Proyecto de Iluminación Domótica

**Universidad de Magallanes - Curso de Sistemas Digitales**  
**Carrera de Ingeniería en Computación e Informática**  
**Autores: Milton Hernández, Bruno Martínez y Cristian Flores**  
**Profesor guía: Eduardo Peña**  
**Basado en el proyecto original de Juan Pablo Arancibia**

## Descripción

Este proyecto consiste en un **servidor web que controla los pines de un Arduino** para administrar la iluminación de un hogar de manera remota. A través de una interfaz web que utiliza **JavaScript y la API `fetch()`**, se ejecutan solicitudes HTTP que permiten **encender y apagar luces** de forma sencilla e intuitiva.

El sistema se conecta mediante **Ethernet**, lo que permite una comunicación estable y directa en redes locales cableadas, sin depender de módulos Wi-Fi.

Este trabajo forma parte del curso de **Sistemas Digitales** de la **Universidad de Magallanes**, y tiene como objetivo aplicar conocimientos de microcontroladores, electrónica digital y tecnologías web en un proyecto funcional de automatización doméstica.

---

## Características actuales

- Control de luces mediante **solicitudes HTTP utilizando `fetch()` desde JavaScript**.
- Interfaz web accesible desde cualquier navegador.
- **Diseño web responsive**, adaptable a celulares, tablets y PC.
- Comunicación mediante **conexión Ethernet**.
- Control individual de pines digitales para encendido/apagado.

---

## Objetivos de expansión

En una segunda fase del proyecto, se planea integrar sensores y mejoras que apunten a una gestión más eficiente y segura del hogar:

- 🔥 **Sensor de temperatura:** Para automatizar el control térmico o emitir alertas.
- 🕵️‍♂️ **Sensor de movimiento (PIR):** Para activar luces automáticamente o detectar presencia.

---

## Tecnologías utilizadas

- **Arduino UNO + Módulo Ethernet**
- **Lenguaje C/C++ (Arduino IDE)**
- **JavaScript (con `fetch()` para llamadas HTTP)**
- **HTML/CSS (interfaz de usuario responsive)**
- **Servidor web embebido sobre red Ethernet**

---

## Estructura del proyecto

El proyecto está organizado de la siguiente forma:

- `main.ino`: Script principal que se sube al Arduino, contiene la lógica de control de pines y servidor web.
- Carpeta `HTML/`: Contiene los archivos del frontend:
  - `index.html`: Página principal.
  - `script.js`: Código JavaScript para manejar eventos y realizar solicitudes `fetch()`.
  - `style.css`: Estilos para diseño visual y responsive.

---

## Cómo usar

1. Conectar el Arduino a la red local mediante cable Ethernet.
2. Subir el código `main.ino` desde el IDE de Arduino.
3. Verificar la dirección IP asignada al Arduino.
4. Acceder desde el navegador a la IP del dispositivo.
5. Utilizar la interfaz para enviar comandos a través de la red local.
