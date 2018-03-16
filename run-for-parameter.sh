#!/bin/sh

#HCQC_OPTIONS="--v"
HCQC_OPTIONS=
#HCQC_REPORT_OPTIONS="--v"
HCQC_REPORT_OPTIONS=

case $# in
1) ;;
*) echo 'usage: run-for-parameter.sh parameter-file' 1>&2; exit 2;;
esac

. ./$1

for tp in ${TP_LIST}
do
    for config in ${CONFIG_LIST}
    do
	./command/hcqc ${HCQC_OPTIONS} ${config} ${tp} ${METRIC_LIST}
    done
done

for tp in ${TP_LIST}
do
    for config in ${CONFIG_LIST}
    do
        for metric in ${METRIC_LIST}
        do
            ./command/hcqc-report ${HCQC_REPORT_OPTIONS} ${config} ${tp} ${metric} ${metric}
        done
    done
done
