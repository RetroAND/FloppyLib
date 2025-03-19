#include <vector>
#include <iostream>
#include <fstream>
#include <filesystem>
#include "Disk.h"
#include "ImdFunctions.h"

using namespace std;

#define UNKNOWN 0
#define ECMA_58 58
#define IBM_INTERCHANGE 1
#define IBM_SYSTEM_32 32
#define IBM_SYSTEM_34 34
#define PDP11_RX01 11
#define PDP11_RX02 12

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
		index += ImdParseTrack(*this, rawFile, index);
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
	Track track0 = this->tracks[0];
	if (track0.GetSectorNumber() == 1 && track0.GetSectorSize() == 4096)
	{
		this->type = IBM_SYSTEM_34; // System/34 Executable Disk
		return;
	}
	if (track0.GetSectorNumber() == 26 && track0.GetSectorSize() == 128)
	{
		Sector volume = track0.GetSectors()[6];
		vector<char> sector = volume.GetData();
		if (this->IsEcmaDisk(sector))
		{
			this->type = ECMA_58; //ECMA-58 Interchange Disk
			return;
		}
		if (this->IsInterchangeDisk(sector))
		{
			this->type = IBM_INTERCHANGE;	//IBM Interchange disk
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
		case UNKNOWN: return "Unknown";
		case IBM_INTERCHANGE: return "IBM Interchange";
		case PDP11_RX01: return "DEC PDP-11 RX01";
		case PDP11_RX02: return "DEC PDP-11 RX02";
		case IBM_SYSTEM_32: return "IBM System/32";
		case IBM_SYSTEM_34: return "IBM System/34";
		case ECMA_58: return "ECMA-58";
		default: return "Reserved";
	}
}

DiskStatistics Disk::GetStatistics()
{
	char tracks = this->tracks.size();
	char heads = 1;
	short sectors = 0;
	short errors = 0;
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
		if (trackHead > heads)
		{
			heads = trackHead + 1;
		}
	}
	return heads;
}