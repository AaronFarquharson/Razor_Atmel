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
static u8 fwdSpeed = PWM_FWD7;
static u8 bckSpeed = PWM_REV7;
static u8 speed = 7;
static u8 * speedmsg = "Current speed = 7";
static u8 * newspeed = "New speed = 7";
static u8 * newchannel = "New channel = 1";
static u8 channel = CH1;

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
  LCDCommand(LCD_CLEAR_CMD);
  LCDMessage(LINE1_START_ADDR,"IR Remote Control");
  LCDMessage(LINE2_START_ADDR, speedmsg);
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
    singleOut(0, PWM_BRK, DRIVE, channel);
    UserApp2_StateMachine = UserApp2SM_Idle;
  }
  
  // if button 2 is pressed, turn left, and go to new state
  if(IsButtonPressed(BUTTON2)){
    singleOut(0, PWM_REV7, TURN, channel);
    UserApp2_StateMachine = UserApp2SM_fwd_lft;
  }
  // if button 3 is pressed, turn right, and go to new state
  if(IsButtonPressed(BUTTON3)){
    singleOut(0, PWM_FWD7, TURN, channel);
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
    singleOut(0, PWM_BRK, DRIVE, channel);
    UserApp2_StateMachine = UserApp2SM_Idle;
  }
  
  // if button 2 is pressed, turn left, and go to new state
  if(IsButtonPressed(BUTTON2)){
    singleOut(0, PWM_REV7, TURN, channel);
    UserApp2_StateMachine = UserApp2SM_bck_lft;
  }
  // if button 3 is pressed, turn right, and go to new state
  if(IsButtonPressed(BUTTON3)){
    singleOut(0, PWM_FWD7, TURN, channel);
    UserApp2_StateMachine = UserApp2SM_bck_rht;
  } 
}

/* in this state, the car is only turning left */
static void UserApp2SM_lft(void)
{
  // if button 2 is no longer pressed, stop turning, and go back to idle
  if(!IsButtonPressed(BUTTON2)){
    LedOff(PURPLE);
    singleOut(0, PWM_BRK, TURN, channel);
    UserApp2_StateMachine = UserApp2SM_Idle;
  }
  
  // if button 0 is pressed, go forward, and go to new state
  if(IsButtonPressed(BUTTON0)){
    LedOff(RED);
    LedOn(GREEN);
    singleOut(0, fwdSpeed, DRIVE, channel);
    UserApp2_StateMachine = UserApp2SM_fwd_lft;
  }
  // if button 1 is pressed, go backward, and go to new state
  if(IsButtonPressed(BUTTON1)){
    LedOff(RED);
    LedOn(BLUE);
    singleOut(0, bckSpeed, DRIVE, channel);
    UserApp2_StateMachine = UserApp2SM_bck_lft;
  }
}

/* in this state, the car is only turning right */
static void UserApp2SM_rht(void)
{
  // if button 3 is no longer pressed, stop turning, and go back to idle
  if(!IsButtonPressed(BUTTON3)){
    LedOff(YELLOW);
    singleOut(0, PWM_BRK, TURN, channel);
    UserApp2_StateMachine = UserApp2SM_Idle;
  }
  
  // if button 0 is pressed, go forward, and go to new state
  if(IsButtonPressed(BUTTON0)){
    LedOff(RED);
    LedOn(GREEN);
    singleOut(0, fwdSpeed, DRIVE, channel);
    UserApp2_StateMachine = UserApp2SM_fwd_rht;
  }
  // if button 1 is pressed, go backward, and go to new state
  if(IsButtonPressed(BUTTON1)){
    LedOff(RED);
    LedOn(BLUE);
    singleOut(0, bckSpeed, DRIVE, channel);
    UserApp2_StateMachine = UserApp2SM_bck_rht;
  }
}

