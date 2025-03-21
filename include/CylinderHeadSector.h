#pragma once

#include <string>

class CylinderHeadSector
{
	private:
		unsigned char cylinder;
		unsigned char head;
		unsigned char sector;
	public:
		CylinderHeadSector(unsigned char cylinder = 0, unsigned char header = 0, unsigned char sector = 0);
		CylinderHeadSector(std::string chs);
		unsigned char GetCylinder();
		unsigned char GetHead();
		unsigned char GetSector();
		bool Compare(CylinderHeadSector chs);
		bool Equals(CylinderHeadSector chs);
		void Increment(unsigned char sectorsPerTrack, bool doubleSided);
		CylinderHeadSector Clone();
};