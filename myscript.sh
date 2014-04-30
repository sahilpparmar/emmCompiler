#!
for i in `seq 1 24`;
do
    ./demo -debug mytests/in$i > myoutputs/out$i 2>&1;
done  
