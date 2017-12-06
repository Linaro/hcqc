import sys
import re

import graph

verbose_p = False

last_bb = None

bb_list = []

class BasicBlock:
    def __init__(self, start_line_number, label):
        self.start_line_number = start_line_number
        self.label = label
        self.line_list = []
        self.fall_through_p = True
        self.target_label = None
        self.target_bb = None
        self.fall_through_bb = None
        self.table_bb_list = None
        self.depth = 0

    def append_line(self, line):
        self.line_list.append(line)

    def set_fall_through_p(self, bool):
        self.fall_through_p = bool

def error_message(message):
    print('Error: ' + message + '\n')
    sys.exit(1)

def debug_print_for_line(line_number, label, branch_info, line):
    label_name = '?ln'
    if label:
        label_name = label
    bn = '?bn'
    bt = '?bt'
    if branch_info:
        if branch_info[0]:
            bn = branch_info[0]
        if branch_info[1]:
            bt = branch_info[1]
    print('>>> ' + str(line_number) + ':' + label_name + ':' + bn + ':' + bt + ':' + '>>> ' + line[:-1])

def make_sure_last_bb(line_number, label):
    global bb_list
    global last_bb
    if not last_bb:
        last_bb = BasicBlock(line_number, label)
        bb_list.append(last_bb)

def deal_with_line(target_config, table_branch_map, line_number, line):
    global last_bb
    label = target_config.bb_label(line)
    branch_info = target_config.bb_branch(line)
    if verbose_p:
        debug_print_for_line(line_number, label, branch_info, line)
    if label:
        if branch_info:
            error_message("deal_with_line")
        last_bb = None
        make_sure_last_bb(line_number, label)
        return
    if branch_info:
        branch_op = branch_info[0]
        branch_target = branch_info[1]
        make_sure_last_bb(line_number, None)
        last_bb.append_line(line)
        if target_config.call_p(branch_op, branch_target):
            pass
        elif target_config.tail_call_p(branch_op, branch_target):
            last_bb.set_fall_through_p(False)
            last_bb = None
        elif target_config.branch_by_register_p(branch_op, branch_target):
            table_branch_label = find_table_branch_label(table_branch_map, last_bb)
            if target_config.table_branch_p(branch_op, branch_target, table_branch_label, last_bb.line_list):
                last_bb.set_fall_through_p(False)
                last_bb.table_bb_list = [ table_branch_label ]
                last_bb = None
            else:
                # This case is for tail calls using function pointers.
                last_bb.set_fall_through_p(False)
                last_bb = None
        elif target_config.fall_through_p(branch_op):
            if branch_target:
                last_bb.target_label = branch_target
            last_bb = None
        else:
            last_bb.set_fall_through_p(False)
            if branch_target:
                last_bb.target_label = branch_target
            last_bb = None
        return
    if target_config.op(line):
        make_sure_last_bb(line_number, None)
        last_bb.append_line(line)
    else:
        if verbose_p:
            print('  Ignored: ' + line)

def make_previous_map(bb_list):
    last_bb = None
    previous_map = {}
    for bb in bb_list:
        previous_map[bb] = []
    for bb in bb_list:
        if bb.target_bb:
            previous_map[bb.target_bb].append(bb)
        if bb.table_bb_list:
            for tbb in bb.table_bb_list:
                if not (bb in previous_map[tbb]):
                    previous_map[tbb].append(bb)
        if last_bb and last_bb.fall_through_p:
            if not (last_bb in previous_map[bb]):
                previous_map[bb].append(last_bb)
        last_bb = bb
    return previous_map

def make_next_map(bb_list):
    last_bb = None
    next_map = {}
    for bb in bb_list:
        next_map[bb] = []
    for bb in bb_list:
        if bb.target_bb:
            next_map[bb].append(bb.target_bb)
        if bb.table_bb_list:
            for tbb in bb.table_bb_list:
                if not (tbb in next_map[bb]):
                    next_map[bb].append(tbb)
        if last_bb and last_bb.fall_through_p:
            if not (bb in next_map[last_bb]):
                next_map[last_bb].append(bb)
        last_bb = bb
    return next_map

def reachable_p(reachable_map, previous_bb_list):
    for bb in previous_bb_list:
        if reachable_map[bb]:
            return True
    return False

def make_reachable_map(bb_list):
    reachable_map = {}
    previous_map = make_previous_map(bb_list)
    for bb in bb_list:
        reachable_map[bb] = False
    reachable_map[bb_list[0]] = True
    changed_p = True
    while changed_p:
        changed_p = False
        for bb in bb_list:
            if reachable_map[bb] == False and reachable_p(reachable_map, previous_map[bb]):
                reachable_map[bb] = True
                changed_p = True
    return reachable_map

