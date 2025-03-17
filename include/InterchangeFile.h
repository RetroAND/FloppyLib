#pragma once

#include <vector>
#include <string>
#include "Sector.h"

using namespace std;

class InterchangeFile
{
	private:
		string fileIdentifier;
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
		string volumeSequence;
		string creationDate;
		int recordLength;
		string offsetToNextRecordSpace;
		string expirationDate;
		char verifyCopyIndicator;
		char datasetOrganization;
		CylinderHeadSector endOfData;
		vector<Sector> sectors;
	public:
		InterchangeFile(string fileIdentifier, short blockLength, char recordAttribute, CylinderHeadSector beginningOfExtent, char physicalRecordLength, CylinderHeadSector endOfExtent, bool fixedLengthRecord, bool bypassIndicator, char datasetSecurity, bool writeProtect, char exchangeTypeIndicator, char multiVolumeIndicator, string volumeSequence, string creationDate, int recordLength, string offsetToNextRecordSpace, string expirationDate, char verifyCopyIndicator, char datasetOrganization, CylinderHeadSector endOfData);
		string GetFileIdentifier();
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
		string GetVolumeSequence();
		string GetCreationDate();
		int GetRecordLength();
		string GetOffsetToNextRecordSpace();
		string GetExpirationDate();
		char GetVerifyCopyIndicator();
		char GetDatasetOrganization();
		CylinderHeadSector GetEndOfData();
		vector<Sector>& GetSectors();
		void SaveToFile(string path);
};