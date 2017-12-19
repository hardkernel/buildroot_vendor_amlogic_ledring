TAR1=lightDaemon
TAR2=ledring
TAR3=lightTest
PRE1=i2cWrite.c leds.c lightDaemon.c qipc.c
PRE2=ledClient.c
PRE3=ledClient.c clientTest.c

INCLUDES=ledClient.h

$(TAR1):$(PRE1)
	$(CC) $(CFLAGS) $(LDFLAGS) -lpthread -lrt -o $@ $^

$(TAR2):$(PRE2)
	$(CC) -c $(CFLAGS) $(LDFLAGS) -fpic -lpthread -lrt -o $@.o $^
	$(CC) $(CFLAGS) $(LDFLAGS) -lpthread -lrt -shared $@.o -olib$@.so

$(TAR3):$(PRE3)
	$(CC) $(CFLAGS) $(LDFLAGS) -lpthread -lrt -o $@ $^

all:$(TAR2) $(TAR3)

clean:
#	rm -f $(TARGET_DIR)/sbin/$(TAR1)
	rm -f $(STAGING_DIR)/usr/include/$(INCLUDES)
	rm -f $(TARGET_DIR)/usr/lib$(TAR2).so
	rm -f $(STAGING_DIR)/usr/lib/lib$(TAR2).so
	rm -f $(TARGET_DIR)/sbin/$(TAR3)
install:
#	install -m 0755 $(TAR1) $(TARGET_DIR)/sbin/
	install -m 0644 $(INCLUDES) $(STAGING_DIR)/usr/include/
	install -m 0644 lib$(TAR2).so $(STAGING_DIR)/usr/lib/
	install -m 0644 lib$(TAR2).so $(TARGET_DIR)/usr/lib/
	install -m 0755 $(TAR3) $(TARGET_DIR)/sbin/
uninstall:
#	rm -f $(TARGET_DIR)/sbin/$(TAR1)
	rm -f $(TARGET_DIR)/sbin/$(TAR2)
