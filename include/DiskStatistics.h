#pragma once

class DiskStatistics
{
	private:
		unsigned char tracks;
		unsigned char heads;
		unsigned short sectors;
		unsigned short errors;
	public:
		DiskStatistics(unsigned char tracks = 0, unsigned char heads = 1, unsigned short sectors = 0, unsigned short errors = 0);
		unsigned char GetTracks();
		unsigned char GetHeads();
		unsigned short GetSectors();
		unsigned short GetErrors();
};