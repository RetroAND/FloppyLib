#pragma once

#include <string>
#include <vector>
#include "CylinderHeadSector.h"

class Sector
{
	private:
		CylinderHeadSector location;
		bool error;
		std::vector<char> data;
	public:
		Sector(CylinderHeadSector location, bool error = false, std::vector<char> data = std::vector<char>());
		Sector(CylinderHeadSector location, bool error = false, char data = 0, short length = 1);
		CylinderHeadSector GetLocation();
		std::vector<char> GetData();
		bool IsEmpty();
		bool Compare(Sector sector);
		bool HasError();
		std::string GetString(int index, int length);
		char GetEbcdicChar(int index);
		std::string GetEbcdicString(int index, int length);
		void SaveToFile(std::string path);
};