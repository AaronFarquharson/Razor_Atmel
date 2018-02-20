/**********************************************************************************************************************
File: user_app2.c                                                                

Description:
This is a user_app2.c file template 

------------------------------------------------------------------------------------------------------------------------
API:

Public functions:


Protected System functions:
void UserApp2Initialize(void)
Runs required initialzation for the task.  Should only be called once in main init section.

void UserApp2RunActiveState(void)
Runs current task state.  Should only be called once in main loop.


**********************************************************************************************************************/

#include "configuration.h"
#include "ir_remote.h"
#include "user_app2.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_UserApp2"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserApp2Flags;                       /* Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp2_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp2_StateMachine;            /* The state machine function pointer */
//static u32 UserApp2_u32Timeout;                      /* Timeout counter used across states */


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
Function: UserApp2Initialize

Description:
Initializes the State Machine and its variables.

Requires:
  -

Promises:
  - 
*/
void UserApp2Initialize(void)
{
  LedOn(RED);
  /* If good initialization, set state to Idle */
  if( 1 )
  {
    UserApp2_StateMachine = UserApp2SM_Idle;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    UserApp2_StateMachine = UserApp2SM_FailedInit;
  }

} /* end UserApp2Initialize() */

  
/*----------------------------------------------------------------------------------------------------------------------
Function UserApp2RunActiveState()

Description:
Selects and runs one iteration of the current state in the state machine.
All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
  - State machine function pointer points at current state

Promises:
  - Calls the function to pointed by the state machine function pointer
*/
void UserApp2RunActiveState(void)
{
  UserApp2_StateMachine();

} /* end UserApp2RunActiveState */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/


/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/

/* in this state, the car is only moving forward */
static void UserApp2SM_fwd(void)
{
  // if button 0 is no longer pressed, stop moving, and go back to idle
  if(!IsButtonPressed(BUTTON0)){
    LedOn(RED);
    LedOff(GREEN);
    UserApp2_StateMachine = UserApp2SM_Idle;
  }
  
  // if button 2 is pressed, turn left, and go to new state
  if(IsButtonPressed(BUTTON2)){
    UserApp2_StateMachine = UserApp2SM_fwd_lft;
  }
  // if button 3 is pressed, turn right, and go to new state
  if(IsButtonPressed(BUTTON3)){
    UserApp2_StateMachine = UserApp2SM_fwd_rht;
  }  
}

/* in this state, the car is only moving backward */
static void UserApp2SM_bck(void)
{
  // if button 1 is no longer pressed, stop moving, and go back to idle
  if(!IsButtonPressed(BUTTON1)){
    LedOn(RED);
    LedOff(BLUE);
    UserApp2_StateMachine = UserApp2SM_Idle;
  }
  
  // if button 2 is pressed, turn left, and go to new state
  if(IsButtonPressed(BUTTON2)){
    UserApp2_StateMachine = UserApp2SM_bck_lft;
  }
  // if button 3 is pressed, turn right, and go to new state
  if(IsButtonPressed(BUTTON3)){
    UserApp2_StateMachine = UserApp2SM_bck_rht;
  } 
}

/* in this state, the car is only turning left */
static void UserApp2SM_lft(void)
{
  // if button 2 is no longer pressed, stop turning, and go back to idle
  if(!IsButtonPressed(BUTTON2)){
    LedOn(RED);
    LedOff(PURPLE);
    UserApp2_StateMachine = UserApp2SM_Idle;
  }
  
  // if button 0 is pressed, go forward, and go to new state
  if(IsButtonPressed(BUTTON0)){
    UserApp2_StateMachine = UserApp2SM_fwd_lft;
  }
  // if button 1 is pressed, go backward, and go to new state
  if(IsButtonPressed(BUTTON1)){
    UserApp2_StateMachine = UserApp2SM_bck_lft;
  } 
}

/* in this state, the car is only turning right */
static void UserApp2SM_rht(void)
{
  // if button 3 is no longer pressed, stop turning, and go back to idle
  if(!IsButtonPressed(BUTTON3)){
    LedOn(RED);
    LedOff(YELLOW);
    UserApp2_StateMachine = UserApp2SM_Idle;
  }
  
  // if button 0 is pressed, go forward, and go to new state
  if(IsButtonPressed(BUTTON0)){
    UserApp2_StateMachine = UserApp2SM_fwd_rht;
  }
  // if button 1 is pressed, go backward, and go to new state
  if(IsButtonPressed(BUTTON1)){
    UserApp2_StateMachine = UserApp2SM_bck_rht;
  }
}

