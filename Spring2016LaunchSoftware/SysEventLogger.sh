#!/bin/sh

EVENTLOG=/mnt/sd/trapsat/logs/eventlog.txt
LOGDIR=/mnt/sd/trapsat/logs

EXIT_SUCCESS=0
EXIT_USAGE=1
EXIT_NO_SD=2

USAGE="USAGE: ./SysEventLogger string1 string2"

if [ -d $LOGDIR ]; then
        #echo Log Dir Already Exists!
        if [ ! -f $EVENTLOG ]; then
                #echo eventlog File Did Not Exist!
                touch $EVENTLOG
        #else
        #       echo eventlog File Already Exists!
        #       touch $EVENTLOG
        fi
else
        #echo Log Dir Did Not Exist!
        #echo Making Log Dir And Files!
        mkdir -p $LOGDIR
        touch $EVENTLOG
fi

#echo There are $# arguments to $0: $*

#
# Don't continue if there are no args
#
if [ "${#}" = "0" ]; then
        echo $USAGE
        exit $EXIT_USAGE
fi

for arg in $*; do
        echo -n "${arg} " >> $EVENTLOG
done

echo >> $EVENTLOG

exit $EXIT_SUCCESS