import sys

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
        #
        self.target_bb = None
        self.fall_through_bb = None
        #
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
    
def deal_with_line(target_config, line_number, line):
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
        if last_bb and last_bb.fall_through_p:
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
        if last_bb and last_bb.fall_through_p:
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
            
def link_bb(bb_list):
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
    return bb_list

def debug_print_for_bb_list(bb_list):
    for bb in bb_list:
        n = bb.label
        if not n:
            n = ''
        print('BB(' + str(bb.start_line_number) + ':' + n + ')')
        for x in bb.line_list:
            print('  ' + x[:-1])

def draw_by_dot(fout, bb_list):
    print('digraph CFG {', file=fout)
    for bb in bb_list:
        line_number = bb.start_line_number
        label = bb.label
        depth = bb.depth
        if not label:
            label = ''
        print(str(line_number) + ' [label="' + str(line_number) + ':' + label + ':' + str(depth) + '"];', file=fout)
    for bb in bb_list:
        line_number = bb.start_line_number        
        if bb.target_bb:
            print(str(line_number) + ' -> ' + str(bb.target_bb.start_line_number) + ';', file=fout)
        if bb.fall_through_bb:
            print(str(line_number) + ' -> ' + str(bb.fall_through_bb.start_line_number) + ';', file=fout)
    print('}',file=fout)

### Depth First Search

current_time = None

def white_p(status_map, bb):
    if status_map[bb][0] is None and status_map[bb][1] is None:
        return True
    else:
        return False
    
def gray_p(status_map, bb):
    if status_map[bb][0] is not None and status_map[bb][1] is None:
        return True
    else:
        return False

def black_p(status_map, bb):
    if status_map[bb][0] is not None and status_map[bb][1] is not None:
        return True
    else:
        return False    
    
def paint_gray(status_map, bb):
    global current_time
    if not white_p(status_map, bb):
        error_message("paint_gray")
    status_map[bb][0] = current_time
    current_time += 1

def paint_black(status_map, bb):
    global current_time
    if not gray_p(status_map, bb):
        error_message("paint_black")
    status_map[bb][1] = current_time
    current_time += 1    

def depth_first_search(next_map, status_map, bb):
    paint_gray(status_map, bb)
    next_bb_list = next_map[bb]
    for next_bb in next_bb_list:
        if white_p(status_map, next_bb):
            depth_first_search(next_map, status_map, next_bb)
    paint_black(status_map, bb)

def make_back_edge_map(bb_list, next_map):
    global current_time
    current_time = 0
    start_bb = bb_list[0]
    status_map = {}
    for bb in bb_list:
        status_map[bb] = [ None, None ]
    depth_first_search(next_map, status_map, start_bb)
    for bb in bb_list:
        if not black_p(status_map, bb):
            error_message("make_back_edge_map")
    back_edge_map = {}
    for bb in bb_list:
            back_edge_map[bb] = []
    for bb in bb_list:
        next_bb_list = next_map[bb]
        for next_bb in next_bb_list:
            if bb == next_bb:
                back_edge_map[bb].append(next_bb)
            if status_map[next_bb][0] < status_map[bb][0] and status_map[bb][1] < status_map[next_bb][1]:
                back_edge_map[bb].append(next_bb)
    return back_edge_map

def make_header_map(bb_list, next_map):
    back_edge_map = make_back_edge_map(bb_list, next_map)
    header_map = {}
    for bb in bb_list:
        dest_list = back_edge_map[bb]
        if len(dest_list) != 0:
            for dest_bb in dest_list:
                if dest_bb not in header_map:
                    header_map[dest_bb] = []
                header_map[dest_bb].append(bb)
    return header_map

def find_depth(bb_list):
    global verbose_p
    previous_map = make_previous_map(bb_list)
    next_map = make_next_map(bb_list)
    header_map = make_header_map(bb_list, next_map)
    add_space_list = []
    for bb in bb_list:
        if bb in header_map:
            header = bb
            src_list = header_map[header]
            if len(src_list) == 1 and len(next_map[src_list[0]]) == 1:
                pass
            else:
                if verbose_p:
                    print('add_bb_on_edge (src_list -> header)')
                add_space_list.append((src_list,header))
            external_list = []
            for x in previous_map[header]:
                if x not in src_list:
                    external_list.append(x)
            if len(external_list) == 1 and len(next_map[external_list[0]]) == 1:
                pass
            else:
                if verbose_p:
                    print('add_bb_on_edge (external_list -> header)')
                add_space_list.append((external_list,header))
    find_depth_main(bb_list, previous_map, next_map, add_space_list)

def find_depth_main(bb_list, previous_map, next_map, add_space_list):
    fix_bb_list = bb_list.copy()
    fix_previous_map = previous_map.copy()
    fix_next_map = next_map.copy()
    dummy_bb = 0
    for add_space in add_space_list:
        (src_list, dest) = add_space
        for src in src_list:
            delete_edge(fix_previous_map, dest, src)
            delete_edge(fix_next_map, src, dest)
        fix_bb_list.append(dummy_bb)
        fix_previous_map[dummy_bb] = []
        fix_next_map[dummy_bb] = []
        for src in src_list:
            fix_previous_map[dummy_bb].append(src)
            fix_next_map[src].append(dummy_bb)
        fix_previous_map[dest].append(dummy_bb)
        fix_next_map[dummy_bb].append(dest)
        dummy_bb += 1
    loop_info_map = construct_natural_loop(fix_bb_list, fix_previous_map, fix_next_map)
    for h in loop_info_map.keys():
        for loop_info in loop_info_map[h]:
            (bb, loop_set) = loop_info
            for bb in loop_set:
                if not isinstance(bb, int):
                    bb.depth += 1

def delete_edge(map, src, dest):
    dest_list = map[src]
    if not dest in dest_list:
        error_message("delete_edge")
    dest_list.remove(dest)
    
def construct_natural_loop(bb_list, previous_map, next_map):
    back_edge_map = make_back_edge_map(bb_list, next_map)
    loop_info_map = {}
    for bb in bb_list:
        dest_list = back_edge_map[bb]
        for dest in dest_list:
            loop_set = construct_natural_loop_sub(bb_list, previous_map, bb, dest)
            if not dest in loop_info_map:
                loop_info_map[dest] = []
            loop_info_map[dest].append((bb, loop_set))
    return loop_info_map

def construct_natural_loop_sub(bb_list, previous_map, src, dest):
    stack = []
    loop_set = [dest]
    if src == dest:
        error_message("xxx")
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
        
def build_cfg(target_config, fin, function_name):
    global bb_list
    in_region_p = False
    line = None
    line_number = 0
    while True:
        line = fin.readline()
        line_number += 1
        if not line:
            break
        if not in_region_p:
            if target_config.function_entry_p(function_name, line):
                in_region_p = True
        if in_region_p:
            deal_with_line(target_config, line_number, line)
            if target_config.function_exit_p(function_name, line):
                break
    if not in_region_p:
        error_message("cannot create control flow graph")
    bb_list = link_bb(bb_list)
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
        bb_list = build_cfg(target_config, fin, function_name)
    return bb_list
