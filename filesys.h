#ifndef FILESYS_H
#define FILESYS_H
#include<iostream> 
#include<fstream>
#include<string>
#include<vector>
#include<sstream>
#include <string.h>
#include "sdisk.h"
#include "block.h"
 
 using namespace std;
 


class Filesys: public Sdisk
{
 public:
 
 Filesys(string diskname, int numberofblockss, int blocksize);
 int fsclose();
 int newfile(string file);
 int rmfile(string file);
 int getfirstblock(string file);
 int addblock(string file, string block);
 int delblock(string file, int blocknumber);

 int readblock(string file, int blocknumber, string & buffer);
 int writeblock(string file, int blocknumber, string buffer);

 int nextblock(string file , int blocknumber);
 bool checkblock(string file, int block);
 vector<string>ls();

 private:
 int rootsize;// maximun entrries i the root ;
 int fatsize;// number of blocks acccupied by fat this is the number of blocks to store FAT;
 vector<string> filename;// filenames in root
 vector<int>firstblock; // firstblock in root
 vector<int>fat; // FAt the the nimbe of blocks is the Sdisk
 int buildfs(); // builds the file system
 int readfs(); // reads the file system or void for buildfs and readfs.....m
 int fssynch();// writes the FAT and ROOT to the Sdisk)
 

};
#endif


