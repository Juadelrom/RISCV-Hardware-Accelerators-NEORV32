/* ******************************************************************************
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
 * Código de aplicación principal del demostrador interactivo del sistema. 
 * Implementa una interfaz de usuario a través de UART que permite evaluar y 
 * comparar el rendimiento (en ciclos de reloj) de los distintos casos de  
 * estudio del algoritmo AES-128 (Tiny-AES, extensiones ISA Zkne/Zknd y acelerador 
 * hardware integrado en el CFS). Además, incluye un comando de inyección de 
 * errores de memoria para provocar el colapso de la CPU, demostrando así la 
 * robustez y el desacoplamiento hw/sw del sistema de visualización.
 * Para más detalle veáse el Capítulo 8 de la memoria de este trabajo.
 * *****************************************************************************/

#include <neorv32.h>
#include <stdint.h>
#include <string.h>
#include "aes.h"                      // Libreria Tiny-AES (C puro)
#include "neorv32_aes128_v1.h"	      // Libreria ISA ZKne/d Version 1
#include "neorv32_aes128_v2.h"        // Libreria ISA Zkne/d Version 2
#include "neorv32_aes128_v3.h"        // Libreria ISA Zkne/d Version 3 (descifrado optimizado)
#include "neorv32_aes128_acelerador.h"  // Libreria AES128 Aclerador HW 


#define BAUD_RATE  19200

// Vectores de prueba
uint8_t key[16]   = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
uint8_t in[16] = {0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d, 0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34};

char hex[] = "0123456789abcdef";

