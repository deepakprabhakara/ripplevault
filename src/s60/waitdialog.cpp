/*******************************************************************

 Copyright(c) 2000-2005 

 PurpleACE Wireless Solutions Pvt. Ltd.
 www.purpleace.com
 #708, A Wing, Carlton Towers, Airport Road, Bangalore - 560017,
 India
 All rights reserved.

 This software is the confidential and proprietary information of
 PurpleACE Wireless Solutions Pvt. Ltd. ("Confidential Information").
 You shall not disclose such Confidential Information and
 shall use it only in accordance with the terms of the license
 Agreement you entered into with PurpleACE Wireless Solutions Pvt. Ltd..

 Project Name               : Ripple Vault Symbian Client
 Module Name				: WaitDialog.cpp
 Language used : C++
 Coding Standard			: Symbian Coding Standards
 Author                     : Ezhil,Deepak
 Date Created               : 14-08-2005
 Rational Rose Model Name                                :
 Use Case Document Name                                :
 Use Case Realization Document Name   :
 Test Case Document Name       :
 Abbreviations                :

 Description                   :     Progress dialog for S60
 

 Code Reviews
 Code Reviewed By                    :
 Code Reviewed Date     :

 Recommendations                    :


***************************************************************/

#include "WaitDialog.h"
#include "coeaui.h"

// Constructor Function of the Class
// (just in case we need second phase construction)
void CWaitDialog::ConstructL()
{
 Counter=0;
}

// This function handles one step of the loop.
// In this case it will just increment Counter
void CWaitDialog::StepL()
{
 Counter++;
}

// This function is called 
// when IsProcessDone returns ETrue
void CWaitDialog::ProcessFinished()
{
}

// This function is in case the StepL function leaves.
// I just return the error then.
TInt CWaitDialog::CycleError(TInt aError)
{
 return aError;
}

// This function is called after the dialog is missed.
// I reset the counter in case the user presses cancel
void CWaitDialog::DialogDismissedL(TInt dismissed)
{
 //If the cancel button has been pressed.
 // Reset the counter.
 if (dismissed==-1)
   Counter=0;
}

// This function informs the system 
// when the dialog is to be dismissed 
// or when the processing is done. 
TBool CWaitDialog::IsProcessDone() const
{
 if (Counter>10000000)
   return ETrue;
 else 
   return EFalse;
}

// This function is my entry point to the class 
// and the one that starts the wait dialog.
void CWaitDialog::IssueRequest()
{
 CAknWaitNoteWrapper* dialog = CAknWaitNoteWrapper::NewL();
 dialog->ExecuteL(R_WAITNOTE_SCAN_DIALOG,*this,ETrue);
}
