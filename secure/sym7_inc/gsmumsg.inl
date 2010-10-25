// GSMUMSG.INL
//
// Copyright (c) 1999 Symbian Ltd.  All rights reserved.
//



inline CSmsPDU::TSmsPDUType CSmsMessage::Type() const
/**
 *
 * @return The SMS-XXX message type
 */
	{
	return iSmsPDU->Type();
	}

inline TBool CSmsMessage::IsComplete() const
/**
 *
 * @return True if the SMS Message is complete
 */
	{
	return iFlags&ESmsFlagIsComplete;
	}

inline TBool CSmsMessage::IsDecoded() const
/**
 *
 * @return True if the SMS Message is Decoded
 */
	{
	return iFlags&ESmsFlagIsDecoded;
	}

inline CSmsMessage::TMobileSmsStorage CSmsMessage::Storage() const
/**
 *
 * @return The physical location where the Message represented by CSmsMessage is stored
 */
	{
	return (TMobileSmsStorage) (iFlags&ESmsStorageMask);
	}

inline void CSmsMessage::SetStorage(TMobileSmsStorage aStorage)
/**
 *
 * @param aStorage Set to the physical location where the Message represented by CSmsMessage is stored
 */
	{
	iFlags=aStorage|(iFlags&(~ESmsStorageMask));
	}

inline RMobileSmsStore::TMobileSmsStoreStatus CSmsMessage::Status() const
/**
 *
 * @return The TMobileSmsStoreStatus of the Message represented by CSmsMessage
 */
	{
	return iStatus;
	}

inline void CSmsMessage::SetStatus(RMobileSmsStore::TMobileSmsStoreStatus aStatus)
/**
 *
 * @param Set to the TMobileSmsStoreStatus of the Message represented by CSmsMessage
 */
	{
	iStatus=aStatus;
	}

inline TInt CSmsMessage::LogServerId() const
/**
 *
 * @return Log Server Id
 */
	{
	return iLogServerId;
	}

inline void CSmsMessage::SetLogServerId(TInt aId)
/**
 *
 * @param aId To set Log Server Id
 */
	{
	iLogServerId=aId;
	}

inline const TTime& CSmsMessage::Time() const
/**
 *
 * @return Time of Message creation
 */
	{
	return iTime;
	}

inline void CSmsMessage::SetTime(const TTime& aTime)
/**
 *
 * @param aTime To set time of Message creation
 */
	{
	iTime=aTime;
	}

inline CSmsPDU& CSmsMessage::SmsPDU()
/**
 *
 * @return The class CSmsPDU 
 */
	{
	return *iSmsPDU;
	}

inline const CSmsPDU& CSmsMessage::SmsPDU() const
/**
 *
 * @return The class CSmsPDU (const) 
 */
	{
	return *iSmsPDU;
	}

inline TPtrC CSmsMessage::ServiceCenterAddress() const
/**
 *
 * @return The class Service Center Address
 */
	{
	return iSmsPDU->ServiceCenterAddress();
	}

inline void CSmsMessage::SetServiceCenterAddressL(const TDesC& aAddress)
/**
 *
 * @param aAddress To set the class Service Center Address
 */
	{
	iSmsPDU->SetServiceCenterAddressL(aAddress);
	}

/*
inline void CSmsMessage::ParsedServiceCenterAddress(TGsmSmsTelNumber& aParsedAddress) const
	{
	iSmsPDU->ParsedServiceCenterAddress(aParsedAddress);
	}

inline void CSmsMessage::SetParsedServiceCenterAddressL(const TGsmSmsTelNumber& aParsedAddress)
	{
	iSmsPDU->SetParsedServiceCenterAddressL(aParsedAddress);
	}
*/

inline TPtrC CSmsMessage::ToFromAddress() const
/**
 *
 * @return Un-parsed To and From Addresses
 */
	{
	return iSmsPDU->ToFromAddress();
	}

inline void CSmsMessage::SetToFromAddressL(const TDesC& aAddress)
/**
 *
 * @param aAddress To set un-parsed To and From Addresses
 */
	{
	iSmsPDU->SetToFromAddressL(aAddress);
	}

/*
inline void CSmsMessage::ParsedToFromAddress(TGsmSmsTelNumber& aParsedAddress) const
	{
	iSmsPDU->ParsedToFromAddress(aParsedAddress);
	}

inline void CSmsMessage::SetParsedToFromAddressL(const TGsmSmsTelNumber& aParsedAddress)
	{
	iSmsPDU->SetParsedToFromAddressL(aParsedAddress);
	}
*/

inline CSmsBufferBase& CSmsMessage::Buffer()
/**
 *
 * @return The text portion of the CSmsMessage
 */
	{
	return *iBuffer;
	}

inline const CSmsBufferBase& CSmsMessage::Buffer() const
/**
 *
 * @return The text portion of the CSmsMessage (const)
 */
	{
	return *iBuffer;
	}

inline void CSmsMessage::SetIsComplete(TBool aIsComplete)
/**
 *
 * @param aIsComplete Set to True for CSmsMessage complete
 */
	{
	iFlags=aIsComplete? iFlags|ESmsFlagIsComplete: iFlags&(~ESmsFlagIsComplete);
	}

inline void CSmsMessage::SetIsDecoded(TBool aIsDecoded)
/**
 *
 * @param aIsDecoded Set to True for CSmsMessage decoded
 */
	{
	iFlags=aIsDecoded? iFlags|ESmsFlagIsDecoded: iFlags&(~ESmsFlagIsDecoded);
	}

inline TBool CSmsMessage::BinaryData() const
/**
 *
 * @return True if the User Data is Binary
 */
	{
	return iSmsPDU->UserData().IsBinaryData();
	}

inline void CSmsMessage::Set16BitConcatenation(TBool aIs16BitConcatenation)
	{
	iIs16BitConcatenation=aIs16BitConcatenation;
	}

inline TBool CSmsMessage::Is16BitConcatenation()const 
	{
	return iIs16BitConcatenation;
	}
