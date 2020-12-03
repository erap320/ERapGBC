#pragma once

///////Addresses of notable registers that reside in ram

//Port/Mode Registers
#define P1 0xFF00
#define SB 0xFF01
#define SC 0xFF02
#define DIV 0xFF04
#define TIMA 0xFF05
#define TMA 0xFF06
#define TAC 0xFF07
#define KEY1 0xFF4D
#define RP 0xFF56

//Bank Control Registers
#define VBK 0xFF4F
#define SVBK 0xFF70

//Interrupt flags
#define IF 0xFF0F
#define IE 0xFFFF

//LCD Display Registers
#define LCDC 0xFF40
#define STAT 0xFF41
#define SCY 0xFF42
#define SCX 0xFF43
#define LY 0xFF44
#define LYC 0xFF45
#define DMA 0xFF46
#define BGP 0xFF47
#define OBP0 0xFF48
#define OBP1 0xFF49
#define WY 0xFF4A
#define WX 0xFF4B
#define HDMA1 0xFF51
#define HDMA2 0xFF52
#define HDMA3 0xFF53
#define HDMA4 0xFF54
#define HDMA5 0xFF55
#define BCPS 0xFF68
#define BCPD 0xFF69
#define OCPS 0xFF6A
#define OCPD 0xFF6B
#define OAM 0xFE00 //TODO verify address

//Additional useful addresses without an official name
#define CART_RAM_ENABLE 0x0000
#define CART_ROM_BANK 0x2000
#define CART_RAM_BANK 0x4000
#define CART_BANK_TYPE 0x6000