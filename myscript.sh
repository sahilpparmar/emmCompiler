#!
make
for i in `seq 1 28`;
do
    ./demo -debug mytests/in$i > mytests/out$i 2>&1;
done  
