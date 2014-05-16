#!
make clean
rm -rf tempfile
make
./demo mytests/in1 > tempfile
vim tempfile
