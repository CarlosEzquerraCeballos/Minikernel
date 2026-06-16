#!/bin/bash

if test $# -ne 1 || test ! -f init_$1.c
then
    echo "Introduzca un número con dos dígitos (00, 01...) que corresponde a la versión del init que quiere usar" >&2
else
    ln -sf init_$1.c init.c; touch init.c
fi
