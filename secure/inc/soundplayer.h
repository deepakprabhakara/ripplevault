/*
* ============================================================================
*  Name     : CSoundPlayer from SoundPlayer.h
*  Part of  : Sound1
*  Created  : 03/03/2003 by Eric
*  Implementation notes:
*
*  Version  :
*  Copyright: (c) 2003 - NewLC (http://www.newlc.com)
* ============================================================================
*/
#include <MdaAudioSamplePlayer.h>

class CSoundPlayer: public CBase, public MMdaAudioPlayerCallback
{
public:
    static CSoundPlayer* NewL(const TDesC& aFile);
    static CSoundPlayer* NewLC(const TDesC& aFile);
    ~CSoundPlayer();

    void Play();
    void Stop();

    //
    // from MMdaAudioPlayerCallback
    //
    void MapcInitComplete(TInt aError, const TTimeIntervalMicroSeconds& aDuration);
    void MapcPlayComplete(TInt aError);


private:
    CSoundPlayer();
    void ConstructL(const TDesC& aFile);

private:
    enum TState
    {
        ENotReady,
        EReady,
        EPlaying
    };
    
    TState iState;
    CMdaAudioPlayerUtility* iMdaPlayer;
};


