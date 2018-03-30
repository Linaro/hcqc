#!/usr/bin/env python3
import sys
import re

import driver

def cut_comment(line):
    pattern = '//'
    index = line.find(pattern)
    if index:
        return line[0:index]
    else:
        return line

def get_arg_part(line, op):
    line = cut_comment(line)
    m = re.match('^\t[^\t]*\t+(.*)$', line)
    if not m:
        return ''
    tmp = m.groups()
    return tmp[0]

def get_use_and_def_items(target_config, line, op):
    sub_line = get_arg_part(line, op)
    term_a_list = make_term_a_list(sub_line)
    term_a_list = fix_term_a_list(term_a_list)
    fix_term_a_list_2(term_a_list)
    fix_symbol_term(op, term_a_list)
    (use_a_list, def_a_list) = split_use_and_def(target_config, op, term_a_list)
    use_id_list = get_register_id_list(use_a_list)
    def_id_list = get_register_id_list(def_a_list)
    if target_config.load_op_p(op):
        use_id_list.append('MEM')
    elif target_config.store_op_p(op):
        def_id_list.append('MEM')
    else:
        if read_flags_op(op):
            use_id_list.append('NZCV')
        if write_flags_op(op):
            def_id_list.append('NZCV')
    return (use_id_list, def_id_list)

def make_term_a_list(line):
    size = len(line)
    term_a_list = []
    pos = 0
    a_part_p = False
    b_part_p = False
    start_pos = None
    while pos < size:
        c = line[pos]
        if c == ',':
            start_pos = make_term_a_list_sub(line, term_a_list, start_pos, pos)
            if a_part_p or b_part_p:
                term_a_list.append(('c', None))
            else:
                term_a_list.append(('C', None))
        elif c == ' ' or c == '\t':
            start_pos = make_term_a_list_sub(line, term_a_list, start_pos, pos)
        elif c == '[':
            start_pos = make_term_a_list_sub(line, term_a_list, start_pos, pos)
            a_part_p = True
            term_a_list.append(('AS', None))
        elif c == ']':
            start_pos = make_term_a_list_sub(line, term_a_list, start_pos, pos)
            a_part_p = False
            term_a_list.append(('AE', None))
        elif c == '{':
            start_pos = make_term_a_list_sub(line, term_a_list, start_pos, pos)
            b_part_p = True
            term_a_list.append(('BS', None))
        elif c == '}':
            start_pos = make_term_a_list_sub(line, term_a_list, start_pos, pos)
            b_part_p = False
            term_a_list.append(('BE', None))
        elif c == '!':
            start_pos = make_term_a_list_sub(line, term_a_list, start_pos, pos)
            term_a_list.append(('BANG', None))
        elif c == '-' and b_part_p:
            start_pos = make_term_a_list_sub(line, term_a_list, start_pos, pos)
            term_a_list.append(('BAR', None))
        else:
            if start_pos == None:
                start_pos = pos
        pos += 1
    if a_part_p or b_part_p:
        driver.internal_error('make_term_a_list')
    make_term_a_list_sub(line, term_a_list, start_pos, pos)
    return term_a_list

def register_pattern_p(item):
    reg_1_pattern = re.compile('b\d+|h\d+|s\d+|d\d+|q\d+|r\d+|x\d+|w\d+|xzr|wzr|sp|wsp')
    m = reg_1_pattern.match(item, 0)
    if m:
        return True
    reg_2_pattern = re.compile('v\d+\.\d*[bhsd]')
    m = reg_2_pattern.match(item, 0)
    if m:
        return True
    reg_3_pattern = re.compile('v\d+')
    m = reg_3_pattern.match(item, 0)
    if m:
        return True
    return False

def make_term_a_list_sub(line, term_a_list, start_pos, pos):
    if start_pos != None:
        item = line[start_pos:pos]
        if register_pattern_p(item):
            term_a_list.append(('REG', item))
        else:
            term_a_list.append(('TERM', item))
    return None