def cut_dead_bb(bb_list):
    global verbose_p
    fix_bb_list = []
    reachable_map = make_reachable_map(bb_list)
    for bb in bb_list:
        if reachable_map[bb]:
            fix_bb_list.append(bb)
    return fix_bb_list

def cut_redundant_tail_bb(bb_list):
    global verbose_p
    reachable_map = make_reachable_map(bb_list)
    if verbose_p:
        for bb in bb_list:
            if reachable_map[bb]:
                print(str(bb.start_line_number) + ' : ' + "live")
            else:
                print(str(bb.start_line_number) + ' : ' + "dead")
    changed_p = True
    while changed_p:
        changed_p = False
        tail_bb = bb_list[-1]
        if not reachable_map[tail_bb]:
            del bb_list[-1]
            if verbose_p:
                print(' cut --> ' + str(tail_bb.start_line_number))
            changed_p = True
    return bb_list

def link_bb(target_config, bb_list, table_branch_map):
    previous_bb = None
    map = {}
    for bb in bb_list:
        if previous_bb and previous_bb.fall_through_p:
            previous_bb.fall_through_bb = bb
        if bb.label:
            map[bb.label] = bb
        previous_bb = bb
    for bb in bb_list:
        if bb.target_label:
            tbb = map[bb.target_label]
            if not tbb:
                error_message("cannot find bb")
            bb.target_bb = tbb
        elif bb.table_bb_list:
            table_branch_label = bb.table_bb_list[0]
            bb.table_bb_list = []
            label_list = table_branch_map[table_branch_label]
            for label in label_list:
                tbb = map[label]
                if not tbb:
                    error_message("cannot find bb")
                bb.table_bb_list.append(tbb)
    return bb_list

def find_table_branch_label(table_branch_map, bb):
    label_list = table_branch_map.keys()
    r_line_list = bb.line_list.copy()
    r_line_list.reverse()
    for label in label_list:
        pattern = '\\' + label + r'[^\w\d]+'
        for line in r_line_list:
            m = re.search(pattern, line)
            if m:
                return label
    return None

def debug_print_for_bb_list(bb_list):
    for bb in bb_list:
        label = bb.label
        if not label:
            label = ''
        print('BB(' + str(bb.start_line_number) + ':' + label + ')')
        for x in bb.line_list:
            print('  ' + x[:-1])

def draw_by_dot(fout, bb_list):
    print('digraph CFG {', file=fout)
    for bb in bb_list:
        line_number = bb.start_line_number
        id = str(line_number)
        label = bb.label
        depth = bb.depth
        if not label:
            label = ''
        print(id + ' [label="' + id + ':' + label + ':' + str(depth) + '"];', file=fout)
    for bb in bb_list:
        line_number = bb.start_line_number
        id = str(line_number)
        if bb.target_bb:
            print(id + ' -> ' + str(bb.target_bb.start_line_number) + ';', file=fout)
        if bb.fall_through_bb:
            print(id + ' -> ' + str(bb.fall_through_bb.start_line_number) + ';', file=fout)
        if bb.table_bb_list:
            for tbb in bb.table_bb_list:
                print(id + ' -> ' + str(tbb.start_line_number) + ';', file=fout)
    print('}',file=fout)

def find_depth(bb_list):
    previous_map = make_previous_map(bb_list)
    next_map = make_next_map(bb_list)
    find_depth_main(bb_list, previous_map, next_map)

def find_depth_main(bb_list, previous_map, next_map):
    loop_info_map = construct_natural_loop(bb_list, previous_map, next_map)
    for h in loop_info_map.keys():
        for loop_info in loop_info_map[h]:
            (bb, loop_set) = loop_info
            for bb in loop_set:
                bb.depth += 1
    for bb in bb_list:
        if bb in next_map[bb]:
            bb.depth += 1

def make_back_edge_list(bb_list, previous_map, next_map):
    dominator_tree = graph.make_dominator_tree(bb_list, previous_map, next_map)
    #dump_tree(bb_list, dominator_tree)
    back_edge_list = []
    for src in bb_list:
        for dest in next_map[src]:
            if graph.dominate_p(dominator_tree, dest, src):
                back_edge_list.append((src, dest))
    return back_edge_list

