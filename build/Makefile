CC = gcc
CFLAGS = -std=c17 -Wall -O2 -march=native
#CFLAGS = -Wall -std=c17 -fsanitize=address -static-libasan -g3 -march=native
BUILD_DIR = ../build
SRC_DIR = ../src
OBJS = $(BUILD_DIR)/WY_SC_Helper.o
.PHONY: clean distclean buildobjs buildmsg

all: buildobjs $(BUILD_DIR)/WY_TCP_Server $(BUILD_DIR)/WY_TCP_Client

$(BUILD_DIR)/WY_TCP_Server: $(SRC_DIR)/WY_TCP_Server.c $(OBJS)
	@echo Building WY_TCP_Server...
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/WY_TCP_Server $(SRC_DIR)/WY_TCP_Server.c $(OBJS) 

$(BUILD_DIR)/WY_TCP_Client: $(SRC_DIR)/WY_TCP_Client.c $(OBJS)
	@echo Building WY_TCP_Client...
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/WY_TCP_Client $(SRC_DIR)/WY_TCP_Client.c $(OBJS) 

$(BUILD_DIR)/WY_SC_Helper.o: $(SRC_DIR)/WY_SC_Helper.h $(SRC_DIR)/WY_SC_Helper.c
	$(CC) $(CFLAGS) -c $(SRC_DIR)/WY_SC_Helper.c -o $(BUILD_DIR)/WY_SC_Helper.o

buildobjs: buildmsg $(OBJS)

buildmsg:
	@echo Building object files...

clean:
	@echo Cleaning up objects...
	rm -f $(BUILD_DIR)/*.o

distclean: clean
	@echo Cleaning up executables
	rm -f $(BUILD_DIR)/WY_TCP_Server
	rm -f $(BUILD_DIR)/WY_TCP_Client