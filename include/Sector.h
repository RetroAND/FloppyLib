#pragma once

#include <string>
#include <vector>
#include "CylinderHeadSector.h"

using namespace std;

class Sector
{
	private:
		CylinderHeadSector location;
		bool error;
		vector<char> data;
	public:
		Sector(CylinderHeadSector location, bool error = false, vector<char> data = vector<char>());
		Sector(CylinderHeadSector location, bool error = false, char data = 0, short length = 1);
		CylinderHeadSector GetLocation();
		vector<char> GetData();
		bool IsEmpty();
		bool Compare(Sector sector);
		bool HasError();
		string GetString(int index, int length);
		char GetEbcdicChar(int index);
		string GetEbcdicString(int index, int length);
		void SaveToFile(string path);
};