#pragma once

class DiskStatistics
{
	private:
		char tracks;
		char heads;
		short sectors;
		short errors;
	public:
		DiskStatistics(char tracks = 0, char heads = 1, short sectors = 0, short errors = 0);
		char GetTracks();
		char GetHeads();
		short GetSectors();
		short GetErrors();
};