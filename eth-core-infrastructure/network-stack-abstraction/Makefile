#############################################################################
# Copyright (c) Focus Corporation 2021
# All rights reserved
#
# The reproduction, transmission or use of this document or its contents is
# not permitted without express written authority.
# Offenders will be liable for damages. All rights, including rights created
# by patent grant or registration of a utility model or design, are reserved.
#############################################################################

CPP= g++
CPP_FLAGS := -std=c++11 -g
LD_FLAGS := -lAbstractionAPI
THIS_DIR := $(dir $(abspath $(firstword $(MAKEFILE_LIST))))
DOC_DIR  := $(THIS_DIR)doc
SRC_DIR  := $(THIS_DIR)src
INC_DIR  := $(THIS_DIR)inc
PUB_DIR  := $(THIS_DIR)pub


all: object_files
	mkdir -p lib
	$(CPP) -shared -o lib/libAbstractionAPI.so obj/*.o

install:
	ls -al ./lib/
	ls -al /usr
	cp ./lib/libAbstractionAPI.so /usr/lib

uninstall:
	sudo rm  /usr/lib/libAbstractionAPI.so

test:
	mkdir -p bin
	$(CPP) $(CPP_FLAGS)    -I$(PUB_DIR) test_apis/TCP_API_Test.cpp $(LD_FLAGS)   -o bin/TCP_API_Test
	$(CPP) $(CPP_FLAGS)    -I$(PUB_DIR) test_apis/ICMP_API_Test.cpp $(LD_FLAGS)   -o bin/ICMP_API_Test
	$(CPP) $(CPP_FLAGS)    -I$(PUB_DIR) test_apis/ICMP_Timestamp_API_Test.cpp $(LD_FLAGS)   -o bin/ICMP_Timestamp_API_Test
	$(CPP) $(CPP_FLAGS)    -I$(PUB_DIR) test_apis/IP_API_Test.cpp $(LD_FLAGS)   -o bin/IP_API_Test
	$(CPP) $(CPP_FLAGS)    -I$(PUB_DIR) test_apis/UDP_API_Test.cpp $(LD_FLAGS)   -o bin/UDP_API_Test
	$(CPP) $(CPP_FLAGS)    -I$(PUB_DIR) test_apis/DHCP_API_Test.cpp $(LD_FLAGS)   -o bin/DHCP_API_Test
	$(CPP) $(CPP_FLAGS)    -I$(PUB_DIR) test_apis/ARP_API_Test.cpp $(LD_FLAGS)   -o bin/ARP_API_Test
	$(CPP) $(CPP_FLAGS)    -I$(PUB_DIR) test_apis/IP_OPTION_API_Test.cpp $(LD_FLAGS)   -o bin/IP_OPTION_API_Test
	$(CPP) $(CPP_FLAGS)    -I$(PUB_DIR) test_apis/IP_OPTION_API_Test2.cpp $(LD_FLAGS)   -o bin/IP_OPTION_API_Test2
	$(CPP) $(CPP_FLAGS)    -I$(INC_DIR)  test_class/test_ip_option.cpp $(LD_FLAGS)   -o bin/IP_OPTION_CLASS
	$(CPP) $(CPP_FLAGS)    -I$(INC_DIR)  test_class/test_icmp.cpp $(LD_FLAGS)   -o bin/ICMP_CLASS
	$(CPP) $(CPP_FLAGS)    -I$(INC_DIR)  test_class/test_dhcp.cpp $(LD_FLAGS)   -o bin/DHCP_CLASS

object_files:
	$(CPP) $(CPP_FLAGS) -c -fPIC $(SRC_DIR)/*.cpp -I$(INC_DIR) -I$(PUB_DIR)
	mkdir -p obj
	mv *.o obj/


clean: 
	rm -rf obj bin lib 
