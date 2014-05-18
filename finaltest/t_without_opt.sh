#/bin/sh
cd ..
##make clean
make || exit
./demo_without_opt finaltest/in03 > codegen/sample.i 
cd codegen
./easm sample.i
./erun sample.i
cd ..
cd finaltest
