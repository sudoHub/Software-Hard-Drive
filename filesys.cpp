#include <iostream>
#include<string>
#include<sstream>
#include <vector>
#include<fstream>

#include "filesys.h"
#include "sdisk.h"
#include "block.h"


using namespace std;
/*
* Filesysy
*/
Filesys::Filesys(string filename, int numberofblocks, int blocksize) : Sdisk(filename, numberofblocks, blocksize)
{
    string buffer;
    int code = getblock(1, buffer);

    if (code == 0)
    {
        cout<< "bad device"<<endl;
        return;
    }

    rootsize = getblocksize()/12;
    
    fatsize =(getnumberofblocks() * 6 )/ getblocksize() + 1;

    code = getblock(1, buffer);

    if (buffer[0]== '#')
    {
        buildfs();
    }
    else
    {
        readfs();
    }
    fssynch();
}

/*
* buildfs
*/
int Filesys::buildfs() 
{
    ostringstream ostream;
    for (int i = 0; i < rootsize; i++)
    {
        filename.push_back("XXXXX"); 

        firstblock.push_back(0); 
        ostream <<"XXXXX" << " " << 0 << " ";
    }

    //FAT
    string buffer = ostream.str();
    vector<string>blocks=block(buffer,getblocksize());
    putblock(1,blocks[0]);

    ostringstream ostream2;
    fat.push_back(fatsize + 2);
    fat.push_back(1);

    for(int i =0; i< fatsize ; i++)
    {
        fat.push_back(-1);
    }

    for (int i = fatsize + 2; i < getnumberofblocks(); i++)
    {
        fat.push_back(i + 1);
    }

    fat[fat.size() - 1] = 0;
    for(int i =0; i < fat.size(); i++)
    {
        ostream2 << fat[i] << " ";
    }

    string buffer2 = ostream2.str();
    vector<string>blocks2=block(buffer2,getblocksize());
    for(int i = 0; i <blocks2.size(); i ++)
    {
        putblock(2 + i,blocks2[i]);
    }
    return 0;
}

/*
* newfile
*/
int Filesys::newfile(string file) 
{
    for (int i = 0; i < filename.size(); i++)
    { 
        if (filename[i] == file)
        {
            cout << "file exist" << endl;
            return 0;
        }
    }
    // file name dont exist creat a file
    for (int i = 0; i < filename.size(); i++)
    {
        if (filename [i]=="XXXXX") 
        {
            filename[i] = file;
            firstblock[i]=0;
            fssynch();
            return 1;

        }
    }

    return 0;
}

int Filesys::addblock(string file, string block) //-------------------------------------------
{
    int blockid = getfirstblock(file);
    if (blockid == -1)
    {
        return 0;
    }

    int allocate = fat[0];
    if (allocate == 0)
    {
        cout << "Disk Full" << endl;
        return 0;
    }
    fat[0] = fat[fat[0]];
    fat[allocate] = 0;   
    
    if (blockid == 0)
    {
        for (int i = 0; i < rootsize; i++)
        {
            if (filename[i] == file)
            {
                firstblock[i] = allocate;
            }
        }
    }
    else
    {
        int b =blockid;
        while (fat[b] !=0)
        {
            b =fat[b];
        }
    }
    
    putblock(allocate, block);
    fssynch();
    return allocate;
}

/*
*   delblock
*/
int Filesys::delblock(string file, int blocknumber) //-----------------------------------------------
{

    if (!checkblock(file, blocknumber))
    {
        cout << "error in del block" << endl;
        return 0;
    }

    int block = getfirstblock(file);
    if (block == blocknumber)
    {
        for (int i = 0; i < filename.size(); i++)
        {
            if (filename[i] == file)
            {
                firstblock[i] = fat[blocknumber];
            }
        }
    }
    else
    {
        int b = block;
        while (fat[b] != blocknumber)
        {
            b = fat[b];
        }
        fat[b] = fat[blocknumber];
    }

    fat[blocknumber] = fat[0];
    fat[0] = blocknumber;
    fssynch();
    return 1;
}

/*
*   fssynch
*/
int Filesys::fssynch()
{
    ostringstream ostream;
    for (int i =0; i < rootsize; i ++)
    {
        ostream << filename[i] << " " << firstblock[i] << " ";
    }

    string buffer1 = ostream.str();

    vector<string>block1 = block(buffer1,getblocksize());
    putblock(1,block1[0]) ;
                           
    ostringstream ostream2;
    for (int i =0; i < fat.size(); i ++)
    {
        ostream2 << fat[i]<<" ";
    }

    string buffer2 = ostream2.str();
    vector<string>block2 = block(buffer2,getblocksize());

    for (int i =0; i < block2.size(); i ++)
    {
        putblock(2 + i, block2[i]);
    }
    return 1;
}

/*
*   readfs
*/
int Filesys::readfs() 
{
    istringstream istream;
    string buffer,buffer2;
    getblock(1, buffer); 
    string tempbuffer;
    istream.str(buffer);

    for (int i = 0; i < rootsize; i++)
    {
        getblock(2+i, tempbuffer);   
        buffer2 += tempbuffer;
        tempbuffer.clear();
    }
    istringstream istream1, istream2;
    istream1.str(buffer);
    istream2.str(buffer2); 
    
    for (int i = 0; i <rootsize ; i++)
    {
        string f;
        int n;
        istream1 >> f >> n; 
        filename.push_back(f);
        firstblock.push_back(n);
    }
   
   //fat
   int k;
    for (int i = 0; i < getnumberofblocks(); i++)
    {
        istream2 >> k;
        fat.push_back(k);
    }
    return 1;
} 

/*
*  checkblock
*/
 int Filesys::getfirstblock(string file) 
{
    for (int i = 0; i < filename.size(); i++)
    {
        if (filename[i] == file)
        {
            return firstblock[i];
        }
     }
     return -1; 
 }

/*
*  checkblock
*/
bool Filesys::checkblock(string file, int block) 
{                                                
    int blockid = getfirstblock(file);
    
    if (blockid == -1)
    {
        return false;
    }
    
    while (blockid != 0)
    {
        if (blockid == block)
        {
            return true;
        }
        blockid = fat[blockid];
    }
    return true;
}

/*
*  readblock
*/
int Filesys::readblock(string file, int blocknumber, string& buffer) 
{
    if (checkblock (file, blocknumber) )
    {
        getblock(blocknumber, buffer);
         return 1;
    }
    else
    {
        return 0;
    }
}


/*
*   writeblock
*/
int Filesys::writeblock(string file, int blocknumber, string buffer) 
{
    if (checkblock (file, blocknumber) )
    {
        putblock(blocknumber, buffer);
         return 1;
    }
    else
    {
        return 0;
    }
}

/*
*   nextblock
*/
int Filesys::nextblock(string file , int blocknumber) 
{
    if (checkblock (file, blocknumber))
    {
        return fat[blocknumber];
    }
    else   
    {
        return -1;
    }

}

/*
*   rmfile
*/
int Filesys::rmfile(string file)
{
    for( int i  =0; i < filename.size(); i++)
    {
        if(filename[i]==file)
        {
            if(firstblock[i]==0)
            {
                filename[i] = "XXXXX";
                fssynch();
                return 1;
            }
        }
    }
    return 0;
}

/*
*   ls
*/
vector<string> Filesys::ls()
{ 
    vector <string> flist;

  for (int i=0; i<filename.size(); i++)
    {
    if (filename[i] != "XXXXX")
    {
        flist.push_back(filename[i]);
    }
    }
  return flist;
}





















