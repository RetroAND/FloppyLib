#include "Sector.h"
#include "Ebcdic.h"
#include <iostream>
#include <fstream>

using namespace std;

Sector::Sector(CylinderHeadSector location, bool error, vector<char> data)
{
	this->location = location;
	this->error = error;
	int length = data.size();
	this->data = vector<char>();
	for (short cell = 0; cell < length; cell++)
	{
		this->data.push_back(data[cell]);
	}
}

Sector::Sector(CylinderHeadSector location, bool error, char data, short length)
{
	this->location = location;
	this->error = error;
	this->data = vector<char>();
	for (short cell = 0; cell < length; cell++)
	{
		this->data.push_back(data);
	}
}

CylinderHeadSector Sector::GetLocation()
{
	return this->location;
}

vector<char> Sector::GetData()
{
	return this->data;
}

bool Sector::IsEmpty()
{
	return (sizeof(this->data) == 0);
}

bool Sector::Compare(Sector sector)
{
	return this->location.Compare(sector.location);
}

bool Sector::HasError()
{
	return this->error;
}

string Sector::GetString(int index, int length)
{
	vector<char> bytes = vector<char>();
	for (int idx = index; idx < (index + length); idx++)
	{
		bytes.push_back(this->data[idx]);
	}
	bytes.push_back(0);
	string result(bytes.begin(), bytes.end());
	return result;
}

char Sector::GetEbcdicChar(int index)
{
	int code = this->data[index] & 0xff;
	return ebcdicToAscii[code];
}

string Sector::GetEbcdicString(int index, int length)
{
	vector<char> bytes = vector<char>();
	for (int idx = index; idx < (index + length); idx++)
	{
		int code = data[idx] & 0xff;
		bytes.push_back(ebcdicToAscii[code]);
	}
	//bytes.push_back(0);
	string result(bytes.begin(), bytes.end());
	return result;
}

void Sector::SaveToFile(string path)
{
	ofstream file;
	file.open(path, std::ios::out | std::ios::binary);
	copy(this->data.cbegin(), this->data.cend(), std::ostreambuf_iterator<char>(file));
	file.close();
}