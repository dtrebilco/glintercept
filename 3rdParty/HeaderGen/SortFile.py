#!/usr/bin/env python
#-*- coding: ascii -*-

from __future__ import print_function
try:
  from StringIO import StringIO
except:
  from io import StringIO


if __name__ == "__main__":
  import sys
  lines = open(sys.argv[1]).readlines()
  lines.sort()
  
  file = open(sys.argv[1] + ".out","w")
  
  for line in lines:
    file.write(line)
  file.close()
    
