#!/bin/sh
#This is a small script to put the required db/conf/npc etc folder to the bin directory 
#since our exe require that relative path to work

cd "$(dirname "$0")"

if [ ! -d ./db ]; then ln -s ../../db ./db; fi
if [ ! -d ./conf ]; then ln -s ../../conf ./conf; fi
if [ ! -d ./npc ]; then ln -s ../../npc ./npc; fi
if [ ! -d ./log ]; then ln -s ../../log ./log; fi
if [ ! -d ./doc ]; then ln -s ../../doc ./doc; fi
