#!/bin/sh

TP="sample"

CONFIG="gcc-config"

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
