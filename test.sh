#!/bin/sh
: "${CMD:=$(realpath per)}"
TESTDIR=$(mktemp -d)
trap "rm -rf $TESTDIR" 0
cd $TESTDIR

# testing "args" "expected", green if passed, red if failed
testing () {
  $CMD $1 > got || echo "WARNING: $1 exited $?"
  printf "$2\n" > wanted
  ! diff -u "$TESTDIR/wanted" "$TESTDIR/got"
  printf "\033[1;%sm%s\033[0m\n" "$((31+$?))" "$1"
}

touch f777 f7520
chmod 777 f777
chmod 7520 f7520
# Compact Expected Result string
R="read" W="write" X="execute"

testing "0000" "user: \ngroup: \nother: \n000\n---------"
testing "120" "user: $X\ngroup: $W\nother: \n120\n--x-w----"
testing "-S 4567" "user: $R, $X\ngroup: $R, $W\n\
other: $R, $W, $X\nspecial: suid\n4567\nr-srw-rwx"
testing "-Sn rwxr-xr-x" "0755"
testing "-Sns f7520" "7520\nr-s-wS--T"
testing "-nv f777" "777\nuser: $R, $W, $X\ngroup: $R, $W, $X\nother: $R, $W, $X"
