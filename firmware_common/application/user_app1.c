/**********************************************************************************************************************
File: user_app1.c                                                                

----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app1 as a template:
 1. Copy both user_app1.c and user_app1.h to the Application directory
 2. Rename the files yournewtaskname.c and yournewtaskname.h
 3. Add yournewtaskname.c and yournewtaskname.h to the Application Include and Source groups in the IAR project
 4. Use ctrl-h (make sure "Match Case" is checked) to find and replace all instances of "user_app1" with "yournewtaskname"
 5. Use ctrl-h to find and replace all instances of "UserApp1" with "YourNewTaskName"
 6. Use ctrl-h to find and replace all instances of "USER_APP1" with "YOUR_NEW_TASK_NAME"
 7. Add a call to YourNewTaskNameInitialize() in the init section of main
 8. Add a call to YourNewTaskNameRunActiveState() in the Super Loop section of main
 9. Update yournewtaskname.h per the instructions at the top of yournewtaskname.h
10. Delete this text (between the dashed lines) and update the Description below to describe your task
----------------------------------------------------------------------------------------------------------------------

Description:
This is a user_app1.c file template 

------------------------------------------------------------------------------------------------------------------------
API:

Public functions:


Protected System functions:
void UserApp1Initialize(void)
Runs required initialzation for the task.  Should only be called once in main init section.

void UserApp1RunActiveState(void)
Runs current task state.  Should only be called once in main loop.


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_UserApp1"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserApp1Flags;                       /* Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */

extern u8 G_au8DebugScanfBuffer[];  /* From debug.c */
extern u8 G_u8DebugScanfCharCount;  /* From debug.c */

/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_StateMachine;            /* The state machine function pointer */
//static u32 UserApp1_u32Timeout;                      /* Timeout counter used across states */

/* Char buffer: */
static u8 au8UserInputBuffer[U16_USER_INPUT_BUFFER_SIZE  ];
static u8 au8UserInput[U16_USER_INPUT_BUFFER_SIZE  ];

/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/

int compareName( u16 count, u8 read)
{
  static char nameU[] = "AARON";
  static char nameL[] = "aaron";
  if(count > 4 && read){
    for(int i = 0; nameU[i] != '\0'; i++){
      if(au8UserInput[count - 5 + i] != nameU[i] && au8UserInput[count - 5 + i] != nameL[i]){
        return 0;
      }
      if(i == 4){
        return 1;
      }
    }
  }
  return 1;
}


int readBuffer(u16 *cCount)
{
  int charRead = DebugScanf(au8UserInputBuffer);
  if(charRead){
    au8UserInput[*cCount] = au8UserInputBuffer[0];
    *cCount++;
    return 1;
  }
  return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------
Function: UserApp1Initialize

Description:
Initializes the State Machine and its variables.

Requires:
  -

Promises:
  - 
*/
void UserApp1Initialize(void)
{
  /*clear the buffer*/
  for(u16 i = 0; i < U16_USER_INPUT_BUFFER_SIZE  ; i++)
  {
    au8UserInputBuffer[i] = 0;
  }
  
  /* If good initialization, set state to Idle */
  if( 1 )
  {
    UserApp1_StateMachine = UserApp1SM_Idle;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    UserApp1_StateMachine = UserApp1SM_Error;
  }

} /* end UserApp1Initialize() */

  
/*----------------------------------------------------------------------------------------------------------------------
Function UserApp1RunActiveState()

Description:
Selects and runs one iteration of the current state in the state machine.
All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
  - State machine function pointer points at current state

Promises:
  - Calls the function to pointed by the state machine function pointer
*/
void UserApp1RunActiveState(void)
{
  UserApp1_StateMachine();

} /* end UserApp1RunActiveState */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/


/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ??? */
static void UserApp1SM_Idle(void)
{
  static u16 charcount = 0;
  static u16 asterisc = 10;
  //static char nameU[] = "AARON";
  //static char nameL[] = "aaron";
  static u16 namecountnew = 0;
  static u16 namecountold = 0;
  static u8 charRead = 0;
  charRead = readBuffer(&charcount);
#if 0
  charRead = DebugScanf(au8UserInputBuffer);
  if(charRead){
    au8UserInput[charcount] = au8UserInputBuffer[0];
    charcount++;
  }
#endif
  namecountnew += compareName(charcount , charRead);
#if 0
  if(charcount > 4 && charRead){
    for(int i = 0; nameU[i] != '\0'; i++){
      if(au8UserInput[charcount - 5 + i] != nameU[i] && au8UserInput[charcount - 5 + i] != nameL[i]){
        break;
      }
      if(i == 4){
        namecountnew++;
      }
    }
  }
#endif
  if(namecountnew != namecountold){
    int i = 0;
    if(namecountnew >= asterisc){
      asterisc *= 10;
    }
    u16 temp = asterisc;
    for(i; temp != 0; i++){
      temp /= 10;
    }
    i++;
    DebugPrintf("\r\n");
    for(int j = 0; j < i; j++){
      DebugPrintf("*");
    }
    DebugPrintf("\r\n*");
    DebugPrintNumber(namecountnew);
    DebugPrintf("*\r\n");
    for(int j = 0; j < i; j++){
      DebugPrintf("*");
    }
    DebugLineFeed();
    namecountold = namecountnew;
  }
} /* end UserApp1SM_Idle() */
    

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
