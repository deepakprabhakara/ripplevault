/*
* ============================================================================
*  Name     : CSoundPlayer from SoundPlayer.cpp
*  Part of  : Sound1
*  Created  : 03/03/2003 by Eric
*  Implementation notes:
*
*  Version  :
*  Copyright: (c) 2003 - NewLC (http://www.newlc.com)
* ============================================================================
*/

#include "SoundPlayer.h"

CSoundPlayer* CSoundPlayer::NewL(const TDesC& aFile)
{
    CSoundPlayer* self = NewLC(aFile);
    CleanupStack::Pop(self);
    return self;
}

    
CSoundPlayer* CSoundPlayer::NewLC(const TDesC& aFile)
{
    CSoundPlayer* self = new (ELeave) CSoundPlayer();
    CleanupStack::PushL(self);
    self->ConstructL(aFile);
    return self;
}

CSoundPlayer::CSoundPlayer()
{
}


CSoundPlayer::~CSoundPlayer()
{
    delete iMdaPlayer;    
    iMdaPlayer = NULL;
}


void CSoundPlayer::ConstructL(const TDesC& aFile)
{
  //
  // Create a file audio player utility instance 
  //
  iMdaPlayer=CMdaAudioPlayerUtility::NewFilePlayerL(aFile,*this);  
}



void CSoundPlayer::Play()
{
    if(iState==EReady)
    {
        iState=EPlaying;
        iMdaPlayer->SetVolume(iMdaPlayer->MaxVolume());
        //iMdaPlayer->SetVolume(10);
        iMdaPlayer->Play();
    }
}


void CSoundPlayer::Stop()
{
    if(iState==EPlaying)
    {
        iMdaPlayer->Stop();
        iState = EReady;
    }
}


// from MMdaAudioPlayerCallback
void CSoundPlayer::MapcInitComplete(TInt aError, const TTimeIntervalMicroSeconds& /*aDuration*/)
{
    iState = aError ? ENotReady : EReady;
}


void CSoundPlayer::MapcPlayComplete(TInt aError)
{
    iState = aError ? ENotReady : EReady;
}