import sys
import re

import driver

def get_spill_in_and_out(target_config, bb):
    spill_in = 0
    spill_out = 0
    for line in bb.line_list:
        comment = target_config.get_asm_comment(line)
        if not comment:
            continue
        m = re.match('^\s*\d+-byte Reload$', comment)
        if m:
            spill_in += 1
        m = re.match('^\s*\d+-byte Folded Reload$', comment)
        if m:
            spill_in += 1
        m = re.match('^\s*\d+-byte Spill$', comment)
        if m:
            spill_out += 1
        m = re.match('^\s*\d+-byte Folded Spill$', comment)
        if m:
            spill_out += 1
    return (spill_in, spill_out)

summary_spill_in = 0
summary_spill_out = 0

class RegallocMetricWorker(driver.MetricWorker):
    def match_p(self, target_config, test_name):
        if target_config.compiler == "ClangLLVM":
            return True
        return False

    def set_up_before_getting_data(self, target_config, db_filename, bb_list, column_list):
        pass

    def get_column_name_list(self):
        return ["spill in", "spill out"]

    def get_data_list(self, target_config, bb):
        global summary_spill_in
        global summary_spill_out
        spill_in, spill_out = get_spill_in_and_out(target_config, bb)
        summary_spill_in += spill_in
        summary_spill_out += spill_out
        sin = str(spill_in)
        sout = str(spill_out)
        return [sin, sout]

    def get_summary_list(self, target_config):
        global summary_spill_in
        global summary_spill_out
        return [str(summary_spill_in), str(summary_spill_out)]

if __name__ == "__main__":
    regalloc_metric_worker = RegallocMetricWorker()
    driver.run_metric_work_if_match_p(sys.argv, regalloc_metric_worker)
