/*
* ============================================================================
*  Name     : AknExSettingListItemData.h
*  Part of  : 
*  Copyright (c) 2000-2005 PurpleACE. All rights reserved.
* ============================================================================
*/

#ifndef AKNEXSETTINGLISTITEMDATA_H
#define AKNEXSETTINGLISTITEMDATA_H

// INCLUDES
#include <e32base.h>
#include <in_sock.h>
/**
* CAknExSettingListItemData 
*/
class CAknExSettingListItemData : public CBase
    {
    public:
        static CAknExSettingListItemData* NewL();
        ~CAknExSettingListItemData();

    private:
        CAknExSettingListItemData();
        void ConstructL();
    	void DefaultAccessPoint();

    public:
        TBuf<80> iIpAddress;		
		TInt  iPort;
//		TInt  iMode;
		TBuf<30> iApn;
		TInt  iAuto;
		TBuf<20>  iPw;
//		TInt  iRoam;
//		TInt  iDesc;
//contacts
		TInt iContactsRoam;
		TInt iContactsUpload;
//photos
		TInt  iMode;
		TInt  iPublishRoam;
		TInt  iDesc;
		TInt  iSecureGallery;

//sms
		TInt iSmsType;
		TTime iTime;
		TTime iOldTime;
		TInt  iSmsRoam;
		
//secure
		TInt  iAutoSecure;
		TBuf<20> iSecureFriendNumber;
		TBuf<200> iSecureText;
		TInt  iAlarmSecure;

        friend class CAknExSettingListView;
		friend class CAknExSettingListListbox;
    };


#endif

// End of File