def fix_term_a_list(term_a_list):
    fix_term_a_list = []
    prev_tag = None
    prev_term = None
    for term in term_a_list:
        (tag, item) = term
        if tag == 'BANG':
            if prev_tag == 'AE':
                fix_term_a_list.append(('AE!', None))
            else:
                driver.internal_error('fix_term_a_list')
            prev_tag = None
            prev_term = None
        else:
            if prev_term:
                fix_term_a_list.append(prev_term)
            prev_tag = tag
            prev_term = term
    if prev_term:
        fix_term_a_list.append(prev_term)
    return fix_term_a_list

def fix_term_a_list_2(term_a_list):
    index = 0
    for term in term_a_list:
        (tag, item) = term
        if tag == 'AS':
            fix_term_a_list_2_sub(term_a_list, index)
        index += 1

def fix_term_a_list_2_sub(term_a_list, index):
    (tag1, item1) = term_a_list[index+1]
    if tag1 == 'TERM':
        (tag2, item2) = term_a_list[index+2]
        if tag2 == 'AE':
            term_a_list[index] = ('SS', None)
            term_a_list[index+2] = ('SE', None)

address_op_dict = {'adr': None,
                   'adrl': None,
                   'adrp': None,
                   'ldr': None,
                   'ldrsw': None,
                   'prfm': None}

def fix_symbol_term(op, term_a_list):
    global address_op_dict
    if op in address_op_dict:
        (tag, item) = term_a_list[2]
        if tag == 'REG':
            term_a_list[2] = ('TERM', item)

def split_use_and_def(target_config, op, term_a_list):
    use_a_list = []
    def_a_list = []
    if target_config.load_op_p(op):
        (first_a_list, second_a_list) = cut_load_term(term_a_list)
        use_a_list = second_a_list
        def_a_list = first_a_list
        base = get_pre_or_post_index_reg(term_a_list)
        if base:
            def_a_list.append(base)
    elif target_config.store_op_p(op):
        use_a_list = term_a_list.copy()
        base = get_pre_or_post_index_reg(term_a_list)
        if base:
            def_a_list.append(base)
    elif overwrite_op_p(op):
        (first_a_list, second_a_list) = cut_first_term(term_a_list)
        use_a_list = term_a_list.copy()
        def_a_list = first_a_list
    elif only_use_op_p(op):
        use_a_list = term_a_list.copy()
    else:
        (first_a_list, second_a_list) = cut_first_term(term_a_list)
        use_a_list = second_a_list
        def_a_list = first_a_list
    return (use_a_list, def_a_list)

def get_pre_or_post_index_reg(term_a_list):
    address_mode = 0
    base = None
    for term in term_a_list:
        (tag, item) = term
        if tag == 'AS':
            address_mode = 1
        elif tag == 'AE':
            address_mode = 2
        elif tag == 'AE!':
            if base == None:
                driver.internal_error('get_pre_or_post_index_reg 1')
            return base
        elif address_mode == 1:
            if tag == 'REG':
                base = (tag, item)
                address_mode = 3
            else:
                driver.internal_error('get_pre_or_post_index_reg 2')
        elif address_mode == 2:
            if tag == 'TERM':
                return base
        else:
            pass
    return None

def cut_first_term(term_a_list):
    first_p = True
    first_a_list = []
    second_a_list = []
    for term in term_a_list:
        (tag, item) = term
        if tag == 'C':
            first_p = False
        elif first_p:
            first_a_list.append(term)
        else:
            second_a_list.append(term)
    return (first_a_list, second_a_list)

def cut_load_term(term_a_list):
    first_p = True
    first_a_list = []
    second_a_list = []
    for term in term_a_list:
        (tag, item) = term
        if tag == 'AS':
            first_p = False
            second_a_list.append(term)
        elif first_p:
            first_a_list.append(term)
        else:
            second_a_list.append(term)
    return (first_a_list, second_a_list)

# TODO: ???
# register overlap (x3, w3)
# partial register usage
# hidden registers (status registers)
def get_register_id_list(term_a_list):
    id_list = []
    for term in term_a_list:
        (tag, item) = term
        if tag == 'REG':
            id = get_register_id(item)
            if id:
                id_list.append(id)
    index = 0
    for term in term_a_list:
        (tag, item) = term
        if tag == 'BAR':
            add_id_list = get_register_id_list_sub(term_a_list, index)
            for id in add_id_list:
                id_list.append(id)
        index += 1
    return id_list

