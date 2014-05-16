#!
make
rm -rf myoutputs
mkdir myoutputs
for i in `seq 1 31`;
do
    echo;echo "---->Running in$i"
    ./demo -debug mytests/in$i > myoutputs/out$i 2>&1;
    echo;echo "---->Diff"
    diff -w mytests/out$i myoutputs/out$i
    echo;
done  

