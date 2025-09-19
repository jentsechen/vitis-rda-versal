#!/bin/bash

infile="aie_control_xrt.cpp"

awk '
  # Detect start of stub block
  /\/\/ Kernel Stub Definition/ { print; skip=1; next }
  # Detect end of stub block
  /#endif/ {
    skip=0
    print
    next
  }
  # Skip all lines in between
  skip { next }

  # Match #include "..."
  /^#include *"[^"]+"/ {
    line=$0
    start=index(line, "\"") + 1
    end=index(substr(line, start), "\"") - 1
    path=substr(line, start, end)
    if (index(path, "/") == 0) {
      next  # remove if no "/" in path
    }
  }

  { print }
' "$infile" > tmpfile && mv tmpfile "$infile"

