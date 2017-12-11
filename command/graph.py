import sys

def error_message(message):
    print('Error: ' + message + '\n')
    sys.exit(1)

### Depth First Search

current_time = None

def init_depth_first_search():
    global current_time
    current_time = 0

def white_p(status_map, v):
    if status_map[v][0] is None and status_map[v][1] is None:
        return True
    else:
        return False

def gray_p(status_map, v):
    if status_map[v][0] is not None and status_map[v][1] is None:
        return True
    else:
        return False

def black_p(status_map, v):
    if status_map[v][0] is not None and status_map[v][1] is not None:
        return True
    else:
        return False

def paint_gray(status_map, v):
    global current_time
    if not white_p(status_map, v):
        error_message("paint_gray")
    status_map[v][0] = current_time
    current_time += 1

def paint_black(status_map, v):
    global current_time
    if not gray_p(status_map, v):
        error_message("paint_black")
    status_map[v][1] = current_time
    current_time += 1

def depth_first_search(next_map, status_map, tree_edge_map, v):
    paint_gray(status_map, v)
    next_v_list = next_map[v]
    for next_v in next_v_list:
        if white_p(status_map, next_v):
            # tree_edge_map : dest -> src
            tree_edge_map[next_v].append(v)
            depth_first_search(next_map, status_map, tree_edge_map, next_v)
    paint_black(status_map, v)

### Dominator

def dominate_p(dominator_tree, x, y):
    p = y
    while p != None:
        p = get_parent(dominator_tree, p)
        if p == x:
            return True
    return False

def make_dominator_tree(vertex_list, previous_map, next_map):
    start_vertex = vertex_list[0]
    (dfs_vector, spanning_tree) = dominator_init(vertex_list, previous_map, next_map, start_vertex)
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
    idom_map[start_vertex] = None
    dominator_tree = {}
    for v in vertex_list:
        dominator_tree[v] = []
    for v in vertex_list:
        idom = idom_map[v]
        if idom:
            # dominator_tree : v -> dom
            add_edge(dominator_tree, v, idom)
        else:
            if v != start_vertex:
                error_message("make_dominator_tree")
    return dominator_tree

def dominator_init(vertex_list, previous_map, next_map, start_vertex):
    status_map = {}
    tree_edge_map = {}
    for v in vertex_list:
        status_map[v] = [ None, None ]
        tree_edge_map[v] = []
    init_depth_first_search()
    depth_first_search(next_map, status_map, tree_edge_map, start_vertex)
    for v in vertex_list:
        if not black_p(status_map, v):
            error_message("dominator_init")
    pair_vector = []
    for v in vertex_list:
        dt = status_map[v][0]
        pair_vector.append((dt, v))
    pair_vector = sorted(pair_vector, key=lambda x: x[0])
    dfs_vector = []
    for (n, v) in pair_vector:
        dfs_vector.append(v)
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
