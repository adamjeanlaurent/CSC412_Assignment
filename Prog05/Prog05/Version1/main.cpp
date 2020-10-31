#include <iostream>
#include <unistd.h>
#include <string>
#include <cstdio>
#include <vector>

using namespace std;

int main(int argc, char** argv)
{
    // parse input arguments
    if(argc != 4)
    {
        // invalid arguments
        cout << "usage: " << argv[0] << " <pathToJobFile> <pathToImages> <pathToOutput>" << endl;
        return 0;
    }   

    string jobFilePath(argv[1]);
    string pathToImages(argv[2]);
    string pathToOutput(argv[3]);

   // read job file


    return 0;
}