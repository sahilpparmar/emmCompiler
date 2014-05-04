#!
for i in `seq 1 27`;
do
    ./demo -debug mytests/in$i > mytests/out$i 2>&1;
done  
