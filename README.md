####################################################################
#
#            egts parser (server side)
#
####################################################################

## Description

Simple parser-server for receiving data via EGTS protocol from gps/glonass devices


## Package files:

```
* Makefile      - make file (compilation scenario file)

* egtsFunc.c    - function's library source file

* egtsFunc.h    - function's library header file

* main.c        - main source file
```

Binary file name - egts


start :

./egts 9009

where

9009 - bind port (default 9999)


## P.S.
```
* Server listen clients on port 9999 (by default)
* Server write main log_file "log.txt" in current folder (max size 32M)
* Server accept next Signals :
* SIGKILL, SIGTERM, SIGINT ... - interrupt program
```


Reference to the documentation - http://docs.cntd.ru/document/1200119664


Beta version of the test mode in the process :

Only the following services are partially supported : EGTS_AUTH_SERVICE, EGTS_TELEDATA_SERVICE



