#include <iostream>
#include "../include/FloppyLib.h"

using namespace std;

int main()
{
	cout << "FloppyLib library for floppy images" << endl;
	Disk disk = Disk();
	//disk.LoadFromImd("C:/Users/RetroAND/Downloads/5322-System-Diagnostic.dsk.imd");
	disk.LoadFromImd("C:/Users/RetroAND/Downloads/apc8cpm.imd");
	InterchangeDisk interchange = InterchangeDisk(disk);
	DiskStatistics stats = disk.GetStatistics();

	return 0;
}