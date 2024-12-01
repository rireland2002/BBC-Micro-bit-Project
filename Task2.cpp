#include "MicroBit.h"

int row = 1;//Setting row variable for beHappyAndFree function

void beHappy()
{
    while(1)//Repeat forever
    {
        for(volatile int i = 0; i<7000; i++)//A busy wait loop at a speed that stops blurring and doesn't flash
        {
            NRF_P0 -> DIR = 0b00000000010000000000100000000000;//Setting the direction of P0 to turn on relevant columns and specific row
            NRF_P0 -> OUT = 0b00000000010000000000000000000000;//Setting power to specific row
            NRF_P1 -> DIR = 0b00000000000000000000000000100000;//Setting direction of P1 to turn on row 4
        }
        for(volatile int i = 0; i<7000; i++)//Repeat of the above for subsequent rows
        {
            NRF_P0 -> DIR = 0b01010001000000000000000000100000;
            NRF_P0 -> OUT = 0b00000001000000000000000000000000;
            NRF_P1 -> DIR = 0b00000000000000000000000000000000;
        }
        for(volatile int i = 0; i<7000; i++)
        {
            NRF_P0 -> DIR = 0b10000000000010000000100000000000;
            NRF_P0 -> OUT = 0b00000000000010000000000000000000;
            NRF_P1 -> DIR = 0b00000000000000000000000000100000;
        }
    }
}

void beVeryHappy()
{
    NRF_TIMER1 -> BITMODE = 3;//Ensuring timer has 32 bit clock
    NRF_TIMER1 -> CC[0] = 7000;//Setting the capture compare register of Timer 1 to 7000, will tick once reached
    NRF_TIMER1 -> TASKS_START = 1;//Starting timer 1
    while(1)
    {
        while(NRF_TIMER1 -> EVENTS_COMPARE[0] == 0)//Checking the relevant events compare register, when triggered move on from loop
        {
            NRF_P0 -> DIR = 0b00000000010000000000100000000000;//Setting GPIO pins as above
            NRF_P0 -> OUT = 0b00000000010000000000000000000000;
            NRF_P1 -> DIR = 0b00000000000000000000000000100000;
        }
        NRF_TIMER1 -> EVENTS_COMPARE[0] = 0;//Clearing the events compare register
        NRF_TIMER1 -> TASKS_CLEAR = 1;//Resetting clock

        while(NRF_TIMER1 -> EVENTS_COMPARE[0] == 0)//Same as above
        {
            NRF_P0 -> DIR = 0b01010001000000000000000000100000;
            NRF_P0 -> OUT = 0b00000001000000000000000000000000;
            NRF_P1 -> DIR = 0b00000000000000000000000000000000;
        }
        NRF_TIMER1 -> EVENTS_COMPARE[0] = 0;
        NRF_TIMER1 -> TASKS_CLEAR = 1;
  
        while(NRF_TIMER1 -> EVENTS_COMPARE[0] == 0)
        {
            NRF_P0 -> DIR = 0b10000000000010000000100000000000;
            NRF_P0 -> OUT = 0b00000000000010000000000000000000;
            NRF_P1 -> DIR = 0b00000000000000000000000000100000;
        }
        NRF_TIMER1 -> EVENTS_COMPARE[0] = 0;
        NRF_TIMER1 -> TASKS_CLEAR = 1;
    }
}


void row1()
{
    if(NRF_TIMER1 -> EVENTS_COMPARE[0] == 1)//Checking to see if the events compare register has been triggered
    {
    switch(row)//Switch case using the row variable
    {
        case 1:
            row++;//Increment to turn on next row for next interrupt
            NRF_P0 -> DIR = 0b00000000010000000000100000000000;//Same as above
            NRF_P0 -> OUT = 0b00000000010000000000000000000000;
            NRF_P1 -> DIR = 0b00000000000000000000000000100000;
            break;
        case 2:
            row++;//Same as above
            NRF_TIMER1 -> TASKS_CLEAR = 1;//Clearing the timer 
            NRF_P0 -> DIR = 0b01010001000000000000000000100000;
            NRF_P0 -> OUT = 0b00000001000000000000000000000000;
            NRF_P1 -> DIR = 0b00000000000000000000000000000000;
            break;
        case 3:
            row = 1;//Reset back to step 1
            NRF_TIMER1 -> TASKS_CLEAR = 1;//Clearing the timer
            NRF_P0 -> DIR = 0b10000000000010000000100000000000;
            NRF_P0 -> OUT = 0b00000000000010000000000000000000;
            NRF_P1 -> DIR = 0b00000000000000000000000000100000;
            break;
    }
    NRF_TIMER1 -> EVENTS_COMPARE[0] = 0;//Setting events compare to 0
    NRF_TIMER1 -> TASKS_CLEAR = 1;//Resetting timer
    }
}