/* in this state, the car is moving forward and turning right */
static void UserApp2SM_fwd_rht(void)
{
  // if button 3 is no longer pressed, stop turning, and go back to another state
  if(!IsButtonPressed(BUTTON3)){
    singleOut(0, PWM_BRK, TURN, channel);
    UserApp2_StateMachine = UserApp2SM_fwd;
  }
  // if button 0 is no longer pressed, stop moving, and go back to another state
  if(!IsButtonPressed(BUTTON0)){
    LedOff(GREEN);
    LedOn(RED);
    singleOut(0, PWM_BRK, DRIVE, channel);
    UserApp2_StateMachine = UserApp2SM_rht;
  }
}

/* in this state, the car is moving forward and turning left */
static void UserApp2SM_fwd_lft(void)
{
  // if button 2 is no longer pressed, stop turning, and go back to another state
  if(!IsButtonPressed(BUTTON2)){
    singleOut(0, PWM_BRK, TURN, channel);
    UserApp2_StateMachine = UserApp2SM_fwd;
  }
  // if button 0 is no longer pressed, stop moving, and go back to another state
  if(!IsButtonPressed(BUTTON0)){
    LedOff(GREEN);
    LedOn(RED);
    singleOut(0, PWM_BRK, DRIVE, channel);
    UserApp2_StateMachine = UserApp2SM_lft;
  }
}

/* in this state, the car is moving backward and turning right */
static void UserApp2SM_bck_rht(void)
{
  // if button 3 is no longer pressed, stop turning, and go back to another state
  if(!IsButtonPressed(BUTTON3)){
    singleOut(0, PWM_BRK, TURN, channel);
    UserApp2_StateMachine = UserApp2SM_bck;
  }
  // if button 1 is no longer pressed, stop moving, and go back to another state
  if(!IsButtonPressed(BUTTON1)){
    LedOff(BLUE);
    LedOn(RED);
    singleOut(0, PWM_BRK, DRIVE, channel);
    UserApp2_StateMachine = UserApp2SM_rht;
  }
}

/* in this state, the car is moving backward and turning left */
static void UserApp2SM_bck_lft(void)
{
  // if button 2 is no longer pressed, stop turning, and go back to another state
  if(!IsButtonPressed(BUTTON2)){
    singleOut(0, PWM_BRK, TURN, channel);
    UserApp2_StateMachine = UserApp2SM_bck;
  }
  // if button 1 is no longer pressed, stop moving, and go back to another state
  if(!IsButtonPressed(BUTTON1)){
    LedOff(BLUE);
    LedOn(RED);

    singleOut(0, PWM_BRK, DRIVE, channel);
    UserApp2_StateMachine = UserApp2SM_lft;
  }
}


/* this function is used to set the speed for the function */
static void UserApp2SM_setSpeed(void)
{
  // press the right button to set the new speed
  if(WasButtonPressed(BUTTON3))
  {
    ButtonAcknowledge(BUTTON3);
    speed++;
    // there are only seven speeds to cycle through
    if(speed > 7)
    {
      speed = 1;
    }
    // for each of the following cases, we must set both the home screen message
    // and the new speed message and forward and backward speeds
    switch(speed){
      case 1:
        speedmsg = "Current speed = 1";
        newspeed = "New speed = 1";
        fwdSpeed = PWM_FWD1;
        bckSpeed = PWM_REV1;
        break;
      case 2:
        speedmsg = "Current speed = 2";
        newspeed = "New speed = 2";
        fwdSpeed = PWM_FWD2;
        bckSpeed = PWM_REV2;
        break;
      case 3:
        speedmsg = "Current speed = 3";
        newspeed = "New speed = 3";
        fwdSpeed = PWM_FWD3;
        bckSpeed = PWM_REV3;
        break;
      case 4:
        speedmsg = "Current speed = 4";
        newspeed = "New speed = 4";
        fwdSpeed = PWM_FWD4;
        bckSpeed = PWM_REV4;
        break;
      case 5:
        speedmsg = "Current speed = 5";
        newspeed = "New speed = 5";
        fwdSpeed = PWM_FWD5;
        bckSpeed = PWM_REV5;
        break;
      case 6:
        speedmsg = "Current speed = 6";
        newspeed = "New speed = 6";
        fwdSpeed = PWM_FWD6;
        bckSpeed = PWM_REV6;
        break;
      case 7:
        speedmsg = "Current speed = 7";
        newspeed = "New speed = 7";
        fwdSpeed = PWM_FWD7;
        bckSpeed = PWM_REV7;
        break;
    }
    // cycle to the new message
    LCDMessage(LINE1_START_ADDR, newspeed);
  }
  
  // reset all parameters as they were before
  if(!IsButtonPressed(BUTTON4)){
    LCDCommand(LCD_CLEAR_CMD);
    LCDMessage(LINE1_START_ADDR,"IR Remote Control");
    LCDMessage(LINE2_START_ADDR, speedmsg);
    LedOn(LCD_RED);
    LedOff(LCD_GREEN);
    LedOn(RED);
    UserApp2_StateMachine = UserApp2SM_Idle;
  }
}

