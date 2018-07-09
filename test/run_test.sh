#!/bin/sh
gcc test.c ../pmh_definitions.h ../pmh_parser.c ../pmh_parser.h -o test
./test test.md
