#pragma once

#include <vector>
#include "mode.h"
#include "sector.h"

class Track
{
	private:
		Mode mode;
		char cylinder;
		char header;
		char sectorNumber;
		short sectorSize;
		std::vector<Sector> sectors;
	public:
		Track(Mode mode, char cylinder, char head, char sectorNumber, short sectorSize);
		Mode GetMode();
		char GetCylinder();
		char GetHeader();
		char GetSectorNumber();
		short GetSectorSize();
		std::vector<Sector>& GetSectors();
};