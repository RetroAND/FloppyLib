#include <iostream>
#include "../include/FloppyLib.h"

using namespace std;

int main()
{
	cout << "FloppyLib library for floppy images" << endl;
	Disk disk = Disk();
	//disk.LoadFromImd("C:/Users/RetroAND/Downloads/5322-System-Diagnostic.dsk.imd");
	disk.LoadFromImd("C:/Users/RetroAND/Downloads/IBM-System-23-Diagnostic_6841645_05-10-82.dsk.imd");
	InterchangeDisk interchange = InterchangeDisk(disk);
	DiskStatistics stats = disk.GetStatistics();

	return 0;
}