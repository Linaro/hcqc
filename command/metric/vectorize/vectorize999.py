#!/usr/bin/env python3
import sys
import re

import driver

def use_vector_register_p(line):
    m = re.search('v\d+\.\d*[bhsd]', line)
    if m:
        return True    
    return False

def get_opmap(target_config, bb):
    opmap = {}
    for line in bb.line_list:
        label = target_config.bb_label(line)
        branch_info = target_config.bb_branch(line)
        op = target_config.op(line)
        if label:
            continue
        if branch_info:
            continue
        if op:
            if use_vector_register_p(line):
                add_entry(target_config, opmap, op)
            continue
    return opmap

def add_entry(target_config, opmap, op):
    canonical_op = target_config.canonical_op(op)
    if canonical_op in opmap:
        opmap[canonical_op] += 1
    else:
        opmap[canonical_op] = 1

v_op_list = []
bb_to_opmap = {}
summary_opmap = {}

def make_all_opmap(target_config, bb_list):
    global bb_to_opmap
    global v_op_list
    global summary_opmap
    opset = {}
    for bb in bb_list:
        opmap = get_opmap(target_config, bb)
        for k, v in opmap.items():
            opset[k] = 1
            if k in summary_opmap:
                summary_opmap[k] += v
            else:
                summary_opmap[k] = v
        bb_to_opmap[bb] = opmap
    for k in opset.keys():
        v_op_list.append(k)
    v_op_list.sort()

class VectorizeMetricWorker(driver.MetricWorker):
    def match_p(self, target_config, test_name):
        return True

    def set_up_before_getting_data(self, target_config, bb_list):
        make_all_opmap(target_config, bb_list)

    def get_column_name_list(self):
        global v_op_list
        return v_op_list

    def get_data_list(self, target_config, bb):
        global bb_to_opmap
        global v_op_list
        opmap = bb_to_opmap[bb]
        data_list = []
        for op in v_op_list:
            if op in opmap:
                data_list.append(str(opmap[op]))
            else:
                data_list.append("0")
        return data_list

    def get_summary_list(self, target_config):
        global v_op_list
        global summary_opmap
        summary_list = []
        for op in v_op_list:
            summary_list.append(str(summary_opmap[op]))
        return summary_list

if __name__ == "__main__":
    vectorize_metric_worker = VectorizeMetricWorker()
    driver.run_metric_work_if_match_p(sys.argv, vectorize_metric_worker)
