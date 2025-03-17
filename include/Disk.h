#pragma once

#include <vector>
#include <string>
#include "Track.h"
#include "DiskStatistics.h"

using namespace std;

class Disk
{
	private:
		char type;
		vector<Track> tracks;
		bool IsEcmaDisk(vector<char> sector);
		bool IsInterchangeDisk(vector<char> sector);
	public:
		Disk(char type = 0);
		vector<Track>& GetTracks();
		int LoadFromImd(string path);
		void Identify();
		char GetType();
		void SetType(char type);
		string GetTypeString();
		DiskStatistics GetStatistics();
		Track GetTrack(char cylinder, char head);
		Sector GetSector(CylinderHeadSector location);
};