int main(void) {

    // 1. Inicialización del entorno NEORV32
    neorv32_rte_setup(); // Configura las excepciones y el entorno de ejecución
    
    // 2. Inicialización de la UART0 a 19200 baudios (Típico en NEORV32)
    neorv32_uart0_setup(BAUD_RATE, 0);

    neorv32_uart0_printf("\n\n");
    neorv32_uart0_printf("================================================\n");
    neorv32_uart0_printf("   DEMOSTRADOR Criptografico y Visualizador     \n");
    neorv32_uart0_printf("================================================\n");

    char cmd;	// Comando menú principal
    char sub_cmd;	// Comando Submenú 
    uint32_t ciclos_inicio, ciclos_fin, ciclos_total;	// varibale medidas de cilos de reloj
    uint32_t w[44]; // Variable KeySchedule
    uint8_t out[16]; // Variable Salida
    // Bucle infinito del programa principal
    while (1) {
        neorv32_uart0_printf("\n--- MENU PRINCIPAL ---\n");
        neorv32_uart0_printf(" c : Cifrado (Medicion de ciclos)\n");
        neorv32_uart0_printf(" d : Descifrado (Medicion de ciclos)\n");
        neorv32_uart0_printf(" e : Inyeccion de Error\n");
        neorv32_uart0_printf("\nSeleccione un comando: ");

        // Leer comando del usuario y hacer eco
        cmd = neorv32_uart0_getc();
        neorv32_uart0_putc(cmd);
        neorv32_uart0_printf("\n");

        // ---------------------------------------------------------
        // OPCIÓN C: CIFRADO
        // ---------------------------------------------------------
        if (cmd == 'c') {
            neorv32_uart0_printf("\n  [SUBMENU CIFRADO]\n");
            neorv32_uart0_printf("  1 : Tiny-AES (C Puro)\n");
            neorv32_uart0_printf("  2 : ISA Zkne (Version 1 - Basica)\n");
            neorv32_uart0_printf("  3 : ISA Zkne (Version 2 - Optimizada)\n");
            neorv32_uart0_printf("  4 : Acelerador Hardware (CFS)\n");
            neorv32_uart0_printf("  Seleccione opcion: ");
            
            sub_cmd = neorv32_uart0_getc();
            neorv32_uart0_putc(sub_cmd);
            neorv32_uart0_printf("\n");
            neorv32_uart0_printf("\n================================\n");
	    neorv32_uart0_printf("\nPLAINTEXT: Ox"); // Impresión plaintext
	    for(int i = 0; i < 16; i++){
		neorv32_uart0_putc(hex[(in[i] >> 4) & 0xF]); // nibble alto
		neorv32_uart0_putc(hex[in[i] & 0xF]); // nibble bajo
	    }
	    neorv32_uart0_printf("\n\n");
            neorv32_uart0_printf("KEY: Ox"); // Impresión key
            for(int i = 0; i < 16; i++){
                neorv32_uart0_putc(hex[(key[i] >> 4) & 0xF]); // nibble alto
                neorv32_uart0_putc(hex[key[i] & 0xF]); // nibble bajo
            }
            neorv32_uart0_printf("\n================================\n");

            switch (sub_cmd) {
                case '1':
                    // Cifrado Tiny-AES
                    neorv32_uart0_printf("  -> Ejecutando Tiny-AES...\n");
		    struct AES_ctx ctx; // Definición estructura para keyschedule
        	    AES_init_ctx(&ctx,key); // Cálculo keyschedule
		    memcpy(out, in, 16);  // copia de seguridad de in
		    ciclos_inicio = neorv32_cpu_csr_read(CSR_MCYCLE);
		    AES_ECB_encrypt(&ctx,in); // Cifrado
        	    ciclos_fin = neorv32_cpu_csr_read(CSR_MCYCLE); // Lectura ciclos de reloj después de cifrado
            	    ciclos_total = ciclos_fin - ciclos_inicio; // Cálculo cilos de reloj cifrado
                    neorv32_uart0_printf("\nCipherText: Ox"); // Impresión ciphertext
                    for(int i = 0; i < 16; i++){
                        neorv32_uart0_putc(hex[(in[i] >> 4) & 0xF]); // nibble alto
                        neorv32_uart0_putc(hex[in[i] & 0xF]); // nibble bajo
                    }
	            neorv32_uart0_printf("\n\n");
	            neorv32_uart0_printf("\nEl numero de ciclos de reloj ha sido: %u\n\n",ciclos_total); // Impresión ciclos de reloj
		    memcpy(in,out,16);  // restablecimiento de in
                    break;
                case '2':
                    // Cifrado ISA Zkne Version 1
                    neorv32_uart0_printf("  -> Ejecutando Zkne V1...\n");
		    KeyExpansionV1(key,w); // Cálculo KeySchedule
		    ciclos_inicio = neorv32_cpu_csr_read(CSR_MCYCLE); // Lectura ciclos de reloj antes del cifrado
		    CipherV1(in,out,w); // Cifrado
   		    ciclos_fin = neorv32_cpu_csr_read(CSR_MCYCLE); // Lectura ciclos de reloj después del cifrado
                    neorv32_uart0_printf("\nCipherText: Ox"); // Impresión ciphertext
                    for(int i = 0; i < 16; i++){
                        neorv32_uart0_putc(hex[(out[i] >> 4) & 0xF]); // nibble alto
                        neorv32_uart0_putc(hex[out[i] & 0xF]); // nibble bajo
                    }
                    neorv32_uart0_printf("\n\n");
   		    ciclos_total = ciclos_fin - ciclos_inicio; // Cálculo ciclos de reloj cifrado
        	    neorv32_uart0_printf("\nEl numero de ciclos de reloj ha sido: %u\n\n",ciclos_total); // Impresión ciclos de reloj
                    break;
                case '3':
                    // Cifrado ISA Zkne Version 2
                    neorv32_uart0_printf("  -> Ejecutando Zkne V2...\n");
                    KeyExpansionV2(key,w); // Cálculo KeySchedule
                    ciclos_inicio = neorv32_cpu_csr_read(CSR_MCYCLE); // Lectura ciclos de reloj antes d>
                    CipherV2(in,out,w); // Cifrado
                    ciclos_fin = neorv32_cpu_csr_read(CSR_MCYCLE); // Lectura ciclos de reloj después de>
                    ciclos_total = ciclos_fin - ciclos_inicio; // Cálculo ciclos de reloj cifrado
                    neorv32_uart0_printf("\nCipherText: Ox"); // Impresión ciphertext
                    for(int i = 0; i < 16; i++){
                        neorv32_uart0_putc(hex[(out[i] >> 4) & 0xF]); // nibble alto
                        neorv32_uart0_putc(hex[out[i] & 0xF]); // nibble bajo
                    }
                    neorv32_uart0_printf("\n\n");
                    neorv32_uart0_printf("\nEl numero de ciclos de reloj ha sido: %u\n\n",ciclos_total); // Impresion cilos de reloj
                    break;
                case '4':
                    // Cifrado AES128 Aclerador HW
                    neorv32_uart0_printf("  -> Ejecutando Acelerador HW...\n");
                    ciclos_inicio = neorv32_cpu_csr_read(CSR_MCYCLE); // Lectura ciclos de reloj antes d>
                    CipherAcelerador(in,key,out); // Cifrado
                    ciclos_fin = neorv32_cpu_csr_read(CSR_MCYCLE); // Lectura ciclos de reloj después de>
                    ciclos_total = ciclos_fin - ciclos_inicio; // Cálculo ciclos de reloj cifrado
                    neorv32_uart0_printf("\nCipherText: Ox"); // Impresión ciphertext
                    for(int i = 0; i < 16; i++){
                        neorv32_uart0_putc(hex[(out[i] >> 4) & 0xF]); // nibble alto
                        neorv32_uart0_putc(hex[out[i] & 0xF]); // nibble bajo
                    }
                    neorv32_uart0_printf("\n\n");
                    neorv32_uart0_printf("\nEl numero de ciclos de reloj ha sido: %u\n\n",ciclos_total); // Impresion de ciclos de reloj
                    break;
                default:
                    neorv32_uart0_printf("  [!] Opcion no valida.\n");
                    continue; // Vuelve al inicio del while(1)
            }

        }

        // ---------------------------------------------------------
        // OPCIÓN D: DESCIFRADO
        // ---------------------------------------------------------
        else if (cmd == 'd') {
            neorv32_uart0_printf("\n  [SUBMENU DESCIFRADO]\n");
            neorv32_uart0_printf("  1 : Tiny-AES (C Puro)\n");
            neorv32_uart0_printf("  2 : ISA Zknd (Version 2 - Sin Optimizar)\n");
            neorv32_uart0_printf("  3 : ISA Zknd (Version 3 - Optimizada)\n");
            neorv32_uart0_printf("  Seleccione opcion: ");
            
            sub_cmd = neorv32_uart0_getc();
            neorv32_uart0_putc(sub_cmd);
            neorv32_uart0_printf("\n");
            neorv32_uart0_printf("\n================================\n");
            neorv32_uart0_printf("\nPLAINTEXT: Ox"); // Impresión plaintext
            for(int i = 0; i < 16; i++){
                neorv32_uart0_putc(hex[(in[i] >> 4) & 0xF]); // nibble alto
                neorv32_uart0_putc(hex[in[i] & 0xF]); // nibble bajo
            }
            neorv32_uart0_printf("\n\n");
            neorv32_uart0_printf("KEY: Ox"); // Impresión key
            for(int i = 0; i < 16; i++){
                neorv32_uart0_putc(hex[(key[i] >> 4) & 0xF]); // nibble alto
                neorv32_uart0_putc(hex[key[i] & 0xF]); // nibble bajo
            }
            neorv32_uart0_printf("\n================================\n");


            switch (sub_cmd) {
                case '1':
                    // Descifrado tiny-AES
                    neorv32_uart0_printf("  -> Ejecutando Tiny-AES Descifrado...\n");
                    struct AES_ctx ctx; // Definición estructura para keyschedule
		    memcpy(out,in,16); // copia de suguridad de in
                    AES_init_ctx(&ctx,key); // Cálculo keyschedule
                    AES_ECB_encrypt(&ctx,in); // Cifrado
                    neorv32_uart0_printf("\nCipherText: Ox"); // Impresión ciphertext
                    for(int i = 0; i < 16; i++){
                        neorv32_uart0_putc(hex[(in[i] >> 4) & 0xF]); // nibble alto
                        neorv32_uart0_putc(hex[in[i] & 0xF]); // nibble bajo
                    }
                    neorv32_uart0_printf("\n\n");
                    ciclos_inicio = neorv32_cpu_csr_read(CSR_MCYCLE); // Lectura ciclos de reloj antes d>
		    AES_ECB_decrypt(&ctx, in);// Descifrado
                    ciclos_fin = neorv32_cpu_csr_read(CSR_MCYCLE); // Lectura ciclos de reloj después de>
                    ciclos_total = ciclos_fin - ciclos_inicio; // Cálculo ciclos de reloj cifrado
                    neorv32_uart0_printf("\nPlainText: Ox"); // Impresión plaintext
                    for(int i = 0; i < 16; i++){
                        neorv32_uart0_putc(hex[(in[i] >> 4) & 0xF]); // nibble alto
                        neorv32_uart0_putc(hex[in[i] & 0xF]); // nibble bajo
                    }
                    neorv32_uart0_printf("\n\n");
                    neorv32_uart0_printf("\nEl numero de ciclos de reloj ha sido: %u\n\n",ciclos_total); // Impresion ciclos de reloj
		    memcpy(in,out,16);  // restablecimiento de in
                    break;
                case '2':
                    // Descifrado ISA Zknd Version 2
                    neorv32_uart0_printf("  -> Ejecutando Zknd V2...\n");
                    KeyExpansionV2(key,w); // Cálculo KeySchedule
                    CipherV2(in,out,w); // Cifrado
                    neorv32_uart0_printf("\nCipherText: Ox"); // Impresión ciphertext
                    for(int i = 0; i < 16; i++){
                        neorv32_uart0_putc(hex[(out[i] >> 4) & 0xF]); // nibble alto
                        neorv32_uart0_putc(hex[out[i] & 0xF]); // nibble bajo
                    }
                    neorv32_uart0_printf("\n\n");
                    ciclos_inicio = neorv32_cpu_csr_read(CSR_MCYCLE); // Lectura ciclos de reloj antes d>
		    InvCipherV2(out,out,w);
                    ciclos_fin = neorv32_cpu_csr_read(CSR_MCYCLE); // Lectura ciclos de reloj después de>
                    ciclos_total = ciclos_fin - ciclos_inicio; // Cálculo ciclos de reloj cifrado
                    neorv32_uart0_printf("\nPlainText: Ox"); // Impresión plaintext
                    for(int i = 0; i < 16; i++){
                        neorv32_uart0_putc(hex[(out[i] >> 4) & 0xF]); // nibble alto
                        neorv32_uart0_putc(hex[out[i] & 0xF]); // nibble bajo
                    }
                    neorv32_uart0_printf("\n\n");
                    neorv32_uart0_printf("\nEl numero de ciclos de reloj ha sido: %u\n\n",ciclos_total); // Impre>
                    break;
                case '3':
                    // Descifrado ISA Zknd Version 3
                    neorv32_uart0_printf("  -> Ejecutando Zknd V2...\n");
                    KeyExpansionV3(key,w); // Cálculo KeySchedule
                    CipherV3(in,out,w); // Cifrado
                    neorv32_uart0_printf("\nCipherText: Ox"); // Impresión ciphertext
                    for(int i = 0; i < 16; i++){
                        neorv32_uart0_putc(hex[(out[i] >> 4) & 0xF]); // nibble alto
                        neorv32_uart0_putc(hex[out[i] & 0xF]); // nibble bajo
                    }
                    neorv32_uart0_printf("\n\n");
                    ciclos_inicio = neorv32_cpu_csr_read(CSR_MCYCLE); // Lectura ciclos de reloj antes d>
                    InvCipherV3(out,out,w);
                    ciclos_fin = neorv32_cpu_csr_read(CSR_MCYCLE); // Lectura ciclos de reloj después de>
                    ciclos_total = ciclos_fin - ciclos_inicio; // Cálculo ciclos de reloj cifrado
                    neorv32_uart0_printf("\nPlainText: Ox"); // Impresión plaintext
                    for(int i = 0; i < 16; i++){
                        neorv32_uart0_putc(hex[(out[i] >> 4) & 0xF]); // nibble alto
                        neorv32_uart0_putc(hex[out[i] & 0xF]); // nibble bajo
                    }
                    neorv32_uart0_printf("\n\n");
                    neorv32_uart0_printf("\nEl numero de ciclos de reloj ha sido: %u\n\n",ciclos_total); // Impre>
                    break;
                default:
                    neorv32_uart0_printf("  [!] Opcion no valida.\n");
                    continue;
            }

        }

        // ---------------------------------------------------------
        // OPCIÓN E: INYECCIÓN DE ERROR (Demostración Conduits CFS)
        // ---------------------------------------------------------
        else if (cmd == 'e') {
            neorv32_uart0_printf("\n Simulacion de error por acceso ilegal a memoria.\n");
            neorv32_uart0_printf("Introduzca el numero de inyecciones de error (ej. 1000): ");
            
            // Leer número desde el puerto serie (usamos neorv32_uart0_scan)
            char buffer[16];
            neorv32_uart0_scan(buffer, 16, 1); // Lee string con eco
            
            // Convertir string a entero
            uint32_t num_errores = 0;
            for(int i = 0; buffer[i] != '\0' && buffer[i] != '\r'; i++) {
                if(buffer[i] >= '0' && buffer[i] <= '9') {
                    num_errores = num_errores * 10 + (buffer[i] - '0');
                }
            }

            neorv32_uart0_printf("\nInyectando %u accesos ilegales...\n", num_errores);
            neorv32_uart0_printf("Observa la placa: La CPU colapsara pero el Display/ADC seguira funcionando.\n");

                uint32_t direccion_invalida = 50000; 
                uint32_t valor_leido_basura;

            for (uint32_t i = 0; i < num_errores; i++) {
                asm volatile (
                    "lw %[destino], 0(%[fuente])\n\t"  
                    : [destino] "=r" (valor_leido_basura) 
                    : [fuente]  "r"  (direccion_invalida)
                    :                                     
                );
            }
        }

        // ---------------------------------------------------------
        // OPCIÓN NO RECONOCIDA
        // ---------------------------------------------------------
        else {
            neorv32_uart0_printf("\n[!] Comando no reconocido. Intentalo de nuevo.\n");
        }
    }

    return 0;
}
