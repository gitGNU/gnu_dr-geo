mkdir dxf
cd dxf
rm dxflib-3.3.4-src.tar.gz
wget -c www.ribbonsoft.com/archives/dxflib/dxflib-3.3.4-src.tar.gz
tar -xzf dxflib-3.3.4-src.tar.gz
cd dxflib-3.3.4-src
qmake dxflib.pro
make
cp libdxflib.a /usr/lib/
cp src/*.h /usr/include
cd ..
cd ..
rm -R dxf
make
