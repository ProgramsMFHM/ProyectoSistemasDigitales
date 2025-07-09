# Proyecto de Iluminación Domótica 💡🤖

**Universidad de Magallanes - Curso de Sistemas Digitales**  
**Ingeniería en Computación e Informática**  
**Autores: Milton Hernández, Bruno Martínez y Cristian Flores**  
**Profesor guía: Eduardo Peña**  
**Basado en el proyecto original de Juan Pablo Arancibia**

## Descripción

Este proyecto consiste en un **servidor web embebido en un Arduino**, que permite controlar la iluminación del hogar mediante una red local **por conexión Ethernet**. El sistema incluye una interfaz web desarrollada en HTML, CSS y JavaScript, que utiliza la **API `fetch()` para realizar solicitudes HTTP tipo REST** al servidor del Arduino.

Estas solicitudes permiten activar o desactivar los pines digitales del microcontrolador, encendiendo o apagando luces de forma remota, desde cualquier navegador moderno.

Este trabajo forma parte del curso de **Sistemas Digitales** de la **Universidad de Magallanes**, y tiene como objetivo aplicar conocimientos de microcontroladores, electrónica digital y tecnologías web en un proyecto funcional de automatización doméstica.

---

## Características actuales

- **Servidor web embebido** en Arduino accesible vía red Ethernet.
- Control de luces mediante **solicitudes HTTP tipo REST** utilizando `fetch()` en JavaScript.
- Interfaz web accesible desde cualquier navegador.
- **Diseño web responsive**, adaptable a celulares, tablets y PC.
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
- **HTML y CSS (interfaz responsive)**
- **JavaScript**, utilizando **la API `fetch()` para implementar una comunicación tipo REST**
- **Protocolo HTTP** para el intercambio de comandos entre cliente y servidor

---

## Estructura del proyecto

El proyecto está organizado de la siguiente forma:

- `main.ino`: Script principal que se sube al Arduino, contiene la lógica de control de pines y el servidor web embebido.
- Carpeta `HTML/`: Contiene los archivos del cliente web:
  - `index.html`: Página principal con botones de control.
  - `script.js`: Lógica en JavaScript que usa `fetch()` para enviar comandos al servidor.
  - `style.css`: Estilos para la interfaz con diseño responsive.

---

## Cómo usar

1. Conectar el Arduino a la red local mediante cable Ethernet.
2. Subir el código `main.ino` desde el IDE de Arduino.
3. Verificar la dirección IP asignada al Arduino.
4. Abrir un navegador y acceder a la IP del dispositivo.
5. Utilizar los botones de la interfaz web para enviar comandos REST (encender/apagar luces).
