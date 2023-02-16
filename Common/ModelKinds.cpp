#include "ModelKinds.h"
#include <fstream>

std::vector<std::string> GetObjStr()
{
	std::vector<std::string> result;

    std::string line;
    std::ifstream myfile("../Models/ImplementedFileList.ipl");
    if (myfile.is_open())
    {
        while (std::getline(myfile, line))
        {
            result.push_back(line);
        }
        myfile.close();
    }

	return result;

}
