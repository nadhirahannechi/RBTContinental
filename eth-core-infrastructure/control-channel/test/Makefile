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

CFLAGS= -Wextra -g 

THIS_DIR := $(dir $(abspath $(firstword $(MAKEFILE_LIST))))

all: test

test: test.o
	$(CC) -o $@ test.o $(CFLAGS)  -L$(THIS_DIR)../  -Wl,-rpath=$(THIS_DIR)../ -lTestabilityProtocol

test.o: test.c
	$(CC) -c  $< -o $@ $(CFLAGS)  -I../inc 

clean:
	rm -rf test.o test

