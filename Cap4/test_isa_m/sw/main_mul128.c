/*******************************************************************************
 * Archivo:     main_mul128.c
 * Autor:       Juan Manuel Delgado Romero
 * Fecha:       Febrero 2026
 *
 * Institución: Universidad de Sevilla - Escuela Politécnica Superior
 * Titulación:  Grado en Ingeniería Electrónica Industrial
 * Proyecto:    Trabajo Fin de Grado (TFG)
 * Licencia:    MIT (Ver archivo LICENSE)
 *
 * Descripción: 
 * Código de aplicación de usuario para comprobar la función mul128() que 
 * multiplica dos enteros de 64 bits haciendo uso de la extensión ISA M de 
 * RISC-V.
 ******************************************************************************/


#include <neorv32.h>
#include <stdint.h>

#define BAUD_RATE 19200

typedef struct {
    uint32_t lo_lo;
    uint32_t lo_hi;
    uint32_t hi_lo;
    uint32_t hi_hi;
} uint128_t;

uint128_t mul128(uint64_t a, uint64_t b) {
    uint128_t result;

    uint32_t a_lo = (uint32_t)a;
    uint32_t a_hi = (uint32_t)(a >> 32);
    uint32_t b_lo = (uint32_t)b;
    uint32_t b_hi = (uint32_t)(b >> 32);

    uint32_t p0_lo, p0_hi;
    uint32_t p1_lo, p1_hi;
    uint32_t p2_lo, p2_hi;
    uint32_t p3_lo, p3_hi;
    uint32_t mid_lo, mid_hi;
    uint32_t carry;

    __asm__ volatile(
        // --- p0 = a_lo * b_lo ---
        "mul   %[p0_lo], %[a_lo], %[b_lo]\n\t"
        "mulhu %[p0_hi], %[a_lo], %[b_lo]\n\t"

        // --- p1 = a_lo * b_hi ---
        "mul   %[p1_lo], %[a_lo], %[b_hi]\n\t"
        "mulhu %[p1_hi], %[a_lo], %[b_hi]\n\t"

        // --- p2 = a_hi * b_lo ---
        "mul   %[p2_lo], %[a_hi], %[b_lo]\n\t"
        "mulhu %[p2_hi], %[a_hi], %[b_lo]\n\t"

        // --- p3 = a_hi * b_hi ---
        "mul   %[p3_lo], %[a_hi], %[b_hi]\n\t"
        "mulhu %[p3_hi], %[a_hi], %[b_hi]\n\t"

        // --- mid = (p0_hi + p1_lo + p2_lo) ---
        "add   %[mid_lo], %[p1_lo], %[p2_lo]\n\t"
        "add   %[mid_lo], %[mid_lo], %[p0_hi]\n\t"

        // --- Detectar carry ---
        "sltu  %[carry], %[mid_lo], %[p0_hi]\n\t"

        // --- mid_hi = (p1_hi + p2_hi + carry) ---
        "add   %[mid_hi], %[p1_hi], %[p2_hi]\n\t"
        "add   %[mid_hi], %[mid_hi], %[carry]\n\t"

        // --- result.lo_lo = p0_lo ---
        "mv    %[lo_lo], %[p0_lo]\n\t"

        // --- result.lo_hi = mid_lo ---
        "mv    %[lo_hi], %[mid_lo]\n\t"

        // --- result.hi_lo = p3_lo + mid_hi ---
        "add   %[hi_lo], %[p3_lo], %[mid_hi]\n\t"

        // --- result.hi_hi = p3_hi + carry de hi_lo ---
        "sltu  %[carry], %[hi_lo], %[mid_hi]\n\t"
        "add   %[hi_hi], %[p3_hi], %[carry]\n\t"

        : [lo_lo]"=r"(result.lo_lo),
          [lo_hi]"=r"(result.lo_hi),
          [hi_lo]"=r"(result.hi_lo),
          [hi_hi]"=r"(result.hi_hi),
          [p0_lo]"=r"(p0_lo), [p0_hi]"=r"(p0_hi),
          [p1_lo]"=r"(p1_lo), [p1_hi]"=r"(p1_hi),
          [p2_lo]"=r"(p2_lo), [p2_hi]"=r"(p2_hi),
          [p3_lo]"=r"(p3_lo), [p3_hi]"=r"(p3_hi),
          [mid_lo]"=r"(mid_lo), [mid_hi]"=r"(mid_hi),
          [carry]"=r"(carry)
        : [a_lo]"r"(a_lo), [a_hi]"r"(a_hi),
          [b_lo]"r"(b_lo), [b_hi]"r"(b_hi)
    );

    return result;
}

int main(){

	neorv32_uart0_setup(BAUD_RATE,0);

   /*Test multiplicación de dos enteros de 64 bits --> entero de 128 bits*/
	neorv32_uart0_printf("\n\nTest: Multiplicacion extendida de 128 bits para enteros de 64 bits\n");
	uint64_t a = 0x8000000000000000; // a = 2^63
	uint64_t b = 0x0000000000000002; // b = 2
	// 2^63 * 2 = 2^64 --> deborda los 64 bits [63:0]
	// Impresión de a y b
	uint32_t a_lo = (uint32_t)a;		// parte baja de a
	uint32_t a_hi = (uint32_t)(a >> 32);	// parte alta de a
	uint32_t b_lo = (uint32_t)b;		// parte baja de b
	uint32_t b_hi = (uint32_t)(b >> 32);	// parte alta de b

	neorv32_uart0_printf("\na = 0x%x%x\n",a_hi,a_lo);
	neorv32_uart0_printf("\nb = 0x%x%x\n",b_hi,b_lo);
	
	// llamada a la funcion mul128()
	uint128_t c = mul128(a,b);
	// impresión de c
	neorv32_uart0_printf("\na * b = 0x%x%x%x%x\n",c.hi_hi,c.hi_lo,c.lo_hi,c.lo_lo);
   /*Fin Test*/

	return 0;
}
