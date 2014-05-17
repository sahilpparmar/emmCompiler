#/bin/sh
cd ..
##make clean
make || exit
./demo finaltest/in04 > codegen/sample.i 
cd codegen
./easm sample.i
./erun sample.i
cd ..
cd finaltest
