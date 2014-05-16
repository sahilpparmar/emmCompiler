#!
make
for i in `seq 1 31`;
do
    ./demo -debug mytests/in$i > mytests/out$i 2>&1;
done  
