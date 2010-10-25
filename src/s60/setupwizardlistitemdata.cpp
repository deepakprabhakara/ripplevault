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
 Module Name				: SetupWizardListItemData.cpp
 Language used : C++
 Coding Standard			: Symbian Coding Standards
 Author                     : Ezhil,Deepak
 Date Created               : 10-05-2005
 Rational Rose Model Name                                :
 Use Case Document Name                                :
 Use Case Realization Document Name   :
 Test Case Document Name       :
 Abbreviations                :

 Description                   :  Settings Screen used during Setup Wizard
 

 Code Reviews
 Code Reviewed By                    :
 Code Reviewed Date     :

 Recommendations                    :


***************************************************************/

// INCLUDE FILES
#include <eikenv.h>
#include <in_sock.h>
#include "SetupWizardlistitemdata.h"
#include "SetupWizardlistview.h"

#include <AgentClient.h>
#include <cdbcols.h>
#include <commdb.h>

// ================= MEMBER FUNCTIONS =========================================

// ----------------------------------------------------------------------------
// CSetupWizardListListbox::NewL
// Two-phase constructor
// ----------------------------------------------------------------------------
//
CSetupWizardListItemData* CSetupWizardListItemData::NewL()
    {
    CSetupWizardListItemData* data = new(ELeave) CSetupWizardListItemData;
    CleanupStack::PushL(data);
    data->ConstructL();
    CleanupStack::Pop(); // data
    return data;
    }

// Destructor
//
CSetupWizardListItemData::~CSetupWizardListItemData()
    {
    }

// Constructor
//
CSetupWizardListItemData::CSetupWizardListItemData() 
    {
    }

// Epoc default constructor
//
void CSetupWizardListItemData::ConstructL()
{
	DefaultAccessPoint();
}

void CSetupWizardListItemData::DefaultAccessPoint()
{
	RGenericAgent netAgent1;
	if (netAgent1.Open() == KErrNone)
	{			
		TBuf<50> Name;		
		TUint32 ida=0;
		TUint32 ida1;
		TBuf<20> TableName;
		TableName.Copy(_L("IAP"));

		TBuf<20> ColName1;
		ColName1.Copy(_L("Id"));

		TBuf<20> ColName2;
		ColName2.Copy(_L("Name"));

		CleanupClosePushL(netAgent1);
		CCommsDatabase* db=CCommsDatabase::NewL(EDatabaseTypeIAP);
		CleanupStack::PushL(db);			
		
		CCommsDbConnectionPrefTableView* 
		view1 = db->OpenConnectionPrefTableViewOnRankLC(ECommDbConnectionDirectionOutgoing,1);
		CCommsDbConnectionPrefTableView::TCommDbIapConnectionPref pref;
		if(view1->GotoFirstRecord()!=KErrNotFound)
		{
			view1->ReadConnectionPreferenceL(pref);
			ida=pref.iBearer.iIapId;			
		}
		
		CCommsDbTableView* 
		view = db->OpenTableLC(TableName);

		if(view->GotoFirstRecord()!=KErrNotFound)
		{
			do
			{
				view->ReadUintL(ColName1, ida1);
				if(ida==ida1)
				{
					view->ReadTextL(ColName2, Name);
					iApn.Copy(Name);
				}
			}
			while(view->GotoNextRecord()!=KErrNotFound);
		}
		CleanupStack::PopAndDestroy(4); // view, db, netAgent			
	}
}

