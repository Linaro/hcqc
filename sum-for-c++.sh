#!/bin/sh

. ./c++-parameter.sh

HCQC_DIR=$PWD

for metric in ${METRIC_LIST}
do
    for tp in ${TP_LIST}
    do
	./command/hcqc-m-report R${metric} ${tp} --c ${COMPARE_A} --m ${metric}
	./command/hcqc-m-report R${metric} ${tp} --c ${COMPARE_B} --m ${metric}
    done
done

for tp in ${TP_LIST}
do
    for config in ${CONFIG_LIST}
    do
	dot -Tps ${HCQC_DIR}/work/${tp}/${config}/op/kernel.s.dot -o ${HCQC_DIR}/report/${tp}/${config}.eps
    done
done