#if 1
/* this state allows the user to change the channel of operation of the car */
static void UserApp2SM_setChannel(void)
{
  // press the right button to set the new channel
  if(WasButtonPressed(BUTTON3))
  {
    ButtonAcknowledge(BUTTON3);
    channel++;
    // there are only four channels to cycle through
    if(channel > 3)
    {
      channel = 0;
    }
    // for each of the following cases, a new message must be displayed on the LCD
    switch(channel){
      case 0:
        newchannel = "New channel = 1";
        channel = CH1;
        break;
      case 1:
        newchannel = "New channel = 2";
        channel = CH2;
        break;
      case 2:
        newchannel = "New channel = 3";
        channel = CH3;
        break;
      case 3:
        newchannel = "New channel = 4";
        channel = CH4;
        break;
    }
    // cycle to the new message
    LCDMessage(LINE1_START_ADDR, newchannel);
  }
  
  // reset all parameters as they were before
  if(!IsButtonPressed(BUTTON5)){
    LCDCommand(LCD_CLEAR_CMD);
    LCDMessage(LINE1_START_ADDR,"IR Remote Control");
    LCDMessage(LINE2_START_ADDR, speedmsg);
    LedOn(LCD_RED);
    LedOn(RED);
    UserApp2_StateMachine = UserApp2SM_Idle;
  }
}
#endif
/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ??? */
static void UserApp2SM_Idle(void)
{
  // drive forward
  if(IsButtonPressed(BUTTON0)){
    LedOn(GREEN);
    LedOff(RED);
    singleOut(0, fwdSpeed, DRIVE, channel);
    UserApp2_StateMachine = UserApp2SM_fwd;
  }
  //drive backward
  if(IsButtonPressed(BUTTON1)){
    LedOn(BLUE);
    LedOff(RED);
    singleOut(0, bckSpeed, DRIVE, channel);
    UserApp2_StateMachine = UserApp2SM_bck;
  }
  // turn left
  if(IsButtonPressed(BUTTON2)){
    LedOn(PURPLE);
    singleOut(0, PWM_REV7, TURN, channel);
    UserApp2_StateMachine = UserApp2SM_lft;
  }
  // turn right
  if(IsButtonPressed(BUTTON3)){
    LedOn(YELLOW);
    singleOut(0, PWM_FWD7, TURN, channel);
    UserApp2_StateMachine = UserApp2SM_rht;
  }
  // set speed
  if(IsButtonPressed(BUTTON4)){
    LedOff(LCD_RED);
    LedOn(LCD_GREEN);
    LedOff(RED);
    LCDCommand(LCD_CLEAR_CMD);
    LCDMessage(LINE1_START_ADDR, newspeed);
    LCDMessage(LINE2_START_ADDR, "Press right to set");
    UserApp2_StateMachine = UserApp2SM_setSpeed;
  }
  // set channel
  if(IsButtonPressed(BUTTON5)){
    LedOff(LCD_RED);
    LedOff(RED);
    LCDCommand(LCD_CLEAR_CMD);
    LCDMessage(LINE1_START_ADDR, newchannel);
    LCDMessage(LINE2_START_ADDR, "Press right to set");
    UserApp2_StateMachine = UserApp2SM_setChannel;
  }
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
