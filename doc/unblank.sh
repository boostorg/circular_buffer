#!/bin/bash

function rb {
    sed -r 's/\t/    /g;s/\s*$//' $1 > $1.temp
    mv $1.temp $1
}

rb 'test.txt'
