/**********************************************************************************************************************
File: ir_remote.c                                                                

----------------------------------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------------------------------

Description:
This is a ir_remote.c file template 

------------------------------------------------------------------------------------------------------------------------
API:

Public functions:


Protected System functions:
void ir_remoteInitialize(void)
Runs required initialzation for the task.  Should only be called once in main init section.

void ir_remoteRunActiveState(void)
Runs current task state.  Should only be called once in main loop.


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_ir_remote"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32ir_remoteFlags;                       /* Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "ir_remote_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type ir_remote_StateMachine;            /* The state machine function pointer */
//static u32 ir_remote_u32Timeout;                      /* Timeout counter used across states */
static int ir_toggle[4] = {0, 0, 0, 0};
static int delay_code1, delay_code2, messagecount = 0;


/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/
// This function sets the correct nibbles for the single output mode as defined by the protocol
void singleOut( int mode, int power, int rb, int ch)
{
  int nibble1, nibble2, nibble3, nibble4;
  
  // set the nibbles
  nibble1 = ir_toggle[ch] | ch;
  nibble2 = 0x4 | mode | rb;
  nibble3 = power;
  nibble4 = 0xf ^ nibble1 ^ nibble2 ^ nibble3;
  
  pause(ch, messagecount);
  send_signal((nibble1 << 4) | nibble2, (nibble3 << 4) | nibble4);
  
  if (ir_toggle[ch] == 0)
        ir_toggle[ch] = 8;
  else
        ir_toggle[ch] = 0;
}


// This function writes the input pin to the input level, given the input for the current level
void writePin(u32 pin, u8 level)
{
  // set the level to output high
  if(level == HIGH){
    AT91C_BASE_PIOA->PIO_SODR |= pin;
  }
  else{
    AT91C_BASE_PIOA->PIO_CODR |= pin;
  }  
}

/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/
// This function sends the ir signal from the board to the car
void send_signal(int code1, int code2)
{
  if (code1 == delay_code1 && code2 == delay_code2) 
    {
        if (messagecount < 4)
            messagecount++;
    } else 
    {
        delay_code1 = code1;
        delay_code2 = code2;
        messagecount = 0;
    }
	
    //cli(); // make it uninterruptable
    start_stop_bit();

    int x = 128;
    while (x) {
        oscillationWrite(_IR_Pin, 156);

        if (code1 & x) //high bit
            delayMicroseconds(546);
        else //low bit
            delayMicroseconds(260);

        x >>= 1; //next bit
    }

    x = 128;
    while (x) {
        oscillationWrite(_IR_Pin, 156);

        if (code2 & x) // high bit
            delayMicroseconds(546);
        else //low bit
            delayMicroseconds(260);

        x >>= 1; //next bit
    }
    start_stop_bit();
    //sei();
}

// This function sends the codes to start the transmission and delays for the manditory time
void start_stop_bit() {
    oscillationWrite(_IR_Pin, 156);
    delayMicroseconds(1014);
}

// This function actually sends the signal to the pin in the form of ones and zeros.
void oscillationWrite(int pin, int time) {
    for (int i = 0; i <= time / 26; i++) {
        digitalWrite(pin, HIGH);
        delayMicroseconds(9);
        digitalWrite(pin, LOW);
        delayMicroseconds(9);
    }
}

// This function inplements the timeout for lost IR
void pause(int channel, int count) {
    unsigned char a = 0;

    if (count == 0)
        a = 4 - channel;
    else if (count == 1 || count == 2)
        a = 5;
    else if (count == 3 || count == 4)
        a = (6 + 2 * channel);

    delay(a * 16);
}
/*--------------------------------------------------------------------------------------------------------------------
Function: ir_remoteInitialize

Description:
Initializes the State Machine and its variables.

Requires:
  -

Promises:
  - 
*/
void ir_remoteInitialize(void)
{
 
  /* If good initialization, set state to Idle */
  if( 1 )
  {
    ir_remote_StateMachine = ir_remoteSM_Idle;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    ir_remote_StateMachine = ir_remoteSM_Error;
  }

} /* end ir_remoteInitialize() */

  
/*----------------------------------------------------------------------------------------------------------------------
Function ir_remoteRunActiveState()

Description:
Selects and runs one iteration of the current state in the state machine.
All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
  - State machine function pointer points at current state

Promises:
  - Calls the function to pointed by the state machine function pointer
*/
void ir_remoteRunActiveState(void)
{
  ir_remote_StateMachine();

} /* end ir_remoteRunActiveState */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/


/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ??? */
static void ir_remoteSM_Idle(void)
{

} /* end ir_remoteSM_Idle() */
    

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void ir_remoteSM_Error(void)          
{
  
} /* end ir_remoteSM_Error() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
