#include "MicroBit.h"
#include <cstdio>

void timerInterrupt(int value)// A timer to make sure only one bit is sent every 8.6us
{
    while(1)
    {
   
        NRF_TIMER1 -> CC[0] = value;// Setting the ticks of the CC[0] register
        NRF_TIMER1 -> PRESCALER = 2;//Setting the prescaler
        NRF_TIMER1 -> TASKS_START = 1;//Starting timer
        if(NRF_TIMER1-> EVENTS_COMPARE[0] == 1)//Waiting for timer to be triggered
        {
            NRF_TIMER1-> TASKS_STOP = 1;//Stopping clock once triggered
            NRF_TIMER1 -> TASKS_CLEAR = 1;//Clearing clock
            NRF_TIMER1 -> EVENTS_COMPARE[0] = 0;//Clearing events register so it can be retriggered next run
            break;
        }
    }
}
        //read x,y,z from address in register for 12c

void bitBangSerial(char *str)
{
    NRF_P0 -> DIR = 0b00000000000000000000000001000000;//Turining on the UART_INT_RX register
    int i = 0;//Creating an incrementer for loop
    while(str[i] != '\0')//Checking a string until the null character is reached
        {
            NRF_P0 -> OUT = 0b00000000000000000000000000000000;//Send start bit
            timerInterrupt(29);//Wait 8.6us
            char pos = str[i];//Get character of string based on incrementer
            for(int x = 7; x>-1; x--)//Iterate through each bit in the character from most to least significant
            {
                if(pos & (1<<(7-x)))//Shifting mask to get each individual bit
                {
                    NRF_P0 -> OUT = 0b00000000000000000000000001000000;//Outputting 1 if a 1 bit is detected
                    timerInterrupt(29);//Wait 8.6us
                }
                else
                {
                    NRF_P0 -> OUT = 0b00000000000000000000000000000000;//Outputting a 0 if a 0 bit is detected  
                    timerInterrupt(29);//Wait 8.6us
                }
            }
            NRF_P0 -> OUT = 0b00000000000000000000000001000000;//Send stop bit for current character 
            timerInterrupt(29);//Wait 8.6us     
            i++;//Increment for next character
        }
         
}

void reset_i2c_flags()
{
    NRF_TWI0->EVENTS_STOPPED = 0;       // Clear any error flags.
    NRF_TWI0->EVENTS_RXDREADY = 0;
    NRF_TWI0->EVENTS_TXDSENT = 0;
    NRF_TWI0->EVENTS_ERROR = 0;
    NRF_TWI0->EVENTS_BB = 0;
    NRF_TWI0->EVENTS_SUSPENDED = 0;
    NRF_TWI0->ERRORSRC = 0xFFFFFFFF;
}

int who_am_i(int sendREG, int sendVAL)
{
    uint32_t result = 0;

    NRF_TWI0->ENABLE = 0;             // Disable the TWI peripheral

    NRF_P0->PIN_CNF[P0_08] = 0x00000600;  // Configure SCL pin mode for I2C
    NRF_P0->PIN_CNF[P0_16] = 0x00000600;  // Configure SDA pin mode for I2C

    NRF_TWI0->PSEL.SCL = 8;           // Select the in number associated with the internal I2C SCL line
    NRF_TWI0->PSEL.SDA = 6;           // Select the in number associated with the internal I2C SDA line
    NRF_TWI0->ADDRESS = 0x19;            // I2C Address of accelerometer
    NRF_TWI0->FREQUENCY = 0x04000000;          // Set I2C bus speed to 250kbps
    NRF_TWI0->SHORTS = 1;               // Suspend the peripheral after every byte. We can then resume it by writing to the TASKS_RESUME register.

    reset_i2c_flags();                  // Always reset flags, so that we can tell when new ones are set smile

    NRF_TWI0->ENABLE = 5;             // Enable the TWI peripheral

    NRF_TWI0->TXD = sendREG; 
                                        // Address of the WHO_AM_I register inside the accelerometer (see datasheet: should contain decimal value 51)      
    NRF_TWI0->TASKS_STARTTX = 1;        // Start an I2C write transaction

    timerInterrupt(1000);
    // Wait for data to be sent
    while(NRF_TWI0->EVENTS_TXDSENT == 0);
    NRF_TWI0->EVENTS_TXDSENT = 0;

    NRF_TWI0->TXD = sendVAL;//Value to send to a register in case of output
    
    // Start receiving
    NRF_TWI0->TASKS_RESUME = 1;
    NRF_TWI0->TASKS_STARTRX = 1;
    
    // Wait for data to be received
    while(NRF_TWI0->EVENTS_RXDREADY == 0);
    NRF_TWI0->EVENTS_RXDREADY = 0;
   
    // Store the byte that has been received
    result = NRF_TWI0->RXD;

    // Finish the transaction
    NRF_TWI0->TASKS_RESUME = 1;


    NRF_TWI0->TASKS_STOP = 1;

    return result;
}

