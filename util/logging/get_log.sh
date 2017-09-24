#!/bin/bash

set -e

SPATH=$(dirname $0)
cd $SPATH
DATE=$(/bin/date +%Y%m%d)
OUTF=log_$DATE.txt
curl -N -s waterpump.local/get/log | ./silly.py >> $OUTF
sort $OUTF|uniq -u > /tmp/waterpump.txt
mv /tmp/waterpump.txt $OUTF

