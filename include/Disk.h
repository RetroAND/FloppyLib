#pragma once

#include <vector>
#include <string>
#include "Track.h"
#include "DiskStatistics.h"
#include "DiskGeometry.h";

class Disk
{
	private:
		unsigned int type;
		std::vector<Track> tracks;
		bool IsInterchangeDisk(std::vector<char> sector);
		bool IsEcmaDisk(std::vector<char> sector);
	public:
		Disk(char type = 0);
		bool IsIBMInterchangeDisk();
		std::vector<Track>& GetTracks();
		int LoadFromImd(std::string path);
		void LoadFromImg(std::string path, DiskGeometry geometry);
		void Identify();
		unsigned int GetType();
		void SetType(unsigned int type);
		std::string GetTypeString();
		DiskStatistics GetStatistics();
		Track GetTrack(char cylinder, char head);
		Sector GetSector(CylinderHeadSector location);
		std::vector<Track> GetTracksByHead(char head);
		char GetHeads();
};