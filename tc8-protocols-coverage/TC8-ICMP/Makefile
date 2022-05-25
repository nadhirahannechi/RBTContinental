#############################################################################
# Copyright (c) Focus Corporation 2021
# All rights reserved
#
# The reproduction, transmission or use of this document or its contents is
# not permitted without express written authority.
# Offenders will be liable for damages. All rights, including rights created
# by patent grant or registration of a utility model or design, are reserved.
#############################################################################

AbstractionAPI = I../../eth-core-infrastructure/network-stack-abstraction/pub

all: object_files
	g++ -shared -o libICMPModule.so bin/*.o
#	cp ./pub/* ../../frontend-com-module/com-module/LinuxComModule/inc/TC8-ICMP
	mkdir -p lib
	mv libICMPModule.so ./lib

object_files:
	g++ -std=c++11 -c -fPIC src/*.c -$(AbstractionAPI) -lAbstractionAPI -Iinc -Ipub 
	mkdir -p bin
	mv *.o bin/

install: ./lib/libICMPModule.so
	sudo cp ./lib/libICMPModule.so /usr/lib

uninstall:
	sudo rm -rf /usr/lib/libICMPModule.so

tests: object_files
	g++ -$(AbstractionAPI) -g -std=c++11 test/TEST_ICMP_ERROR.c bin/*.o -I./pub -I./inc -o TEST_ICMP_ERROR -lAbstractionAPI
	g++ -$(AbstractionAPI) -g -std=c++11 test/TEST_ICMP_TYPE.c bin/*.o -I./pub -I./inc -o TEST_ICMP_TYPE -lAbstractionAPI

clean:
	rm -rf bin lib *.o *.so
	rm -rf TEST_ICMP_ERROR
	rm -rf TEST_ICMP_TYPE
