#pragma once

#include <vector>
#include "InterchangeVolume.h"
#include "InterchangeFile.h"
#include "Disk.h"
#include "Sector.h"

class InterchangeDisk
{
	private:
		Disk disk;
		InterchangeVolume volume;
		vector<InterchangeFile> files;
		void InitializeEcmaVolume();
		void InitializeIbmVolume();
		void InitializeEcmaFiles();
		void InitializeIbmFiles();
		void GenerateFiles(InterchangeFile& file);
	public:
		Disk& GetDisk();
		InterchangeDisk(Disk disk = Disk());
		InterchangeVolume& GetVolume();
		vector<InterchangeFile>& GetFiles();
};