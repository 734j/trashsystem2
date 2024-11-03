#!/bin/bash

for i in {1..5}
do
  ./gentestfiles.sh
  ./test_deletefiles.sh
done
