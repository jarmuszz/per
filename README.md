per - Simple unix permission viewer and converter
---
Per is a simple utility that can verbosely print unix permissions and
convert between symbolic and numeric notations and vice-versa.

## Synopsis:
`per [OPTIONS] TARGET`

## OPTIONS
 - -S      Treat notations as special notations.
 - -v      Print permissions verbosely.
 - -n      Print permissions in a numeric notation.
 - -s      Print permissions in a symbolic notation.

Options may be chained (e.g. -vns). Providing no options defaults to -vns.
Providing only -S as an option defaults to -Svns.

TARGET may be:
  * A symbolic notation (e.g. rwxr-xr-x)
  * A numeric notation (e.g. 755)
  * A file (e.g. /etc/passwd)
  * A literal dash '-'

If TARGET is a literal dash '-', it will be replaced with a first line from stdin.

If -S option is used, per will accept and print special notations (e.g.
rwsrwsrwT or 6775).

## Examples
###  Printing a file permission verbosely
`per -v /etc/passwd`

###  Converting a special numeric (octal) notation into symbolic
`per -Ss 6775`

###  Copying a file permission
```chmod `per -Sn file1` file2```

## Installing
Build deps: GNU make, gzip (for documentation)

To build and install the program with documentation run:
```
make
sudo make install
```

To build and install the program *without* documentation run:
```
make
sudo make install-bin
```

Building was tested and works on: Artix, OpenBSD, NixOS and macOS.

Thanks for Legend and grylem for testing per on NixOS and macOS respectively.

Bugs report to the github repo.

Made by Mariusz Jakoniuk (jarmusz at tuta dot io).