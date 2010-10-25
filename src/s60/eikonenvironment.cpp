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
 Module Name				: EikonEnvironment.cpp
 Language used : C++
 Coding Standard			: Symbian Coding Standards
 Author                     : Ezhil,Deepak
 Date Created               : 14-03-2005
 Rational Rose Model Name                                :
 Use Case Document Name                                :
 Use Case Realization Document Name   :
 Test Case Document Name       :
 Abbreviations                :

 Description                   :  
				Helper for RipplevaultAppUi.cpp

 Code Reviews
 Code Reviewed By                    :
 Code Reviewed Date     :

 Recommendations                    :


***************************************************************/

#include <eikappui.h>
#include <eikenv.h>
#include <eikapp.h>
#include <s32file.h>
#include <aknnotewrappers.h>

#include "EikonEnvironment.h"


TFileName NEikonEnvironment::AppFullName()
	{
	return EikAppUi().Application()->AppFullName();
	}


TFileName NEikonEnvironment::ApplicationDriveAndPath()
	{
	TParse parse;
//On WINS the application is on the z drive
#ifdef __WINS__
	TFileName appfullname = AppFullName(); 
	parse.Set(_L("c:"), &appfullname, NULL);
#else
	parse.Set(AppFullName(), NULL, NULL);
#endif
	return parse.DriveAndPath();
	}


TFileName NEikonEnvironment::AddPath(const TDesC& aFileName)
	{
#ifdef _DEBUG //check that we have not been sent a file name with the drive included
	TParse theFile;
	theFile.Set(aFileName, NULL, NULL);
	ASSERT(theFile.Drive().Length() == 0);
#endif

	TParse thePath;
	thePath.Set(ApplicationDriveAndPath(),  &aFileName,  NULL);
	return thePath.FullName();

	}


CEikonEnv& NEikonEnvironment::EikEnv()
	{
	return *CEikonEnv::Static();
	}


CEikAppUi& NEikonEnvironment::EikAppUi()
	{
	return *(EikEnv().EikAppUi());
	}


CEikDocument& NEikonEnvironment::EikDocument()
	{
	return *(EikAppUi().Document());
	}


void NEikonEnvironment::AddToStackL(CCoeControl& aControl, TInt aPriority, TInt aStackingFlags)
	{
	RemoveFromStack(aControl);
	EikAppUi().AddToStackL(&aControl, aPriority, aStackingFlags);
	}


void NEikonEnvironment::AddToStackL(CCoeControl& aControl)
	{
	RemoveFromStack(aControl);
	EikAppUi().AddToStackL(&aControl);
	}


void NEikonEnvironment::RemoveFromStack(CCoeControl& aControl)
	{
	EikAppUi().RemoveFromStack(&aControl);
	}


void NEikonEnvironment::MessageBox(const TDesC&  aMessage)
	{
	TRAPD(err, 
		  CAknInformationNote* informationNote = new (ELeave) CAknInformationNote;
          informationNote->ExecuteLD(aMessage); );
	if (err != KErrNone)
		{
		User::Panic (_L("Helper"), 1);
		}
	}


void NEikonEnvironment::FlushWindowServer()
	{
	CCoeEnv::Static()->WsSession().Flush();
	}


RFs& NEikonEnvironment::TheRFs()
	{
	return CCoeEnv::Static()->FsSession();
	}