void beHappyAndFree()
{
    NRF_TIMER1 -> BITMODE = 3;//Same as above
    NRF_TIMER1 -> CC[0] = 7000;//^^
    NRF_TIMER1 -> INTENSET = 0x10000;//Setting the intterupts for CC[0] to be allowed for Timer 1
    NRF_TIMER1 -> TASKS_START = 1;//Starting clock
    
    NVIC_SetVector(TIMER1_IRQn, (uint32_t)row1);//Setting interrupt vector to function above
    NVIC_EnableIRQ(TIMER1_IRQn);//Enabling interrupts for timer 1
    
    while(1)//Loop to ensure program doesnt run off main
    {
    }
}

void showNumber(int n)//Pass in a digit from 0-9
{
    if(n == 0)
    {
        while(1)//Repeat forever
        {
            for(volatile int i = 0; i<7000; i++)//Busy loop to remove blurring and flashing
            {
                NRF_P0 -> DIR = 0b10000000001000000000100000000000;//Setting GPIO pins as above functions, set all rows below
                NRF_P0 -> OUT = 0b00000000001000000000000000000000;
                NRF_P1 -> DIR = 0b00000000000000000000000000100000;
            }
            for(volatile int i = 0; i<7000; i++)
            {
                NRF_P0 -> DIR = 0b00000000010000000000100000000000;
                NRF_P0 -> OUT = 0b00000000010000000000000000000000;
                NRF_P1 -> DIR = 0b00000000000000000000000000100000;
            }
            for(volatile int i = 0; i<7000; i++)
            {
                NRF_P0 -> DIR = 0b00000000000000001000100000000000;
                NRF_P0 -> OUT = 0b00000000000000001000000000000000;
                NRF_P1 -> DIR = 0b00000000000000000000000000100000;
            }
            for(volatile int i = 0; i<7000; i++)
            {
                NRF_P0 -> DIR = 0b00000001000000000000100000000000;
                NRF_P0 -> OUT = 0b00000001000000000000000000000000;
                NRF_P1 -> DIR = 0b00000000000000000000000000100000;
            }
            for(volatile int i = 0; i<7000; i++)
            {
                NRF_P0 -> DIR = 0b10000000000010000000100000000000;
                NRF_P0 -> OUT = 0b00000000000010000000000000000000;
            }
            
        }
    }
    
    if(n == 1)//Repeat the above steps for each individual number
    {
        while(1)
        {
            for(volatile int i = 0; i<7000; i++)
            {
                NRF_P0 -> DIR = 0b10000000001000000000000000000000;
                NRF_P0 -> OUT = 0b00000000001000000000000000000000;
            }
            for(volatile int i = 0; i<7000; i++)
            {
                NRF_P0 -> DIR = 0b10000000010000000000000000000000;
                NRF_P0 -> OUT = 0b00000000010000000000000000000000;
            }
            for(volatile int i = 0; i<7000; i++)
            {
                NRF_P0 -> DIR = 0b10000000000000001000000000000000;
                NRF_P0 -> OUT = 0b00000000000000001000000000000000;
            }
            for(volatile int i = 0; i<7000; i++)
            {
                NRF_P0 -> DIR = 0b10000001000000000000000000000000;
                NRF_P0 -> OUT = 0b00000001000000000000000000000000;
            }
            for(volatile int i = 0; i<7000; i++)
            {
                NRF_P0 -> DIR = 0b10000000000010000000000000000000;
                NRF_P0 -> OUT = 0b00000000000010000000000000000000;
            }
        }
    }
    if(n == 2)
    {
      while(1)
        {
            for(volatile int i = 0; i<7000; i++)
            {
                NRF_P0 -> DIR = 0b10000000001000000000100000000000;
                NRF_P0 -> OUT = 0b00000000001000000000000000000000;
                NRF_P1 -> DIR = 0b00000000000000000000000000000000;
            }
            for(volatile int i = 0; i<7000; i++)
            {
                NRF_P0 -> DIR = 0b00010000010000000000000000000000;
                NRF_P0 -> OUT = 0b00000000010000000000000000000000;
                NRF_P1 -> DIR = 0b00000000000000000000000000100000;
            }
            for(volatile int i = 0; i<7000; i++)
            {
                NRF_P0 -> DIR = 0b10000000000000001000000000000000;
                NRF_P0 -> OUT = 0b00000000000000001000000000000000;
                NRF_P1 -> DIR = 0b00000000000000000000000000000000;
            }
            for(volatile int i = 0; i<7000; i++)
            {
                NRF_P0 -> DIR = 0b00000001000000000000100000000000;
                NRF_P0 -> OUT = 0b00000001000000000000000000000000;
                NRF_P1 -> DIR = 0b00000000000000000000000000000000;
            }
            for(volatile int i = 0; i<7000; i++)
            {
                NRF_P0 -> DIR = 0b10010000001010000000100000000000;
                NRF_P0 -> OUT = 0b00000000000010000000000000000000;
                NRF_P1 -> DIR = 0b00000000000000000000000000100000;

            }
            
        } 
    }
    if(n == 3)
    {
        while(1)
        {
            for(volatile int i = 0; i<7000; i++)
            {
                NRF_P0 -> DIR = 0b10010000001000000000100000000000;
                NRF_P0 -> OUT = 0b00000000001000000000000000000000;
                NRF_P1 -> DIR = 0b00000000000000000000000000000000;
            }
            for(volatile int i = 0; i<7000; i++)
            {
                NRF_P0 -> DIR = 0b00000000010000000000000000000000;
                NRF_P0 -> OUT = 0b00000000010000000000000000000000;
                NRF_P1 -> DIR = 0b00000000000000000000000000100000;
            }
            for(volatile int i = 0; i<7000; i++)
            {
                NRF_P0 -> DIR = 0b10010000001000001000100000000000;
                NRF_P0 -> OUT = 0b00000000000000001000000000000000;
                NRF_P1 -> DIR = 0b00000000000000000000000000100000;
            }
            for(volatile int i = 0; i<7000; i++)
            {
                NRF_P0 -> DIR = 0b00000001000000000000000000000000;
                NRF_P0 -> OUT = 0b00000001000000000000000000000000;
                NRF_P1 -> DIR = 0b00000000000000000000000000100000;
            }
            for(volatile int i = 0; i<7000; i++)
            {
                NRF_P0 -> DIR = 0b10010000001010000000100000000000;
                NRF_P0 -> OUT = 0b00000000000010000000000000000000;
                NRF_P1 -> DIR = 0b00000000000000000000000000000000;
            }
        }
    }
    if(n == 4)
    {
        while(1)
        {
            for(volatile int i = 0; i<7000; i++)
            {
                NRF_P0 -> DIR = 0b10000000001000000000000000000000;
                NRF_P0 -> OUT = 0b00000000001000000000000000000000;
                NRF_P1 -> DIR = 0b00000000000000000000000000000000;
            }
            for(volatile int i = 0; i<7000; i++)
            {
                NRF_P0 -> DIR = 0b10000000010000000000100000000000;
                NRF_P0 -> OUT = 0b00000000010000000000000000000000;
                NRF_P1 -> DIR = 0b00000000000000000000000000000000;
            }
            for(volatile int i = 0; i<7000; i++)
            {
                NRF_P0 -> DIR = 0b10010000000000001000100000000000;
                NRF_P0 -> OUT = 0b00000000000000001000000000000000;
                NRF_P1 -> DIR = 0b00000000000000000000000000100000;
            }
            for(volatile int i = 0; i<7000; i++)
            {
                NRF_P0 -> DIR = 0b10000001000000000000000000000000;
                NRF_P0 -> OUT = 0b00000001000000000000000000000000;
                NRF_P1 -> DIR = 0b00000000000000000000000000000000;
            }
            for(volatile int i = 0; i<7000; i++)
            {
                NRF_P0 -> DIR = 0b10000000000010000000000000000000;
                NRF_P0 -> OUT = 0b00000000000010000000000000000000;
                NRF_P1 -> DIR = 0b00000000000000000000000000000000;
            }
        }
    }
    if(n == 5)
    {
        while(1)
        {
            for(volatile int i = 0; i<7000; i++)
            {
                NRF_P0 -> DIR = 0b11010000001000000000100000000000;
                NRF_P0 -> OUT = 0b00000000001000000000000000000000;
                NRF_P1 -> DIR = 0b00000000000000000000000000100000;
            }
            for(volatile int i = 0; i<7000; i++)
            {
                NRF_P0 -> DIR = 0b00010000010000000000000000000000;
                NRF_P0 -> OUT = 0b00000000010000000000000000000000;
                NRF_P1 -> DIR = 0b00000000000000000000000000000000;
            }
            for(volatile int i = 0; i<7000; i++)
            {
                NRF_P0 -> DIR = 0b10010000001000001000100000000000;
                NRF_P0 -> OUT = 0b00000000000000001000000000000000;
                NRF_P1 -> DIR = 0b00000000000000000000000000100000;
            }
            for(volatile int i = 0; i<7000; i++)
            {
                NRF_P0 -> DIR = 0b01000001000000000000000000000000;
                NRF_P0 -> OUT = 0b00000001000000000000000000000000;
                NRF_P1 -> DIR = 0b00000000000000000000000000000000;
            }
            for(volatile int i = 0; i<7000; i++)
            {
                NRF_P0 -> DIR = 0b10010000000010000000100000000000;
                NRF_P0 -> OUT = 0b00000000000010000000000000000000;
                NRF_P1 -> DIR = 0b00000000000000000000000000100000;
            }
        }
    }
    if(n == 6)
    {
        while(1)
        {
            for(volatile int i = 0; i<7000; i++)
            {
                NRF_P0 -> DIR = 0b00000000001000000000000000000000;
                NRF_P0 -> OUT = 0b00000000001000000000000000000000;
                NRF_P1 -> DIR = 0b00000000000000000000000000100000;
            }
            for(volatile int i = 0; i<7000; i++)
            {
                NRF_P0 -> DIR = 0b10000000010000000000000000000000;
                NRF_P0 -> OUT = 0b00000000010000000000000000000000;
                NRF_P1 -> DIR = 0b00000000000000000000000000000000;
            }
            for(volatile int i = 0; i<7000; i++)
            {
                NRF_P0 -> DIR = 0b10000000000000001000100000000000;
                NRF_P0 -> OUT = 0b00000000000000001000000000000000;
                NRF_P1 -> DIR = 0b00000000000000000000000000100000;
            }
            for(volatile int i = 0; i<7000; i++)
            {
                NRF_P0 -> DIR = 0b00000001000000000000100000000000;
                NRF_P0 -> OUT = 0b00000001000000000000000000000000;
                NRF_P1 -> DIR = 0b00000000000000000000000000100000;
            }
            for(volatile int i = 0; i<7000; i++)
            {
                NRF_P0 -> DIR = 0b10000000000010000000100000000000;
                NRF_P0 -> OUT = 0b00000000000010000000000000000000;
                NRF_P1 -> DIR = 0b00000000000000000000000000100000;
            }
        }
    }
    if(n == 7)
    {
        while(1)
        {
            for(volatile int i = 0; i<7000; i++)
            {
                NRF_P0 -> DIR = 0b11010000001000000000100000000000;
                NRF_P0 -> OUT = 0b00000000001000000000000000000000;
                NRF_P1 -> DIR = 0b00000000000000000000000000100000;
            }
            for(volatile int i = 0; i<7000; i++)
            {
                NRF_P0 -> DIR = 0b10000000010000000000000000000000;
                NRF_P0 -> OUT = 0b00000000010000000000000000000000;
                NRF_P1 -> DIR = 0b00000000000000000000000000100000;
            }
            for(volatile int i = 0; i<7000; i++)
            {
                NRF_P0 -> DIR = 0b00000000000000001000000000000000;
                NRF_P0 -> OUT = 0b00000000000000001000000000000000;
                NRF_P1 -> DIR = 0b00000000000000000000000000000000;
            }
            for(volatile int i = 0; i<7000; i++)
            {
                NRF_P0 -> DIR = 0b00000001000000000000100000000000;
                NRF_P0 -> OUT = 0b00000001000000000000000000000000;
                NRF_P1 -> DIR = 0b00000000000000000000000000000000;
            }
            for(volatile int i = 0; i<7000; i++)
            {
                NRF_P0 -> DIR = 0b00010000000010000000000000000000;
                NRF_P0 -> OUT = 0b00000000000010000000000000000000;
                NRF_P1 -> DIR = 0b00000000000000000000000000000000;
            }
        }
    }
    if(n == 8)
    {
        while(1)
        {
            for(volatile int i = 0; i<7000; i++)
            {
                NRF_P0 -> DIR = 0b10000000001000000000100000000000;
                NRF_P0 -> OUT = 0b00000000001000000000000000000000;
                NRF_P1 -> DIR = 0b00000000000000000000000000100000;
            }
            for(volatile int i = 0; i<7000; i++)
            {
                NRF_P0 -> DIR = 0b00000000010000000000100000000000;
                NRF_P0 -> OUT = 0b00000000010000000000000000000000;
                NRF_P1 -> DIR = 0b00000000000000000000000000100000;
            }
            for(volatile int i = 0; i<7000; i++)
            {
                NRF_P0 -> DIR = 0b10000000000000001000000000000000;
                NRF_P0 -> OUT = 0b00000000000000001000000000000000;
                NRF_P1 -> DIR = 0b00000000000000000000000000000000;
            }
            for(volatile int i = 0; i<7000; i++)
            {
                NRF_P0 -> DIR = 0b00000001000000000000100000000000;
                NRF_P0 -> OUT = 0b00000001000000000000000000000000;
                NRF_P1 -> DIR = 0b00000000000000000000000000100000;
            }
            for(volatile int i = 0; i<7000; i++)
            {
                NRF_P0 -> DIR = 0b10000000000010000000100000000000;
                NRF_P0 -> OUT = 0b00000000000010000000000000000000;
                NRF_P1 -> DIR = 0b00000000000000000000000000100000;
            }
        }
    }
    if(n == 9)
    {
        while(1)
        {
            for(volatile int i = 0; i<7000; i++)
            {
                NRF_P0 -> DIR = 0b10000000001000000000100000000000;
                NRF_P0 -> OUT = 0b00000000001000000000000000000000;
                NRF_P1 -> DIR = 0b00000000000000000000000000100000;
            }
            for(volatile int i = 0; i<7000; i++)
            {
                NRF_P0 -> DIR = 0b00010000010000000000000000000000;
                NRF_P0 -> OUT = 0b00000000010000000000000000000000;
                NRF_P1 -> DIR = 0b00000000000000000000000000100000;
            }
            for(volatile int i = 0; i<7000; i++)
            {
                NRF_P0 -> DIR = 0b10000000000000001000100000000000;
                NRF_P0 -> OUT = 0b00000000000000001000000000000000;
                NRF_P1 -> DIR = 0b00000000000000000000000000100000;
            }
            for(volatile int i = 0; i<7000; i++)
            {
                NRF_P0 -> DIR = 0b10000001000000000000000000000000;
                NRF_P0 -> OUT = 0b00000001000000000000000000000000;
                NRF_P1 -> DIR = 0b00000000000000000000000000000000;
            }
            for(volatile int i = 0; i<7000; i++)
            {
                NRF_P0 -> DIR = 0b00000000000010000000100000000000;
                NRF_P0 -> OUT = 0b00000000000010000000000000000000;
            }
        }
    }
}
