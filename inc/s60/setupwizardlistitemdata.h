/*
* ============================================================================
*  Name     : SetupWizardListItemData.h
*  Part of  : 
*  Copyright (c) 2000-2005 PurpleACE. All rights reserved.
* ============================================================================
*/

#ifndef SETUPWIZARDLISTITEMDATA_H
#define SETUPWIZARDLISTITEMDATA_H

// INCLUDES
#include <e32base.h>
#include <in_sock.h>
/**
* CSetupWizardListItemData 
*/
class CSetupWizardListItemData : public CBase
    {
    public:
        static CSetupWizardListItemData* NewL();
        ~CSetupWizardListItemData();

    private:
        CSetupWizardListItemData();
        void ConstructL();
    	void DefaultAccessPoint();

    public:
        TBuf<80> iIpAddress;		
		TInt  iPort;
//		TInt  iMode;
		TBuf<30> iApn;
		TInt  iAuto;
//		TInt  iRoam;
//		TInt  iDesc;
//contacts
		TInt iContactsRoam;
//photos
		TInt  iMode;
		TInt  iPublishRoam;
		TInt  iDesc;
		TInt iSecureGallery;

//sms
		TInt iSmsType;
		TTime iTime;
		TTime iOldTime;
		TInt  iSmsRoam;

        friend class CSetupWizardListView;
		friend class CSetupWizardListListbox;
    };


#endif

// End of File
