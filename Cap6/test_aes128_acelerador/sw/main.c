/*******************************************************************************
 * Archivo:     main.c
 * Autor:       Juan Manuel Delgado Romero
 * Fecha:       Febrero 2026
 *
 * Institución: Universidad de Sevilla - Escuela Politécnica Superior
 * Titulación:  Grado en Ingeniería Electrónica Industrial
 * Proyecto:    Trabajo Fin de Grado (TFG)
 * Licencia:    MIT (Ver archivo LICENSE)
 *
 * Descripción: 
 * Código de aplicación para la validación y testeo del acelerador criptográfico 
 * AES-128 integrado en el Custom Functions Subsystem (CFS) del NEORV32. Se lleva 
 * a cabo la medición de ciclos de reloj para el cifrado de un bloque de 128 bits.
 ******************************************************************************/

#include <neorv32.h>
#include "neorv32_aes128_acelerador.h"
#include <stdio.h>
#include <stdint.h>

#define BAUD_RATE 19200


int main(){

		if (neorv32_cfs_available() == 0) {
		neorv32_uart0_printf("Error! No CFS synthesized!\n");
		return 1;
		}
 
		neorv32_rte_setup();
		neorv32_uart0_setup(BAUD_RATE,0);

		neorv32_uart0_printf("\n\n-- Prueba Medida Ciclos de Reloj AES128 (Acelerador HW) --\n\r");
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
		
		uint8_t out[16] = {0};
		uint32_t ini = neorv32_cpu_csr_read(CSR_MCYCLE);
		CipherAcelerador(in,key,out);
		uint32_t fin = neorv32_cpu_csr_read(CSR_MCYCLE); 
		uint32_t ciclos = fin - ini;
		neorv32_uart0_printf("El numero de ciclos de reloj ha sido: %u\n",ciclos); // Impresión ciclos de reloj
		neorv32_uart0_printf("\n-- CIPHERTEXT: 0x");
                for(int i = 0; i < 16; i++){
                        neorv32_uart0_putc(hex[(out[i] >> 4) & 0xF]); // nibble alto
                        neorv32_uart0_putc(hex[out[i] & 0xF]);        // nibble bajo
                }
                neorv32_uart0_printf("\n\r");



		return 0;
}
