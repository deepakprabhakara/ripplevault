

#ifndef __RIPPLEVAULT_PAN__
#define __RIPPLEVAULT_PAN__

/** Camera application panic codes */
enum TCameraPanics 
    {
    ECameraBasicUi = 1
    // add further panics here
    };

_LIT(KPanicSockets, "Sockets");
_LIT(KPanicSocketsEngine, "SocketsEngine");
_LIT(KPanicSocketsEngineWrite, "SocketsEngineWrite");
_LIT(KPanicSocketsEngineRead, "SocketsEngineRead");

/** Sockets application panic codes */
enum TSocketsPanics 
    {
    ESocketsBasicUi = 1,
    ESocketsBadStatus,
    ESocketsAppView,
    ESocketsBadState
    };

#endif // __RIPPLEVAULT_PAN__
