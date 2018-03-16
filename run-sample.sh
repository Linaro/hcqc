#!/bin/sh

TP="sample"

case $# in
1) ;;
*) echo 'usage: run-sample.sh config-name' 1>&2; exit 2;;
esac

CONFIG=$1

if [ ! -r "./config/${CONFIG}.json" ]
then
    echo "There is no './config/${CONFIG}.json'."
    exit 3
fi

#HCQC_OPTIONS="--v"
#HCQC_REPORT_OPTIONS="--v"
HCQC_OPTIONS=
HCQC_REPORT_OPTIONS=

./command/hcqc ${HCQC_OPTIONS} ${CONFIG} ${TP} op
./command/hcqc ${HCQC_OPTIONS} ${CONFIG} ${TP} kind
./command/hcqc ${HCQC_OPTIONS} ${CONFIG} ${TP} regalloc

./command/hcqc-report ${HCQC_REPORT_OPTIONS} ${CONFIG} ${TP} op op
./command/hcqc-report ${HCQC_REPORT_OPTIONS} ${CONFIG} ${TP} kind kind
./command/hcqc-report ${HCQC_REPORT_OPTIONS} ${CONFIG} ${TP} regalloc regalloc

./command/hcqc-report ${HCQC_REPORT_OPTIONS} ${CONFIG} ${TP} all op kind regalloc

if [ -x "/usr/bin/dot" ]
then
    dot -Tpng ./work/${TP}/${CONFIG}/op/kernel.s.dot -o ./report/${TP}/${CONFIG}.png
else
    echo "There is no '/usr/bin/dot'."
    exit 4
fi
