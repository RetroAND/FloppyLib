#include <string>
#include <algorithm>
#include "ImdFunctions.h"


using namespace std;

const vector<short> ImdSectorSizes = { 128, 256, 512, 1024, 2048, 4096, 8192 };

int ImdHeaderEnd(vector<char> file, int length)
{
	int index = 0;
	while (index < length)
	{
		if (file[index] == 0x1a) return index;
		index++;
	}
	return -1;
}

bool FileIsImd(vector<char> file)
{
	string signature = "IMD";
	for (int character = 0; character < 3; character++)
	{
		if (file[character] != signature[character]) return false;
	}
	return true;
}

Mode ImdParseTrackMode(char modeId)
{
	switch (modeId)
	{
		case 0: return new Mode(false, 500);
		case 1: return new Mode(false, 300);
		case 2: return new Mode(false, 250);
		case 3: return new Mode(true, 500);
		case 4: return new Mode(true, 300);
		case 5: return new Mode(true, 250);
		default: throw exception("ERROR: Unknown track mode");
	}
}

bool CompareCHS(Sector a, Sector b)
{
	return a.Compare(b);
}

int ImdParseTrack(Disk& disk, vector<char> file, int index)
{
	//Parse track start
	Mode mode = ImdParseTrackMode(file[index]);
	char cylinder = file[index + 1];
	char head = file[index + 2] & 0x3f;
	char sectorNumber = file[index + 3];
	short sectorSize = ImdSectorSizes[file[index + 4]];
	bool cylinderSectorMap = file[index + 2] & 0x80;
	bool headSectorMap = file[index + 2] & 0x40;
	Track track = Track(mode, cylinder, head, sectorNumber, sectorSize);
	index += 5;
	//Parse track end
	printf("Cylinder: %d\n", cylinder);
	printf("Head: %d\n", head);
	printf("Sector Number: %d\n", sectorNumber);
	printf("Sector Size: %d\n", sectorSize);
	printf("Cylinder Sector Map: %s\n", cylinderSectorMap ? "true" : "false");
	printf("Head Sector Map: %s\n", headSectorMap ? "true" : "false");
	//Parse sectors start
	vector<char> sectorMap = vector<char>();
	for (int sector = 0; sector < sectorNumber; sector++)
	{
		sectorMap.push_back(file[index + sector] - 1);
	}
	index += sectorNumber;
	if (cylinderSectorMap)
	{
		index += sectorNumber;
	}
	if (headSectorMap)
	{
		index += sectorNumber;
	}
	for (int sector = 0; sector < sectorNumber; sector++)
	{
		CylinderHeadSector location = CylinderHeadSector(cylinder, head, sectorMap[sector]);
		index += ImdParseSector(track, location, file, index);
	}
	sort(track.GetSectors().begin(), track.GetSectors().end(), CompareCHS);
	disk.GetTracks().push_back(track);
	return index;
}

vector<char> CopySectorAtIndex(vector<char> file, int index, short sectorSize)
{
	vector<char> sector = vector<char>();
	for (int byte = 0; byte < sectorSize; byte++)
	{
		sector.push_back(file[index + byte + 1]);
	}
	return sector;
}

int ImdParseSector(Track& track, CylinderHeadSector location, vector<char> file, int index)
{
	char sectorInfo = file[index];
	short sectorSize = track.GetSectorSize();
	switch (sectorInfo)
	{
		case 0:
		{
			vector<char> empty = vector<char>();
			track.GetSectors().push_back(Sector(location, true, empty));
			return 1;
		}
		case 1:
		case 3:
		{
			vector<char> data = CopySectorAtIndex(file, index, sectorSize);
			track.GetSectors().push_back(Sector(location, false, data));
			return sectorSize + 1;
		}
		case 2:
		case 4:
		{
			track.GetSectors().push_back(Sector(location, false, file[index + 1], sectorSize));
			return 2;
		}
		case 5:
		case 7:
		{
			vector<char> data = CopySectorAtIndex(file, index, sectorSize);
			track.GetSectors().push_back(Sector(location, true, data));
			return sectorSize + 1;
		}
		case 6:
		case 8:
		{
			track.GetSectors().push_back(Sector(location, true, file[index + 1], sectorSize));
			return 2;
		}
	}
	return 0;
}

