#include "mode.h"

Mode::Mode(bool mfm, short kbps)
{
	this->mfm = mfm;
	this->kbps = kbps;
}

bool Mode::IsMFM()
{
	return this->mfm;
}

short Mode::GetKbps()
{
	return this->kbps;
}