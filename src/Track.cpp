#include "Track.h"

using namespace std;

Track::Track(Mode mode, char cylinder, char head, char sectorNumber, short sectorSize)
{
	this->mode = mode;
	this->cylinder = cylinder;
	this->header = head;
	this->sectorNumber = sectorNumber;
	this->sectorSize = sectorSize;
	this->sectors = vector<Sector>();
}

Mode Track::GetMode()
{
	return this->mode;
}

char Track::GetCylinder()
{
	return this->cylinder;
}

char Track::GetHeader()
{
	return this->header;
}

char Track::GetSectorNumber()
{
	return this->sectorNumber;
}

short Track::GetSectorSize()
{
	return this->sectorSize;
}

vector<Sector>& Track::GetSectors()
{
	return this->sectors;
}