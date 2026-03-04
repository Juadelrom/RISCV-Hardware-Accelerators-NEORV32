/*******************************************************************************
 * Archivo:     neorv32_aes128_acelerador.c
 * Autor:       Juan Manuel Delgado Romero
 * Fecha:       Febrero 2026
 *
 * Institución: Universidad de Sevilla - Escuela Politécnica Superior
 * Titulación:  Grado en Ingeniería Electrónica Industrial
 * Proyecto:    Trabajo Fin de Grado (TFG)
 * Licencia:    MIT (Ver archivo LICENSE)
 *
 * Descripción: 
 * Driver para el uso desde alto nivel del acelerador criptográfico 
 * AES-128 integrado en el Custom Functions Subsystem (CFS) del SoC NEORV32.
 ******************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <neorv32.h>

/*Definiciones de las constantes para el AES-128*/
enum {
     Nk = 4,    //Número de palabras copiadas de la clave original en la clave expandida para AES-128
     Nb = 4,    //Número de columnas de 32 bits de cada estado, siempre 4 en el estándar NIST
     Nr = 10    //Número de rondas a ejecutar en el cifrado, 10 para AES-128
};
/*Fin declaración*/

// Registro de Control
#define CFS_CTRL            (NEORV32_CFS->REG[0])
// Entradas
#define CFS_PLTXT_0         (NEORV32_CFS->REG[1])
#define CFS_PLTXT_1         (NEORV32_CFS->REG[2])
#define CFS_PLTXT_2         (NEORV32_CFS->REG[3])
#define CFS_PLTXT_3         (NEORV32_CFS->REG[4])

#define CFS_KEY_0           (NEORV32_CFS->REG[5])
#define CFS_KEY_1           (NEORV32_CFS->REG[6])
#define CFS_KEY_2           (NEORV32_CFS->REG[7])
#define CFS_KEY_3           (NEORV32_CFS->REG[8])

// Salidas
#define CFS_CIPTXT_0        (NEORV32_CFS->REG[1])
#define CFS_CIPTXT_1        (NEORV32_CFS->REG[2])
#define CFS_CIPTXT_2        (NEORV32_CFS->REG[3])
#define CFS_CIPTXT_3        (NEORV32_CFS->REG[4])

// Bits de Control
#define CFS_CTRL_START      (1 << 0)
#define CFS_CTRL_BUSY       (1 << 1)
#define ERROR                1

void CipherAcelerador(uint8_t in[4*Nb], uint8_t key[4*Nb], uint8_t out[4*Nb]){
    // Bytes 0:3 -> Registro 4 (PLTXT_3 / 127:96)
    CFS_PLTXT_3 = ((uint32_t)in[0] << 24) | ((uint32_t)in[1] << 16) | ((uint32_t)in[2] << 8) | (uint32_t)in[3];
    // Bytes 4:7 -> Registro 3 (PLTXT_2 / 95:64)
    CFS_PLTXT_2 = ((uint32_t)in[4] << 24) | ((uint32_t)in[5] << 16) | ((uint32_t)in[6] << 8) | (uint32_t)in[7];
    // Bytes 8:11 -> Registro 2 (PLTXT_1 / 63:32)
    CFS_PLTXT_1 = ((uint32_t)in[8] << 24) | ((uint32_t)in[9] << 16) | ((uint32_t)in[10] << 8)| (uint32_t)in[11];
    // Bytes 12:15 -> Registro 1 (PLTXT_0 / 31:0)
    CFS_PLTXT_0 = ((uint32_t)in[12] << 24)| ((uint32_t)in[13] << 16)| ((uint32_t)in[14] << 8)| (uint32_t)in[15];

    CFS_KEY_3   = ((uint32_t)key[0] << 24) | ((uint32_t)key[1] << 16) | ((uint32_t)key[2] << 8) | (uint32_t)key[3];
    CFS_KEY_2   = ((uint32_t)key[4] << 24) | ((uint32_t)key[5] << 16) | ((uint32_t)key[6] << 8) | (uint32_t)key[7];
    CFS_KEY_1   = ((uint32_t)key[8] << 24) | ((uint32_t)key[9] << 16) | ((uint32_t)key[10] << 8)| (uint32_t)key[11];
    CFS_KEY_0   = ((uint32_t)key[12] << 24)| ((uint32_t)key[13] << 16)| ((uint32_t)key[14] << 8)| (uint32_t)key[15];

    CFS_CTRL |= CFS_CTRL_START;

    while(CFS_CTRL & CFS_CTRL_BUSY);

    CFS_CTRL &= ~CFS_CTRL_START;
    
    // Registro 4 -> Bytes 0:3 (Byte 0 esta en parte superior, bits 31:24)
    out[0]  = (uint8_t)((CFS_CIPTXT_3 >> 24) & 0xFF);
    out[1]  = (uint8_t)((CFS_CIPTXT_3 >> 16) & 0xFF);
    out[2]  = (uint8_t)((CFS_CIPTXT_3 >> 8)  & 0xFF);
    out[3]  = (uint8_t)(CFS_CIPTXT_3 & 0xFF);
    
    // Registro 3 -> Bytes 4:7
    out[4]  = (uint8_t)((CFS_CIPTXT_2 >> 24) & 0xFF);
    out[5]  = (uint8_t)((CFS_CIPTXT_2 >> 16) & 0xFF);
    out[6]  = (uint8_t)((CFS_CIPTXT_2 >> 8)  & 0xFF);
    out[7]  = (uint8_t)(CFS_CIPTXT_2 & 0xFF);

    // Registro 2 -> Bytes 8:11
    out[8]  = (uint8_t)((CFS_CIPTXT_1 >> 24) & 0xFF);
    out[9]  = (uint8_t)((CFS_CIPTXT_1 >> 16) & 0xFF);
    out[10] = (uint8_t)((CFS_CIPTXT_1 >> 8)  & 0xFF);
    out[11] = (uint8_t)(CFS_CIPTXT_1 & 0xFF);

    // Registro 1 -> Bytes 12:15
    out[12] = (uint8_t)((CFS_CIPTXT_0 >> 24) & 0xFF);
    out[13] = (uint8_t)((CFS_CIPTXT_0 >> 16) & 0xFF);
    out[14] = (uint8_t)((CFS_CIPTXT_0 >> 8)  & 0xFF);
    out[15] = (uint8_t)(CFS_CIPTXT_0 & 0xFF);
}
