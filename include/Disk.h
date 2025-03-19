#pragma once

#include <vector>
#include <string>
#include "Track.h"
#include "DiskStatistics.h"

class Disk
{
	private:
		char type;
		std::vector<Track> tracks;
		bool IsEcmaDisk(std::vector<char> sector);
		bool IsInterchangeDisk(std::vector<char> sector);
	public:
		Disk(char type = 0);
		std::vector<Track>& GetTracks();
		int LoadFromImd(std::string path);
		void Identify();
		char GetType();
		void SetType(char type);
		std::string GetTypeString();
		DiskStatistics GetStatistics();
		Track GetTrack(char cylinder, char head);
		Sector GetSector(CylinderHeadSector location);
		std::vector<Track> GetTracksByHead(char head);
		char GetHeads();
};