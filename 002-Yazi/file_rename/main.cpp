#include <iostream>
#include <dirent.h>
#include <sys/types.h>
#include <ctype.h>

#include <vector>
#include <string>
#include <sstream>

using namespace std;

int main(void)
{
    std::string path = "C:\\Users\\tyeung\\Desktop\\20230915聖文德書院";

    DIR * dr;
    struct dirent * en;
    dr = opendir(path.c_str()); //open all directory
    if (dr)
    {
        while ((en = readdir(dr)) != NULL)
        {
//            cout << " \n" << en->d_name; //print all directory name

            std::stringstream ss;
            ss << en->d_name;

            std::string name;
            ss >> name;

            if ("." == name.substr(0, 1) || !std::isdigit(name[0]))
                continue;

            std::string new_name = name.substr(0, 2) + '-' + name.substr(2, 2) + ".jpg";
            string old_path = path + "\\" + name;
            string new_path = path + "\\" + new_name;

            std::cout << old_path << "\n";
            std::cout << new_path << "\n";

            std::rename(old_path.c_str(), new_path.c_str());
        }
        closedir(dr); //close all directory
    }

    return (0);
}