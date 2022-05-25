#############################################################################
# Copyright (c) Focus Corporation 2021
# All rights reserved
#
# The reproduction, transmission or use of this document or its contents is
# not permitted without express written authority.
# Offenders will be liable for damages. All rights, including rights created
# by patent grant or registration of a utility model or design, are reserved.
#############################################################################

#---------------------------------------------------------------------------------
# Directories, file names
#---------------------------------------------------------------------------------
LIBTCP = libTCPModule.so
SRCDIR = ./src
INCDIR = ./inc
PUBDIR = ./pub
BINDIR = ./bin
DOCDIR = ./doc
TESTDIR = ./test
INSTALLPATH = /usr/lib
AbstractionINCPath = ../../eth-core-infrastructure/network-stack-abstraction/pub
TestabilityINCPath = ../../eth-core-infrastructure/control-channel/inc
SOURCES := $(wildcard $(SRCDIR)/*.c)
TESTS := $(wildcard $(TESTDIR)/*.c)
OBJECTS := $(patsubst $(SRCDIR)/%.c, $(BINDIR)/%.o, $(SOURCES))
TESTEXE := $(patsubst $(TESTDIR)/%.c, $(BINDIR)/%, $(TESTS))

#---------------------------------------------------------------------------------
# Flags, libraries and Includes
#---------------------------------------------------------------------------------
CC = g++
LIBRARIES = -lAbstractionAPI -lTestabilityProtocol -lpthread
LDFLAGS = -shared
CFLAGS = -std=c++11 -g -fpermissive -I$(INCDIR)/ -I$(PUBDIR)/ -I$(AbstractionINCPath) -I$(TestabilityINCPath)
CFLAGS_LIB = $(CFLAGS) -fpic

#---------------------------------------------------------------------------------
# Makefile rules
#---------------------------------------------------------------------------------

all: $(LIBTCP)

tests: clean $(OBJECTS) $(TESTEXE)

b: clean all install tests

$(LIBTCP): $(OBJECTS)
	@echo "Creating TCP library..."
	@$(CC) $(OBJECTS) $(LDFLAGS) -o $(LIBTCP)
	@echo "Library created."

$(TESTEXE): $(BINDIR)/% : $(TESTDIR)/%.c
	@echo "Creating test executable '$@'..."
	@$(CC) $(CFLAGS) $< $(BINDIR)/*.o -o $@ $(LIBRARIES)
	@echo "Executable created."

$(OBJECTS): $(BINDIR)/%.o : $(SRCDIR)/%.c
	@echo "Creating object file '$@'..."
	@mkdir -p $(BINDIR)
	@$(CC) $(CFLAGS_LIB) -c $< -o $@
	@echo "Object created successfully."

install: $(LIBTCP)
	sudo cp $(LIBTCP) $(INSTALLPATH)
	echo "Library installed under '$(INSTALLPATH)'"

uninstall:
	@rm -rf $(INSTALLPATH)/$(LIBTCP)
	@echo "Library uninstalled"

clean:
	@echo "Removing binaries..."
	@rm -rf $(BINDIR) $(OBJECTS)
	@echo "Removing library..."
	@rm -f $(LIBTCP)
	@echo "Deleting test executables..."
	@rm -f $(TESTEXE)
	@echo "Cleaning is done."
