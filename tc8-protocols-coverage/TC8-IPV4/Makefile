#############################################################################
# Copyright (c) Focus Corporation 2021
# All rights reserved
#
# The reproduction, transmission or use of this document or its contents is
# not permitted without express written authority.
# Offenders will be liable for damages. All rights, including rights created
# by patent grant or registration of a utility model or design, are reserved.
#############################################################################

Testability = I../../eth-core-infrastructure/control-channel/inc
AbstractionAPI = I../../eth-core-infrastructure/network-stack-abstraction/pub

all: object_files
	g++ -shared -Iinc -Ipub -o libIPv4Module.so bin/*
#	cp ./pub/* ../../frontend-com-module/com-module/LinuxComModule/inc/TC8-IPV4
	mkdir -p lib
	mv libIPv4Module.so ./lib

object_files:
	g++ -std=c++11 -c -fPIC src/*.c -$(AbstractionAPI) -lAbstractionAPI -$(Testability) -lTestabilityProtocol -Iinc -Ipub
	mkdir -p bin
	mv *.o bin/

install: ./lib/libIPv4Module.so
	sudo cp ./lib/libIPv4Module.so /usr/lib

uninstall:
	sudo rm -rf /usr/lib/libIPv4Module.so

tests: object_files
	g++ -$(AbstractionAPI) -$(Testability) -g -std=c++11 test/TEST_IPv4_ADDRESSING.c bin/*.o -Iinc -Ipub -o TEST_IPv4_ADDRESSING -lAbstractionAPI -lTestabilityProtocol
	g++ -$(AbstractionAPI) -$(Testability) -g -std=c++11 test/TEST_IPv4_CHECKSUM.c bin/*.o -Iinc -Ipub -o TEST_IPv4_CHECKSUM -lAbstractionAPI -lTestabilityProtocol
	#g++ -$(AbstractionAPI) -$(Testability) -g -std=c++11 test/TEST_IPv4_AUTOCONF_ADDRESS_SELECTION.c bin/*.o -Iinc -Ipub -o TEST_IPv4_AUTOCONF_ADDRESS_SELECTION -lAbstractionAPI -lTestabilityProtocol
	#g++ -$(AbstractionAPI) -$(Testability) -g -std=c++11 test/TEST_IPv4_AUTOCONF_ANNOUNCING.c bin/*.o -Iinc -Ipub -o TEST_IPv4_AUTOCONF_ANNOUNCING -lAbstractionAPI -lTestabilityProtocol
	#g++ -$(AbstractionAPI) -$(Testability) -g -std=c++11 test/TEST_IPv4_AUTOCONF_CONFLICT.c bin/*.o -Iinc -Ipub -o TEST_IPv4_AUTOCONF_CONFLICT -lAbstractionAPI -lTestabilityProtocol
	#g++ -$(AbstractionAPI) -$(Testability) -g -std=c++11 test/TEST_IPv4_AUTOCONF_INTRO.c bin/*.o -Iinc -Ipub -o TEST_IPv4_AUTOCONF_INTRO -lAbstractionAPI -lTestabilityProtocol
	#g++ -$(AbstractionAPI) -$(Testability) -g -std=c++11 test/TEST_IPv4_AUTOCONF_LINKLOCAL_PACKETS.c bin/*.o -Iinc -Ipub -o TEST_IPv4_AUTOCONF_LINKLOCAL_PACKETS -lAbstractionAPI -lTestabilityProtocol
	#g++ -$(AbstractionAPI) -$(Testability) -g -std=c++11 test/TEST_IPv4_AUTOCONF_NETWORK_PARTITIONS.c bin/*.o -Iinc -Ipub -o TEST_IPv4_AUTOCONF_NETWORK_PARTITIONS -lAbstractionAPI -lTestabilityProtocol
	g++ -$(AbstractionAPI) -$(Testability) -g -std=c++11 test/TEST_IPv4_FRAGMENTS.c bin/*.o -Iinc -Ipub -o TEST_IPv4_FRAGMENTS -lAbstractionAPI -lTestabilityProtocol
	g++ -$(AbstractionAPI) -$(Testability) -g -std=c++11 test/TEST_IPv4_HEADER.c bin/*.o -Iinc -Ipub -o TEST_IPv4_HEADER -lAbstractionAPI -lTestabilityProtocol
	g++ -$(AbstractionAPI) -$(Testability) -g -std=c++11 test/TEST_IPv4_REASSEMBLY.c bin/*.o -Iinc -Ipub -o TEST_IPv4_REASSEMBLY -lAbstractionAPI -lTestabilityProtocol
	g++ -$(AbstractionAPI) -$(Testability) -g -std=c++11 test/TEST_IPv4_TTL.c -Iinc -Ipub -o bin/*.o -o TEST_IPv4_TTL -lAbstractionAPI -lTestabilityProtocol
	g++ -$(AbstractionAPI) -$(Testability) -g -std=c++11 test/TEST_IPv4_VERSION.c bin/*.o -Iinc -Ipub -o TEST_IPv4_VERSION -lAbstractionAPI -lTestabilityProtocol

clean:
	rm -rf bin lib *.o *.so
	rm -rf TEST_IPv4_ADDRESSING
	rm -rf TEST_IPv4_CHECKSUM
	#rm -rf TEST_IPv4_AUTOCONF_ADDRESS_SELECTION
	#rm -rf TEST_IPv4_AUTOCONF_ANNOUNCING
	#rm -rf TEST_IPv4_AUTOCONF_CONFLICT
	#rm -rf TEST_IPv4_AUTOCONF_INTRO
	#rm -rf TEST_IPv4_AUTOCONF_LINKLOCAL_PACKETS
	#rm -rf TEST_IPv4_AUTOCONF_NETWORK_PARTITIONS
	rm -rf TEST_IPv4_FRAGMENTS
	rm -rf TEST_IPv4_HEADER
	rm -rf TEST_IPv4_REASSEMBLY
	rm -rf TEST_IPv4_TTL
	rm -rf TEST_IPv4_VERSION


