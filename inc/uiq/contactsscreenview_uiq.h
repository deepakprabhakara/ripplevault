

#ifndef __CONTACTSSCREENLISTVIEW_H
#define __CONTACTSSCREENLISTVIEW_H

#include <eikdialg.h> // MCoeControlContext
#include <eikclb.h>   // CEikColumnListBox
#include <e32base.h>

#include "RippleVaultAppUi.h"

class CRippleVaultAppUi;
class CReadContactsAO;

class CContactsScreenView : public CCoeControl,
                        public MCoeControlContext
{
public:

  static CContactsScreenView* NewL(const TRect& aRect,CRippleVaultAppUi& aAppUi );

  ~CContactsScreenView();

  CRippleVaultAppUi& iAppUi;

  CContactsScreenView(CRippleVaultAppUi& aAppUi);

  void ConstructL(const TRect& aRect);

  TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType);

  void HandlePointerEventL(const TPointerEvent& aPointerEvent);

  void HandleCommandL(TInt aCommand);

  TInt SendSharingXml();

  TInt GetStoredContactsCount();

  void CloseReadContact();

  TInt iContactCount;

  TInt iCount;

  CDesCArray* array;

  CArrayFix<TInt>* iContactsIdArray;

  void DeleteContacts();
  
  void ResetData();

  void AddContacts();

  TInt AddNewDatatoDB();

private:

  void Draw(const TRect& /* aRect */) const;

  TInt CountComponentControls() const;

  CCoeControl* ComponentControl(TInt aIndex) const;

  void CreateList();

  TInt GetContactNamesForSharingAlbum(TInt id, TDesC& name, TDesC& mobile);

  void GetTextFieldFromFieldSet(CContactItemFieldSet& aFieldSet, const TUid aField,TDesC& name);

private:
  HBufC* iTextListEmpty;
  CEikColumnListBox* iListBox;
  CContactDatabase* iContactsDB;
  
  CReadContactsAO	*iReadContactsAO;
};

#endif
