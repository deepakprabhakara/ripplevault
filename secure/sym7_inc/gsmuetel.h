// GSMUETEL.H
//
// Copyright (c) 2001 Symbian Ltd.  All rights reserved.
//


/**
 *    
 * @file  gsmuetel.h This file contains the header file of the etel functions that have to be adapted later
 *
 */

#ifndef __GSMUETEL_H__
#define __GSMUETEL_H__

#include <e32base.h>
#include <etelmm.h>
#include "gsmuelem.h"

class TGsmSms
/**
 * Basic SMS class that contains some of the etelmm SMS types
 */
	{
public:
	inline TGsmSms();
	inline const TDesC8& Pdu() const;
	inline void SetPdu(const TDesC8& aPdu);
	//inline const TGsmSmsTelNumber& Sca() const;
	//inline void SetSca(const TGsmSmsTelNumber& aSca);

private:
	RMobileSmsMessaging::TMobileSmsGsmTpdu iPdu;	///< Storage for the PDU
	//TGsmSmsTelNumber iSca;							///< Storage for the SCA
	};

	enum TSmsReportResponsibility
		/**
		 * Responsibility for submit/delivery reports
		 */
		{
		ESmsReportME,
		ESmsReportClient
		};
	struct TSmsReportSetting
		/**
		 * Flags for report resposibility
		 */
		{
		TSmsReportResponsibility iDeliverReports;
		TSmsReportResponsibility iSubmitReports;
		};

#include "gsmuetel.inl"

#endif // !defined __GSMUETEL_H__



