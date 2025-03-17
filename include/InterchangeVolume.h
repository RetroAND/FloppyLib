#pragma once

#include <string>
#include <vector>
#include "Disk.h"
#include "InterchangeFile.h"

using namespace std;

class InterchangeVolume
{
	private:
		string identifier;
		char accessibility;
		string machine;
		string owner;
		char surfaceIndicator;
		bool extentArrangementConstraint;
		bool nonSequentialRecording;
		short sectorSize;
		string sectorSequenceCode;
		char labelStandard;
	public:
		InterchangeVolume(string identifier = "IBMIRD", char accessibility = ' ', string = "", string owner = "", char surfaceIndicator = '1', bool extentArrangementConstraint = false, bool nonSequentialRecording = false, short sectorSize = 128, string sectorSequenceCode = "00", char labelStandard = 'W');
		string GetIdentifier();
		char GetAccessibility();
		string GetMachine();
		string GetOwner();
		char GetSurfaceIndicator();
		bool HasExtentArrangementConstraint();
		bool HasNonSequentialRecording();
		short GetSectorSize();
		string GetSectorSequenceCode();
		char GetLabelStandard();
		bool IsDoubleSided();

};