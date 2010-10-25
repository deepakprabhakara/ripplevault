// --------------------------------------------------------------------------
// MyListBoxAppView.h
//
//
// A view class for VideoScreen list box example.
// --------------------------------------------------------------------------

#ifndef __SMSSCREENVIEW_UIQ_H
#define __SMSSCREENVIEW_UIQ_H

// INCLUDES
#include <coecntrl.h>
#include <coeview.h>
#include <eiklbo.h>  // MEikListBoxObserver
#include "ripplevaultappui.h"

#include <badesca.h> 
#include <msvstd.h> 
#include <mtclbase.h> 
#include <msvids.h> 
#include <smut.h> 
#include <smuthdr.h> 
#include <mtclreg.h> 
#include <txtrich.h> 
#include <smsclnt.h>
#include <gsmupdu.h>
#include <msvapi.h> 

class CRippleVaultAppUi;  
class CMsvSession;
class MMsvSessionObserver; 

// FORWARD DECLARATIONS
class CMediaScreenListBox;
class CReadSmsAO;

// CLASS DECLARATION
/**
 *  CMyListBoxAppView container control class.
 */
class CSmsScreenView: public CCoeControl, public MEikListBoxObserver, MMsvSessionObserver
	{
public: // constructors and destructor
	
	static CSmsScreenView* NewL(const TRect& aRect,CRippleVaultAppUi& aAppUi );
	CSmsScreenView(CRippleVaultAppUi& aAppUi );		
	~CSmsScreenView();
	void ConstructL(const TRect& aRect);

public:
	CMediaScreenListBox* ListBox()
	{ return iListBox; }

	void UpdateListBoxL(TInt aIsDoubleLine);

	void SetFileList(TInt aIsmark);

	void MarkUnMarkSelectedSms();
	void MarkUnMarkSelectedItem();
	void MarkAllItems();
	void UnMarkAllItems();
	void MarkUnMarAllSms(TInt aIsMark);
	TInt IsSmsSelected();
	void ChangeScreen();
	void CloseReadSms();
	
	void DrawText();

	void SendSmsToServer();

	void GetSms(TBool aVal);
	
	void GetTopIndex();
	
	TInt Start();
	
	void ResetData();

	void SetIconsArray();

	void ResetIcons();

	//void WriteSmsToFile(TInt aId,TInt aSmsState);

	CRippleVaultAppUi& iAppUi;
	
	TInt GetCount();
	void MarkSelectedSms();
	
	void HandleSessionEventL(TMsvSessionEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3);

private: // from CCoeControl
	void SizeChanged();
	TInt CountComponentControls() const;
	CCoeControl* ComponentControl(TInt aIndex) const;
	void Draw(const TRect& aRect) const;
	virtual TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent,
		TEventCode aType);

private: // from MEikListBoxObserver
	virtual void HandleListBoxEventL(CEikListBox* aListBox,
		TListBoxEvent aEventType);

private: // New functions
	void CreateListBoxL();
        
private: //data
	CMediaScreenListBox* iListBox;

	CArrayPtr<CGulIcon>* iIconList;

	RFs fs;
	RFile iFile;
	TInt iCursorPos;
	TBool iIsMark;
	
	TInt8 iNewFolder;
	TBuf<120> iFileName;

	TInt iTempCount;

	CReadSmsAO	*iReadSmsAO;
	
public:	
	TInt iCount1;
	TInt iCount2;
	TInt iInboxCount;
	TInt iFlag;
	TInt iSentItemCount;
	
	CDesCArray *InboxSmsDetailsarray;
	CDesCArray *InboxSmsMessagearray;
	CDesCArray *InboxSmsDatearray;

	CDesCArray *SentSmsDetailsarray;
	CDesCArray *SentSmsMessagearray;
	CDesCArray *SentSmsDatearray;

	CArrayFix<TInt>* iInboxSelectedSms;
	CArrayFix<TInt>* iSentItemSelectedSms;
	};

#endif // __SMSSCREENVIEW_UIQ_H

// End of File
