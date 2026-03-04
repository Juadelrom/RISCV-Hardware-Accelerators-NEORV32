# Capítulo 5: Introducción de Aceleradores

Este directorio contiene los diseños hardware y los *drivers* software empleados para demostrar la integración de coprocesadores externos mapeados en memoria mediante el *Custom Functions Subsystem* (CFS) del NEORV32.

## Casos de Estudio
En este capítulo se abordan dos enfoques opuestos en cuanto a requisitos temporales y de control, documentados en las siguientes secciones del TFG:

* **Acelerador Combinacional (Media de 4 Enteros de 32 bits):** * Detallado en la **Sección 5.4.1**.
  * Ubicación: Carpeta `demo_cfs_media`. Integración de hardware combinacional que prescinde de máquina de estados.
* **Acelerador Secuencial (Divisor Entero de 32 bits Non-Restoring):** * Detallado en la **Sección 5.4.2**.
  * Ubicación: Carpeta `demo_cfs_divisor32`. Implementación de una FSM y validación del protocolo de comunicación HW/SW (Start, Busy, Error) entre la CPU y el CFS.

## Estructura del Directorio
Cada proyecto incluye
* `/sw`: Código de aplicación (`main.c`), `makefiles` locales y ejecutables (`.bin`).
* `/hw`: Códigos fuente VHDL de los aceleradores (`media_4.vhd`, `divisor_32.vhd`), junto con el archivo de integración en el bus `neorv32_cfs.vhd`, archivo de restricción de pines (`.xdc`) y *bitstreams* de Vivado.

## Requisitos y Entorno
* **Procesador:** NEORV32 framework v1.11.2
* **Toolchain:** xPack GNU RISC-V Embedded GCC v14.2.0
* **Síntesis Hardware:** Xilinx Vivado 2024.2
* **Simulación:** GHDL
* **Plataforma:** Digilent Nexys 4 DDR (Artix-7 XC7A100T)
