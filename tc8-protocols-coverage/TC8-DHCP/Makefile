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

all: object_files
	g++ -shared -o libDHCPModule.so bin/*.o
#	cp ./pub/* ../../frontend-com-module/com-module/LinuxComModule/inc/TC8-DHCP

object_files:
	g++ -std=c++11 -c -fPIC -$(testability) -$(AbstractionAPI) -Iinc -Ipub src/*.c
	mkdir -p bin
	mv *.o bin/

install: libDHCPModule.so
	sudo cp libDHCPModule.so /usr/lib

uninstall:
	sudo rm -rf /usr/lib/libDHCPModule.so


tests:
	g++ -g -std=c++11 -Ipub -lDHCPModule -lAbstractionAPI -lTestabilityProtocol test/TEST_DHCPv4_CLIENT_ALLOCATING.c -o TEST_DHCPv4_CLIENT_ALLOCATING
	g++ -g -std=c++11 -Ipub -lDHCPModule -lAbstractionAPI -lTestabilityProtocol test/TEST_DHCPv4_CLIENT_CONSTRUCTING_MESSAGES.c  -o TEST_DHCPv4_CLIENT_CONSTRUCTING_MESSAGES
	g++ -g -std=c++11 -Ipub -lDHCPModule -lAbstractionAPI -lTestabilityProtocol test/TEST_DHCPv4_CLIENT_INITIALIZATION_ALLOCATION.c  -o TEST_DHCPv4_CLIENT_INITIALIZATION_ALLOCATION
	g++ -g -std=c++11 -Ipub -lDHCPModule -lAbstractionAPI -lTestabilityProtocol test/TEST_DHCPv4_CLIENT_PARAMETERS.c  -o TEST_DHCPv4_CLIENT_PARAMETERS
	g++ -g -std=c++11 -Ipub -lDHCPModule -lAbstractionAPI -lTestabilityProtocol test/TEST_DHCPv4_CLIENT_PROTOCOL.c  -o TEST_DHCPv4_CLIENT_PROTOCOL
	g++ -g -std=c++11 -Ipub -lDHCPModule -lAbstractionAPI -lTestabilityProtocol test/TEST_DHCPv4_CLIENT_REACQUISITION.c  -o TEST_DHCPv4_CLIENT_REACQUISITION
	g++ -g -std=c++11 -Ipub -lDHCPModule -lAbstractionAPI -lTestabilityProtocol test/TEST_DHCPv4_CLIENT_REQUEST.c  -o TEST_DHCPv4_CLIENT_REQUEST
	g++ -g -std=c++11 -Ipub -lDHCPModule -lAbstractionAPI -lTestabilityProtocol test/TEST_DHCPv4_CLIENT_SUMMARY.c  -o TEST_DHCPv4_CLIENT_SUMMARY
	g++ -g -std=c++11 -Ipub -lDHCPModule -lAbstractionAPI -lTestabilityProtocol test/TEST_DHCPv4_CLIENT_USAGE.c  -o TEST_DHCPv4_CLIENT_USAGE

clean:
	rm -rf bin *.o *.so
	rm -f TEST_DHCPv4_CLIENT_ALLOCATING
	rm -f TEST_DHCPv4_CLIENT_CONSTRUCTING_MESSAGES
	rm -f TEST_DHCPv4_CLIENT_INITIALIZATION_ALLOCATION
	rm -f TEST_DHCPv4_CLIENT_PARAMETERS
	rm -f TEST_DHCPv4_CLIENT_PROTOCOL
	rm -f TEST_DHCPv4_CLIENT_REACQUISITION
	rm -f TEST_DHCPv4_CLIENT_REQUEST
	rm -f TEST_DHCPv4_CLIENT_SUMMARY
	rm -f TEST_DHCPv4_CLIENT_USAGE


