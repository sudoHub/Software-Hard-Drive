project:sdisk.o filesys.o main.o block.o shell.o table.o
	c++ -o project2 sdisk.o filesys.o main.o block.o shell.o table.o
sdisk.o:sdisk.cpp
	c++ -c sdisk.cpp	

filesys.o:filesys.cpp
	c++ -c filesys.cpp
	
block.o:block.cpp
	c++ -c block.cpp
	
shell.o:shell.cpp
	c++ -c shell.cpp

table.o:table.cpp
	c++ -c table.cpp	

main.o:main.cpp
	c++ -c main.cpp

clean:
	rm *.o			
