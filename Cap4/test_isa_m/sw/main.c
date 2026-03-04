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
 * Código de aplicación de usuario para comprobar la extensión ISA M de RISC-V
 * en el SoC NEORV32 con toolchain GCC.
 ******************************************************************************/

#include <neorv32.h>
#include <stdint.h>

#define BAUD_RATE 19200

int main() {

    neorv32_uart0_setup(BAUD_RATE, 0);

    /*Primera prueba: multiplicación de dos enteros sin signo de 32 bits*/
    neorv32_uart0_printf("\n\nMultiplicacion dos enteros sin signo de 32 bits, instrucciones mul y mulhu\n");
    volatile uint32_t var1 = 100000;
    volatile uint32_t var2 = 500000;

    neorv32_uart0_printf("var1 = 0x%x, %d en decimal\n", var1, var1);
    neorv32_uart0_printf("var2 = 0x%x, %d en decimal\n", var2, var2);

    uint64_t r1 = (uint64_t)var1 * var2;

    uint32_t r1_low = (uint32_t)r1;
    uint32_t r1_high = (uint32_t)(r1 >> 32);

    neorv32_uart0_printf("\nvar1*var2 = 0x%x%x\n", r1_high, r1_low);
    /*Fin Primera Prueba*/


    /*Segunda prueba: multiplicación de un entero con signo y un entero sin signo de 32 bits*/
    neorv32_uart0_printf("\n\nMultiplicacion entero sin signo y con signo de 32 bits, instrucciones mul y mulhsu\n");
    volatile int32_t var3 = -100000;
    volatile uint32_t var4 = 500000;

    neorv32_uart0_printf("var3 = 0x%x, %d en decimal\n", var3, var3);
    neorv32_uart0_printf("var4 = 0x%x, %d en decimal\n", var4, var4);

    int64_t r2 = (int64_t)var3 * var4;

    uint32_t r2_low = (uint32_t)r2;
    uint32_t r2_high = (uint32_t)(r2 >> 32);

    neorv32_uart0_printf("\nvar3*var4 = 0x%x%x\n", r2_high, r2_low);
    /*Fin Segunda Prueba*/


    /*Tercera prueba: multiplicación de dos enteros con signo de 32 bits*/
    neorv32_uart0_printf("\n\nMultiplicacion de dos enteros con signo de 32 bits, instrucciones mul y mulh\n");
    volatile int32_t var5 = -100000;
    volatile int32_t var6 = -500000;

    neorv32_uart0_printf("var5 = 0x%x, %d en decimal\n", var5, var5);
    neorv32_uart0_printf("var6 = 0x%x, %d en decimal\n", var6, var6);

    int64_t r3 = (int64_t)var5 * var6;

    uint32_t r3_low = (uint32_t)r3;
    uint32_t r3_high = (uint32_t)(r3 >> 32);

    neorv32_uart0_printf("\nvar5*var6 = 0x%x%x\n", r3_high, r3_low);
    /*Fin Tercera Prueba*/


    /*Cuarta Prueba: división de dos enteros con signo de 32 bits, instrucción div*/
    neorv32_uart0_printf("\n\nDivision de dos enteros con signo de 32 bits, instruccion div\n");
    volatile int32_t var7 = 100000;
    volatile int32_t var8 = -500000;

    neorv32_uart0_printf("var7 = 0x%x, %d en decimal\n", var7, var7);
    neorv32_uart0_printf("var8 = 0x%x, %d en decimal\n", var8, var8);

    int32_t r4 = var8 / var7;

    neorv32_uart0_printf("\nvar8/var7 = 0x%x, en decimal %d\n", r4, r4);
    /*Fin Cuarta Prueba*/


    /*Quinta Prueba: división de dos enteros sin signo de 32 bits, instrucción divu*/
    neorv32_uart0_printf("\n\nDivision de dos enteros sin signo de 32 bits, instruccion divu\n");
    volatile uint32_t var9 = 100000;
    volatile uint32_t var10 = 500000;

    neorv32_uart0_printf("var9 = 0x%x, %d en decimal\n", var9, var9);
    neorv32_uart0_printf("var10 = 0x%x, %d en decimal\n", var10, var10);

    uint32_t r5 = var10 / var9;

    neorv32_uart0_printf("\nvar10/var9 = 0x%x, en decimal %d\n", r5, r5);
    /*Fin Quinta Prueba*/


    /*Sexta Prueba: resto de dos enteros con signo de 32 bits, instrucción rem*/
    neorv32_uart0_printf("\n\nResto de dos enteros con signo de 32 bits, instruccion rem\n");
    volatile int32_t var11 = 200000;
    volatile int32_t var12 = -500000;

    neorv32_uart0_printf("var11 = 0x%x, %d en decimal\n", var11, var11);
    neorv32_uart0_printf("var12 = 0x%x, %d en decimal\n", var12, var12);

    int32_t r6 = var12 % var11;

    neorv32_uart0_printf("\nvar12%%var11 = 0x%x, en decimal %d\n", r6, r6);
    /*Fin Sexta Prueba*/


    /*Séptima Prueba: resto de dos enteros sin signo de 32 bits, instrucción remu*/
    neorv32_uart0_printf("\n\nResto de dos enteros sin signo de 32 bits, instruccion remu\n");
    volatile uint32_t var13 = 200000;
    volatile uint32_t var14 = 500000;

    neorv32_uart0_printf("var13 = 0x%x, %d en decimal\n", var13, var13);
    neorv32_uart0_printf("var14 = 0x%x, %d en decimal\n", var14, var14);

    uint32_t r7 = var14 % var13;

    neorv32_uart0_printf("\nvar14%%var13 = 0x%x, en decimal %d\n", r7, r7);
    /*Fin Séptima Prueba*/


    /*Octava Prueba: multiplicación de 64 bits, algoritmo de multiplicación cruzada*/
    neorv32_uart0_printf("\n\nMultiplicacion de dos enteros sin signo de 64 bits, algoritmo de multiplicacion cruzada\n\n");
    volatile uint64_t var15 = 0x0000000111111111;
    uint32_t var15_low = (uint32_t)var15;
    uint32_t var15_high = (uint32_t)(var15 >> 32);

    neorv32_uart0_printf("var15 = 0x%x%x\n", var15_high, var15_low);

    volatile uint64_t var16 = 0x0000000022222222;
    uint32_t var16_low = (uint32_t)var16;
    uint32_t var16_high = (uint32_t)(var16 >> 32);

    neorv32_uart0_printf("var16 = 0x%x%x\n", var16_high, var16_low);

    uint64_t r8 = var15 * var16;
    uint32_t r8_low = (uint32_t)r8;
    uint32_t r8_high = (uint32_t)(r8 >> 32);

    neorv32_uart0_printf("\nvar15*var16 = 0x%x%x\n", r8_high, r8_low);
    /*Fin Octava Prueba*/

    return 0;
}
