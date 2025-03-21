#include "DiskGeometry.h"

const DiskGeometry DiskGeometry::Floppy720 = DiskGeometry(80, 1, 18, 512);
const DiskGeometry DiskGeometry::Floppy1440 = DiskGeometry(80, 2, 18, 512);

DiskGeometry::DiskGeometry(unsigned char tracks, unsigned char heads, unsigned char sectorsPerTrack, unsigned short sectorSize)
{
	this->tracks = tracks;
	this->heads = heads;
	this->sectorsPerTrack = sectorsPerTrack;
	this->sectorSize = sectorSize;
}

unsigned char DiskGeometry::GetTracks()
{
	return this->tracks;
}

unsigned char DiskGeometry::GetHeads()
{
	return this->heads;
}

unsigned char DiskGeometry::GetSectorsPerTrack()
{
	return this->sectorsPerTrack;
}

unsigned short DiskGeometry::GetSectorSize()
{
	return this->sectorSize;
}

unsigned int DiskGeometry::GetDiskCapacity()
{
	return this->tracks * this->heads * this->sectorsPerTrack * this->sectorSize;
}
