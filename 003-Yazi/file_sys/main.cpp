#include <iostream>

#include <fs/file.h>
#include <fs/directory.h>

using namespace demo::fs;

int main()
{
//    File f("./../test.txt");
//    std::cout << f.path() << std::endl;

//    File f("./../test/a/b/c/test.txt");
//    std::cout << f.create() << std::endl;

//    Directory dir("./../test/a/b/c");
//    std::cout << dir.path() << std::endl;

//    Directory dir("D://cpp/hello/x/./../test////a/b/c");
//    Directory dir("D://cpp/hello/x/y/z/./../test////a/b/c");
//    std::cout << dir.path() << std::endl;

// Beware the output path may different by using Cygwin instead of MinGW

//    Directory dir("./../test/a/b/c");
//    std::cout << dir.path() << std::endl;
//    dir.create();

//    Directory dir("./../test/a/b/c");
//    std::cout << dir.path() << std::endl;
//    dir.remove();

//    Directory dir("./../test");
//    std::cout << dir.path() << std::endl;
//    dir.remove();

////    Directory dir("./../test/test/a/b/c");
////    std::cout << dir.create() << std::endl;
//    Directory dir("./../test/");
//    dir.clear();
//    Directory dir("./../test/");
//    dir.remove();

//    Directory dir("./../fs");
//    std::vector<File> files = dir.file();
//    for (auto f: files)
//    {
//        std::cout << f.path() << std::endl;
//    }

    Directory dir("./../fs");
    dir.copy("./../test");

    return 0;
}
