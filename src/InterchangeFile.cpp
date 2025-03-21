#include <fstream>
#include "InterchangeFile.h"
#include <iostream>

using namespace std;

InterchangeFile::InterchangeFile(string fileIdentifier = "", short blockLength = 128, char recordAttribute=' ', CylinderHeadSector beginningOfExtent = CylinderHeadSector(1,0,0), char physicalRecordLength = 128, CylinderHeadSector endOfExtent = (75,0,0), bool fixedLengthRecord = false, bool bypassIndicator = false, char datasetSecurity = ' ', bool writeProtect = false, char exchangeTypeIndicator = 'E', char multiVolumeIndicator = ' ', string volumeSequence = "", string creationDate="", int recordLength = 128, string offsetToNextRecordSpace = "", string expirationDate = "999999", char verifyCopyIndicator = ' ', char datasetOrganization = ' ', CylinderHeadSector endOfData = CylinderHeadSector(75,0,0), std::string creatingSystem = "")
{
	this->fileIdentifier = fileIdentifier;
	this->blockLength = blockLength;
	this->recordAttribute = recordAttribute;
	this->beginningOfExtent = beginningOfExtent;
	this->physicalRecordLength = physicalRecordLength;
	this->endOfExtent = endOfExtent;
	this->fixedLengthRecord = fixedLengthRecord;
	this->bypassIndicator = bypassIndicator;
	this->datasetSecurity = datasetSecurity;
	this->writeProtect = writeProtect;
	this->exchangeTypeIndicator = exchangeTypeIndicator;
	this->multiVolumeIndicator = multiVolumeIndicator;
	this->volumeSequence = volumeSequence;
	this->creationDate = creationDate;
	this->recordLength = recordLength;
	this->offsetToNextRecordSpace = offsetToNextRecordSpace;
	this->expirationDate = expirationDate;
	this->verifyCopyIndicator = verifyCopyIndicator;
	this->datasetOrganization = datasetOrganization;
	this->endOfData = endOfData;
	this->creatingSystem = creatingSystem;
	this->sectors = vector<Sector>();
}

string InterchangeFile::GetFileIdentifier()
{
	return this->fileIdentifier;
}

short InterchangeFile::GetBlockLength()
{
	return this->blockLength;
}

char InterchangeFile::GetRecordAttribute()
{
	return this->recordAttribute;
}

CylinderHeadSector InterchangeFile::GetBeginningOfExtent()
{
	return this->beginningOfExtent;
}

char InterchangeFile::GetPhysicalRecordLength()
{
	return this->physicalRecordLength;
}

CylinderHeadSector InterchangeFile::GetEndOfExtent()
{
	return this->endOfExtent;
}

bool InterchangeFile::HasFixedRecordLength()
{
	return this->fixedLengthRecord;
}

bool InterchangeFile::HasBypassIndicator()
{
	return this->bypassIndicator;
}

char InterchangeFile::GetDatasetSecurity()
{
	return this->datasetSecurity;
}

bool InterchangeFile::IsWriteProtected()
{
	return this->writeProtect;
}

char InterchangeFile::GetExchangeTypeIndicator()
{
	return this->exchangeTypeIndicator;
}

char InterchangeFile::GetMultiVolumeIndicator()
{
	return this->multiVolumeIndicator;
}

string InterchangeFile::GetVolumeSequence()
{
	return this->volumeSequence;
}

string InterchangeFile::GetCreationDate()
{
	return this->creationDate;
}

int InterchangeFile::GetRecordLength()
{
	return this->recordLength;
}

string InterchangeFile::GetOffsetToNextRecordSpace()
{
	return this->offsetToNextRecordSpace;
}

string InterchangeFile::GetExpirationDate()
{
	return this->expirationDate;
}

char InterchangeFile::GetVerifyCopyIndicator()
{
	return this->verifyCopyIndicator;
}

char InterchangeFile::GetDatasetOrganization()
{
	return this->datasetOrganization;
}

CylinderHeadSector InterchangeFile::GetEndOfData()
{
	return this->endOfData;
}

std::string InterchangeFile::GetCreatingSystem()
{
	return this->creatingSystem;
}

vector<Sector>& InterchangeFile::GetSectors()
{
	return this->sectors;
}

void InterchangeFile::SaveToFile(string path)
{
	if (path.size() == 0)
	{
		throw exception("ERROR: Path cannot be empty.");
	}
	if (path[path.size()] != '/')
	{
		path += '/';
	}
	path += this->fileIdentifier + ".bin";
	vector<char> rawFile = vector<char>();
	for (int currentSector = 0; currentSector < this->sectors.size(); currentSector++)
	{
		vector<char> data = this->sectors[currentSector].GetData();
		rawFile.insert(rawFile.end(), data.begin(), data.end());
	}
	ofstream file;
	file.open(path, ios::out | ios::binary);
	copy(rawFile.cbegin(), rawFile.cend(), std::ostreambuf_iterator<char>(file));
	file.close();

}