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

def depth_first_search(next_map, status_map, tree_edge_map, bb):
    paint_gray(status_map, bb)
    next_bb_list = next_map[bb]
    for next_bb in next_bb_list:
        if white_p(status_map, next_bb):
            # tree_edge_map : dest -> src
            tree_edge_map[next_bb].append(bb)
            depth_first_search(next_map, status_map, tree_edge_map, next_bb)
    paint_black(status_map, bb)

def find_depth(bb_list):
    global verbose_p
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
    dominator_tree = make_dominator_tree(bb_list, previous_map, next_map)
    back_edge_list = []
    for src in bb_list:
        for dest in next_map[src]:
            if dominate_p(dominator_tree, dest, src):
                back_edge_list.append((src, dest))
    return back_edge_list

def dominate_p(dominator_tree, x, y):
    p = y
    while p != None:
        p = get_parent(dominator_tree, p)
        if p == x:
            return True
    return False

def make_dominator_tree(bb_list, previous_map, next_map):
    start_bb = bb_list[0]
    (dfs_vector, spanning_tree) = dominator_init(bb_list, previous_map, next_map, start_bb)
    semi_map = {}
    bucket_map = {}
    label_map = {}
    ancestor_map = {}
    idom_map = {}
    dfs_number = 0
    for v in dfs_vector:
        semi_map[v] = dfs_number
        label_map[v] = v
        bucket_map[v] = []
        ancestor_map[v] = None
        idom_map[v] = None
        dfs_number += 1
    dfs_number = len(dfs_vector) - 1
    while dfs_number != 0:
        w = dfs_vector[dfs_number]
        # Step 2
        for v in previous_map[w]:
            u = dominator_eval(semi_map, label_map, ancestor_map, v)
            if semi_map[u] < semi_map[w]:
                semi_map[w] = semi_map[u]
        bucket_map[dfs_vector[semi_map[w]]].insert(0, w)
        dominator_link(ancestor_map, get_parent(spanning_tree, w), w)
        # Step 3
        v_list = bucket_map[get_parent(spanning_tree, w)]
        for v in v_list:
            u = dominator_eval(semi_map, label_map, ancestor_map, v)
            if semi_map[u] < semi_map[v]:
                idom_map[v] = u
            else:
                idom_map[v] = get_parent(spanning_tree, w)
        bucket_map[get_parent(spanning_tree, w)] = []
        dfs_number -= 1
    # Step 4
    dfs_number = 1
    while dfs_number < len(dfs_vector):
        w = dfs_vector[dfs_number]
        if idom_map[w] != dfs_vector[semi_map[w]]:
            idom_map[w] = idom_map[idom_map[w]]
        dfs_number += 1
    idom_map[start_bb] = None
    dominator_tree = {}
    for bb in bb_list:
        dominator_tree[bb] = []
    for bb in bb_list:
        idom = idom_map[bb]
        if idom:
            # dominator_tree : bb -> dom
            add_edge(dominator_tree, bb, idom)
        else:
            if bb != start_bb:
                error_message("make_dominator_tree")
    #dump_tree(bb_list, dominator_tree)
    return dominator_tree

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

def dominator_init (bb_list, previous_map, next_map, start_bb):
    global current_time
    status_map = {}
    tree_edge_map = {}
    for bb in bb_list:
        status_map[bb] = [ None, None ]
        tree_edge_map[bb] = []
    current_time = 0
    depth_first_search(next_map, status_map, tree_edge_map, start_bb)
    for bb in bb_list:
        if not black_p(status_map, bb):
            error_message("dominator_init")
    pair_vector = []
    for bb in bb_list:
        dt = status_map[bb][0]
        pair_vector.append((dt, bb))
    pair_vector = sorted(pair_vector, key=lambda x: x[0])
    dfs_vector = []
    for (n, bb) in pair_vector:
        dfs_vector.append(bb)
    return (dfs_vector, tree_edge_map)

def dominator_eval(semi_map, label_map, ancestor_map, v):
    a = ancestor_map[v]
    if a:
        dominator_compress(semi_map, label_map, ancestor_map, v)
        return label_map[v]
    else:
        return v

def dominator_compress(semi_map, label_map, ancestor_map, v):
    a = ancestor_map[ancestor_map[v]]
    if a:
        dominator_compress(semi_map, label_map, ancestor_map, ancestor_map[v])
        if semi_map[label_map[ancestor_map[v]]] < semi_map[label_map[v]]:
            label_map[v] = label_map[ancestor_map[v]]
        ancestor_map[v] = ancestor_map[ancestor_map[v]]

def dominator_link(ancestor_map, v, w):
    ancestor_map[w] = v

def add_edge(map, src, dest):
    if not dest in map[src]:
        map[src].append(dest)

def get_parent(tree, v):
    prev_list = tree[v]
    if len(prev_list) == 0:
        return None
    elif len(prev_list) == 1:
        return prev_list[0]
    else:
        error_message("get_parent")
    
def find_end_bb(bb_list, next_map):
    end_bb = None
    for bb in bb_list:
        if len(next_map[bb]) == 0:
            if end_bb != None:
                error_message("find_end_bb")
            end_bb = bb
    if end_bb == None:
        error_message("find_end_bb")
    return end_bb

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
    find_depth(bb_list.copy())
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
