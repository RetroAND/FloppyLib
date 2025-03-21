#include <filesystem>
#include <fstream>
#include "DiskSet.h"
#include "D88Functions.h"

using namespace std;

DiskSet::DiskSet(string name)
{
	this->name = name;
	this->disks = vector<Disk>();
}

vector<Disk>& DiskSet::GetDisks()
{
	return this->disks;
}

void DiskSet::LoadFromD88(string path)
{
	string extension = path.substr(path.length() - 4);
	for (int character = 0; character < extension.length(); character++)
	{
		extension[character] = tolower(extension[character]);
	}
	if (extension != ".d88")
	{
		throw exception("ERROR: File cannot be loaded because it is not a d88");	//ERROR: file cannot be loaded because it is not a d88
	}
	filesystem::path p = filesystem::path(path);
	DiskSet set = DiskSet(p.stem().string());
	ifstream::pos_type size;
	ifstream file(path, ios::in | ios::binary | ios::ate);
	if (!file.is_open())
	{
		throw exception("Error: File cannot be opened");	//ERROR: file cannot be opened
	}
	int fileSize = filesystem::file_size(path);
	size = file.tellg();
	file.seekg(0, ios::beg);
	vector<char> rawFile = vector<char>();
	rawFile.reserve(fileSize);
	file.unsetf(std::ios::skipws);
	rawFile.insert(rawFile.begin(),
		std::istream_iterator<char>(file),
		std::istream_iterator<char>());
	file.close();
	int index = 0;
	while (index < fileSize)
	{
		index += D88ParseDisk(set, rawFile, index);
	}
}