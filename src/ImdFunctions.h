#pragma once

#include <vector>
#include "Mode.h"
#include "Track.h"
#include "Disk.h"
#include "CylinderHeadSector.h"

int ImdHeaderEnd(std::vector<char> file, int length);
bool FileIsImd(std::vector<char> file);
int ImdParseTrack(Disk& disk, std::vector<char> file, int index);
Mode ImdParseTrackMode(char modeId);
int ImdParseSector(Track& track, CylinderHeadSector location, std::vector<char> file, int index);