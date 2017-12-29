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

def parse_arg_part_core(line):
    size = len(line)
    assoc_list = []
    pos = 0
    reg1_pattern = re.compile('b\d+|h\d+|s\d+|d\d+|q\d+|r\d+|x\d+|w\d+|xzr|wzr|sp|wsp')
    reg2_pattern = re.compile('v\d+')
    reg2_1_pattern = re.compile('v\d+\.\d*[bhsd]')
    reg2_2_pattern = re.compile('v\d+\.\d*[bhsd]\[\d+\]')
    space_pattern = re.compile('\s+|\t+')
    comma_pattern = re.compile(',')
    as_pattern = re.compile('\[')
    aep_pattern = re.compile('\]!')
    ae_pattern = re.compile('\]')
    bs_pattern = re.compile('{')
    be_pattern = re.compile('}')
    dash_pattern = re.compile('-')
    s_pattern = re.compile('\s|\t|,|\[|\]|}')
    imm1_pattern = re.compile('#?0x[abcdef\d]+')
    imm2_pattern = re.compile('#?-?\d+')
    lo12_pattern = re.compile('#?:lo12:')
    sym_pattern = re.compile('\.?[\w_][\._\w\d]*')
    sym_tail_pattern = re.compile('\+\d+')
    b_part_p = False
    while pos < size:
        lo12_head = None
        m = space_pattern.match(line, pos)
        if m:
            tmp = m.group()
            pos = pos + len(tmp)
            continue
        m = comma_pattern.match(line, pos)
        if m:
            tmp = m.group()
            pos = pos + len(tmp)
            continue
        m = as_pattern.match(line, pos)
        if m:
            assoc_list.append(('AS', None))
            pos = pos + 1
            continue
        m = aep_pattern.match(line, pos)
        if m:
            assoc_list.append(('AE!', None))
            pos = pos + 2
            continue
        m = ae_pattern.match(line, pos)
        if m:
            assoc_list.append(('AE', None))
            pos = pos + 1
            continue
        m = bs_pattern.match(line, pos)
        if m:
            assoc_list.append(('BS', None))
            pos = pos + 1
            b_part_p = True
            continue
        m = be_pattern.match(line, pos)
        if m:
            assoc_list.append(('BE', None))
            pos = pos + 1
            b_part_p = False
            continue
        if b_part_p:
            m = dash_pattern.match(line, pos)
            if m:
                tmp = m.group()
                assoc_list.append(('SYM', tmp))
                pos = pos + len(tmp)
                continue
        m = reg1_pattern.match(line, pos)
        if not m:
            m = reg2_pattern.match(line, pos)
            if m:
                m1 = reg2_1_pattern.match(line, pos)
                if m1:
                    m2 = reg2_2_pattern.match(line, pos)
                    if m2:
                        m = m2
                    else:
                        m = m1
        if m:
            tmp = m.group()
            tmp_pos = pos + len(tmp)
            if tmp_pos == size:
                assoc_list.append(('R', tmp))
                pos = tmp_pos
                continue
            m = s_pattern.match(line, tmp_pos)
            if m:
                assoc_list.append(('R', tmp))
                pos = tmp_pos
                continue
            #print('>>> ' + tmp + ':' + line[pos:])
            #driver.error_message('check')
        m = imm1_pattern.match(line, pos)
        if not m:
            m = imm2_pattern.match(line, pos)
        if m:
            tmp = m.group()
            assoc_list.append(('IMM', tmp))
            pos = pos + len(tmp)
            continue
        m = lo12_pattern.match(line, pos)
        if m:
            lo12_head = m.group()
            pos = pos + len(lo12_head)
        m = sym_pattern.match(line, pos)
        if m:
            tmp = m.group()
            pos = pos + len(tmp)
            m = sym_tail_pattern.match(line, pos)
            if m:
                tmp_t = m.group()
                pos = pos + len(tmp_t)
                tmp = tmp + tmp_t
            if lo12_head:
                tmp = lo12_head + tmp
            assoc_list.append(('SYM', tmp))
            continue
        driver.error_message('parse_arg_part_core: ' + line)
    return assoc_list

