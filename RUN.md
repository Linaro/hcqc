## Running samples

Please fix both `COMPILER' and `VERSION' fields in hcqc/config/gcc-config.json and hcqc/config/clang-config.json to suit your environment.

The following commands:

    % cd hcqc
    % ./run-sample.sh

generate the following results:

    hcqc/report/sample/gcc-config--sample--all.csv
    hcqc/report/sample/gcc-config--sample--kind.csv
    hcqc/report/sample/gcc-config--sample--op.csv
    hcqc/report/sample/gcc-config--sample--regalloc.csv

The following commands:

    % cd hcqc
    % ./run-real.sh

generate the following results:

    hcqc/report/HimenoBMT-static/clang-config--HimenoBMT-static--kind.csv
    hcqc/report/HimenoBMT-static/clang-config--HimenoBMT-static--op.csv
    hcqc/report/HimenoBMT-static/clang-config--HimenoBMT-static--regalloc.csv
    hcqc/report/HimenoBMT-static/gcc-config--HimenoBMT-static--kind.csv
    hcqc/report/HimenoBMT-static/gcc-config--HimenoBMT-static--op.csv
    hcqc/report/HimenoBMT-static/gcc-config--HimenoBMT-static--regalloc.csv
    hcqc/report/hpcg-3.0/clang-config--hpcg-3.0--kind.csv
    hcqc/report/hpcg-3.0/clang-config--hpcg-3.0--op.csv
    hcqc/report/hpcg-3.0/clang-config--hpcg-3.0--regalloc.csv
    hcqc/report/hpcg-3.0/gcc-config--hpcg-3.0--kind.csv
    hcqc/report/hpcg-3.0/gcc-config--hpcg-3.0--op.csv
    hcqc/report/hpcg-3.0/gcc-config--hpcg-3.0--regalloc.csv

The following commands:

    % cd hcqc
    % ./realclean-all.sh

remove all generated files under hcqc.
