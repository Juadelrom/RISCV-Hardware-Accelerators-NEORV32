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
 * Código de aplicación de usuario para probar el driver del acelerador media_4.vhd
 * integrado en el CFS del SoC NEORV32.
 ******************************************************************************/


#include <neorv32.h>

#define BAUD_RATE 19200

/* -----------------------------------------------------------------
 * Función: media_cfs 
 * -----------------------------------------------------------------
 * Descripción: 
 * Interfaz de alto nivel para usar el acelerador hardware
 * media_4.vhd integrado en el subsistema CFS. Envía los datos de 
 * entrada a los registros del CFS y recupera su media.
 * -----------------------------------------------------------------
 * Interfaz:
 * - Entradas: 
 *   a, b, c y d: enteros de 32 bits sin signo. Se escriben en los
 *   primeros 4 registros del subsistema CFS, CFS_REG[0]...[3].
 * - Salida:
 *   CFS_REG[0]: media aritmética de a, b, c y d.
 * -----------------------------------------------------------------
 * * Nota: al operar con enteros, el resultado se trunca.
 * ----------------------------------------------------------------- */
uint32_t media_cfs(uint32_t a, uint32_t b, uint32_t c, uint32_t d){
  NEORV32_CFS->REG[0] = a;
  NEORV32_CFS->REG[1] = b;
  NEORV32_CFS->REG[2] = c;
  NEORV32_CFS->REG[3] = d;

  return NEORV32_CFS->REG[0];
}


int main() {


  neorv32_rte_setup();

  // setup UART at default baud rate, no interrupts
  neorv32_uart0_setup(BAUD_RATE, 0);

  // check if CFS is implemented at all
  if (neorv32_cfs_available() == 0) {
    neorv32_uart0_printf("Error! No CFS synthesized!\n");
    return 1;
  }
  // intro
  neorv32_uart0_printf("<<< NEORV32 Custom Functions Subsystem (CFS) Media >>>\n\n");
  // datos de entrada
  uint32_t a = 1000;
  uint32_t b = 2000;
  uint32_t c = 3000;
  uint32_t d = 4000;
  // llamada a la función media_cfs
  uint32_t media = media_cfs(a, b, c, d);
  // impresión por pantalla del resultado
  neorv32_uart0_printf("La media de %u, %u, %u y %u es: %u\n", a, b, c, d, media);

  neorv32_uart0_printf("\nCFS programa media completado.\n");

  return 0;
}
