#!
for i in `seq 1 26`;
do
    ./demo -debug mytests/in$i > mytests/out$i 2>&1;
done  
