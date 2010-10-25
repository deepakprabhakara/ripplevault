
#ifndef __CONTACTSHANDLER_H__
#define __CONTACTSHANDLER_H__

#include <e32std.h>
#include <e32base.h>
#include <cntdb.h>
#include <cntfield.h>
#include <cntitem.h>
#include <cntfldst.h>
#include <cntdbobs.h>

#include "EventQueue.h"

#include "constants.h"

class CRippleVaultAppUi;
class CChangesDB;
class CRecordContactDB;
class CList;
class CContactsSocketsEngineWriter;
	
enum EDBHandlerState
    {
        KStateNone,
        KDBChange,
        KDBStore,
		KDBConnected,
        KConnReq,
        KSendReq,
        KSendComplete,
        KPending
    };

/**
 * class CContactsHandler
 * 
 * All the logic around synching the contacts database to the server is 
 * implemented by this class. The service logic is implemented with 
 * a set of states. The state transition is driven by events.
 */
class CContactsHandler : public CActive, public MContactDbObserver
{
public: // Creation and destruction

    /**
     * Function:    NewL
     *
     * Description: Get instance of a CContactsBackup object.
     */
    static CContactsHandler* NewL(CRippleVaultAppUi *aAppUi);

    /**
     * Function:    ~CContactsBackup
     *
     * Description: Destroy the object.
     */
    ~CContactsHandler();

private:
	// CActive methods
	void DoCancel();

    void RunL();

	TInt RunError(TInt aError);

private:
    /**
     * Function:    ConstructL
     *
     * Description: Perform the second phase construction of a CContactsBackup
     *              object
     */
    void ConstructL();

    /**
     * Function:    CContactsBackup
     *
     * Description: Perform the first phase of two phase construction.
     */
    CContactsHandler(CRippleVaultAppUi *aAppUi);

    /**
     * Function:    CContactsBackup
     *
     * Description: Perform the first phase of two phase construction.
     */

    /**
     * Function:    CContactsBackup
     *
     * Description: Initialize DB
     */
     void Init();

	virtual void HandleDatabaseEventL(TContactDbObserverEvent aEvent);

	/**
	 * Function:	ProcessContactsChange
	 *
	 * Description: Process the change to contacts database.
	 */
	TInt ProcessContactsChange(const TContactDbObserverEvent& aEvent);

    /**
     * Function:    CreateAddedRecordContactDB
     *
     * Description: Create a record for adding to ContactsBackup
     */
public:
	/**
     * Function:    Notify
     *
     * Description: Signal self
     */
    void Notify();

	void Poll(TInt seconds, TInt aType);
	void SendAgain();
	void SendCompleteI(TInt aPacketNo);
	void SendCompleteN();
	void SendComplete();
	void MoreToUpload();
	void SendError();
	TInt IsNetworkInProgress() { return iSocketsEngine?1:0; } // if network operation going

	/** Changes database **/
	CChangesDB *iChangesDB;

	/** ContactDB triggered State transitions **/
	EDBHandlerState iContactState;
    void SyncToServer();
	void ConnectToServer();

	void ResetState();

	TInt iNumSync;
	TInt iCurrSync;
	TInt iPendingSync;

	void SetSyncParams(TInt currRecord, TInt numRecords);
	void GetSyncParams(TInt &currRecord, TInt &numRecords);
	TInt GetNumPackets(TInt numRecords);
	TInt GetNumRecords(TInt currRecord, TInt numRecords);
	
	// needed by CopyContactsAO
    CRecordContactDB* CreateAddedRecordContactDB(const CContactItem& aContact);
	CRecordContactDB* CreateUpdatedRecordContactDB(const CContactItem& aContact);
	CRecordContactDB* CreateDeletedRecordContactDB(TContactItemId contactid);
	CRecordContactDB* CreateAddUpdateRecordContactDB(const CContactItem& aContact);
	CRecordContactDB* CreateIdListRecordContactDB(CList *idList);

	CContactsSocketsEngineWriter* iSocketsEngine;

private:

	void ReplaceTag(TDes& data, TChar fromTag, TPtrC toTag);
	void ConvertXMLTags(TDes& data);
	void ReplaceTag(TDes8& data, TChar fromTag, TPtrC8 toTag);
	void ConvertXMLTags(TDes8& data);
	void GetPhonesXMLForAdd(TPtr buffer, CList *items, TPtrC type);
	void GetPhonesXMLForUpdate(TPtr buffer, CList *items, TPtrC type, HBufC* timestamp);
	void GetXMLData(TPtr8 buffer, CRecordContactDB *record);
	void CreateHeaderXML(TDesC &msgType);
	void ProcessEventQueue();

    HBufC* GetTextFieldFromFieldSet(CContactItemFieldSet& aFieldSet, const TUid index);

	/** Master instance **/
	CRippleVaultAppUi *iAppUi;

    /** ContactsChange events Queue **/
    CEventQueue iContactEvents;

	/** Buffer to send data **/
	TBuf8<MAX_BUFFER_LEN> iBuffer;

	/** Retry count **/
	TInt iRetryCount;

	TInt iChangesCount;

	TBool iSMS;
	
public:
	TInt8 iContactsRoaming;
	TInt8 iContactsUpload;
	TInt iContactsChanges;

};

#endif // __CONTACTSHANDLER_H__
