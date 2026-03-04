# Capítulo 8: Demostrador

Este directorio engloba la integración conjunta de todos los sistemas desarrollados en los Capítulos 6 y 7 en un único entorno de evaluación ejecutado sobre la FPGA Nexys-4 DDR.

## Descripción del Demostrador
Detallado desde el apartado 8.1 al 8.4 de la memoria, el proyecto conecta:
1. El subsistema de adquisición y visualización concurrente (XADC + Visualizador 7 segmentos) gobernado de forma totalmente hardware mediante el módulo CFS.
2. Un menú interactivo por puerto serie (UART) para ejecutar las distintas alternativas del algoritmo AES-128, tanto para cifrado como descifrado.

## Pruebas de Rendimiento y Robustez
El código en `\sw\main.c` permite:
* **Medida de ciclos de reloj:** Medición empírica comparando Tiny-AES, ISA Zkne/Zknd (en todas sus versiones) y el Acelerador HW integrado en el CFS en la FPGA.
* **Inyección de errores:** A través del menú UART, el usuario puede forzar lecturas a direcciones de memoria no mapeadas, provocando un error en la CPU. Esta prueba demuestra que, aunque la CPU del NEORV32 colapse y detenga su ejecución, el hardware de visualización (integrado vía *conduits* del CFS) sigue operando de manera ininterrumpida y estable.

## Estructura del Directorio
Cada proyecto incluye
* `/sw`: *Drivers* para el uso de cada una de las versiones (`neorv32_aes128_v1.c/h`, etc.), código de aplicación (`main.c`), `makefiles` locales y ejecutables (`.bin`).
* `\hw`: Archivos de descripción hardware (`.vhd`) modificados, top-levels, archivo de restricción de pines (`.xdc`) y archivos de configuración o *bitstreams* de Vivado.

## Requisitos y Entorno
* **Procesador:** NEORV32 framework v1.11.2
* **Toolchain:** xPack GNU RISC-V Embedded GCC v14.2.0
* **Síntesis Hardware:** Xilinx Vivado 2024.2
* **Plataforma:** Digilent Nexys 4 DDR (Artix-7 XC7A100T)