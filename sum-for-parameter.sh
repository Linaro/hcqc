#!/bin/sh

case $# in
1) ;;
*) echo 'usage: sum-for-parameter.sh parameter-file' 1>&2; exit 2;;
esac

. ./$1

HCQC_DIR=$PWD

for metric in ${METRIC_LIST}
do
    for tp in ${TP_LIST}
    do
	if [ "${COMPARE_A}" != "" ]
	then
	    ./command/hcqc-m-report R${metric} ${tp} --c ${COMPARE_A} --m ${metric}
	fi
	if [ "${COMPARE_B}" != "" ]
	then
	    ./command/hcqc-m-report R${metric} ${tp} --c ${COMPARE_B} --m ${metric}
	fi
    done
done

for tp in ${TP_LIST}
do
    for config in ${CONFIG_LIST}
    do
	for d in ${HCQC_DIR}/work/${tp}/${config}/op/*.s.dot
	do
	    dot -Tps $d -o ${HCQC_DIR}/report/${tp}/${config}.eps
	    break
	done
    done
done
