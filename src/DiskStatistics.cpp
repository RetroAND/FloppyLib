#include "DiskStatistics.h"

DiskStatistics::DiskStatistics(unsigned char tracks, unsigned char heads, unsigned short sectors, unsigned short errors)
{
	this->tracks = tracks;
	this->heads = heads;
	this->sectors = sectors;
	this->errors = errors;
}

unsigned char DiskStatistics::GetTracks()
{
	return this->tracks;
}

unsigned char DiskStatistics::GetHeads()
{
	return this->heads;
}

unsigned short DiskStatistics::GetSectors()
{
	return this->sectors;
}

unsigned short DiskStatistics::GetErrors()
{
	return this->errors;
}