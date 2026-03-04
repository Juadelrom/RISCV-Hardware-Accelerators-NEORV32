/*******************************************************************************
 * Archivo:     neorv32_aes128_v1.c
 * Autor:       Juan Manuel Delgado Romero
 * Fecha:       Febrero 2026
 *
 * Institución: Universidad de Sevilla - Escuela Politécnica Superior
 * Titulación:  Grado en Ingeniería Electrónica Industrial
 * Proyecto:    Trabajo Fin de Grado (TFG)
 * Licencia:    MIT (Ver archivo LICENSE)
 *
 * Descripción:
 * Librería para el uso del algoritmo AES-128 acelerado con el uso de las exten-
 * siones Zkne y Zknd de RISC-V (versión 1). Se optimiza la función Cipher con 
 * funciones con ensamblador en línea con las instrucciones: 
 * aes32esmi y aes32esi.
 ******************************************************************************/

#include "neorv32_aes128_v1.h"
#include <neorv32.h>

/* Declaración de la SBox para el estándar AES (Privada) */
static const uint8_t sbox[16][16] = {
    {0x63,0x7C,0x77,0x7B,0xF2,0x6B,0x6F,0xC5,0x30,0x01,0x67,0x2B,0xFE,0xD7,0xAB,0x76},
    {0xCA,0x82,0xC9,0x7D,0xFA,0x59,0x47,0xF0,0xAD,0xD4,0xA2,0xAF,0x9C,0xA4,0x72,0xC0},
    {0xB7,0xFD,0x93,0x26,0x36,0x3F,0xF7,0xCC,0x34,0xA5,0xE5,0xF1,0x71,0xD8,0x31,0x15},
    {0x04,0xC7,0x23,0xC3,0x18,0x96,0x05,0x9A,0x07,0x12,0x80,0xE2,0xEB,0x27,0xB2,0x75},
    {0x09,0x83,0x2C,0x1A,0x1B,0x6E,0x5A,0xA0,0x52,0x3B,0xD6,0xB3,0x29,0xE3,0x2F,0x84},
    {0x53,0xD1,0x00,0xED,0x20,0xFC,0xB1,0x5B,0x6A,0xCB,0xBE,0x39,0x4A,0x4C,0x58,0xCF},
    {0xD0,0xEF,0xAA,0xFB,0x43,0x4D,0x33,0x85,0x45,0xF9,0x02,0x7F,0x50,0x3C,0x9F,0xA8},
    {0x51,0xA3,0x40,0x8F,0x92,0x9D,0x38,0xF5,0xBC,0xB6,0xDA,0x21,0x10,0xFF,0xF3,0xD2},
    {0xCD,0x0C,0x13,0xEC,0x5F,0x97,0x44,0x17,0xC4,0xA7,0x7E,0x3D,0x64,0x5D,0x19,0x73},
    {0x60,0x81,0x4F,0xDC,0x22,0x2A,0x90,0x88,0x46,0xEE,0xB8,0x14,0xDE,0x5E,0x0B,0xDB},
    {0xE0,0x32,0x3A,0x0A,0x49,0x06,0x24,0x5C,0xC2,0xD3,0xAC,0x62,0x91,0x95,0xE4,0x79},
    {0xE7,0xC8,0x37,0x6D,0x8D,0xD5,0x4E,0xA9,0x6C,0x56,0xF4,0xEA,0x65,0x7A,0xAE,0x08},
    {0xBA,0x78,0x25,0x2E,0x1C,0xA6,0xB4,0xC6,0xE8,0xDD,0x74,0x1F,0x4B,0xBD,0x8B,0x8A},
    {0x70,0x3E,0xB5,0x66,0x48,0x03,0xF6,0x0E,0x61,0x35,0x57,0xB9,0x86,0xC1,0x1D,0x9E},
    {0xE1,0xF8,0x98,0x11,0x69,0xD9,0x8E,0x94,0x9B,0x1E,0x87,0xE9,0xCE,0x55,0x28,0xDF},
    {0x8C,0xA1,0x89,0x0D,0xBF,0xE6,0x42,0x68,0x41,0x99,0x2D,0x0F,0xB0,0x54,0xBB,0x16}
};

/* Definición del vector Rcon (Privada) */
static const uint32_t Rcon[Nr+1] = {
    0x00000000,
    0x01000000,
    0x02000000,
    0x04000000,
    0x08000000,
    0x10000000,
    0x20000000,
    0x40000000,
    0x80000000,
    0x1B000000,
    0x36000000
};

