#include "InterchangeVolume.h"

using namespace std;

InterchangeVolume::InterchangeVolume(string identifier, char accessibility, string machine, string owner, char surfaceIndicator, bool extentArrangementConstraint, bool nonSequentialRecording, short sectorSize, string sectorSequenceCode, char labelStandard)
{
	this->identifier = identifier;
	this->accessibility = accessibility;
	this->machine = machine;
	this->owner = owner;
	this->surfaceIndicator = surfaceIndicator;
	this->extentArrangementConstraint = extentArrangementConstraint;
	this->nonSequentialRecording = nonSequentialRecording;
	this->sectorSize = sectorSize;
	this->sectorSequenceCode = sectorSequenceCode;
	this->labelStandard = labelStandard;
}

string InterchangeVolume::GetIdentifier()
{
	return this->identifier;
}

char InterchangeVolume::GetAccessibility()
{
	return this->accessibility;
}

string InterchangeVolume::GetMachine()
{
	return this->machine;
}

string InterchangeVolume::GetOwner()
{
	return this->owner;
}

char InterchangeVolume::GetSurfaceIndicator()
{
	return this->surfaceIndicator;
}

bool InterchangeVolume::HasExtentArrangementConstraint()
{
	return this->extentArrangementConstraint;
}

bool InterchangeVolume::HasNonSequentialRecording()
{
	return this->nonSequentialRecording;
}

short InterchangeVolume::GetSectorSize()
{
	return this->sectorSize;
}

string InterchangeVolume::GetSectorSequenceCode()
{
	return this->sectorSequenceCode;
}

char InterchangeVolume::GetLabelStandard()
{
	return this->labelStandard;
}

bool InterchangeVolume::IsDoubleSided()
{
	return this->surfaceIndicator == '2' || this->surfaceIndicator == 'M';
}
