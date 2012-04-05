include ../common.makefile

all:
	cd demo_socket && make all
	cd demo_http && make all

dist:
	cp demo*/*.$(DLL_SUFFIX) ../dist/autodeploy	

clean:
	cd demo_socket && make clean	
	cd demo_http && make clean
	
