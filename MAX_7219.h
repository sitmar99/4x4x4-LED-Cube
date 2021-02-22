#include <stdlib.h>
#include <util/delay.h>
#include <avr/io.h>

#define REG_DECODE          0x09                      // "decode mode" register
#define REG_INTENSITY       0x0a                      // "intensity" register
#define REG_SCAN_LIMIT      0x0b                      // "scan limit" register
#define REG_SHUTDOWN        0x0c                      // "shutdown" register
#define REG_DISPLAY_TEST    0x0f                      // "display test" register


#define DDR         DDRD
#define PORT        PORTD
#define PIN_DIN     PORTD2                          // DIN pin                           
#define PIN_CS      PORTD1                          // CS pin
#define PIN_CLK     PORTD0                          // CLK pin
#define REFRESH    300

#define DATA_0()    (PORT &= ~(1 << PIN_DIN))
#define DATA_1()    (PORT |= (1 << PIN_DIN))

#define LOAD_0()    (PORT &= ~(1 << PIN_CS))
#define LOAD_1()    (PORT |= (1 << PIN_CS))

#define CLK_0()    (PORT &= ~(1 << PIN_CLK))
#define CLK_1()    (PORT |= (1 << PIN_CLK))

static void MAX7219_Send(unsigned char data)
{
    for (char i=7; i>=0; i--)
    {
        CLK_0();
        (data & (1 << i)) ? DATA_1() : DATA_0();
        CLK_1();
    }
}

void MAX7219_Write(unsigned char reg, unsigned char data)
{
    LOAD_0();

        MAX7219_Send(reg);
        MAX7219_Send(data);

    LOAD_1();
    LOAD_0();
}

void MAX7219_Init()
{
    DDR |= (1 << PIN_CS) | (1 << PIN_CLK) | (1 << PIN_DIN);
    
    LOAD_1();
    
    MAX7219_Write(REG_SHUTDOWN, 1);
    MAX7219_Write(REG_SCAN_LIMIT, 0x07);
    MAX7219_Write(REG_DECODE, 0x00);
    MAX7219_Write(REG_DISPLAY_TEST, 0x00);
    MAX7219_Write(REG_INTENSITY, 0x01);
}

void MAX7219_Clear()
{
    for (uint8_t i=9; i > 0; --i)
        MAX7219_Write(i, 0x00);
}

void MAX7219_Animate(uint8_t* animation[], uint8_t size)
{
    for (uint8_t frames = size; frames; frames--)
    {
        for (uint8_t i=9; i > 0; --i)
        {
            MAX7219_Write(i, *(animation+frames-1) >> (8*(8-i)));
        }
        _delay_ms(REFRESH);
    }
}