def dump_tree(bb_list, tree):
    print('digraph CFG {')
    mmm = {}
    index = 0
    for bb in bb_list:
        mmm[bb] = index
        index += 1
    for bb in bb_list:
        index = mmm[bb]
        line_number = bb.start_line_number
        label = bb.label
        depth = bb.depth
        if not label:
            label = ''
        print(str(index) + ' [label="' + str(line_number) + ':' + label + ':' + str(depth) + '"];')
    for bb in bb_list:
        next_bb_list = tree[bb]
        if next_bb_list:
            for nbb in next_bb_list:
                print(str(mmm[bb]) + ' -> ' + str(mmm[nbb]) + ';')
    print('}')

def construct_natural_loop(bb_list, previous_map, next_map):
    back_edge_list = make_back_edge_list(bb_list, previous_map, next_map)
    loop_info_map = {}
    for (src, dest) in back_edge_list:
        loop_set = construct_natural_loop_sub(bb_list, previous_map, src, dest)
        if not dest in loop_info_map:
            loop_info_map[dest] = []
        loop_info_map[dest].append((src, loop_set))
    return loop_info_map

def construct_natural_loop_sub(bb_list, previous_map, src, dest):
    stack = []
    loop_set = [dest]
    if src == dest:
        error_message("construct_natural_loop_sub")
    if not src in loop_set:
        loop_set.insert(0, src)
        stack.insert(0, src)
    while True:
        if len(stack) == 0:
            break
        m = stack.pop(0)
        p_list = previous_map[m]
        for p in p_list:
            if not p in loop_set:
                loop_set.insert(0, p)
                stack.insert(0, p)
    return loop_set

def build_table_branch_map(target_config, fin):
    table_branch_map = {}
    table_branch_label = None
    region_status = 0
    line = None
    while True:
        line = fin.readline()
        if not line:
            break
        if region_status == target_config.get_table_branch_prologue_number():
            label = target_config.get_table_branch_content(line)
            if label:
                if not table_branch_label in table_branch_map:
                    table_branch_map[table_branch_label] = []
                if not label in table_branch_map[table_branch_label]:
                    table_branch_map[table_branch_label].append(label)
            else:
                region_status = 0
                table_branch_label = None
        else:
            (flag_p, label) = target_config.trace_table_branch_prologue(region_status, line)
            if flag_p:
                region_status += 1
                if label:
                    table_branch_label = label
            else:
                region_status = 0
                table_branch_label = None
    return table_branch_map

def build_cfg(target_config, fin, function_name, table_branch_map):
    global bb_list
    global last_bb
    bb_list = []
    last_bb = None
    region_status = 0
    line = None
    line_number = 0
    while True:
        line = fin.readline()
        line_number += 1
        if not line:
            break
        if region_status == 0:
            if target_config.function_entry_p(function_name, line):
                region_status = 1
        if region_status == 1:
            deal_with_line(target_config, table_branch_map, line_number, line)
            if target_config.function_exit_p(function_name, line):
                region_status = 2
    if region_status != 2:
        error_message("cannot create control flow graph")
    bb_list = link_bb(target_config, bb_list, table_branch_map)
    bb_list = cut_dead_bb(bb_list)
    bb_list = cut_redundant_tail_bb(bb_list)
    find_depth(bb_list)
    if verbose_p:
        debug_print_for_bb_list(bb_list)
    return bb_list

def get_bb_name_pair(bb):
    label_name = ''
    ctf_name = ''
    label = bb.label
    if label:
        label_name = label
    target_bb = bb.target_bb
    if target_bb:
        target_name = target_bb.label
        if bb.fall_through_p:
            ctf_name = 'cond ' + target_name
        else:
            ctf_name = 'goto ' + target_name
    elif bb.table_bb_list:
        ctf_name = 'table'
    else:
        if not bb.fall_through_p:
            ctf_name = 'end'
    return (label_name, ctf_name)

def dump_column(bb_list):
    work_list = []
    column_list = []
    max_width = 0
    for bb in bb_list:
        (label_name, ctf_name) = get_bb_name_pair(bb)
        width = len(label_name) + len(ctf_name) + 1
        if max_width < width:
            max_width = width
        depth = str(bb.depth)
        work_list.append((label_name, ctf_name, depth))
    for x in work_list:
        (label_name, ctf_name, depth) = x
        width = len(label_name) + len(ctf_name)
        add_width = max_width - width
        space = ' ' * add_width
        bb_name = label_name + space + ctf_name
        column_list.append([bb_name, depth])
    return column_list

def get_bb_list(target_config, filename, function_name):
    bb_list = None
    with open(filename, 'rt') as fin:
        table_branch_map = build_table_branch_map(target_config, fin)
    with open(filename, 'rt') as fin:
        bb_list = build_cfg(target_config, fin, function_name, table_branch_map)
    return bb_list
