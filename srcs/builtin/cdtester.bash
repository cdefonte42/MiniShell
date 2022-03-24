#! /bin/bash

echo "_________ NORMAL OK TESTS ______"
mkdir -p prout && echo "dir here OK test" && ./a.out prout; echo $?; rmdir prout
mkdir -p "../pouet" && echo "dir au dessus OK test" && ./a.out ../pouet; echo $?; rmdir "../pouet"
mkdir -p "prout" && chmod 644 prout && echo "dir sans x right (644)" && ./a.out .prout; echo $?; rmdir "prout"


echo "_________ HOME TESTS ______"
HSAVE=$HOME
echo "home set" && ./a.out; echo $?
HOME= ; echo "home empty" && ./a.out; echo $?
HOME="" ; echo "home empty bis" && ./a.out; echo $?
unset HOME && echo "home unset" && ./a.out; echo $?; export HOME=$HSAVE


echo "_________ CDPATH TESTS ______"
export CDPATH=$HOME/GNL
echo "GNL_BIS avec EXPORTED CDPATH=" $CDPATH && ./a.out GNL_BIS; echo $?
export CDPATH=$HOME/GNL/
echo "GNL_BIS avec EXPORTED CDPATH=" $CDPATH && ./a.out GNL_BIS; echo $?
export CDPATH=$HOME:/mnt/nfs/homes/cdefonte/MiniShell:/mnt/nfs/homes/cdefonte/MiniShell/:/mnt/nfs/homes/cdefonte/GNL/:/mnt/nfs/homes/cdefonte/MiniShell/cd_bin
echo "GNL_BIS avec EXPORTED CDPATH=" $CDPATH && ./a.out GNL_BIS; echo $?
export CDPATH=$HOME:/mnt/nfs/homes/cdefonte/MiniShell:/mnt/nfs/homes/cdefonte/MiniShell/:/mnt/nfs/homes/cdefonte/:/mnt/nfs/homes/cdefonte/MiniShell/cd_bin
echo "GNL_BIS avec EXPORTED CDPATH=" $CDPATH && ./a.out GNL_BIS; echo $?
