import sys
import driver
import json
import csv

only_summary_p = False

verbose_p = False

result_title = [ ]
result_table = [ ]

def load_json_file(filename):
    with open(filename, 'rt') as fin:
        json_data = json.load(fin)
    return json_data

def get_title(json_data):
    title = json_data[0]
    return title

def report_t1_cl_ml_sub(root_dir, config_name, test_name, metric_name, part_global_title, first_p):
    global result_table
    global verbose_p
    global only_summary_p
    result_dir = root_dir + "result/" + test_name + "/"
    driver.sure_to_exist(result_dir)
    db_fn = config_name + "--" + test_name + "--" + metric_name + ".json"
    db_filename = result_dir + db_fn
    if verbose_p:
        print("use " + db_filename)
    json_data = load_json_file(db_filename)
    title = get_title(json_data)
    if title[0] != 'TITLE':
        driver.error_message('report_c1_t1_mx_sub 0')
    bar = title[1]
    fix_bar = None
    if first_p:
        fix_bar = bar
    else:
        # Skip "CFG" and "DEPTH"
        fix_bar = bar[2:]
    data_size = len(bar) - 1
    index = 0
    row_list = json_data[1:]
    if only_summary_p:
        row_list = row_list[-1:]
    for row in row_list:
        bb = row[0]
        data = row[1]
        if len(data) != data_size:
            driver.error_message('report_c1_t1_mx_sub 1')
        fix_data = None
        if first_p:
            fix_data = [ bb ]
            fix_data.extend(data)
        else:
            # Skip BB
            fix_data = data[1:]
        if len(fix_data) != len(fix_bar):
            driver.error_message('report_c1_t1_mx_sub 2')
        fix_data = pad_data(fix_bar, fix_data, part_global_title)
        if first_p:
            result_table.append(fix_data)
        else:
            result_table[index].extend(fix_data)
        index += 1

def pad_data(bar, data, part_global_title):
    fix_data = []
    g_size = len(part_global_title)
    size = len(bar)
    bar_index = 0
    g_index = 0
    while bar_index < size:
        x = bar[bar_index]
        d = data[bar_index]
        bar_index += 1
        while True:
            g = part_global_title[g_index]
            g_index += 1
            if x == g:
                fix_data.append(d)
                break
            else:
                fix_data.append('')
    while g_index < g_size:
        g = part_global_title[g_index]
        fix_data.append('')
        g_index += 1
    if len(fix_data) != len(part_global_title):
        print(fix_data)
        print(part_global_title)
        driver.error_message('pad_data')
    return fix_data

def gen_csv_file(csv_filename, result_table):
    with open(csv_filename, 'wt') as fout:
        csvout = csv.writer(fout)
        csvout.writerows(result_table)

def report_t1_cl_ml(root_dir, test_name, config_name_list,  metric_name_list, csv_filename, global_title):
    global result_table
    merged_table = []
    real_global_title = make_real_global_title(global_title)
    for config_name in config_name_list:
        result_table = []
        first_p = True
        last_index = -1
        for metric_name in metric_name_list:
            end_index = find_mark(global_title, last_index+1)
            part_global_title = global_title[last_index+1:end_index]
            report_t1_cl_ml_sub(root_dir, config_name, test_name, metric_name, part_global_title, first_p)
            last_index = end_index
            first_p = False
        merged_table.append(real_global_title)
        merged_table.extend(result_table)
    gen_csv_file(csv_filename, merged_table)
    if verbose_p:
        print("generate " + csv_filename)

def make_real_global_title(global_title):
    real_global_title = []
    for x in global_title:
        if x != -1:
            real_global_title.append(x)
    return real_global_title

def find_mark(list, start_index):
    index = start_index
    while True:
        if list[index] == -1:
            return index
        index += 1
    driver.error_message('find_mark')

