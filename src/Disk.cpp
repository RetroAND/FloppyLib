#include <vector>
#include <iostream>
#include <fstream>
#include <filesystem>
#include "Disk.h"
#include "ImdFunctions.h"
#include "Types.h"

using namespace std;


Disk::Disk(char type)
{
	this->type = type;
	this->tracks = vector<Track>();
}

vector<Track>& Disk::GetTracks()
{
	return this->tracks;
}

int Disk::LoadFromImd(string path)
{
	string extension = path.substr(path.length() - 4);
	for (int character = 0; character < extension.length(); character++)
	{
		extension[character] = tolower(extension[character]);
	}
	if (extension != ".imd")
	{
		return 1;	//ERROR: file cannot be loaded because it is not an imd
	}
	ifstream::pos_type size;
	ifstream file(path, ios::in | ios::binary | ios::ate);
	if (!file.is_open())
	{
		return 2;	//ERROR: file cannot be opened
	}
	int fileSize = filesystem::file_size(path);
	size = file.tellg();
	file.seekg(0, ios::beg);	
	vector<char> rawFile = vector<char>();
	rawFile.reserve(fileSize);
	file.unsetf(std::ios::skipws);
	rawFile.insert(rawFile.begin(),
		std::istream_iterator<char>(file),
		std::istream_iterator<char>());
	file.close();
	// IMD Processing start
	if (!FileIsImd(rawFile))
	{
		return 3;	//ERROR: file loaded is not an imd
	}
	int index = ImdHeaderEnd(rawFile, fileSize) + 1;
	if (index == 0)
	{
		return 4;
	}
	while (index < fileSize)
	{
		index = ImdParseTrack(*this, rawFile, index);
	}
	// IMD Processing end
	return 0;
}

void Disk::Identify()
{
	if (this->type != 0)
	{
		return;	// Disk is already identified, don't identify it
	}
	Track track0 = this->GetTrack(0, 0);
	if (track0.GetSectorNumber() == 1 && track0.GetSectorSize() == 4096)
	{
		this->type = Type_IBM_System_34; // System/34 Executable Disk
		return;
	}
	if (track0.GetSectorNumber() == 26 && track0.GetSectorSize() == 128)
	{
		Sector volume = this->GetSector(CylinderHeadSector(0, 0, 6));
		vector<char> sector = volume.GetData();
		if (this->IsEcmaDisk(sector))
		{
			this->type = Type_ECMA_58_Interchange; //ECMA-58 Interchange Disk
			return;
		}
		if (this->IsInterchangeDisk(sector))
		{
			this->type = Type_IBM_Interchange;	//IBM Interchange disk
			return;
		}
	}
	if (track0.GetSectorNumber() == 18)
	{
		Sector volume = this->GetSector(CylinderHeadSector(0,0,0));
		if (volume.GetEbcdicString(0,4) == "VOL1")
		{
			if (volume.GetEbcdicString(24,7) == "IBM5550")
			{
				this->type = Type_IBM_5550;
			}
			else if (volume.GetEbcdicString(24, 7) == "IBM3194")
			{
				this->type = Type_IBM_3194;
			}
		}
	}
}

bool Disk::IsEcmaDisk(vector<char> sector)
{
	char signature[] = { 0x56, 0x4f, 0x4c };
	for (int character = 0; character < 3; character++)
	{
		if (sector[character] != signature[character]) return false;
	}
	return true;
}

bool Disk::IsInterchangeDisk(vector<char> sector)
{
	char signature[] = { 0xe5, 0xd6, 0xd3 };
	for (int character = 0; character < 3; character++)
	{
		if (sector[character] != signature[character]) return false;
	}
	return true;
}

unsigned int Disk::GetType()
{
	return this->type;
}

void Disk::SetType(unsigned int type)
{
	this->type = type;
}

string Disk::GetTypeString()
{
	switch (this->type)
	{
		case Type_Unknown: return "Unknown";
		case Type_IBM_Interchange: return "IBM Interchange";
		case Type_DEC_PDP_RT11: return "DEC PDP-11 RT-11";
		case Type_IBM_System_32: return "IBM System/32";
		case Type_IBM_System_32_Interchange: return "IBM System/32 (interchange)";
		case Type_IBM_System_34: return "IBM System/34";
		case Type_IBM_System_34_Interchange: return "IBM System/34 (interchange)";
		case Type_IBM_System_36: return "IBM System/36";
		case Type_IBM_System_36_Interchange: return "IBM System/36 (interchange)";
		case Type_ECMA_58_Interchange: return "ECMA-58";
		case Type_NEC_PC_88: return "PC-88 (.D88)";
		case Type_NEC_PC_98: return "PC-98";
		default: return "Reserved";
	}
}