def parse_arg_part(line):
    assoc_list = parse_arg_part_core(line)
    address_part_p = False
    address_part = []
    b_part_p = False
    b_part = []
    result_assoc_list = []
    for (tag, item) in assoc_list:
        if tag == 'AS':
            address_part_p = True
            continue
        if tag == 'AE!' or tag == 'AE':
            address_part_p = False
            result_assoc_list.append((tag, address_part))
            continue
        if tag == 'BS':
            b_part_p = True
            continue
        if tag == 'BE':
            b_part_p = False
            result_assoc_list.append(('B', b_part)) 
            continue
        if address_part_p:
            address_part.append((tag, item))
        elif b_part_p:
            b_part.append((tag, item))
        else:
            result_assoc_list.append((tag, item))
    if address_part_p or b_part_p:
        driver.error_message('parse_arg_part')
    return result_assoc_list

def add_item(any_items, item):
    if item and (not item in any_items):
        any_items.append(item)

def overwrite_op_p(op):
    if op in ['movk']:
        return True
    return False

def only_use_op_p(op):
    if op in ['cmp', 'nop']:
        return True
    return False

def label_reference_op_p(op):
    if op in ['adr', 'adrp']:
        return True
    return False

def uc_op_p(op):
    if op in ['prfm', 'msr', 'mrs']:
        return True
    return False


def write_flags_op(op):
    if op in ['adds', 'subs', 'cmp', 'cmn', 'ands', 'tst', 'negs', 'adcs', 'sbcs', 'ngcs', 'bics', 'ccmn', 'ccmp']:
        return True
    if op in ['fcmp', 'fcmpe', 'fccmp', 'fccmpe']:
        return True
    return False

def read_flags_op(op):
    if op in ['addc', 'sbc', 'ngc, ', 'csel', 'csinc', 'csinv', 'csneg', 'cset', 'csetm', 'cinc', 'cinv', 'cneg',  'ccmn', 'ccmp']:
        return True
    if op in ['fcsel']:
        return True
    return False

def form_group_p(form_list, assoc_list):
    for form in form_list:
        if form_p(form, assoc_list):
            return True
    return False

def form_p(form, assoc_list):
    index = 0
    size = len(form)
    if size != len(assoc_list):
        return False
    while index < size:
        f = form[index]
        (tag, content) = assoc_list[index]
        index += 1
        if f == 'R':
            if tag != 'R':
                return False
            if content == 'NZCV' or content == 'FPSR':
                driver.error_message('form_p:' + content)
            continue
        elif f == 'A':
            if not tag in ['AE']:
                return False
            continue
        elif f == '!':
            if not tag in ['AE!']:
                return False
            continue
        elif f == 'B':
            if not tag in ['B']:
                return False
            continue
        elif f == 'S':
            if not tag in ['SYM', 'R']:
                return False
            continue
        elif f == 'I':
            if tag != 'IMM':
                return False
            continue
        else:
            driver.error_message('form_p')
    return True

def extract_items(assoc_list):
    a_head = None
    item_list = []
    for (tag, item) in assoc_list:
        if tag == 'R':
            item_list.append(item)
            continue
        elif tag == 'AE!' or tag == 'AE':
            for (tag2, item2) in item:
                if a_head == None:
                    if tag2 != 'R':
                        driver.error_message('extract_items 1')
                    a_head = item2
                if tag2 == 'R':
                    item_list.append(item2)
            continue
        elif tag == 'B':
            for (tag2, item2) in item:
                if tag2 == 'R':
                    item_list.append(item2)
            continue
        elif tag == 'IMM' or tag == 'SYM':
            continue
        else:
            driver.error_message('extract_items 2')
    return (a_head, item_list)

