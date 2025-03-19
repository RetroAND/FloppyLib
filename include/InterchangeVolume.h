#pragma once

#include <string>
#include <vector>
#include "Disk.h"
#include "InterchangeFile.h"

class InterchangeVolume
{
	private:
		std::string identifier;
		char accessibility;
		std::string machine;
		std::string owner;
		char surfaceIndicator;
		bool extentArrangementConstraint;
		bool nonSequentialRecording;
		short sectorSize;
		std::string sectorSequenceCode;
		char labelStandard;
	public:
		InterchangeVolume(std::string identifier = "IBMIRD", char accessibility = ' ', std::string = "", std::string owner = "", char surfaceIndicator = '1', bool extentArrangementConstraint = false, bool nonSequentialRecording = false, short sectorSize = 128, std::string sectorSequenceCode = "00", char labelStandard = 'W');
		std::string GetIdentifier();
		char GetAccessibility();
		std::string GetMachine();
		std::string GetOwner();
		char GetSurfaceIndicator();
		bool HasExtentArrangementConstraint();
		bool HasNonSequentialRecording();
		short GetSectorSize();
		std::string GetSectorSequenceCode();
		char GetLabelStandard();
		bool IsDoubleSided();

};