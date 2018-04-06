CONFIG_LIST="clang++-cavium clang++-cavium-fast g++-cavium g++-cavium-fast"

METRIC_LIST="op kind regalloc height vectorize"

TP_LIST="LAMMPS-BO"

COMPARE_A="clang++-cavium g++-cavium"
COMPARE_B="clang++-cavium-fast g++-cavium-fast"

if [ ! -r "./lib/aarch64/libHCQC.a" ]
then
    cd ./lib/aarch64
    make
    if [ $? -ne 0 ]
    then
	echo "cannot build ./lib/aarch64/libHCQC.a"
	exit 2
    fi
    cd ../..
fi
