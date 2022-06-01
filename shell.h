#ifndef SHELL_H
#define SHELL_H
#include<iostream> 
#include<fstream>
#include<string>
#include<vector>
#include<sstream>
#include <string.h>

#include "sdisk.h"
#include "filesys.h"
 
using namespace std;

class Shell : public Filesys
{
public:
    Shell(string diskname, int numberofblocks, int blocksize);
    int dir();
    int add(string filename,string buffer);
    int del(string file);
    int type(string file);
    int copy(string file1, string file2);
};
#endif