void turnOnAccel()
{
    uint32_t result = 0;

    NRF_TWI0->ENABLE = 0;             // Disable the TWI peripheral

    NRF_P0->PIN_CNF[P0_08] = 0x00000600;  // Configure SCL pin mode for I2C
    NRF_P0->PIN_CNF[P0_16] = 0x00000600;  // Configure SDA pin mode for I2C

    NRF_TWI0->PSEL.SCL = 8;           // Select the in number associated with the internal I2C SCL line
    NRF_TWI0->PSEL.SDA = 6;           // Select the in number associated with the internal I2C SDA line
    NRF_TWI0->ADDRESS = 0x19;            // I2C Address of accelerometer
    NRF_TWI0->FREQUENCY = 0x04000000;          // Set I2C bus speed to 250kbps
    NRF_TWI0->SHORTS = 1;               // Suspend the peripheral after every byte. We can then resume it by writing to the TASKS_RESUME register.

    reset_i2c_flags();                  // Always reset flags, so that we can tell when new ones are set smile

    NRF_TWI0->ENABLE = 5;             // Enable the TWI peripheral

    NRF_TWI0->TXD = 0x20; 
                                        // Address of the WHO_AM_I register inside the accelerometer (see datasheet: should contain decimal value 51)      
    NRF_TWI0->TASKS_STARTTX = 1;        // Start an I2C write transaction

    timerInterrupt(1000);
    // Wait for data to be sent
    while(NRF_TWI0->EVENTS_TXDSENT == 0);
    NRF_TWI0->EVENTS_TXDSENT = 0;

    NRF_TWI0->TXD = 25;//Value to send to a register in case of output

    NRF_TWI0->TASKS_RESUME = 1;

    while(NRF_TWI0->EVENTS_TXDSENT == 0);
    NRF_TWI0->EVENTS_TXDSENT = 0;

    // Finish the transaction
    NRF_TWI0->TASKS_RESUME = 1;

    NRF_TWI0->TASKS_STOP = 1;
}

void showAccelerometerSample()
{   
    timerInterrupt(800000);
    turnOnAccel();//Turning on the accelerometer
    while(1)
    {
        int lowx = who_am_i(0x28,0);//Reading the low byte of x
        int highx = who_am_i(0x29,0);//Reading the high byte of x
        int lowy = who_am_i(0x2A,0);//Reading the low byte of y
        int highy = who_am_i(0x2B,0);//Reading the high byte of y
        int lowz = who_am_i(0x2C,0);//Reading the low byte of z
        int highz = who_am_i(0x2D,0);//Reading the high byte of z
        
        uint8_t x[2] = {lowx, highx};//Storing both x bytes together in memory
        int16_t *xVal = (int16_t*)x;//Casting the array to combine the high and low bytes
        int16_t printX = (int16_t)(*xVal/16);//Casting the above value to an integer for outputting

        uint8_t y[2] = {lowy,highy};//Storing both y bytes together in memory
        int16_t *yVal = (int16_t*)y;//Same as above
        int16_t printY = (int16_t)(*yVal/16);

        uint8_t z[2] = {lowz,highz};//Storing both z bytes together in memory
        int16_t *zVal = (int16_t*)z;//Same as above
        int16_t printZ = (int16_t)(*zVal/16);



        char printer[5];//Char array used to print values above
        sprintf(printer, "%d", lowx);//Printing values
        bitBangSerial(printer);
        timerInterrupt(800000);
    }
}

int main()
{
   showAccelerometerSample();
}