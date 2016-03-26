CC=gcc
INSTALL_DIR=/usr/local/bin

all: scrbr

clean:
	rm scrbr

scrbr: scrbr.c
	$(CC) -o scrbr scrbr.c

install: scrbr
	sudo cp scrbr $(INSTALL_DIR)
	sudo chown "root:root" $(INSTALL_DIR)/scrbr
	sudo chmod 4755 $(INSTALL_DIR)/scrbr

uninstall:
	sudo rm $(INSTALL_DIR)/scrbr
