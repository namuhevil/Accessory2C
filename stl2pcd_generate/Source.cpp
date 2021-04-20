#include<iostream>
#include<fstream>
#include "stl2pcd.h"

using namespace std;

int main()
{
	const char* stl_file = "screw_1.stl";
	const char* pcd_file = "screw_1.pcd";
	stl2pcd(stl_file, pcd_file);


	system("PAUSE");
	return 0;
}