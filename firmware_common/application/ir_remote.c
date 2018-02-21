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
void singleOut( u8 mode, u8 power, u8 rb, u8 ch)
{
  int nibble1, nibble2, nibble3, nibble4;
  
  // set the nibbles
  nibble1 = ir_toggle[ch] | ch;
  nibble2 = 0x4 | mode | rb;
  nibble3 = power;
  nibble4 = 0xf ^ nibble1 ^ nibble2 ^ nibble3;
  
  //pause(ch, messagecount);
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

// This function should delay the code for a given time in microseconds
void delayMicro(u16 time)
{
  TimerSet(TIMER_CHANNEL1, 0);
  TimerStart(TIMER_CHANNEL1);
  while(((2.67*(double)TimerGetTime(TIMER_CHANNEL1))/(double)time) < 0.99){
  }
  TimerStop(TIMER_CHANNEL1);
}
#if 0
// This function should delay the code for a given time in microseconds
void delayMicro(u16 time, u16 start)
{
  int runs = (int)(((double)time * (double)start) / (double)1000);
  for(int i = 0; i < runs; i++){
  }
}
#endif

/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/
// This function sends the ir signal from the board to the car
void send_signal(u32 code1, u32 code2)
{
  if (code1 == delay_code1 && code2 == delay_code2){
      if (messagecount < 4)
           messagecount++;
  } 
  else {
      delay_code1 = code1;
      delay_code2 = code2;
      messagecount = 0;
  }
	
  //cli(); // make it uninterruptable
  startStopSignal();

  int x = 128;
  while (x) {
      writeSignal(IR_PIN, 156);

      if (code1 & x) //high bit
          delayMicro(546);
      else //low bit
          delayMicro(260);

      x >>= 1; //next bit
  }

  x = 128;
  while (x) {
      writeSignal(IR_PIN, 156);

      if (code2 & x) // high bit
          delayMicro(546);
      else //low bit
          delayMicro(260);

      x >>= 1; //next bit
  }
  startStopSignal();
  //sei();
}

// This function sends the codes to start the transmission and delays for the manditory time
void startStopSignal(void) 
{
    writeSignal(IR_PIN, 156);
    delayMicro(1014);
}

// This function actually sends the signal to the pin in the form of ones and zeros.
void writeSignal(u32 pin, u32 time) 
{
    for (int i = 0; i <= time / 26; i++) {
        writePin(pin, HIGH);
        delayMicro(9);
        writePin(pin, LOW);
        delayMicro(9);
    }
}

// This function inplements the timeout for lost IR
/*void pause(u8 channel, u8 count) 
{
    unsigned char a = 0;

    if (count == 0)
        a = 4 - channel;
    else if (count == 1 || count == 2)
        a = 5;
    else if (count == 3 || count == 4)
        a = (6 + 2 * channel);

    delayMicro(a * 1600, start);
}*/
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