def get_register_id_list_sub(term_a_list, index):
    add_id_list = []
    (tag1, item1) = term_a_list[index-1]
    (tag2, item2) = term_a_list[index+1]
    if tag1 == 'REG' and tag2 == 'REG':
        id1 = get_register_id(item1)
        nid1 = int(id1)
        id2 = get_register_id(item2)
        nid2 = int(id2)
        tmp = nid1 + 1
        while tmp < nid2:
            add_id_list.append(str(tmp))
            tmp += 1
    return add_id_list

def get_register_id(item):
    v_pattern = re.compile('b\d+|h\d+|s\d+|d\d+|q\d+')
    v2_pattern = re.compile('v\d+')
    r_pattern = re.compile('r\d+|x\d+|w\d+')
    if item in ['xzr', 'wzr']:
        return None
    if item in ['sp', 'wsp']:
        return 'sp'
    m = v_pattern.match(item, 0)
    if m:
        r = m.group()
        id = int(r[1:]) + 3000
        return str(id)
    m = v2_pattern.match(item, 0)
    if m:
        r = m.group()
        id = int(r[1:]) + 3000
        return str(id)
    m = r_pattern.match(item, 0)
    if m:
        r = m.group()
        id = int(r[1:]) + 2000
        return str(id)
    driver.internal_error('get_register_id: ' + item)

overwrite_op_dict = {'movk': None,
                     'fmla': None,
                     'mla': None,
                     'saba': None,
                     'sabal': None,
                     'sabal2': None,
                     'sabd': None,
                     'sabdl': None,
                     'sabdl2': None,
                     'sadalp': None,
                     'smlal': None,
                     'smlal2': None,
                     'sqdmlal': None,
                     'sqdmlal2': None,
                     'sqrdmlah': None,
                     'srshr': None,
                     'srsra': None,
                     'sshr': None,
                     'ssra': None,
                     'suqadd': None,
                     'uaba': None,
                     'uabal': None,
                     'uabal2': None,
                     'uabd': None,
                     'uabdl': None,
                     'uabdl2': None,
                     'uadalp': None,
                     'umlal': None,
                     'umlal2': None,
                     'urshr': None,
                     'ursra': None,
                     'ushr': None,
                     'usqadd': None,
                     'usra': None}
    
def overwrite_op_p(op):
    global overwrite_op_dict
    return op in overwrite_op_dict

def only_use_op_p(op):
    if op in ['prfm', 'cmp', 'nop']:
        return True
    return False

def uc_op_p(op):
    if op in ['msr', 'mrs']:
        return True
    return False

write_flags_op_dict = {'adds': None,
                       'subs': None,
                       'cmp': None,
                       'cmn': None,
                       'ands': None,
                       'tst': None,
                       'negs': None,
                       'adcs': None,
                       'sbcs': None,
                       'ngcs': None,
                       'bics': None,
                       'ccmn': None,
                       'ccmp': None,
                       'fcmp': None,
                       'fcmpe': None,
                       'fccmp': None,
                       'fccmpe': None}

def write_flags_op(op):
    global write_flags_op_dict
    return op in write_flags_op_dict

read_flags_op_dict = {'addc': None,
                      'sbc': None,
                      'ngc': None,
                      'csel': None,
                      'csinc': None,
                      'csinv': None,
                      'csneg': None,
                      'cset': None,
                      'csetm': None,
                      'cinc': None,
                      'cinv': None,
                      'cneg': None,
                      'ccmn': None,
                      'ccmp': None,
                      'fcsel': None}

def read_flags_op(op):
    global read_flags_op_dict
    return op in read_flags_op_dict

def get_execution_trace(execution_trace_map, item):
    if item in execution_trace_map:
        return execution_trace_map[item]
    else:
        trace_list = []
        execution_trace_map[item] = trace_list
        return trace_list

def put_execution_trace(execution_trace_map, index, tag, item):
    trace_list = get_execution_trace(execution_trace_map, item)
    trace_list.insert(0, (tag, index))

def add_dependence_edge(dg, p_index, index):
    if p_index in dg:
        next_list = dg[p_index]
        if not (index in next_list):
            next_list.append(index)
    else:
        dg[p_index] = [ index ]

