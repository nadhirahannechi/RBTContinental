all:
	g++ -shared -o libSOMEIPModule.so src/S* -Ipub -fPIC -fpermissive

install: libSOMEIPModule.so
	sudo cp libSOMEIPModule.so /usr/lib

uinstall:
	sudo rm -rf /usr/lib/libSOMEIPModule.so

clean:
	rm -rf bin *.o libSOMEIPModule.so

