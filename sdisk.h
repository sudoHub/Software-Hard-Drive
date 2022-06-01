


#ifndef SDISK_H 
 #define SDISK_H
#include<iostream> 
 #include<fstream>
 #include<string>
 #include<vector>
 using namespace std;

  class Sdisk
  {
    public:

      Sdisk(string diskname,int numberofblocks,int blocksize);
      int getnumberofblocks();
      int getblocksize();
      int getblock(int blocknumber, string& buffer); 
      int putblock(int blocknumber, string  buffer);

    private:
      string diskname;
      int numberofblocks;
      int blocksize; 
};
 #endif
  

