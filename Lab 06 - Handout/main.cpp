//
//  main.cpp
//  Lab 06 Code Handout
//
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

using MyStruct = struct
{
	int m;
	float x;
};

int main(int argc, const char * argv[])
{
	vector<MyStruct> v;
	//--------------------------
	//	File Input reminder
	//--------------------------
	ifstream inFile;
	inFile.open("data.txt");
	cout << "Reading from the file... ";
	int n;
	inFile >> n;
	cout << n << " data points:" << endl;

	for (int k=0; k<n; k++)
	{
		float x;
		int m;
		inFile >> x >> m;
		// cout << "\t" << m << "  " << x << endl;
		MyStruct myStruct;
		myStruct.m = m;
		myStruct.x = x;
		v.push_back(myStruct);
	}
	inFile.close();

	vector<MyStruct> subVec;

	for(auto const& item : v)
	{
		if(item.m % 2 == 0)
		{
			MyStruct s;
			s.m = item.m;
			s.x = item.x * 10;
			subVec.push_back(s);
		}
	}

	for(auto const& item : v)
	{
		cout << "x: " << item.x << "   " << "m: " << item.m << endl;
	}

	cout << "sublist" << endl;

	for(auto const& subItem : subVec)
	{
		cout << "x: " << subItem.x << "   " << "m: " << subItem.m << endl;
	}

	return 0;
}
