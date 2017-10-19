import sys

import driver

def refer_spill_slot_p(target_config, line):
    comment = target_config.get_asm_comment(line)
    if not comment:
        return False
    index = comment.find('%sfp')
    if index < 0:
        return False
    else:
        return True

def load_instruction_p(target_config, line):
    op = target_config.op(line)
    return target_config.load_op_p(op)

def store_instruction_p(target_config, line):
    op = target_config.op(line)
    return target_config.store_op_p(op)
    
def spill_in_p(target_config, line):
    return refer_spill_slot_p(target_config, line) and load_instruction_p(target_config, line)

def spill_out_p(target_config, line):
    return refer_spill_slot_p(target_config, line) and store_instruction_p(target_config, line)

def get_spill_in_and_out(target_config, bb):
    spill_in = 0
    spill_out = 0
    for line in bb.line_list:
        if spill_in_p(target_config, line):
            spill_in += 1
        if spill_out_p(target_config, line):
            spill_out += 1
    return (spill_in, spill_out)

summary_spill_in = 0
summary_spill_out = 0

class RegallocMetricWorker(driver.MetricWorker):
    def match_p(self, target_config, test_name):
        if target_config.compiler == "GCC":
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
