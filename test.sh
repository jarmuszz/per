#!/bin/sh
: "${CMD:=$(realpath per)}"
TESTDIR=$(mktemp -d)
trap "rm -rf $TESTDIR" 0
cd $TESTDIR

# testing "args" "expected", green if passed, red if failed
testing () {
  $CMD $1 > got || echo "WARNING: $1 exited $?"
  printf "%s\n" "$2" > wanted
  ! diff -u "$TESTDIR/wanted" "$TESTDIR/got"
  printf "\033[1;%sm%s\033[0m\n" "$((31+$?))" "$1"
}

touch f777 f7520
chmod 777 f777
chmod 7520 f7520
# Compact Expected Result string
R="read" W="write" X="execute"

testing "0000" "user: 
group: 
other: 
000
---------"
testing "120" "user: $X
group: $W
other: 
120
--x-w----"
testing "-S 4567" "user: $R, $X
group: $R, $W
other: $R, $W, $X
special: suid
4567
r-srw-rwx"
testing "-Sn rwxr-xr-x" "0755"
testing "-Sns f7520" "7520
r-s-wS--T"
testing "-nv f777" "777
user: $R, $W, $X
group: $R, $W, $X
other: $R, $W, $X"
testing "-Sn rwSrwsrwT" "7676"
testing "-Sn rwsrwSr-t" "7765"
