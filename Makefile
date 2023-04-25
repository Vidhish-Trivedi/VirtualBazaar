CC = gcc
TARGET_DIR = target/
SOURCE_DIR = src/

TARGETA = client_exe
TARGETB = server_exe
SRCSA = main_client.c client_menu.c
SRCSB = main_server.c server.c user.c admin.c
OBJSA = $(patsubst %.c,$(TARGET_DIR)%.o,$(SRCSA))
OBJSB = $(patsubst %.c,$(TARGET_DIR)%.o,$(SRCSB))

$(TARGET_DIR)$(TARGETA): $(OBJSA)
	$(CC) -o $@ $^

$(TARGET_DIR)%.o: $(SOURCE_DIR)%.c
	$(CC) -o $@ $<


$(TARGET_DIR)$(TARGETB): $(OBJSB)
	$(CC) -o $@ $^ -lpthread

$(TARGET_DIR)%.o: $(SOURCE_DIR)%.c
	$(CC) -c -o $@ $< -lpthread

clean:
	$(RM) $(TARGET_DIR)*.o $(TARGET_DIR)$(TARGETA) $(TARGET_DIR)$(TARGETB)
