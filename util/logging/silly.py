#!/usr/bin/python

import json

import sys

content =  sys.stdin.read()
json_raw = json.loads( content )
for line in json_raw["log"]:
   print line

