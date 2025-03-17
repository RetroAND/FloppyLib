#pragma once

#include <vector>
#include "Mode.h"
#include "Track.h"
#include "Disk.h"
#include "CylinderHeadSector.h"

int ImdHeaderEnd(vector<char> file, int length);
bool FileIsImd(vector<char> file);
int ImdParseTrack(Disk& disk, vector<char> file, int index);
Mode ImdParseTrackMode(char modeId);
int ImdParseSector(Track& track, CylinderHeadSector location, vector<char> file, int index);