APPNAME = tstinst
BUILD_PATH = ./build/
OUTPUT_FULLPATH := "$(BUILD_PATH)$(APPNAME)"
INSTALL_PATH = /usr/local/bin/
compile:
	gcc -O0 $(APPNAME).c -o $(OUTPUT_FULLPATH) -lrt
install:
	cp $(APPNAME) $(INSTALL_PATH) 
