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
 * Código de aplicación de usuario para probar el driver (neorv32_cfs_divisor32)
 * del acelerador divisor_32.vhd integrado en el CFS del SoC NEORV32.
 ******************************************************************************/

#include <neorv32.h>


#define BAUD_RATE 19200

#define CFS_CTRL            (NEORV32_CFS->REG[0])
#define CFS_DIVIDENDO       (NEORV32_CFS->REG[1])
#define CFS_DIVISOR         (NEORV32_CFS->REG[2])
#define CFS_COCIENTE        (NEORV32_CFS->REG[1])
#define CFS_RESTO           (NEORV32_CFS->REG[2])

#define CFS_CTRL_START      (1 << 0)
#define CFS_CTRL_BUSY       (1 << 1)
#define CFS_CTRL_ERROR      (1 << 2)
#define ERROR                1

int neorv32_cfs_divisor32(uint32_t dividendo, uint32_t divisor, uint32_t *cociente, uint32_t *resto){
    // 1. Comprobar si el acelerador está ocupado
    if(CFS_CTRL & CFS_CTRL_BUSY){
        // Si Bit Busy = '1'...
        return ERROR; // Error y salimos
    }
    // 2. Escritura de parámetros de entrada en registros CFS
    CFS_DIVIDENDO = dividendo;
    CFS_DIVISOR   = divisor;
    // 3. Inicio de la ejecución
    CFS_CTRL |= CFS_CTRL_START; // Bit Start = '1'
    // 4. Bucle de espera activo mientras el acelerador está ocupado
    while(CFS_CTRL & CFS_CTRL_BUSY); // Mientras bit Busy = '1'
    // 5. Comprobación de error antes de acuse de recibo
    if(CFS_CTRL & CFS_CTRL_ERROR){
        // Si Bit Error = '1'...
        CFS_CTRL &= ~CFS_CTRL_START; // Bit Start = '0', indica al HW que..
                                     // ..borre el error
        return ERROR;                // Error y salimos
    }
    // 6. Acuse de recibo de fin de operación, sin error
    CFS_CTRL &= ~CFS_CTRL_START; // Bit Start = '0'
    // 7. Lectura de datos válidos
    *cociente = CFS_COCIENTE;
    *resto = CFS_RESTO;
    // 8. Fin, salida con éxito
  return 0;  // Devolvemos código de salida de éxito
}


int main(){

  if (neorv32_cfs_available() == 0) {
    neorv32_uart0_printf("Error! No CFS synthesized!\n");
    return 1;
  }
 
  neorv32_rte_setup();
  neorv32_uart0_setup(BAUD_RATE,0);
  uint32_t dividendo = 4294967295;
  uint32_t divisor = 2;

  uint32_t cociente;
  uint32_t resto;

  if(neorv32_cfs_divisor32(dividendo,divisor,&cociente,&resto) == ERROR){
    neorv32_uart0_printf("Error:la división entre %u y %u no se ha realizado corretamente\n",dividendo,divisor);
  }else{
    neorv32_uart0_printf("%u / %u = %u , resto = %u\n",dividendo,divisor,cociente,resto);
  }

  return 0;
}
