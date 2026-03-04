# Capítulo 7: Diseño del Sistema de Adquisición y Visualización Multiplexada de Temperatura y Voltaje

Este directorio contiene el desarrollo de una Arquitectura de Dominio Específico (DSA) para la monitorización de magnitudes físicas internas de la FPGA Artix-7, operando de forma autónoma y desacoplada de la CPU principal gracias al empleo de los *conduits* del CFS.

## Descripción del Sistema
Todo el diseño se documenta desde el apartado 7.1 al 7.10 del TFG. El sistema se encarga de adquirir datos desde el bloque IP XADC (Temperatura, Voltaje interno y VRAM) y acondicionarlos para su visualización. 

Ubicación: Carpeta `\test_visualizador`. 

El diseño RTL (carpeta `\hw`) está compuesto por las siguientes entidades VHDL:
1. `xadc_wiz_0`: Macrobloque IP para el ADC interno.
2. `func_trans.vhd` / `func_trans_volt.vhd`: Acondicionamiento de señal y escalado mediante aritmética de enteros.
3. `valor_abs.vhd`: Gestión de números negativos (Ca2) para la temperatura.
4. `bin2bcd.vhd`: Conversor de binario a BCD empleando el algoritmo *Double Dabble*.
5. `visu7seg.vhd`: Controlador del visualizador de 7 segmentos.
6. `adc_top_level.vhd`: Entidad superior que incluye el proceso secuencial (Unidad de Control) para el enrutado dinámico.
7. `neorv32_cfs.vhd`: Archivo modificado del subsistema CFS.

Además, se incluyen el archivo de restricción de pines (`.xdc`) y el *bitstream* (`.bit`) de Vivado.

## Integración
El sistema se acopla al procesador a través del **Subsistema CFS**, aprovechando los puertos E/S (`cfs_in_i` y `cfs_out_o` / *conduits*) para interactuar directamente con interruptores y *displays* sin latencia del bus del procesador.

## Requisitos y Entorno
* **Procesador:** NEORV32 framework v1.11.2
* **Síntesis Hardware:** Xilinx Vivado 2024.2
* **Simulación:** GHDL
* **Plataforma:** Digilent Nexys 4 DDR (Artix-7 XC7A100T)