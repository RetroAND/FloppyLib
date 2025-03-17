#include "DiskStatistics.h"

DiskStatistics::DiskStatistics(char tracks, char heads, short sectors, short errors)
{
	this->tracks = tracks;
	this->heads = heads;
	this->sectors = sectors;
	this->errors = errors;
}

char DiskStatistics::GetTracks()
{
	return this->tracks;
}

char DiskStatistics::GetHeads()
{
	return this->heads;
}

short DiskStatistics::GetSectors()
{
	return this->sectors;
}

short DiskStatistics::GetErrors()
{
	return this->errors;
}