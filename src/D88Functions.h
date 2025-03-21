#pragma once

#include <vector>
#include "DiskSet.h"
#include "Disk.h"
#include "Track.h"


int D88ParseDisk(DiskSet& set, std::vector<char> file, int index);
int D88ParseTrack(Disk& disk, std::vector<char> file, int index);
int D88ParseSector(Track& track, std::vector<char> file, int index);
int GetInteger(std::vector<char>, int index);
int GetWord(std::vector<char>, int index);
