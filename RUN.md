## Running samples

Please fix `COMPILER', `COMMAND' and `VERSION' fields in hcqc/config/gcc-config.json to suit your environment on AArch64.

The following commands:

    % cd hcqc
    % ./run-sample.sh gcc-config

generate the following results for `hcqc/test-program/sample/kernel.c'.

    hcqc/report/sample/gcc-config--sample--all.csv
    hcqc/report/sample/gcc-config--sample--kind.csv
    hcqc/report/sample/gcc-config--sample--op.csv
    hcqc/report/sample/gcc-config--sample--regalloc.csv
    hcqc/report/sample/gcc-config.png

The following commands:

    % cd hcqc
    % ./realclean-all.sh

remove all generated files under hcqc.
