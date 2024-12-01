#include "MicroBit.h"

void bitwait(volatile int value)
{
    for(volatile int i = 0; i<value; i++)//Create a loop to force the microbit to perform operations to make it wait inbetween functions
    {

    }
}

void turnOn()
{
    volatile uint32_t *d = (uint32_t *) 0x50000514;//Creating a pointer to the GPIO direction register
    volatile uint32_t *p = (uint32_t *) 0x50000504;//Creating a pointer to the pin select register
    *d = 0b00000000100000100000011000011110;//Setting the direction to the required GPIO ports
    *p = 0b00000000100000100000011000011110;//Turn on all GPIO LEDs
}

void setLEDs(uint8_t value)
{

    volatile uint32_t *d = (uint32_t *) 0x50000514;//Same as turnOn()
    volatile uint32_t *p = (uint32_t *) 0x50000504;//Same as turnOn()
    int current = 0b00000000000000000000000000000000;//Creating an empty 32 bit value
    if((value & 0b00000001) != 0)//Checking if there is a value in the 8th position
    {
        current = current + 0b00000000000000000000000000000010;//Setting current to include the LED in this position
    }
    if((value & 0b00000010) != 0)//Below are the same operations above for each LED position
    {
        current = current + 0b00000000000000100000000000000000;
    }
    if((value & 0b00000100) != 0)
    {
        current = current + 0b00000000000000000000001000000000;
    }
    if((value & 0b00001000) != 0)
    {
        current = current + 0b00000000100000000000000000000000;
    }
    if((value & 0b000010000) != 0)
    {
        current = current + 0b00000000000000000000010000000000;
    }
    if((value & 0b00100000) != 0)
    {
        current = current + 0b00000000000000000000000000010000;
    }
    if((value & 0b01000000) != 0)
    {
        current = current + 0b00000000000000000000000000001000;
    }
    if((value & 0b10000000) != 0)
    {
        current = current + 0b00000000000000000000000000000100;
    }
    *d = current;//Setting the registers to the final value of current, turning on all corresponding LED
    *p = current;
}

void rollingCounter()
{
    for(int i = 0; i<256;i++)//A loop to count display 1-255 on the LEDs
    {
        setLEDs(i);//Uses the above setLEDs function to count in binary from 1-255, lighting up the corresponding LED
        bitwait(1000000);
    }
}

void knightRider()
{
   while(1)
   {
        for(int i = 0; i<7; i++)//A loop to increment i for shifting below
        {
            setLEDs(1<< i);//Shift the value 1 using i to increase it and set each LED from left to right on
            bitwait(5000000);
        }
        for(int i = 0; i<7; i++)//A loop to increment i for shifting below
        {
            setLEDs(128>>i);//Same as above except since the 8th LED is already on we set 128(LED 7) and then shift to decrease the value and go from right to left
            bitwait(5000000);
        }
   }
}

void countClicks()
{
    volatile uint32_t *d = (uint32_t *) 0x50000514;//Setting GPIO direction register
    volatile uint32_t *p = (uint32_t *) 0x50000504;//Setting GPIO pin select register
    volatile uint32_t *r = (uint32_t *) 0x50000510;//Setting a register to the read/IN register
    volatile uint32_t *cnf = (uint32_t *) 0x5000073c;//Setting a register to the config of pin 15
    *cnf = 0;//Setting the config to 0 to enable the input buffer
    int i = 0;
    while(true)
    {
        if(i == 256)//Break once we reach 256
        {
            break;
        }
        if(((*r>>15) & 0b1) == 0)//Shift the read register 15 bits to read the input buffer of pin 15
        {
            setLEDs(i);//Setting on LEDS for corresponding click
            i++;
            bitwait(500000);//Waiting to ensure button does not bounce
        }
    }
    
}

