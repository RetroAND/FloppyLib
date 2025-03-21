#include "CylinderHeadSector.h"
#include <iostream>

using namespace std;

CylinderHeadSector::CylinderHeadSector(unsigned char cylinder, unsigned char head, unsigned char sector)
{
	this->cylinder = cylinder;
	this->head = head;
	this->sector = sector;
}

CylinderHeadSector::CylinderHeadSector(string chs)
{
	string cylinder = chs.substr(0, 2);
	string head = chs.substr(2, 1);
	string sector = chs.substr(3, 2);
	this->cylinder = stoi(cylinder);
	this->head = stoi(head);
	this->sector = stoi(sector) - 1;
}

unsigned char CylinderHeadSector::GetCylinder()
{
	return this->cylinder;
}

unsigned char CylinderHeadSector::GetHead()
{
	return this->head;
}

unsigned char CylinderHeadSector::GetSector()
{
	return this->sector;
}

bool CylinderHeadSector::Compare(CylinderHeadSector chs)
{
	if (this->cylinder > chs.cylinder)
	{
		return false;
	}
	else if (this->cylinder < chs.cylinder)
	{
		return true;
	}
	else
	{
		if (this->head > chs.head)
		{
			return false;
		}
		else if (this->head < chs.head)
		{
			return true;
		}
		else
		{
			return this->sector < chs.sector;
		}
	}
}

bool CylinderHeadSector::Equals(CylinderHeadSector chs)
{
	return (this->cylinder == chs.cylinder)
		&& (this->head == chs.head)
		&& (this->sector == chs.sector);
}


void CylinderHeadSector::Increment(unsigned char sectorsPerTrack, bool doubleSided)
{
	this->sector++;
	if (this->sector >= sectorsPerTrack)
	{
		this->sector = 0;
		if (doubleSided)
		{
			this->head++;
			if (this->head >= 2)
			{
				this->head = 0;
				this->cylinder++;
			}
		}
		else
		{
			this->cylinder++;
		}
	}
}

CylinderHeadSector CylinderHeadSector::Clone()
{
	return CylinderHeadSector(this->cylinder, this->head, this->sector);
}