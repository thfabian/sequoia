#!/bin/bash

DPATH=$(find ./src/ ./test/unittest/ ./test/benchmark/ -name '*.cpp' -o -name '*.h')
TFILE="/tmp/out.tmp.$$"

for f in $DPATH
do
  echo $f
  if [ -f $f -a -r $f ]; then
     sed 's/\#include \"sequoia/\#include \"sequoia-engine/g' "$f" > $TFILE && mv $TFILE "$f"
  else
     echo "Error: Cannot read $f"
  fi
done