/* Funciones estáticas para enmascarar el uso de ensamblador en línea */
uint32_t neorv32_aes32esmi(uint32_t rs1, uint32_t rs2, int bs){
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

uint32_t neorv32_aes32esi(uint32_t rs1, uint32_t rs2, int bs){
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

uint32_t neorv32_aes32dsmi(uint32_t rs1, uint32_t rs2, int bs){
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

uint32_t neorv32_aes32dsi(uint32_t rs1, uint32_t rs2, int bs){
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

/*
 * uint32_t RotWord(uint32_t word)
 * ---------------------------------------------------------------
 * Recibe una palabra de 32 bits de entrada y devuelve la palabra
 * tras realizar una rotación a la izquierda.
 */
static uint32_t RotWord(uint32_t word){
    uint32_t out;
    out = (word << 8) | (word >> 24);
    return out;
} 

/*
 * uint32_t SubWord(uint32_t word)
 * -----------------------------------------------------------------
 * Recibe 4 bytes a los cuales le aplica la Sbox y devuelve los 4 
 * bytes tras dicha operación.
 */
static uint32_t SubWord(uint32_t word){
    uint32_t temp = 0; // variable temporal
    int fil, col;      // índices filas y columnas
    for(int i = 3; i >= 0; i--){
        uint8_t byte = (word >> 8*i) & 0xFF; // Byte 
        fil = (byte >> 4) & 0x0F;            // Nibble alto
        col = byte & 0x0F;                   // Nibble bajo
        temp |= ((uint32_t)sbox[fil][col]) << (8*i); // Sustitución de bytes usando Sbox
    }
    return temp;
}

/*
 * void KeyExpansionV1 (uint8_t key[4*Nk], uint32_t w[Nb*(Nr+1)])
 */
void KeyExpansionV1(uint8_t key[4*Nk], uint32_t w[Nb*(Nr+1)]){
    uint32_t temp; // Variable temporal 
    int i = 0;     // Variable contador
    
    /* Comenzamos copiando la clave original en las 4 primeras subclaves de ronda */
    while(i < Nk){
        w[i] = ( (uint32_t)key[4*i]     << 24) |
               ( (uint32_t)key[4*i+1]   << 16) |
               ( (uint32_t)key[4*i+2]   <<  8) |
               ( (uint32_t)key[4*i+3]);
        i++;
    }
    
    /* Comenzamos con el resto de subclaves para las rondas restantes */
    i = Nk;
    while( i < (Nb*(Nr+1))){ 
        temp = w[i-1]; 
        if((i % Nk) == 0){ 
            temp = SubWord(RotWord(temp)) ^ Rcon[i/Nk]; 
        } else if((Nk > 6) && ((i%Nk) == 4)){ 
            temp = SubWord(temp); 
        }
        w[i] = w[i - Nk] ^ temp; 
        i++;
    }
}

/*
 * void CipherV1(uint8_t in[4*Nb], uint8_t out[4*Nb], uint32_t w[Nb*(Nr+1)])
 */
void CipherV1(uint8_t in[4*Nb], uint8_t out[4*Nb], uint32_t w[Nb*(Nr+1)]){
    uint32_t state[Nb] = {0};  // Variable estado

    /* Copia de datos de entrada(in) en variable state */
    for(int c = 0; c < 4; c++){
        for(int r = 0; r < 4; r++){
            state[c] |= ((uint32_t)in[r + 4*c]) << (r*8);
        }
    }
    
    /* Round Keys a formato little endian */
    for(int i = 0; i < (Nb*(Nr+1)); i++){
        w[i] =  (w[i]<<24)             |
               ((w[i]<<8)&0x00FF0000) |
               ((w[i]>>8)&0x0000FF00) |
                (w[i]>>24);
    }
    
    /* Ronda Inicial AddRoundKey */
    for(int c = 0; c < 4; c++){
        state[c] = state[c] ^ w[c]; 
    }

    /* Rondas intermedias, aceleradas con la instrucción aes32esmi */
    for(int round = 1; round < Nr; round++){
        for(int c = 0; c < 4; c++){
            for(int bs = 0; bs < 4; bs++){
                w[c+round*4] = neorv32_aes32esmi(w[c+round*4], state[(c+bs+4)%4], bs);
            }
        }
        for(int c = 0; c < 4; c++){
            state[c] = w[c+round*4];
        }
    } 
    
    /* Ronda final acelerada usando aes32esi */
    for(int c = 0; c < 4; c++){
        for(int bs = 0; bs < 4; bs++){
            w[c+Nr*4] = neorv32_aes32esi(w[Nr*4+c], state[(c+bs+4)%4], bs);
        }
    } 
    
    for(int c = 0; c < 4; c++){
        for(int r = 0; r < 4; r++){
            out[r+4*c] = (uint8_t)((w[c+Nr*4] >> (r*8)) & 0xFF);
        } 
    }
}
