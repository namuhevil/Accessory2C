#define _CRT_SECURE_NO_WARNINGS
#include "stl2pcd.h"
#include<fstream>
#include<iostream>
#include<cmath>
#include<vector>

using namespace std;

TRIANGLE::TRIANGLE()
{
}

TRIANGLE::~TRIANGLE()
{
}

POINT::POINT()
{
}

POINT::~POINT()
{
}

VECTOR::VECTOR()
{
}

VECTOR::~VECTOR()
{
}

void stl2pcd(const char * file_stl, const char * file_pcd)
{
	ifstream input_file;
	int file_pos = 0;

	input_file.open(file_stl, ios::in | ios::binary);
	if (!(input_file.is_open()))
	{
		cout << "Read File Error !" << endl;
		input_file.close();
		exit(0);
	}
	else
		cout << "Read \"" << file_stl << "\" Successfully !" << endl << endl;

	//Read Header
	input_file.seekg(0, ios::beg);

	char *Header = new char[80];
	input_file.read(Header, 80);
	file_pos = input_file.tellg();

	//Read the Number of triangles
	char* Number = new char[4];
	input_file.read(Number, 4);
	unsigned long num = *((unsigned long*)Number);
	file_pos = input_file.tellg();

	//Read Triangle data
	TRIANGLE *data = new TRIANGLE[num];
	for (long int i = 0; i < num; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			char* temp = new char[4];
			input_file.read(temp, 4);
			file_pos = input_file.tellg();
			switch (j)
			{
			case 0:data[i].nv.x = *((float*)temp); break;
			case 1:data[i].nv.y = *((float*)temp); break;
			case 2:data[i].nv.z = *((float*)temp); break;

			case 3:data[i].p1.x = *((float*)temp); break;
			case 4:data[i].p1.y = *((float*)temp); break;
			case 5:data[i].p1.z = *((float*)temp); break;

			case 6:data[i].p2.x = *((float*)temp); break;
			case 7:data[i].p2.y = *((float*)temp); break;
			case 8:data[i].p2.z = *((float*)temp); break;

			case 9:data[i].p3.x = *((float*)temp); break;
			case 10:data[i].p3.y = *((float*)temp); break;
			case 11:data[i].p3.z = *((float*)temp); break;
			}
			delete[] temp;
		}
		char* temp = new char[2];
		input_file.read(temp, 2);
		file_pos = input_file.tellg();
		data[i].att = *((short*)temp);
		delete[] temp;
	}

	//Generate more points
	vector<POINT> pcd_point;

	for (int i = 0; i < num; i++)
	{
		int step1 = 50;
		VECTOR v1 = get_vector(data[i].p2, data[i].p1, step1);
		VECTOR v2 = get_vector(data[i].p2, data[i].p3, step1);

		for (int j = 0; j < step1 + 1; j++)
		{ 
			POINT line_1_point = get_line_point(data[i].p2, v1, j);
			POINT line_2_point = get_line_point(data[i].p2, v2, j);

			//int step2 = 50;
			VECTOR v3 = get_vector(line_1_point, line_2_point, j);
			for (int k = 0; k < j; k++)
			{
				POINT real_point = get_line_point(line_1_point, v3, k);
				pcd_point.push_back(real_point);
			}
		}
	}

	//Write out PCD
	ofstream output_file;
	output_file.open(file_pcd, ios::out);
	if (!(output_file.is_open()))
	{
		cout << "Open File Error !" << endl;
		output_file.close();
		exit(0);
	}

	const char* comment = "# .PCD v0.7 - Point Cloud data file format\n";
	output_file.write(comment, strlen(comment));

	const char* version = "VERSION 0.7\n";
	output_file.write(version, strlen(version));

	const char* fields = "FIELDS x y z\n";
	output_file.write(fields, strlen(fields));

	const char* size = "SIZE 4 4 4\n";
	output_file.write(size, strlen(size));

	const char* type = "TYPE F F F\n";
	output_file.write(type, strlen(type));

	const char* count = "COUNT 1 1 1\n";
	output_file.write(count, strlen(count));

	const char* width = "WIDTH ";
	int point_num = static_cast<int>(pcd_point.size());
	int num_len = log10(point_num) + 2;
	char* num_str = new char[num_len];
	sprintf(num_str, "%d", point_num);
	const char* enter = "\n";
	output_file.write(width, strlen(width));
	output_file.write(num_str, strlen(num_str));
	output_file.write(enter, strlen(enter));

	const char* height = "HEIGHT 1\n";
	output_file.write(height, strlen(height));

	const char* viewpoint = "VIEWPOINT 0 0 0 1 0 0 0\n";
	output_file.write(viewpoint, strlen(viewpoint));

	const char* points = "POINTS ";
	output_file.write(points, strlen(points));
	output_file.write(num_str, strlen(num_str));
	output_file.write(enter, strlen(enter));

	const char* data_str = "DATA ascii\n";
	output_file.write(data_str, strlen(data_str));

	for (unsigned int i = 0; i < point_num; i++)
	{
		char* pc_data_str = new char[45];
		POINT p = pcd_point[i];
		sprintf(pc_data_str, "%f %f %f\n", p.x, p.y, p.z);

		output_file.write(pc_data_str, strlen(pc_data_str));
		delete[] pc_data_str;
	}

	cout << "Finish~~~" << endl;

	delete[] Header;
	delete[] Number;
	delete[] data;
	delete[] num_str;
	input_file.close();
	output_file.close();
}

POINT get_line_point(POINT p, VECTOR vec, int index)
{
	POINT line_point;
	line_point.x = p.x + vec.x * index;
	line_point.y = p.y + vec.y * index;
	line_point.z = p.z + vec.z * index;

	return line_point;
}

VECTOR get_vector(POINT beg, POINT end, int step)
{
	VECTOR vec;
	vec.x = (end.x - beg.x) / step;
	vec.y = (end.y - beg.y) / step;
	vec.z = (end.z - beg.z) / step;

	return vec;
}
