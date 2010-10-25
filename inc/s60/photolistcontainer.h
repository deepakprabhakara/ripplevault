/*
* ============================================================================
*  Name     : CPhotoListContainer from PhotoListContainer.h
*  Part of  : To Show Thumbnails Photo List
*  Created  : 17/06/2004 by Ezhil
*  Description:
*     Declares container control for application.
*  Version  :
*  Copyright: PurpleACE
* ============================================================================
*/

#ifndef PHOTOLISTCONTAINER_H
#define PHOTOLISTCONTAINER_H

// INCLUDES
#include <f32file.h> 
#include <e32base.h> 
#include <s32file.h>
#include <coecntrl.h>
#include <aknlists.h>
#include <AknWaitDialog.h>
#include <akntabgrp.h>
#include <aknnavide.h>

// CLASS DECLARATION
class CRippleVaultAppUi;


/**
*  CPhotoListContainer  container control class.
*  
*/
class CPhotoListContainer : public CCoeControl, MCoeControlObserver
    {
    public: // Constructors and destructor
        /**
        * EPOC default constructor.
        * @param aRect Frame rectangle for container.
        */

        void ConstructL(const TRect& aRect);

		CRippleVaultAppUi* iPhotosAppUi;
		
        /**
        * Destructor.
        */
        ~CPhotoListContainer();
    public: // New functions
		void SetFileList(TInt aIsmark);
		void DrawStausPane();
		void ShowListBox();
		void AppendThumnails();
		TInt IsImageSelected();
		void MarkSelectedPhoto();
		void UnMarkSelectedPhoto();
		void MarkUnMarkSelectedItem();
		void MarkAllItems();
		void UnMarkAllItems();
		void SetIconsArray();
		void GetPhoneImage();
		void GetMmcImage();
		void MarkSelectedItem();

    public: // Functions from base classes
		TInt iPhoneCount;
		TInt iMmcCount;
		TInt iFlag;
		CArrayFix<TInt>* iSelectedPhonePhotos;
		CArrayFix<TInt>* iSelectedMmcPhotos;

    private: // Functions from base classes

       /**
        * From CoeControl,SizeChanged.
        */
        void SizeChanged();

       /**
        * From CoeControl,CountComponentControls.
        */
        TInt CountComponentControls() const;

       /**
        * From CCoeControl,ComponentControl.
        */
        CCoeControl* ComponentControl(TInt aIndex) const;

       /**
        * From CCoeControl,Draw.
        */
        void Draw(const TRect& aRect) const;

       /**
        * From ?base_class ?member_description
        */
        
        void HandleControlEventL(CCoeControl* aControl,TCoeEvent aEventType);

        TKeyResponse OfferKeyEventL( const TKeyEvent& aKeyEvent,TEventCode aType);

		TInt CheckMediaExists(TDesC& aFilename);

    private: //data

		CAknNavigationControlContainer * iNaviPane ;
		CAknNavigationDecorator * iNaviDeco ;
		CAknTabGroup * iTabGroup ;

		CAknDoubleLargeStyleListBox* iListBox;

		RFs iRFsession;	
		CDir *dirList;
		CDesCArray* items;

		CArrayPtr<CGulIcon>* iPhoneIconList;
		CArrayPtr<CGulIcon>* iMmcIconList;
		TInt iCursorPos;

		TBool iIsMark;
		CDesCArray *ImageDataPhonearray;
		CDesCArray *ImageDatePhonearray;
		CDesCArray *ImageDataMmcarray;
		CDesCArray *ImageDateMmcarray;
		
		CDesCArray *ImageDataPhoneFilearray;
		CDesCArray *ImageDataMmcFilearray;

		CAknWaitDialog* iWaitDialog;
		CFbsBitmap* iBitmap;
    };

#endif

// End of File

