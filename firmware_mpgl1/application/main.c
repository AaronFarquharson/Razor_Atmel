/***********************************************************************************************************************
File: main.c                                                                

Description:
Container for the MPG firmware.  
***********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32SystemFlags = 0;                     /* Global system flags */
volatile u32 G_u32ApplicationFlags = 0;                /* Global applications flags: set when application is successfully initialized */

/*--------------------------------------------------------------------------------------------------------------------*/
/* External global variables defined in other files (must indicate which file they are defined in) */
extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "Main_" and be declared as static.
***********************************************************************************************************************/


/***********************************************************************************************************************
Main Program
Main has two sections:

1. Initialization which is run once on power-up or reset.  All drivers and applications are setup here without timing
contraints but must complete execution regardless of success or failure of starting the application. 

2. Super loop which runs infinitely giving processor time to each application.  The total loop time should not exceed
1ms of execution time counting all application execution.  SystemSleep() will execute to complete the remaining time in
the 1ms period.
***********************************************************************************************************************/

void main(void)
{
  u8 u8Test = 0xA5;
  u8* pu8Example;
  u32 u32Test = 0x0000ffff;
  u32* pu32Example;

  /* Load the addresses into our pointer variables */
  pu8Example = &u8Test;
  pu32Example = &u32Test;

  /* Access the variables via the pointers (two different ways) */
  *pu8Example += 1;
  (*pu32Example)++;

  /* Move the pointers (watch out for the second one!) */
  pu8Example++;
  *pu32Example++;
  
  u32 au32BigArray[] = {5, 4, 3, 2, 1};
  DrinkType aeDrinkArray[3] = {BEER, SHOOTER};
  
  G_u32SystemFlags |= _SYSTEM_INITIALIZING;
  // Check for watch dog restarts

  /* Low level initialization */
  WatchDogSetup(); /* During development, set to not reset processor if timeout */
  GpioSetup();
  ClockSetup();
  InterruptSetup();
  SysTickSetup();

  /* Driver initialization */
  MessagingInitialize();
  UartInitialize();
  DebugInitialize();
  
  /*----------------------------------------------------------------------/
  Function InitializeServer

  Description:
  Initializes a new server.  A new server has an empty tray of drinks and is
  assigned the next number available.

  Requires:
    - psServer_ points to the server list where a new server is to be initialized; 
      since the server list is a pointer to a linked list, this is a pointer-to-pointer
    - Main_u8Servers holds the current number of active servers

  Promises:
    - Returns TRUE if the server is initialized
    - Returns FALSE if the server cannot be initialized
  */
  bool InitializeServer(ServerType** psServer_)
  {
    if(*psServer_ == NULL)
    {
      return(FALSE);
    }

    return(TRUE);
  } /* end InitializeServer() */
    /* Debug messages through DebugPrintf() are available from here */

  SspInitialize();
  TWIInitialize();

  LedInitialize();
  ButtonInitialize();

#ifndef SIMULATOR_MODE
  LcdInitialize();
  AntInitialize();
  SdCardInitialize();
#endif /* SIMULATOR_MODE */

  /* Application initialization */
//  BoardTestInitialize();
//  AudioTestInitialize();
    UserAppInitialize();
  
  /* Exit initialization */
  SystemStatusReport();
  G_u32SystemFlags &= ~_SYSTEM_INITIALIZING;
  
  u8 u8CurrentServer;
  ServerType sServer1;
  ServerType* psServerParser;

  psServerParser = &sServer1;
  sServer1.u8ServerNumber = 18;
  u8CurrentServer = psServerParser->u8ServerNumber;
  
  /* Super loop */  
  while(1)
  {
    WATCHDOG_BONE();
    
    /* Drivers */
    LedUpdate();
    ButtonRunActiveState();
    UartRunActiveState();
    SspRunActiveState();
    TWIRunActiveState();
    MessagingRunActiveState();
    DebugRunActiveState();
    
#ifndef SIMULATOR_MODE
    LcdRunActiveState();
    AntRunActiveState();
    SdCardRunActiveState();
#endif /* SIMULATOR_MODE */

    /* Applications */
    //BoardTestRunActiveState();
    //AudioTestRunActiveState();
    UserAppRunActiveState();
    
    /* System sleep*/
    HEARTBEAT_OFF();
    SystemSleep();
    HEARTBEAT_ON();
    
  } /* end while(1) main super loop */
  
} /* end main() */


/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File */
/*--------------------------------------------------------------------------------------------------------------------*/
