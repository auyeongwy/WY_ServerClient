CC = gcc
CFLAGS = -std=c17 -Wall -O2 -march=native 
BUILD_DIR = ../build
SRC_DIR = ../src
.PHONY: clean distclean

all: $(BUILD_DIR)/WY_TCP_Server $(BUILD_DIR)/WY_TCP_Client

$(BUILD_DIR)/WY_TCP_Server: $(SRC_DIR)/WY_TCP_Server.c
	@echo Building WY_TCP_Server...
	$(CC) $(CFLAGS) $(SRC_DIR)/WY_TCP_Server.c -o $(BUILD_DIR)/WY_TCP_Server

$(BUILD_DIR)/WY_TCP_Client: $(SRC_DIR)/WY_TCP_Client.c
	@echo Building WY_TCP_Client...
	$(CC) $(CFLAGS) $(SRC_DIR)/WY_TCP_Client.c -o $(BUILD_DIR)/WY_TCP_Client

clean:
	@echo Cleaning up objects...
	rm -f $(BUILD_DIR)/*.o

distclean: clean
	@echo Cleaning up executables
	rm -f $(BUILD_DIR)/WY_TCP_Server
	rm -f $(BUILD_DIR)/WY_TCP_Client