#include "InterchangeDisk.h"
#include <iostream>

using namespace std;

InterchangeDisk::InterchangeDisk(Disk disk)
{
	this->disk = disk;
	char type = disk.GetType();
	switch (type)
	{
	case 1:
	{
		this->InitializeIbmVolume();
		this->InitializeIbmFiles();
	}; break;
		case 58: InitializeEcmaVolume(); break;
	}
}

InterchangeVolume& InterchangeDisk::GetVolume()
{
	return this->volume;
}

vector<InterchangeFile>& InterchangeDisk::GetFiles()
{
	return this->files;
}

short GetSectorSizeFromVolumeChar(char sectorSize)
{
	switch (sectorSize)
	{
		case ' ': return 128;
		case '1': return 256;
		case '2': return 512;
		case '3': return 1024;
		default: throw exception("ERROR: Unknown Sector Size");
	}
}

void InterchangeDisk::InitializeEcmaVolume()
{
	Sector volumeSector = this->disk.GetSector(CylinderHeadSector(0,0,6));
	string identifier = volumeSector.GetString(4,6);
	char accessibility = volumeSector.GetData()[10];
	string machine = volumeSector.GetString(11,25);
	string owner = volumeSector.GetString(37,13);
	char surfaceIndicator = volumeSector.GetData()[71];
	bool extentArrangementConstraint = volumeSector.GetData()[72] == 'P';
	bool nonSequentialRecording = volumeSector.GetData()[73] == 'R';
	short sectorSize = GetSectorSizeFromVolumeChar(volumeSector.GetData()[75]);
	string sectorSequenceCode = volumeSector.GetString(76,2);
	char labelStandard = volumeSector.GetData()[79];
	this->volume = InterchangeVolume(identifier, accessibility, machine, owner, surfaceIndicator, extentArrangementConstraint, nonSequentialRecording, sectorSize, sectorSequenceCode, labelStandard);
}

void InterchangeDisk::InitializeIbmVolume()
{
	Sector volumeSector = this->disk.GetSector(CylinderHeadSector(0, 0, 6));
	string identifier = volumeSector.GetEbcdicString(4, 6);
	char accessibility = volumeSector.GetEbcdicChar(10);
	string machine = volumeSector.GetEbcdicString(24, 13);
	string owner = volumeSector.GetEbcdicString(37, 13);
	char surfaceIndicator = volumeSector.GetEbcdicChar(71);
	bool extentArrangementConstraint = volumeSector.GetEbcdicChar(72) == 'P';
	bool nonSequentialRecording = volumeSector.GetEbcdicChar(73) == 'R';
	short sectorSize = GetSectorSizeFromVolumeChar(volumeSector.GetEbcdicChar(75));
	string sectorSequenceCode = volumeSector.GetEbcdicString(76, 2);
	char labelStandard = volumeSector.GetEbcdicChar(79);
	this->volume = InterchangeVolume(identifier, accessibility, machine, owner, surfaceIndicator, extentArrangementConstraint, nonSequentialRecording, sectorSize, sectorSequenceCode, labelStandard);
}

void InterchangeDisk::InitializeEcmaFiles()
{

}

void InterchangeDisk::InitializeIbmFiles()
{
	for (int sector = 7; sector < 26; sector++)
	{
		CylinderHeadSector location = CylinderHeadSector(0, 0, sector);
		Sector directorySector = this->disk.GetSector(location);
		if (directorySector.GetEbcdicString(0,3) == "HDR")
		{
			string fileIdentifier = directorySector.GetEbcdicString(5,17);
			string bl = directorySector.GetEbcdicString(22, 5);
			short blockLength = 256;
			if (bl != "     ")
			{
				blockLength = stoi(bl);
			}
			char recordAttribute = directorySector.GetEbcdicChar(27);		
			CylinderHeadSector beginningOfExtent = CylinderHeadSector(directorySector.GetEbcdicString(28, 5));		
			char physicalRecordLength = directorySector.GetEbcdicChar(33);		
			CylinderHeadSector endOfExtent = CylinderHeadSector(directorySector.GetEbcdicString(34, 5));		
			bool fixedLengthRecord = directorySector.GetEbcdicChar(39) == 'F';	
			bool bypassIndicator = directorySector.GetEbcdicChar(40) == 'B';		
			char datasetSecurity = directorySector.GetEbcdicChar(41);		
			bool writeProtect = directorySector.GetEbcdicChar(42) == 'P';		
			char exchangeTypeIndicator = directorySector.GetEbcdicChar(43);		
			char multiVolumeIndicator = directorySector.GetEbcdicChar(44);		
			string volumeSequence = directorySector.GetEbcdicString(45, 2);		
			string creationDate = directorySector.GetEbcdicString(47, 6);
			string rl = directorySector.GetEbcdicString(53, 4);
			int recordLength = blockLength;
			if (rl != "    ")
			{
				recordLength = stoi(directorySector.GetEbcdicString(53, 4));
			}
			string offsetToNextRecordSpace = directorySector.GetEbcdicString(57, 5);		
			string expirationDate = directorySector.GetEbcdicString(66, 6);		
			char verifyCopyIndicator = directorySector.GetEbcdicChar(72);			
			char datasetOrganization = directorySector.GetEbcdicChar(73);			
			CylinderHeadSector endOfData = CylinderHeadSector(directorySector.GetEbcdicString(74, 5));
			string creatingSystem = directorySector.GetEbcdicString(95, 13);
			InterchangeFile file = InterchangeFile(fileIdentifier, blockLength, recordAttribute, beginningOfExtent, physicalRecordLength, endOfExtent, fixedLengthRecord, bypassIndicator, datasetSecurity, writeProtect, exchangeTypeIndicator, multiVolumeIndicator, volumeSequence, creationDate, recordLength, offsetToNextRecordSpace, expirationDate, verifyCopyIndicator, datasetOrganization, endOfData, creatingSystem);
			this->GenerateFiles(file);
			this->files.push_back(file);
		}
	}
}

Disk& InterchangeDisk::GetDisk()
{
	return this->disk;
}

void InterchangeDisk::GenerateFiles(InterchangeFile& file)
{
	CylinderHeadSector index = file.GetBeginningOfExtent().Clone();
	while (index.Compare(file.GetEndOfExtent()))
	{
		Sector sector = this->disk.GetSector(index);
		file.GetSectors().push_back(sector);
		index.Increment(this->disk.GetTrack(index.GetCylinder(), index.GetHead()).GetSectorNumber(), this->volume.IsDoubleSided());
	}
}