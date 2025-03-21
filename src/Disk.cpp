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

char Disk::GetType()
{
	return this->type;
}

void Disk::SetType(char type)
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