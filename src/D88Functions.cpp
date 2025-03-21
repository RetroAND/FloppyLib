#include "D88Functions.h"
#include "Types.h"

using namespace std;

int GetInteger(vector<char> file, int index)
{
	return ((((((file[index + 3] & 0xff) << 8) | (file[index + 2] & 0xff)) << 8) | (file[index + 1] & 0xff)) << 8) | (file[index] & 0xff);
}

int GetWord(vector<char> file, int index)
{
	return ((file[index + 1] & 0xff) << 8) | (file[index] & 0xff);
}

int D88ParseDisk(DiskSet& set, vector<char> file, int index)
{
	unsigned int offset = index;
	unsigned int diskSize = GetInteger(file, offset + 28);
	vector<unsigned int> trackTable = vector<unsigned int>();
	index += 32;
	for (int track = 0; track < 164; track ++)
	{
		int trackAddress = offset + GetInteger(file, index);
		trackTable.push_back(trackAddress);
		index += 4;
	}
	Disk disk = Disk(Type_NEC_PC_88);
	for (int currentTrack = 0; currentTrack < trackTable.size(); currentTrack++)
	{
		index = D88ParseTrack(disk, file, trackTable[currentTrack]);
	}
	return diskSize;
}

int D88ParseTrack(Disk& disk, vector<char> file, int index)
{
	Track track = Track(Mode((file[index + 6] & 0x40), 500), file[index], file[index + 1], file[index + 5], 128 << file[index + 3]);
	index += 16;
	for (char currentSector = 0; currentSector < track.GetSectorNumber(); currentSector++)
	{
		index = D88ParseSector(track, file, index);
	}
	return index;
}

int D88ParseSector(Track& track, vector<char> file, int index)
{
	CylinderHeadSector location = CylinderHeadSector(file[index], file[index + 1], file[index + 2]);
	vector<char> data = vector<char>();
	int dataLength = GetWord(file, index + 14);
	for (unsigned short currentCell = 0; currentCell < dataLength; currentCell++)
	{
		data.push_back(file[currentCell]);
	}
	Sector sector = Sector(location, (file[index + 8] != 0) || (dataLength != track.GetSectorSize()), data);
	track.GetSectors().push_back(sector);
	return index + dataLength + 16;
}