CC = gcc
TARGET_DIR = target/
SOURCE_DIR = src/

TARGETA = client_exe
TARGETB = server_exe
SRCSA = main_client.c
SRCSB = main_server.c server.c
OBJSA = $(patsubst %.c,$(TARGET_DIR)%.o,$(SRCSA))
OBJSB = $(patsubst %.c,$(TARGET_DIR)%.o,$(SRCSB))
CFLAGS = -lpthread

$(TARGET_DIR)$(TARGETA): $(OBJSA)
	$(CC) -o $@ $^

$(TARGET_DIR)%.o: $(SOURCE_DIR)%.c
	$(CC) -o $@ $<


$(TARGET_DIR)$(TARGETB): $(OBJSB)
	$(CC) $(CFLAGS) -o $@ $^

$(TARGET_DIR)%.o: $(SOURCE_DIR)%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	$(RM) $(TARGET_DIR)*.o $(TARGET_DIR)$(TARGETA) $(TARGET_DIR)$(TARGETB)
