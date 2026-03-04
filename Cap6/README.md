# Capítulo 6: Implementación, Optimización y Evaluación Comparativa de AES-128 en el SoC NEORV32

Este directorio contiene el núcleo central del proyecto. Contiene todas las versiones y metodologías desarrolladas para la aceleración del algoritmo criptográfico AES-128 en el SoC NEORV32.

## Casos de Estudio
Los enfoques comparados en el análisis de rendimiento (ciclos de reloj vs. área en FPGA) se detallan en las siguientes secciones:

* **Aceleración mediante Extensiones ISA Zknd/Zkne:** (Detallado en la **Sección 6.3**)
  * `test_aes128_v1`: Implementación inicial usando *wrappers* de funciones C.
  * `test_aes128_v2`: Versión optimizada eliminando bucles anidados e integrando ensamblador en línea secuencial.
  * `test_aes128_v3`: **Optimización microarquitectural**. Versión modificada del RTL del procesador (`neorv32_cpu_cp_crypto.vhd`) para aplicar la transformación `InvMixColumns` de forma aislada.
* **Acelerador Hardware Dedicado (On-the-fly):** (Detallado en la **Sección 6.4**)
  * `test_aes128_acelerador`: Integración de un acelerador hardware para cifrado AES en el subsistema CFS, capaz de realizar expansión de claves y cifrado de forma paralela.

*(Nota: La versión base puramente software empleada en la comparativa de la memoria está basada en la librería Open-Source Tiny-AES-C).*

## Estructura del Directorio
Cada proyecto incluye
* `/sw`: *Drivers* para el uso de cada una de las versiones (`neorv32_aes128_v1.c/h`, etc.), código de aplicación (`main.c`), `makefiles` locales y ejecutables (`.bin`).
* `\hw`: Archivos de descripción hardware (`.vhd`) modificados, top-levels, archivo de restricción de pines (`.xdc`) y archivos de configuración o *bitstreams* de Vivado.


## Requisitos y Entorno
* **Procesador:** NEORV32 framework v1.11.2
* **Toolchain:** xPack GNU RISC-V Embedded GCC v14.2.0
* **Síntesis Hardware:** Xilinx Vivado 2024.2
* **Simulación:** GHDL
* **Plataforma:** Digilent Nexys 4 DDR (Artix-7 XC7A100T)
