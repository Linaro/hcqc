import sys

import driver

def get_kindmap(target_config, bb):
    kindmap = { 'memory' : 0, 'branch' : 0, 'other' : 0 }
    for line in bb.line_list:
        label = target_config.bb_label(line)
        branch_info = target_config.bb_branch(line)
        op = target_config.op(line)
        if label:
            continue
        if branch_info:
            add_entry(kindmap, 'branch')
            continue
        if op:
            if target_config.load_op_p(op) or target_config.store_op_p(op):
                add_entry(kindmap, 'memory')
            else:
                add_entry(kindmap, 'other')
            continue
    return kindmap

def add_entry(kindmap, kind):
    kindmap[kind] += 1

all_kind_list = [ 'memory', 'branch', 'other']
bb_to_kindmap = {}
summary_kindmap = { 'memory' : 0, 'branch' : 0, 'other' : 0 }
        
def make_all_kindmap(target_config, bb_list):
    global bb_to_kindmap
    global summary_kindmap
    for bb in bb_list:
        kindmap = get_kindmap(target_config, bb)
        for kind in all_kind_list:
            count = kindmap[kind]
            summary_kindmap[kind] += count
        bb_to_kindmap[bb] = kindmap

class KindMetricWorker(driver.MetricWorker):
    def match_p(self, target_config, test_name):
        return True

    def set_up_before_getting_data(self, target_config, db_filename, bb_list, column_list):
        make_all_kindmap(target_config, bb_list)

    def get_column_name_list(self):
        global all_kind_list
        return all_kind_list

    def get_data_list(self, target_config, bb):
        global bb_to_kindmap
        global all_kind_list
        kindmap = bb_to_kindmap[bb]
        data_list = []
        for kind in all_kind_list:
            data_list.append(str(kindmap[kind]))
        return data_list

    def get_summary_list(self, target_config):
        global all_kind_list
        global summary_kindmap
        summary_list = []
        for kind in all_kind_list:
            summary_list.append(str(summary_kindmap[kind]))
        return summary_list

if __name__ == "__main__":
    kind_metric_worker = KindMetricWorker()
    driver.run_metric_work_if_match_p(sys.argv, kind_metric_worker)
