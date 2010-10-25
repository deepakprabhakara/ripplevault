

#ifndef __PHOTOSENGINE_H__
#define __PHOTOSENGINE_H__

#include <in_sock.h>
//#include "Sockets.hrh"
#ifndef __UIQ__
#include <aknquerydialog.h>
#endif
#include <f32file.h> 
#include <e32base.h> 
#include <s32file.h>
#include <eikmenup.h> 
#include <badesca.h>
#include "RippleVaultAppUi.h"
#ifdef __UIQ__
#include "TimeOutNotifier.h"
#endif

class CRippleVaultAppUi;
#ifdef __UIQ__
#define MODE_AUTO 0
#define MODE_PROMPT 1
#define MODE_OFF 2
class CPhotoTimeoutTimer;
class MUINotifier;
class CPhotosAppUi;
#endif
/*! 
  @class CSocketsEngine
  
  @discussion This class is the main engine part of the sockets application.
  It establishes a TCP connection using its server name and port number (performing a DNS lookup
  operation first, if appropriate).
  It creates instances of separate active objects to perform reading from, and writing to, the socket.
  */
#ifdef __UIQ__
class CPhotosEngine : public CActive, public MTimeOutNotifier 
#else
class CPhotosEngine : public CActive
#endif
{
public: // new methods

	RFs iRFsession;	
	TBuf<160> comment;
	TBuf<160> description;
	TTime iCurrentTimeStamp;	
#ifdef __UIQ__
	TTime TimeStamp;
	TBuf<160> imageFile;
        int PopUp;
#endif
	int Count;		
	TInt8 iNewFolder;
	TInt iCountflag;

	CRippleVaultAppUi& iAppUi;

	static CPhotosEngine* NewL(CRippleVaultAppUi& aAppUi, TDesC& aFolderName);

        static CPhotosEngine* NewLC(CRippleVaultAppUi& aAppUi, TDesC& aFolderName);
#ifdef __UIQ__
void GetTimeStamp();
void TimerExpired();
#endif
	void GetFolder();

	void ProcessImage();

	~CPhotosEngine();

    void Notify();

	enum TSocketsEngineState 
	    {
		ESmartCharge,
	    EOpenMenu,	 
		ECheckFile,
		EDescription,
        EConnecting,        
		EWritten,		
		EConnectFolder,
		EProcessFile
	};

	void ChangeStatus(TSocketsEngineState aEngineStatus);
	void GetImageCount();
	TInt iImageCount;
	
protected: // from CActive
	void DoCancel();

    void RunL();

private: // New methods
	CPhotosEngine(CRippleVaultAppUi& aAppUi);

    void ConstructL(TDesC& aFolderName);

private: // Member variables
#ifdef __UIQ__
CPhotoTimeoutTimer*              iTimer;
static const TInt KTimeOut;  
#endif
    TSocketsEngineState iEngineStatus;
	
	TBool iPublic_Photo;
	TInt icompareflag;	

};

#endif // __PHOTOSENGINE_H__

