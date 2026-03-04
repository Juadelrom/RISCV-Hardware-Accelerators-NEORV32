# Capítulo 4: Extensiones ISA

Este directorio contiene los códigos fuente (hardware y software) desarrollados para validar y analizar el uso de extensiones específicas del conjunto de instrucciones (ISA) en la arquitectura RISC-V.

## Casos de Estudio
Los archivos de este directorio se corresponden con los siguientes casos de estudio detallados en la memoria del TFG:

* **Pruebas de la Extensión ISA M (Multiplicación y División):** * Detallado en la **Sección 4.5.1**.
  * Ubicación: Carpeta `test_isa_m`. Comprueba la inferencia automática del compilador GCC y el uso de ensamblador en línea para operaciones de 128 bits.
* **Pruebas de las Extensiones Criptográficas Zknd y Zkne:** * Detallado en la **Sección 4.5.2**.
  * Ubicación: Carpeta `test_zknd_zkne`. Valida el uso correcto de las instrucciones `aes32esmi`, `aes32esi`, `aes32dsmi` y `aes32dsi` mediante *assembly inline*.

## Estructura del Directorio
Cada caso de estudio se divide internamente en:
* `/sw`: Códigos fuente en lenguaje C (`main.c`), `makefiles` locales y ejecutables (`.bin`).
* `/hw`: Archivos de descripción hardware (`.vhd`) modificados, top-levels, archivo de restricción de pines (`.xdc`) y archivos de configuración o *bitstreams* de Vivado.

## Requisitos y Entorno
* **Procesador:** NEORV32 framework v1.11.2
* **Toolchain:** xPack GNU RISC-V Embedded GCC v14.2.0
* **Síntesis Hardware:** Xilinx Vivado 2024.2
* **Simulación:** GHDL

* **Plataforma:** Digilent Nexys 4 DDR (Artix-7 XC7A100T)

