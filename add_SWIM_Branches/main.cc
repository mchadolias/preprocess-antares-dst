#include <string>
#include <iostream>
#include "addBranches.h"
#include "addCanANTARES.h"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        cout << "Usage: " << argv[0] << " <rootfile>" << endl;
        return 1;
    }

    string input_file = argv[1];
    string output_file = argv[2];

    addBranches(input_file, output_file);
    addCanANTARES(output_file);

    return 0;
}


    