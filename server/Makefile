

SCRIPT_DIR=scripts
DIST_DIR=../dist
PROGNAME=src/locke

all:
	cd api && make all
	cd src && make all

$(DIST_DIR):	
	mkdir $(DIST_DIR)
	mkdir $(DIST_DIR)/autodeploy
	mkdir $(DIST_DIR)/config
	mkdir $(DIST_DIR)/log
	mkdir $(DIST_DIR)/var
	mkdir $(DIST_DIR)/api
	
dist: $(DIST_DIR) all
	cp -v $(PROGNAME) $(DIST_DIR)/
	cp -rv api/*.h api/*.$(DLL_SUFFIX)* $(DIST_DIR)/api
	cp -vr $(SCRIPT_DIR)/* $(DIST_DIR)/

clean:
	cd api && make clean
	cd src && make clean
	rm -fr $(DIST_DIR)
	
