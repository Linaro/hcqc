CONFIG_LIST="clang-cavium clang-cavium-fast gcc-cavium gcc-cavium-fast"

METRIC_LIST="op kind regalloc height vectorize"

TP_LIST="HimenoBMT-dynamic HimenoBMT-static ccs-qcd-bicgstab_hmc ccs-qcd-clover ffb-mini-CALAX3 ffb-mini-FLD3X2 ffb-mini-GRAD3X ffvc-mini-poi_residual ffvc-mini-psor2sma_core hpcg-3.0 mVMC-mini-calculateNewPfMTwo_child mVMC-mini-updateMAllTwo_child mVMC-mini-updateMAll_child ngsa-mini-bwt_match_exact_alt ngsa-mini-bwt_match_gap nicam-dc-mini-vi_path2 QCDpbm-det_pauli_dble QCDpbm-inv_pauli_dble QCDpbm-mul_pauli_dble NPB2.3-omp-C-compute_rhs NPB2.3-omp-C-erhs NPB2.3-omp-C-exact_rhs NPB2.3-omp-C-fftz2 NPB2.3-omp-C-interp"

COMPARE_A="clang-cavium gcc-cavium"
COMPARE_B="clang-cavium-fast gcc-cavium-fast"

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
