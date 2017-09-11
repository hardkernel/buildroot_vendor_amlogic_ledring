TAR1=lightDaemon
TAR2=lightClientTest
PRE1=i2cWrite.c leds.c lightDaemon.c qipc.c
PRE2=clientTest.c

$(TAR1):$(PRE1)
	$(CC) $(CFLAGS) $(LDFLAGS) -lpthread -lrt -o $@ $^

$(TAR2):$(PRE2)
	$(CC) $(CFLAGS) $(LDFLAGS) -lpthread -lrt -o $@ $^

all:$(TAR1) $(TAR2)

clean:
	rm -f $(TARGET_DIR)/sbin/$(TAR1)
	rm -f $(TARGET_DIR)/sbin/$(TAR2)
install:
	install -m 0755 $(TAR1) $(TARGET_DIR)/sbin/
	install -m 0755 $(TAR2) $(TARGET_DIR)/sbin/
uninstall:
	rm -f $(TARGET_DIR)/sbin/$(TAR1)
	rm -f $(TARGET_DIR)/sbin/$(TAR2)
