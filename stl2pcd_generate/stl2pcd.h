#pragma once
class POINT
{
public:
	POINT();
	~POINT();
	float x;
	float y;
	float z;
};

class VECTOR
{
public:
	VECTOR();
	~VECTOR();
	float x;
	float y;
	float z;
};

class TRIANGLE
{
public:
	TRIANGLE();
	~TRIANGLE();
	VECTOR nv;	//normal vector
	POINT p1;	//vertex 1
	POINT p2;	//vertex 2
	POINT p3;	//vertex 3
	short att;		//attribute byte count
};

void stl2pcd(const char* file_stl, const char* file_pcd);

POINT get_line_point(POINT p, VECTOR vec, int index);

VECTOR get_vector(POINT beg, POINT end, int step);