DiskStatistics Disk::GetStatistics()
{
	unsigned char tracks = this->tracks.size();
	unsigned char heads = 1;
	unsigned short sectors = 0;
	unsigned short errors = 0;
	for (int currentTrack = 0; currentTrack < tracks; currentTrack++)
	{
		Track track = this->tracks[currentTrack];
		int sectorsPerTrack = track.GetSectors().size(); // This way we make sure the load is correct
		sectors += sectorsPerTrack;
		for (int currentSector = 0; currentSector < sectorsPerTrack; currentSector++)
		{
			if (track.GetSectors()[currentSector].HasError())
			{
				errors++;
			}
		}
		if (track.GetHeader() == 1)
		{
			heads = 2;
		}
	}
	return DiskStatistics(tracks, heads, sectors, errors);
}

Track Disk::GetTrack(char cylinder, char head)
{
	int index = 0;
	while (index < this->tracks.size())
	{
		if (this->tracks[index].GetCylinder() == cylinder && this->tracks[index].GetHeader() == head) return this->tracks[index];
		index++;
	}
	throw exception("ERROR: Track not found\n");
}

Sector Disk::GetSector(CylinderHeadSector location)
{
	Track track = this->GetTrack(location.GetCylinder(), location.GetHead());
	int index = 0;
	while (index < track.GetSectors().size())
	{
		Sector sector = track.GetSectors()[index];
		if (track.GetSectors()[index].GetLocation().Equals(location)) return track.GetSectors()[index];
		index++;
	}
	printf("Sector not found\n");
	throw exception("Error: Sector not found\n");
}

vector<Track> Disk::GetTracksByHead(char head)
{
	vector<Track> tracks = vector<Track>();
	for (int currentTrack = 0; currentTrack < this->tracks.size(); currentTrack++)
	{
		if (this->tracks[currentTrack].GetHeader() == head)
		{
			tracks.push_back(this->tracks[currentTrack]);
		}
	}
	return tracks;
}

char Disk::GetHeads()
{
	char heads = 1;
	for (int track = 0; track < this->tracks.size(); track++)
	{
		char trackHead = this->tracks[track].GetHeader();
		if (trackHead >= heads)
		{
			heads = trackHead + 1;
		}
	}
	return heads;
}

bool Disk::IsIBMInterchangeDisk()
{
	switch (this->type)
	{
		case Type_IBM_Interchange:
		case Type_IBM_System_32_Interchange:
		case Type_IBM_System_34_Interchange:
		case Type_IBM_System_36_Interchange: return true;
		default: return false;
	}
}

void Disk::LoadFromImg(string path, DiskGeometry geometry)
{
	string extension = path.substr(path.length() - 4);
	for (int character = 0; character < extension.length(); character++)
	{
		extension[character] = tolower(extension[character]);
	}
	if (extension != ".img")
	{
		throw exception("ERROR: File cannot be loaded because it is not an img");	//ERROR: file cannot be loaded because it is not an img
	}
	filesystem::path p = filesystem::path(path);
	ifstream::pos_type size;
	ifstream file(path, ios::in | ios::binary | ios::ate);
	if (!file.is_open())
	{
		throw exception("Error: File cannot be opened");	//ERROR: file cannot be opened
	}
	int fileSize = filesystem::file_size(path);
	size = file.tellg();
	file.seekg(0, ios::beg);
	vector<char> rawFile = vector<char>();
	rawFile.reserve(fileSize);
	file.unsetf(std::ios::skipws);
	rawFile.insert(rawFile.begin(),
		std::istream_iterator<char>(file),
		std::istream_iterator<char>());
	file.close();
	if (fileSize != geometry.GetDiskCapacity())
	{
		throw exception("ERROR: disk geometry does not match the dump");
	}
	int index = 0;
	for (int currentTrack = 0; currentTrack < geometry.GetTracks(); currentTrack++)
	{
		for (int currentHead = 0; currentHead < geometry.GetHeads(); currentHead++)
		{
			Track track = Track(Mode(true, 500), currentTrack, currentHead, geometry.GetSectorsPerTrack(), geometry.GetSectorSize());
			for (int currentSector = 0; currentSector < geometry.GetSectorsPerTrack(); currentSector++)
			{
				vector<char> data = vector<char>();
				for (int currentCell = 0; currentCell < geometry.GetSectorSize(); currentCell++)
				{
					data.push_back(rawFile[(currentTrack * geometry.GetHeads() * geometry.GetSectorsPerTrack() * geometry.GetSectorSize())
						+ (currentHead * geometry.GetSectorsPerTrack() * geometry.GetSectorSize())
						+ (geometry.GetSectorSize() * currentSector)
						+ currentCell]);
				}
				Sector sector = Sector(CylinderHeadSector(currentTrack, currentHead, currentSector), false, data);//IMG format do not inform of errors!
				track.GetSectors().push_back(sector);
			}
			this->tracks.push_back(track);
			printf("C: %d H: %d Sectors: %d\n", track.GetCylinder(), track.GetHeader(), track.GetSectors().size());
		}
	}
}