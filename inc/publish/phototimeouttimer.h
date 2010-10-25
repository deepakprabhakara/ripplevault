

#ifndef __PHOTOTIMEOUTTIMER_H__
#define __PHOTOTIMEOUTTIMER_H__

#include <e32base.h>

class MTimeOutNotifier;

/*! 
  @class CPhotoTimeoutTimer
  
  @discussion This class will notify an object after a specified timeout.
  */
class CPhotoTimeoutTimer : public CTimer
    {
public:
/*!
  @function NewL
   
  @discussion Create a CPhotoTimeoutTimer object
  @param aPriority priority to use for this timer
  @param aTimeOutNotify object to notify of timeout event
  @result a pointer to the created instance of CPhotoTimeoutTimer
  */
	static CPhotoTimeoutTimer* NewL(const TInt aPriority, MTimeOutNotifier& aTimeOutNotify);

/*!
  @function NewL
   
  @discussion Create a CPhotoTimeoutTimer object
  @param aPriority priority to use for this timer
  @param aTimeOutNotify object to notify of timeout event
  @result a pointer to the created instance of CPhotoTimeoutTimer
  */
    static CPhotoTimeoutTimer* NewLC(const TInt aPriority, MTimeOutNotifier& aTimeOutNotify);

/*!
  @function ~CPhotoTimeoutTimer
  
  @discussion Destroy the object and release all memory objects
  */
	~CPhotoTimeoutTimer();

protected: // From CTimer
/*!
  @function RunL

  @discussion Invoked when a timeout occurs
  */
    virtual void RunL();

private:
/*!
  @function CPhotoTimeoutTimer
  
  @discussion Perform the first phase of two phase construction 
  @param aPriority priority to use for this timer
  @param aTimeOutNotify An observer to notify
  */
	CPhotoTimeoutTimer(const TInt aPriority, MTimeOutNotifier& aTimeOutNotify);

/*!
  @function ConstructL
  
  @discussion Perform the second phase construction of a CPhotoTimeoutTimer 
  */
    void ConstructL();

private: // Member variables

    /*! @var The observer for this objects events */
	MTimeOutNotifier& iNotify;
    };

#endif // __phototimeouttimer_H__

