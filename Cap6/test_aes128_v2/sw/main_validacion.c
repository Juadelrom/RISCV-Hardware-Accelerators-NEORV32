/*******************************************************************************
 * Archivo:     main_validacion.c
 * Autor:       Juan Manuel Delgado Romero
 * Fecha:       Febrero 2026
 *
 * Institución: Universidad de Sevilla - Escuela Politécnica Superior
 * Titulación:  Grado en Ingeniería Electrónica Industrial
 * Proyecto:    Trabajo Fin de Grado (TFG)
 * Licencia:    MIT (Ver archivo LICENSE)
 *
 * Descripción: 
 * Código de aplicación para la realización de la prueba de conformidad con el
 * estándar del NIST. Se verifica mediante la impresión de los vectores inter-
 * medios de cada ronda.
 ******************************************************************************/

#include <neorv32.h>
#include <stdio.h>
#include <stdint.h>
#include "neorv32_aes128_validacion_v2.h"

#define BAUD_RATE 19200


int main(){
	/*Configuración UART0*/
  	neorv32_rte_setup();
  	neorv32_uart0_setup(BAUD_RATE,0);
	neorv32_uart0_printf("\n\n-- Prueba de Validacion Vectores de Prueba NIST para AES-128 (Version 2) --\n\r");
	/*Definición PlainText y CipherKey para prueba de Validacion*/
		// PlainText Validación --> 00112233445566778899aabbccddeeff
		uint8_t in[4*Nb] = {0};
		for(int i = 0; i < 16; i++){
			in[i] = i*16 + i; // formula para calcular en decimal el plaintext 
		}
		// Impresión PlainText
		const char hex[] = "0123456789abcdef";
		neorv32_uart0_printf("\nPLAINTEXT: 0x");
		for(int i = 0; i < 16; i++){
        		neorv32_uart0_putc(hex[(in[i] >> 4) & 0xF]); // nibble alto
        		neorv32_uart0_putc(hex[in[i] & 0xF]);        // nibble bajo
		}
		neorv32_uart0_printf("\n\r");
		// Key Validación --> 000102030405060708090a0b0c0d0e0f
 		uint8_t key[4*Nk] = {0};
		for(int i = 0; i < 16; i++){
			key[i] = i; // formula para calcular en decimal la clave
		}
                // Impresión Key
                neorv32_uart0_printf("\nKEY: 0x");
                for(int i = 0; i < 16; i++){
                        neorv32_uart0_putc(hex[(key[i] >> 4) & 0xF]); // nibble alto
                        neorv32_uart0_putc(hex[key[i] & 0xF]);        // nibble bajo
                }
                neorv32_uart0_printf("\n\r");

 	uint32_t w[44];
	uint8_t out[4*Nb];
	neorv32_uart0_printf("\n-- Cifrado Version 2 --\n\r");
        KeyExpansionV2(key,w);
	CipherV2(in,out,w);
	neorv32_uart0_printf("\n-- Descifrado Version 2 --\n\r");
       	InvCipherV2(out,out,w);

  	return 0;

}
