
#ifndef WAITDIALOG_H
#define WAITDIALOG_H


#include <aknwaitdialog.h> 
#include <aknwaitnotewrapper.h> 
//#include "WaitDialog.h"
#include "RippleVault.rsg"

class CWaitDialog: MAknBackgroundProcess  
{
public: //Public Functions
        void IssueRequest();
        void ConstructL();

private: //Private Functions
        void DialogDismissedL(TInt dismissed);
        TInt CycleError(TInt aError);
        void ProcessFinished();
        TBool IsProcessDone() const;
        void StepL();

private://Private Variables
        TInt Counter;
};

#endif