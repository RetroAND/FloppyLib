#pragma once

class DiskGeometry
{
	private: 
		unsigned char tracks;
		unsigned char heads;
		unsigned char sectorsPerTrack;
		unsigned short sectorSize;
	public:
		static const DiskGeometry Floppy720;
		static const DiskGeometry Floppy1440;
		DiskGeometry(unsigned char tracks = 80, unsigned char heads = 2, unsigned char sectorsPerTrack = 18, unsigned short sectorSize = 512);
		unsigned char GetTracks();
		unsigned char GetHeads();
		unsigned char GetSectorsPerTrack();
		unsigned short GetSectorSize();
		unsigned int GetDiskCapacity();
};