/* in this state, the car is moving forward and turning right */
static void UserApp2SM_fwd_rht(void)
{
  // if button 3 is no longer pressed, stop turning, and go back to another state
  if(!IsButtonPressed(BUTTON3)){
    UserApp2_StateMachine = UserApp2SM_fwd;
  }
  // if button 0 is no longer pressed, stop moving, and go back to another state
  if(!IsButtonPressed(BUTTON0)){
    UserApp2_StateMachine = UserApp2SM_rht;
  }
}

/* in this state, the car is moving forward and turning left */
static void UserApp2SM_fwd_lft(void)
{
  // if button 2 is no longer pressed, stop turning, and go back to another state
  if(!IsButtonPressed(BUTTON2)){
    UserApp2_StateMachine = UserApp2SM_fwd;
  }
  // if button 0 is no longer pressed, stop moving, and go back to another state
  if(!IsButtonPressed(BUTTON0)){
    UserApp2_StateMachine = UserApp2SM_lft;
  }
}

/* in this state, the car is moving backward and turning right */
static void UserApp2SM_bck_rht(void)
{
  // if button 3 is no longer pressed, stop turning, and go back to another state
  if(!IsButtonPressed(BUTTON3)){
    UserApp2_StateMachine = UserApp2SM_bck;
  }
  // if button 1 is no longer pressed, stop moving, and go back to another state
  if(!IsButtonPressed(BUTTON1)){
    UserApp2_StateMachine = UserApp2SM_rht;
  }
}

/* in this state, the car is moving backward and turning left */
static void UserApp2SM_bck_lft(void)
{
  // if button 2 is no longer pressed, stop turning, and go back to another state
  if(!IsButtonPressed(BUTTON2)){
    UserApp2_StateMachine = UserApp2SM_bck;
  }
  // if button 1 is no longer pressed, stop moving, and go back to another state
  if(!IsButtonPressed(BUTTON1)){
    UserApp2_StateMachine = UserApp2SM_lft;
  }
}


/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ??? */
static void UserApp2SM_Idle(void)
{
  if(IsButtonPressed(BUTTON0)){
    LedOn(GREEN);
    LedOff(RED);
    UserApp2_StateMachine = UserApp2SM_fwd;
  }
  if(IsButtonPressed(BUTTON1)){
    LedOn(BLUE);
    LedOff(RED);
    UserApp2_StateMachine = UserApp2SM_bck;
  }
  if(IsButtonPressed(BUTTON2)){
    LedOn(PURPLE);
    LedOff(RED);
    UserApp2_StateMachine = UserApp2SM_lft;
  }
  if(IsButtonPressed(BUTTON3)){
    LedOn(YELLOW);
    LedOff(RED);
    UserApp2_StateMachine = UserApp2SM_rht;
  }
#if 0
  static u16 start = 7000;
  if(IsButtonPressed(BUTTON0)){
    LedOff(RED);
    LedOn(GREEN);
    singleOut(0, PWM_FWD7, DRIVE, CH1, 8062);
  }
  if(IsButtonPressed(BUTTON1)){
    LedOff(RED);
    LedOn(BLUE);
    singleOut(0, PWM_REV7, DRIVE, CH1, 7775);
  }
  if(IsButtonPressed(BUTTON2)){
    singleOut(0, PWM_REV7, TURN, CH1, 7890);
  }
  if(IsButtonPressed(BUTTON3)){
    singleOut(0, PWM_FWD7, TURN, CH1, 7219);
    start++;
  }
  else{
    LedOn(RED);
    LedOff(GREEN);
    LedOff(BLUE);
    singleOut(0, PWM_BRK, DRIVE, CH1, start);
    singleOut(0, PWM_BRK, TURN, CH1, start);
  }
#endif
} /* end UserApp2SM_Idle() */
     
#if 0
/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp2SM_Error(void)          
{
  
} /* end UserApp2SM_Error() */
#endif


/*-------------------------------------------------------------------------------------------------------------------*/
/* State to sit in if init failed */
static void UserApp2SM_FailedInit(void)          
{
    
} /* end UserApp2SM_FailedInit() */


/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
