# Usage:
# make        # compile all binary
# make clean  # remove ALL binaries and objects
.PHONY = all clean install uninstall enable

TARGET   = EthTesterComModule

CC = g++ -std=c++11               # compiler to use

ifeq ($(PREFIX),)
    PREFIX := /usr/bin
endif



LINKERFLAG = -lnsl -pthread -lTCPModule -lAbstractionAPI -lTestabilityProtocol -lIPv4Module -lICMPModule -lARPModule -lDHCPModule -lUDPModule -lSOMEIPModule
#INC = -I./inc/ -I./inc/TC8-ARP/ -I./inc/TC8-DHCP/ -I./inc/TC8-ICMP/ -I./inc/TC8-IPV4/ -I./inc/TC8-SOMEIP/ -I./inc/TC8-TCP/ -I./inc/TC8-UDP/ -I./inc/TC8-AbstractionAPI

INC = -I./inc/ -I../../../tc8-protocols-coverage/TC8-ARP/pub/ -I../../../tc8-protocols-coverage/TC8-DHCP/pub -I../../../tc8-protocols-coverage/TC8-ICMP/pub/ -I../../../tc8-protocols-coverage/TC8-IPV4/pub/ -I../../../tc8-protocols-coverage/TC8-SOMEIP/pub/ -I../../../tc8-protocols-coverage/TC8-TCP/pub/ -I../../../tc8-protocols-coverage/TC8-UDP/pub/  -I../../../eth-core-infrastructure/network-stack-abstraction/pub  -I../../../eth-core-infrastructure/control-channel/inc

SRCDIR   = src
OBJDIR   = ./obj
INCDIR   = inc
BINDIR   = ./bin

SRCS := $(wildcard $(SRCDIR)/*.cpp)
DEPS := $(wildcard $(INCDIR)/*.h)
OBJECTS  := $(SRCS:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

all: $(BINDIR)/$(TARGET)

$(BINDIR)/$(TARGET): $(OBJECTS)
		mkdir -p $(BINDIR)
		${CC} -o $@  $(OBJECTS) ${LINKERFLAG}  
		
$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
		mkdir -p $(OBJDIR)
		${CC} -c -o $@ $< $(INC) -Wall -g


clean:
		rm -rvf *.o $(OBJDIR)/* $(BINDIR)/$(TARGET) $(OBJDIR) $(BINDIR)

install: $(BINDIR)/$(TARGET)
		sudo install  $(BINDIR)/$(TARGET) $(DESTDIR)$(PREFIX)
		


uninstall:
		sudo rm  $(PREFIX)/$(TARGET)
		sudo systemctl stop $(TARGET).service
		sudo rm /etc/systemd/system/$(TARGET).service
		sudo systemctl daemon-reload
		

enable:
		sudo cp ./service/$(TARGET).service /etc/systemd/system/
		sudo systemctl daemon-reload
		sudo systemctl start $(TARGET).service
		sudo systemctl enable $(TARGET).service


