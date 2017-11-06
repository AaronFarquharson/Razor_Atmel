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


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_StateMachine;            /* The state machine function pointer */
//static u32 UserApp1_u32Timeout;                      /* Timeout counter used across states */


/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/

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
  LedOff(WHITE);
  LedOff(PURPLE);
  LedOff(BLUE);
  LedOff(CYAN);
  LedOff(GREEN);
  LedOff(YELLOW);
  LedOff(ORANGE);
  LedOn(RED);
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
  static u8 u8counter = 0;
  static u8 u8correct = 2;
  static u8 u8pass_state = 0;
  static u8 u8passcount = 0;
  static int input[10];
  static int passcode[10]= {0,2,0,0,0,0,3,0,0,1};
  
  //password reset code
  
  if(IsButtonHeld(BUTTON3, 3000) && u8pass_state == 0)
  {
    u8pass_state = 1;
    u8passcount = 0;
    LedBlink(GREEN, LED_4HZ);
    LedBlink(RED, LED_4HZ);
  }
  
  if(IsButtonHeld(BUTTON0, 3000) && u8pass_state == 1)
  {
    u8pass_state = 0;
    u8counter = 0;
    LedOff(GREEN);
    LedOn(RED);
  }
  
  if(WasButtonPressed(BUTTON0) && u8pass_state == 1)
  {
    ButtonAcknowledge(BUTTON0);
    if(u8passcount < 10)
    {
      passcode[u8passcount] = 0;
    }
    u8passcount++;
  }
  if(WasButtonPressed(BUTTON1) && u8pass_state == 1)
  {
    ButtonAcknowledge(BUTTON1);
    if(u8passcount < 10)
    {
      passcode[u8passcount] = 1;
    }
    u8passcount++;
  }
  if(WasButtonPressed(BUTTON2) && u8pass_state == 1)
  {
    ButtonAcknowledge(BUTTON2);
    if(u8passcount < 10)
    {
      passcode[u8passcount] = 2;
    }
    u8passcount++;
  }
  if(WasButtonPressed(BUTTON3) && u8pass_state == 1)
  {
    ButtonAcknowledge(BUTTON3);
    if(u8passcount < 10)
    {
      passcode[u8passcount] = 3;
    }
    u8passcount++;
  }
  
  //password checking code
  
  if(WasButtonPressed(BUTTON0) && u8pass_state == 0)
  {
    ButtonAcknowledge(BUTTON0);
    if(u8counter < 10)
    {
      input[u8counter] = 0;
    }
    u8counter++;
    if(u8counter >= 11)
    {
      LedOff(GREEN);
      LedOn(RED);
      u8counter = 0;
    }
  }
  if(WasButtonPressed(BUTTON1) && u8pass_state == 0)
  {
    ButtonAcknowledge(BUTTON1);
    if(u8counter < 10)
    {
      input[u8counter] = 1;
    }
    u8counter++;
    if(u8counter >= 11)
    {
      LedOff(GREEN);
      LedOn(RED);
      u8counter = 0;
    }
  }
  if(WasButtonPressed(BUTTON2) && u8pass_state == 0)
  {
    ButtonAcknowledge(BUTTON2);
    if(u8counter < 10)
    {
      input[u8counter] = 2;
    }
    u8counter++;
    if(u8counter >= 11)
    {
      LedOff(GREEN);
      LedOn(RED);
      u8counter = 0;
    }
  }
  if(WasButtonPressed(BUTTON3) && u8pass_state == 0)
  {
    ButtonAcknowledge(BUTTON3);
    if(u8counter < 10)
    {
      input[u8counter] = 3;
    }
    u8counter++;
    if(u8counter >= 11)
    {
      LedOff(GREEN);
      LedOn(RED);
      u8counter = 0;
    }
  }
  if(u8counter == 10 && u8pass_state == 0)
  {
    for(int i = 0; i<10; i++)
    {
      if(input[i] != passcode[i])
      {
        u8correct = 0;
        break;
      }
      if(i ==9)
      {
        u8correct = 1;
      }
    }
  }
  if(u8correct == 1 && u8pass_state == 0)
  {
    LedBlink(GREEN, LED_2HZ);
    LedOff(RED);
    u8correct = 2;
    u8counter = 11;
  }
  if(u8correct == 0 && u8pass_state == 0)
  {
    LedBlink(RED, LED_2HZ);
    u8correct = 2;
    u8counter = 11;
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
