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
 * Código de aplicación de usuario para comprobar las extensiones ISA Zkne y
 * Zknd en el SoC NEORV32 con toolchain GCC.
 ******************************************************************************/


#include <stdint.h>
#include <neorv32.h>

#define BAUD_RATE 19200

uint32_t neorv32_aes32esmi(uint32_t rs1,uint32_t rs2,int bs){
  uint32_t rd = 0;
  switch(bs){
   case 0:
       __asm__ ("aes32esmi %0, %1, %2, 0" : "=r" (rd) : "r"(rs1), "r"(rs2));
      break;
   case 1:
         __asm__ ("aes32esmi %0, %1, %2, 1" : "=r" (rd) : "r"(rs1), "r"(rs2));
      break;
   case 2:
     __asm__ ("aes32esmi %0, %1, %2, 2" : "=r" (rd) : "r"(rs1), "r"(rs2));
     break;
   case 3:
     __asm__ ("aes32esmi %0, %1, %2, 3" : "=r" (rd) : "r"(rs1), "r"(rs2));
     break;
   default:
     rd = 0;
   }
  return rd;

}

uint32_t neorv32_aes32esi(uint32_t rs1,uint32_t rs2,int bs){
  uint32_t rd = 0;
  switch(bs){
   case 0:
       __asm__ ("aes32esi %0, %1, %2, 0" : "=r" (rd) : "r"(rs1), "r"(rs2));
      break;
   case 1:
         __asm__ ("aes32esi %0, %1, %2, 1" : "=r" (rd) : "r"(rs1), "r"(rs2));
      break;
   case 2:
     __asm__ ("aes32esi %0, %1, %2, 2" : "=r" (rd) : "r"(rs1), "r"(rs2));
     break;
   case 3:
     __asm__ ("aes32esi %0, %1, %2, 3" : "=r" (rd) : "r"(rs1), "r"(rs2));
     break;
   default:
     rd = 0;
   }

  return rd;

}

uint32_t neorv32_aes32dsmi(uint32_t rs1,uint32_t rs2,int bs){
  uint32_t rd = 0;
  switch(bs){
   case 0:
       __asm__ ("aes32dsmi %0, %1, %2, 0" : "=r" (rd) : "r"(rs1), "r"(rs2));
      break;
   case 1:
         __asm__ ("aes32dsmi %0, %1, %2, 1" : "=r" (rd) : "r"(rs1), "r"(rs2));
      break;
   case 2:
     __asm__ ("aes32dsmi %0, %1, %2, 2" : "=r" (rd) : "r"(rs1), "r"(rs2));
     break;
   case 3:
     __asm__ ("aes32dsmi %0, %1, %2, 3" : "=r" (rd) : "r"(rs1), "r"(rs2));
     break;
   default:
     rd = 0;
   }

  return rd;

}

uint32_t neorv32_aes32dsi(uint32_t rs1,uint32_t rs2,int bs){
  uint32_t rd = 0;
  switch(bs){
   case 0:
       __asm__ ("aes32dsi %0, %1, %2, 0" : "=r" (rd) : "r"(rs1), "r"(rs2));
      break;
   case 1:
         __asm__ ("aes32dsi %0, %1, %2, 1" : "=r" (rd) : "r"(rs1), "r"(rs2));
      break;
   case 2:
     __asm__ ("aes32dsi %0, %1, %2, 2" : "=r" (rd) : "r"(rs1), "r"(rs2));
     break;
   case 3:
     __asm__ ("aes32dsi %0, %1, %2, 3" : "=r" (rd) : "r"(rs1), "r"(rs2));
     break;
   default:
     rd = 0;
   }

  return rd;

}


int main(){

	neorv32_uart0_setup(BAUD_RATE,0);
	neorv32_uart0_printf("\n\nTest: Uso de extensiones Zknd y Zkne mediante ensamblador en linea\n");
	// Valores de prueba
	uint32_t a = 0x11223344;
	uint32_t b = 0x55667788;
	// Impresión valores de prueba
	neorv32_uart0_printf("\na = 0x%x\n",a); 
	neorv32_uart0_printf("\nb = 0x%x\n",b); 
	// Variables de salida
	uint32_t r1, r2, r3, r4;
	// Instrucción aes32esmi, cifrado rondas intermedias
	neorv32_uart0_printf("\nTest aes32esmi\n");
	for(int bs = 0; bs < 4; bs++){
		r1 = neorv32_aes32esmi(a,b,bs);
		neorv32_uart0_printf("\taes32esmi r1, a, b, %d --> r1 = 0x%x\n",bs,r1);
	}
	// Instrucción aes32esi, cifrado ronda final
	neorv32_uart0_printf("\nTest aes32esi\n");
        for(int bs = 0; bs < 4; bs++){
                r2 = neorv32_aes32esi(a,b,bs);
		neorv32_uart0_printf("\taes32esi  r2, a, b, %d --> r2 = 0x%x\n",bs,r2);
        }
	// Instrucción aes32dsmi, descifrado rondas intermedias
	neorv32_uart0_printf("\nTest aes32dsmi\n");
        for(int bs = 0; bs < 4; bs++){
                r3 = neorv32_aes32dsmi(a,b,bs);
		neorv32_uart0_printf("\taes32dsmi r3, a, b, %d --> r3 = 0x%x\n",bs,r3);
        }
	// Instrucción aes32dsi, descifrado ronda final
	neorv32_uart0_printf("\nTest aes32dsi\n");
        for(int bs = 0; bs < 4; bs++){
                r4 = neorv32_aes32dsi(a,b,bs);
		neorv32_uart0_printf("\taes32dsi  r4, a, b, %d --> r4 = 0x%x\n",bs,r4);
        }


return 0;

}