def make_global_title_sub(root_dir, config_name, test_name, metric_name, first_p):
    global result_title
    result_dir = root_dir + "result/" + test_name + "/"
    driver.sure_to_exist(result_dir)
    db_fn = config_name + "--" + test_name + "--" + metric_name + ".json"
    db_filename = result_dir + db_fn
    if verbose_p:
        print("use " + db_filename)
    json_data = load_json_file(db_filename)
    title = get_title(json_data)
    if title[0] != 'TITLE':
        driver.error_message('make_global_title_sub')
    bar = title[1]
    if first_p:
        result_title.extend(bar)
    else:
        result_title.append(-1)
        result_title.extend(bar[2:])

def make_global_title(root_dir, test_name, config_name_list,  metric_name_list, csv_filename):
    global result_title
    title_list = []
    for config_name in config_name_list:
        result_title = []
        first_p = True
        for metric_name in metric_name_list:
            make_global_title_sub(root_dir, config_name, test_name, metric_name, first_p)
            first_p = False
        result_title.append(-1)
        title_list.append(result_title)
    global_title = []
    while True:
        if all_empty_p(title_list):
            break
        head_list = make_head_list(title_list)
        if equal_all_p(head_list):
            (i, h) = head_list[0]
            global_title.append(h)
            cut_head(title_list, head_list)
        else:
            cut_head_list = make_cut_head_list(head_list)
            (i, h) = cut_head_list[0]
            global_title.append(h)
            cut_head(title_list, cut_head_list)
    return global_title

def all_empty_p(title_list):
    for x in title_list:
        if 0 < len(x):
            return False
    return True

def make_head_list(title_list):
    title_size = len(title_list)
    head_list = []
    i = 0
    while i < title_size:
        h = title_list[i][0]
        head_list.append((i, h))
        i += 1
    return head_list

def equal_all_p(head_list):
    first_p = True
    common_head = None
    for (index, h) in head_list:
        if first_p:
            common_head = h
        elif common_head == h:
            pass
        else:
            return False
        first_p = False
    return True

def cut_head(title_list, head_list):
    for (index, h) in head_list:
        tmp_list = title_list[index]
        del tmp_list[0]

def make_cut_head_list(head_list):
    cut_head = None
    for (index, h) in head_list:
        if h == -1:
            pass
        elif cut_head == None:
            cut_head = h
        elif h < cut_head:
            cut_head = h
        else:
            pass
    if cut_head == None or cut_head == -1:
        driver.error_message('make_cut_head_list')
    cut_head_list = []
    for (index, h) in head_list:
        if h == cut_head:
            cut_head_list.append((index, h))
    return cut_head_list

def usage():
    driver.error_message('Usage: hcqc-m-report [--v] test-name report-name --c config-name ... --m metric_name ...')

def report_m_entry(arg_list):
    global verbose_p
    config_name_list = []
    metric_name_list = []
    size = len(arg_list)
    if size < 8:
        usage()
    index = 1
    root_dir = arg_list[index]
    index += 1
    if arg_list[index] == '--v':
        verbose_p = True
        index += 1
    if size < 6:
        usage()
    report_name = arg_list[index]
    index += 1
    test_name = arg_list[index]
    index += 1
    if arg_list[index] != '--c':
        usage()
    index += 1
    while True:
        if index == size:
            usage()
        config_name = arg_list[index]
        index += 1
        if config_name == '--m':
            break
        config_name_list.append(config_name)
    if index == size:
        usage()
    while index < size:
        metric_name = arg_list[index]
        metric_name_list.append(metric_name)
        index += 1
    report_dir = root_dir + "report/" + test_name + "/"
    driver.sure_to_exist(report_dir)
    c_list = '--'.join(config_name_list)
    csv_fn = report_name + '--' + test_name + '--' + c_list + '.csv'
    csv_filename = report_dir + csv_fn
    if verbose_p:
        print(config_name_list)
        print(metric_name_list)
    global_title = make_global_title(root_dir, test_name, config_name_list,  metric_name_list, csv_filename)
    report_t1_cl_ml(root_dir, test_name, config_name_list, metric_name_list, csv_filename, global_title)

if __name__ == "__main__":
    report_m_entry(sys.argv)
