Steps to build the project:

	1. Be sure that  the vsomeip stack is installed successfully

	2. Be sure that the network abstraction lib is intalled

	3. Open a terminal in the currect  folder and run the following  commands:
		
	
		$ mkdir build
		$ cd build/
		$ cmake ..
		$ make

These steps will create the SD_API shared library  and then create the binaries needed for the tests

to  run the tests you need to run : 

		$ ./SomeIpTesting 

then follow the steps shown on the screen