def r_to_r_id(items):
    fix_items = []
    v_pattern = re.compile('b\d+|h\d+|s\d+|d\d+|q\d+')
    v2_pattern = re.compile('v\d+')
    r_pattern = re.compile('r\d+|x\d+|w\d+')
    for item in items:
        if item in ['xzr', 'wzr']:
            continue
        if item in ['sp', 'wsp']:
            fix_items.append('sp')
            continue
        m = v_pattern.match(item, 0)
        if m:
            r = m.group()
            id = int(r[1:]) + 3000
            fix_items.append(str(id))
            continue
        m = v2_pattern.match(item, 0)
        if m:
            r = m.group()
            id = int(r[1:]) + 3000
            fix_items.append(str(id))
            continue
        m = r_pattern.match(item, 0)
        if m:
            r = m.group()
            id = int(r[1:]) + 2000
            fix_items.append(str(id))
            continue
        if item in ['MEM', 'NZCV']:
            fix_items.append(item)
            continue
        driver.internal_error('get_use_and_def_items: 3')
    return fix_items

def add_dependence(dg, execution_trace_map, index, use_items, def_items):
    for item in use_items:
        trace_list = get_execution_trace(execution_trace_map, item)
        for (tag, p_index) in trace_list:
            if tag == 'def':
                add_dependence_edge(dg, p_index, index)
                break
    for item in def_items:
        trace_list = get_execution_trace(execution_trace_map, item)
        for (tag, p_index) in trace_list:
            add_dependence_edge(dg, p_index, index)
            if tag == 'def':
                break
    trace_list = get_execution_trace(execution_trace_map, 'CALL')
    for (tag, p_index) in trace_list:
        add_dependence_edge(dg, p_index, index)
        if tag != 'def':
            driver.internal_error('add_dependence')
        break
    for item in use_items:
        put_execution_trace(execution_trace_map, index, 'use', item)
    for item in def_items:
        put_execution_trace(execution_trace_map, index, 'def', item)

def add_call_dependence(dg, execution_trace_map, index):
    for item in execution_trace_map.keys():
        trace_list = get_execution_trace(execution_trace_map, item)
        for (tag, p_index) in trace_list:
            add_dependence_edge(dg, p_index, index)
    put_execution_trace(execution_trace_map, index, 'def', 'CALL')

def get_max_height(dg, max_index):
    map = {}
    index = 0
    while index < max_index:
        map[index] = 1
        index += 1
    index = 0
    while index < max_index:
        if index in dg:
            height = map[index] + 1
            prev_list = dg[index]
            for p in prev_list:
                p_height = map[p]
                if p_height < height:
                    map[p] = height
        index += 1
    max_height = 1
    for height in map.values():
        if max_height < height:
            max_height = height
    return max_height

def get_dependence_height(target_config, bb):
    dg = {}
    execution_trace_map = {}
    index = 0
    for line in bb.line_list:
        label = target_config.bb_label(line)
        branch_info = target_config.bb_branch(line)
        op = target_config.op(line)
        if label:
            pass
        elif branch_info:
            branch_op = branch_info[0]
            branch_target = branch_info[1]
            if target_config.call_p(branch_op, branch_target):
                add_call_dependence(dg, execution_trace_map, index)
        elif op:
            (use_items, def_items) = get_use_and_def_items(target_config, line, op)
            add_dependence(dg, execution_trace_map, index, use_items, def_items)
        else:
            pass
        index += 1
    return get_max_height(dg, index)

class HeightMetricWorker(driver.MetricWorker):
    def match_p(self, target_config, test_name):
        if target_config.arch == "aarch64":
            return True
        return False

    def set_up_before_getting_data(self, target_config, bb_list):
        pass

    def get_column_name_list(self):
        return [ 'height' ]

    def get_data_list(self, target_config, bb):
        len  = get_dependence_height(target_config, bb)
        return [ str(len) ]

    def get_summary_list(self, target_config):
        return ["-"]

if __name__ == "__main__":
    height_metric_worker = HeightMetricWorker()
    driver.run_metric_work_if_match_p(sys.argv, height_metric_worker)
