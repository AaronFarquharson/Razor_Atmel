/**********************************************************************************************************************
File: user_app2.h                                                                

Description:
Header file for user_app2.c

**********************************************************************************************************************/

#ifndef __USER_APP2_H
#define __USER_APP2_H

/**********************************************************************************************************************
Type Definitions
**********************************************************************************************************************/


/**********************************************************************************************************************
Constants / Definitions
**********************************************************************************************************************/


/**********************************************************************************************************************
Function Declarations
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/
void UserApp2Initialize(void);
void UserApp2RunActiveState(void);


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/


/***********************************************************************************************************************
State Machine Declarations
***********************************************************************************************************************/
static void UserApp2SM_Idle(void);    
static void ir_remoteSM_fwd(void);
static void ir_remoteSM_bck(void);
static void ir_remoteSM_rht(void);
static void ir_remoteSM_lft(void);
static void ir_remoteSM_fwd_rht(void);
static void ir_remoteSM_fwd_lft(void);
static void ir_remoteSM_bck_rht(void);
static void ir_remoteSM_bck_lft(void);
static void UserApp2SM_Error(void);         
static void UserApp2SM_FailedInit(void);        


#endif /* __USER_APP2_H */


/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
