PACKAGE_NAME=locke.tgz

default: dist

all:
	cd server && make all
	cd sample_apps && make all
	
docs:
	cd documentation && make all

dist: all 
	cd server && make dist	
	cd sample_apps && make dist
	cp -R documentation dist	
	cd dist && tar -zvcf ../$(PACKAGE_NAME) .
	@echo "========================================="
	@echo "Distro generated at '$(PACKAGE_NAME)'!"
	@echo "========================================="

clean:
	cd server && make clean
	cd sample_apps && make clean
	cd documentation && make clean
	rm -f $(PACKAGE_NAME)
	
	
