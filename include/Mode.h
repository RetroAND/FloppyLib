#pragma once

class Mode
{
	private:
		bool mfm;
		short kbps;
	public:
		Mode(bool mfm = false, short kbps = 500);
		bool IsMFM();
		short GetKbps();
};