def check_1(line, tmp_list):
    if 0 < len(tmp_list):
        return
    driver.error_message('check_1')

# TODO: ???
# register overlap (x3, w3)
# hidden registers (status registers)
def get_use_and_def_items(target_config, line, op):
    use_items = []
    def_items = []
    arg_part = get_arg_part(line, op)
    assoc_list = parse_arg_part(arg_part)
    if uc_op_p(op):
        driver.error_message('get_use_and_def_items: 0')
    if target_config.load_op_p(op):
        (a_head, item_list) = extract_items(assoc_list)
        if form_group_p(['R!', 'RAI'], assoc_list):
            def_items.append(item_list[0])
            def_items.append(a_head)
            for x in item_list[1:]:
                add_item(use_items, x)
        elif form_group_p(['RR!', 'RRAI'], assoc_list):
            def_items.append(item_list[0])
            def_items.append(item_list[1])
            def_items.append(a_head)
            for x in item_list[2:]:
                add_item(use_items, x)
        elif form_group_p(['RA'], assoc_list):
            def_items.append(item_list[0])
            for x in item_list[1:]:
                add_item(use_items, x)
        elif form_group_p(['RRA'], assoc_list):
            def_items.append(item_list[0])
            def_items.append(item_list[1])
            for x in item_list[2:]:
                add_item(use_items, x)
        elif form_group_p(['BA'], assoc_list):
            pass
        else:
            driver.error_message('get_use_and_def_items: 1')
        use_items.append('MEM')
    elif target_config.store_op_p(op):
        (a_head, item_list) = extract_items(assoc_list)
        if form_group_p(['R!', 'RAI'], assoc_list):
            def_items.append(a_head)
        elif form_group_p(['RR!', 'RRAI'], assoc_list):
            def_items.append(a_head)
        elif form_group_p(['RA'], assoc_list):
            pass
        elif form_group_p(['RRA'], assoc_list):
            pass
        elif form_group_p(['BA'], assoc_list):
            pass
        else:
            driver.error_message('get_use_and_def_items: 2')
        for x in item_list:
            add_item(use_items, x)
        def_items.append('MEM')
    elif label_reference_op_p(op):
        (a_head, item_list) = extract_items(assoc_list)
        if form_group_p(['RS', 'RI'], assoc_list):
            def_items.append(item_list[0])
        elif form_group_p(['RR'], assoc_list):
            # RR --> RS
            def_items.append(item_list[0])
        else:
            driver.error_message('get_use_and_def_items: 3')
    elif overwrite_op_p(op):
        (a_head, item_list) = extract_items(assoc_list)
        check_1(line, item_list)
        def_items.append(item_list[0])
        for x in item_list:
            add_item(use_items, x)
    elif only_use_op_p(op):
        (a_head, item_list) = extract_items(assoc_list)
        for x in item_list:
            add_item(use_items, x)
        if read_flags_op(op):
            use_items.append('NZCV')
        if write_flags_op(op):
            def_items.append('NZCV')
    else:
        (a_head, item_list) = extract_items(assoc_list)
        check_1(line, item_list)
        def_items.append(item_list[0])
        for x in item_list[1:]:
            add_item(use_items, x)
        if read_flags_op(op):
            use_items.append('NZCV')
        if write_flags_op(op):
            def_items.append('NZCV')
    return (use_items, def_items)

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
            n = r[1:]
            id = int(r[1:]) + 2000
            fix_items.append(str(id))
            continue
        if item in ['MEM', 'NZCV']:
            fix_items.append(item)
            continue
        driver.error_message('get_use_and_def_items: 3')
    return fix_items
    
def add_dependence(dg, execution_trace_map, index, use_items, def_items):
    use_items = r_to_r_id(use_items)
    def_items = r_to_r_id(def_items)
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
            driver.error_message('add_dependence')
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
        return True

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
