#pragma once

#include <vector>
#include <string>
#include "Disk.h"

class DiskSet
{
	private:
		std::string name;
		std::vector<Disk> disks;
	public:
		DiskSet(std::string name = "");
		std::vector<Disk>& GetDisks();
		void LoadFromD88(string path);
};