#define F_CPU 1000000UL

#define __AVR_ATmega328P__

#include <avr/interrupt.h>
#include "MAX_7219.h"

#define DEBOUNCE_TIME       25        
#define BUTTON     7      

static const uint64_t CUBE[] = {0x9f09099ff99090f9};
static const uint64_t VERTROT[] = {0x9900009999000099, 0x4411882244118822, 0x2288114422881144};
static const uint64_t WAVE[] = {0xa0a0a0a041414141, 0x5050505028282828, 0x2828282814141414, 0x5050505028282828, 0xa0a0a0a041414141, 0x4141414182828282};
static const uint64_t* ANIMATIONS[] = {CUBE, VERTROT, WAVE};
static const uint16_t SIZES[] = {1, 3, 6};
volatile static uint8_t animation = 0;
static const uint8_t NANIMATIONS = 3;

ISR (PCINT2_vect)
{
    if (!(PIND & (1 << BUTTON)))
    {
        _delay_ms(DEBOUNCE_TIME);
        if (!(PIND & (1 << BUTTON)))
        {
            animation = (animation+1) % NANIMATIONS;
        }
    }
}


int main()
{
    DDRD &= ~(1 << BUTTON);

    cli();
    PCICR |= (1 << PCIE2);
    PCMSK2 |= (1 << PCINT23);
    sei();

    MAX7219_Init();
    MAX7219_Clear();
    
    while (1)
        MAX7219_Animate(ANIMATIONS[animation], SIZES[animation]);
}