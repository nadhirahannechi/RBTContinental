#############################################################################
# Copyright (c) Focus Corporation 2021
# All rights reserved
#
# The reproduction, transmission or use of this document or its contents is
# not permitted without express written authority.
# Offenders will be liable for damages. All rights, including rights created
# by patent grant or registration of a utility model or design, are reserved.
#############################################################################

testability = I../../eth-core-infrastructure/control-channel/inc
AbstractionAPI = I../../eth-core-infrastructure/network-stack-abstraction/pub

all:
	g++ -shared -o libARPModule.so src/*.cpp  -$(testability) -$(AbstractionAPI) -Iinc -Ipub -std=c++11 -fPIC -fpermissive
#	cp ./pub/* ../../frontend-com-module/com-module/LinuxComModule/inc/TC8-ARP

install: libARPModule.so
	sudo cp libARPModule.so /usr/lib

uinstall:
	sudo rm -rf /usr/lib/libARPModule.so

tests:
	g++ -std=c++11 test/main.cpp -$(AbstractionAPI) -Ipub -Iinc -lARPModule -lAbstractionAPI -lTestabilityProtocol -o main

clean:
	rm -rf bin *.o *.so main
