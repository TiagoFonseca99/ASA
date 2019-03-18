#!/bin/bash
### use: ./run_custom_tests.sh <program.c>

g++ $1 -o proj1

for i in {1..6..1}
   do
     ./proj1 < student-tests/T0$i.in > student-tests/T0$i.mine.out
     result=$(diff student-tests/T0$i.out student-tests/T0$i.mine.out)
     if [[ $result == "" ]]; then
         echo "T0$i: Passed!"
     else
         echo "T0$i:" $result
     fi
   done
