/*
* ============================================================================
*  Name     : CFindBoxContainer from FindBoxContainer.h
*  Part of  : FindBox
*  Created  : 6/15/2004 by Ezhil
*  Description:
*     Declares container control for application.
*  Version  :
*  Copyright: PurpleAce
* ============================================================================
*/

#ifndef FINDBOXCONTAINER_H
#define FINDBOXCONTAINER_H

// INCLUDES
#include <coecntrl.h>
#include <aknlists.h> 
#include <aknsfld.h>  
#include <aknutils.h>
#include <cntitem.h>
#include <cntfldst.h>
#include <akniconarray.h> 
#include <eiktxlbx.h>
#include <eikfrlb.h> 
#include <eikclb.h>
#include <eikclbd.h>

#define MAILID 0
#define MOBILENO 1

// CLASS DECLARATION
class CRippleVaultAppUi;
class CReadContactsAO;

/**
*  CFindBoxContainer  container control class.
*  
*/
class CFindBoxContainer : public CCoeControl, MCoeControlObserver
    {
    public: // Constructors and destructor
        
        /**
        * EPOC default constructor.
        * @param aRect Frame rectangle for container.
        */
        void ConstructL();

		CRippleVaultAppUi* iPhotosAppUi;	

		CAknSearchField* CreateFindBoxL(CEikListBox* aListBox,
            CTextListBoxModel* aModel,
				CAknSearchField::TSearchFieldStyle aStyle );
        /**
        * Destructor.
        */
        ~CFindBoxContainer();

    public: // New functions		

		void GetNames(TInt aCursorPos, TInt aIdValue);
		
		void SaveContacts();
		TInt OkPressKey();
		
		TInt ShowContactsDialog(TDesC& name);

		TInt iContactCount;		
		
		TBuf<50> iMobileNumber; 
		CDesCArray* items;
		CDesCArray* numbers;

    public: // Functions from base classes

		TInt GetSelectedCount();

		void SetFileList(TInt val);

		void CloseReadContacts();

    private: // Functions from base classes

       /**
        * From CoeControl,SizeChanged.
        */
        void SizeChanged();

		void SizeChangedForFindBox();

		void SetGraphicIconL( CEikColumnListBox* aListBox );

		void GraphicIconL( CArrayPtr<CGulIcon>* aIcons );		

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
        // event handling section
        // e.g Listbox events
        void HandleControlEventL(CCoeControl* aControl,TCoeEvent aEventType);
		
		TKeyResponse CFindBoxContainer::OfferKeyEventL(
	    const TKeyEvent& aKeyEvent,TEventCode aType);
        
		void SetIconAndHeadings();
		TInt GetContactsCount();
		void CreateListBox();

		void MarkUnMarkSelectedItem();
		void MarkSelectedContact();

		void GetMobileNumbers(TInt aCursorPos);
        
    private: //data		
		CEikColumnListBox* iListBox;
		CAknSearchField* iFindBox;
		
		CAknListBoxFilterItems* iIndex; 
		CReadContactsAO	*iReadContactsAO;

		TInt iCursorPos;
	public:
		CArrayFix<TInt>* iSelectedArray;

		CDesCArray *iContactNames;
		CDesCArray *iMobiles;
    };

#endif

// End of File
