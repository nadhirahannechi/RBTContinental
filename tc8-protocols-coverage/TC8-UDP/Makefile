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
	g++ -shared -o libUDPModule.so src/*.c  -$(testability) -$(AbstractionAPI) -Iinc -Ipub -std=c++11 -fPIC -fpermissive

install: libUDPModule.so
	sudo cp libUDPModule.so /usr/lib

uinstall:
	sudo rm -rf /usr/lib/libUDPModule.so

tests:
	g++ -std=c++11 test/main.c -lpthread -$(testability) -$(AbstractionAPI) -Ipub -Iinc -lUDPModule -lAbstractionAPI -lTestabilityProtocol -fpermissive -o main

clean:
	rm -rf bin *.o *.so main

