
all:
	cd server && make all
	cd sample_apps && make all
	
docs:
	cd documentation && make all

dist: all 
	cd server && make dist	
	cd sample_apps && make dist
	cp -R documentation dist	

clean:
	cd server && make clean
	cd sample_apps && make clean
	cd documentation && make clean
	
	
