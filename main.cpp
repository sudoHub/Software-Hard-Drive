
// You can use this to test your Sdisk class nclu

#include<fstream>
#include<iostream>
#include<vector>
#include<sstream>
#include <string>

#include "filesys.h"
#include "sdisk.h"
#include "shell.h"
#include "block.h"
#include "table.h"
using namespace std;
 
int main()
{
 //
 //This main program inputs commands to the shell.
 //It inputs commands as : command op1 op2
 //You should modify it to work for your implementation.
 //
 Shell shell = Shell("diskdata.txt",256,128);
 //Table table = Table("diskdata.txt",256,128,"flatfile","indexfile");

 //table.build_table("records.txt");

 string s;
 string command="go";
 string op1,op2;
 

 while (command != "quit")
     {
       command.clear();
       op1.clear();
       op2.clear();
       cout << "$";

       getline(cin,s);
       int firstblank=s.find(' ');

       if (firstblank < s.length()) s[firstblank]='#';

       int secondblank=s.find(' ');
       command=s.substr(0,firstblank);

       if (firstblank < s.length())
         op1=s.substr(firstblank+1,secondblank-firstblank-1);
       if (secondblank < s.length())
         op2=s.substr(secondblank+1);
       if (command=="dir")
          {
            shell.dir();
            
           }
       if (command=="add")
          {
            shell.add(op1,op2);
           }
       if (command=="del")
          {
            shell.del(op1);
           }
       if (command=="type")
          {
            shell.type(op1);
           }
       if (command=="copy")
          {
            shell.copy(op1,op2);
           }
       if (command=="search")
          {
            table.search(op1);
          }
       
      }

 return 0;
}



