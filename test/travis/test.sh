#!/bin/bash

if [ -d /home/test ];then
    cd /home/test
    module load boost
    ./bootstrap
    ./configure
else
    exit 1
fi


