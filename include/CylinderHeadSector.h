#pragma once

#include <string>

class CylinderHeadSector
{
	private:
		char cylinder;
		char head;
		char sector;
	public:
		CylinderHeadSector(char cylinder = 0, char header = 0, char sector = 0);
		CylinderHeadSector(std::string chs);
		char GetCylinder();
		char GetHead();
		char GetSector();
		bool Compare(CylinderHeadSector chs);
		bool Equals(CylinderHeadSector chs);
		void Increment(char sectorsPerTrack, bool doubleSided);
		CylinderHeadSector Clone();
};