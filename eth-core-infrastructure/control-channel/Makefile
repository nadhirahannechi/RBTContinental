#############################################################################
# Copyright (c) Focus Corporation 2021
# All rights reserved
#
# The reproduction, transmission or use of this document or its contents is
# not permitted without express written authority.
# Offenders will be liable for damages. All rights, including rights created
# by patent grant or registration of a utility model or design, are reserved.
#############################################################################

CC = gcc
CFLAGS= -Wextra -Wall -g
LDFLAGS= -lpthread

.PHONY: all clean install uninstall doc

CFLAGS_LIB= -Wextra -g -DFEATURE_TESTER_SIDE -fPIC

THIS_DIR := $(dir $(abspath $(firstword $(MAKEFILE_LIST))))
DOC_DIR  := $(THIS_DIR)doc
SRC_DIR  := $(THIS_DIR)src
INC_DIR  := $(THIS_DIR)inc

OBJ_FILES_DUT= TestabilityProtocol_Intern_DUT.o TestabilityProtocol_DUT.o

OBJ_FILES_LIB= TestabilityProtocol_Intern_Lib.o TestabilityProtocol_Lib.o

all: libTestabilityProtocol.so DUT

libTestabilityProtocol.so: $(OBJ_FILES_LIB)
	@echo "Creating TestabilityProtocol.so shared library.."
	$(CC) -shared -o $@ $(OBJ_FILES_LIB) $(CFLAGS_LIB) $(LDFLAGS) 

TestabilityProtocol_Intern_Lib.o : $(SRC_DIR)/TestabilityProtocol_Intern.c 
	@echo "Compiling file: TestabilityProtocol_Intern.c for the TestabilityProtocol shared library.."
	$(CC) -c  $< -o $@ $(CFLAGS_LIB) $(LDFLAGS) -I$(INC_DIR) 

TestabilityProtocol_Lib.o : $(SRC_DIR)/TestabilityProtocol.c 
	@echo "Compiling file: TestabilityProtocol.c for the TestabilityProtocol shared library.."
	$(CC) -c  $< -o $@ $(CFLAGS_LIB) $(LDFLAGS) -I$(INC_DIR) 


DUT: $(OBJ_FILES_DUT)
	@echo "Creating DUT binary.."
	$(CC) -o $@ $(OBJ_FILES_DUT) $(CFLAGS) $(LDFLAGS)

TestabilityProtocol_Intern_DUT.o : $(SRC_DIR)/TestabilityProtocol_Intern.c 
	@echo "Compiling file: TestabilityProtocol_Intern.c for the DUT binary.."
	$(CC) -c  $< -o $@ $(CFLAGS) $(LDFLAGS) -I$(INC_DIR) 

TestabilityProtocol_DUT.o : $(SRC_DIR)/TestabilityProtocol.c 
	@echo "Compiling file: TestabilityProtocol.c for the DUT binary.."
	$(CC) -c  $< -o $@ $(CFLAGS) $(LDFLAGS) -I$(INC_DIR) 

install:
	sudo cp ./libTestabilityProtocol.so /usr/lib
	sudo cp ./DUT /usr/bin
	sudo cp ./inc/*.h /usr/include

uninstall:
	sudo rm  /usr/lib/libTestabilityProtocol.so
	sudo rm  /usr/include/TestabilityProtocol*
	sudo rm  /usr/bin/DUT
clean:
	@echo "Deleting the generated files.."
	rm -rf DUT *.so *.o 
	cd $(DOC_DIR) && find . ! -name TP_config ! -name .gitkeep -delete
doc:
	@echo "Generating documentation files.."
	cd $(DOC_DIR) && doxygen TP_config