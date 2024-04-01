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

./egts 9876

where

9876 - bind port (default 9876)


## P.S.
```
* Server listen clients on port 9876 (by default)
* Server support next service :
    - EGTS_AUTH_SERVICE:
          EGTS_SR_TERM_IDENTITY, EGTS_SR_MODULE_DATA, EGTS_SR_VEHICLE_DATA, EGTS_SR_DISPATCHER_IDENTITY,
          EGTS_SR_AUTH_PARAMS, EGTS_SR_AUTH_INFO, EGTS_SR_SERVICE_INFO, EGTS_SR_RESULT_CODE
    - EGTS_TELEDATA_SERVICE:
          EGTS_SR_POS_DATA, EGTS_SR_EXT_POS_DATA, EGTS_SR_AD_SENSORS_DATA
* Server write main log_file "log.txt" in current folder (max size 16M)
* Server accept next Signals :
* SIGKILL, SIGTERM, SIGINT ... - interrupt program
```


Reference to the documentation - http://docs.cntd.ru/document/1200119664


Beta version of the test mode in the process :

Only the following services are partially supported : EGTS_AUTH_SERVICE, EGTS_TELEDATA_SERVICE




