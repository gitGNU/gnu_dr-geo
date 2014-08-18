all: rw

rw: read.cpp write.cpp dxftoxml.cpp xmltodxf.cpp
	g++ -std=c++11 read.cpp dxftoxml.cpp -o dxftoxml -I/usr/include -I/usr/local/include /usr/lib/libdxflib.a
	g++ -std=c++11 write.cpp xmltodxf.cpp -o xmltodxf -I/usr/include/libxml2 -I/usr/include -I/usr/local/include /usr/lib/libdxflib.a -lxml2

clean:
	rm dxftoxml xmltodxf
