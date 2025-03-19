#pragma once

#include <vector>
#include <string>
#include "Sector.h"

class InterchangeFile
{
	private:
		std::string fileIdentifier;
		short blockLength;
		char recordAttribute;
		CylinderHeadSector beginningOfExtent;
		char physicalRecordLength;
		CylinderHeadSector endOfExtent;
		bool fixedLengthRecord;
		bool bypassIndicator;
		char datasetSecurity;
		bool writeProtect;
		char exchangeTypeIndicator;
		char multiVolumeIndicator;
		std::string volumeSequence;
		std::string creationDate;
		int recordLength;
		std::string offsetToNextRecordSpace;
		std::string expirationDate;
		char verifyCopyIndicator;
		char datasetOrganization;
		CylinderHeadSector endOfData;
		std::vector<Sector> sectors;
	public:
		InterchangeFile(std::string fileIdentifier, short blockLength, char recordAttribute, CylinderHeadSector beginningOfExtent, char physicalRecordLength, CylinderHeadSector endOfExtent, bool fixedLengthRecord, bool bypassIndicator, char datasetSecurity, bool writeProtect, char exchangeTypeIndicator, char multiVolumeIndicator, std::string volumeSequence, std::string creationDate, int recordLength, std::string offsetToNextRecordSpace, std::string expirationDate, char verifyCopyIndicator, char datasetOrganization, CylinderHeadSector endOfData);
		std::string GetFileIdentifier();
		short GetBlockLength();
		char GetRecordAttribute();
		CylinderHeadSector GetBeginningOfExtent();
		char GetPhysicalRecordLength();
		CylinderHeadSector GetEndOfExtent();
		bool HasFixedRecordLength();
		bool HasBypassIndicator();
		char GetDatasetSecurity();
		bool IsWriteProtected();
		char GetExchangeTypeIndicator();
		char GetMultiVolumeIndicator();
		std::string GetVolumeSequence();
		std::string GetCreationDate();
		int GetRecordLength();
		std::string GetOffsetToNextRecordSpace();
		std::string GetExpirationDate();
		char GetVerifyCopyIndicator();
		char GetDatasetOrganization();
		CylinderHeadSector GetEndOfData();
		std::vector<Sector>& GetSectors();
		void SaveToFile(std::string path);
};