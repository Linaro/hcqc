import sys
import os
import re
import driver
import json
import csv

verbose_p = False

first_p = True

def read_perf_result_file(filename):
    if not os.path.exists(filename):
        driver.data_error("file not found: `" + filename + "'")
    perf_data = None
    with open(filename, 'rt') as fin:
        perf_data = read_perf_result_file_sub(fin)
    return perf_data

def read_perf_result_file_sub(fin):
    seconds = 0
    cache_misses = 0
    line = None
    while True:
        line = fin.readline()
        if not line:
            break
        if verbose_p:
            print(line)
        m = re.match('^ +([,\d]+) +cache-misses *$', line)
        if m:
            tmp = m.groups()
            s_cache_misses = tmp[0].replace(',','')
            cache_misses=int(s_cache_misses)
        m = re.match('^ +([\.\d]+) +seconds time elapsed *$', line)
        if m:
            tmp = m.groups()
            s_seconds = tmp[0]
            seconds = int(float(s_seconds)*10**9)
    if verbose_p:
        print([seconds, cache_misses])
    return [seconds, cache_misses]

def load_json_file(filename):
    if not os.path.exists(filename):
        driver.data_error("file not found: `" + filename + "'")
    with open(filename, 'rt') as fin:
        json_data = json.load(fin)
    return json_data

def get_title(json_data):
    title = json_data[0]
    return title

def get_weight(metric_name, c):
    return 1

def get_term_score(metric_name, depth, c, d):
    weight = get_weight(metric_name, c)
    depth_value = int(depth) + 1
    term_score = weight * depth_value * int(d)
    return term_score

def get_term_dynamic_score(metric_name, c, d):
    weight = get_weight(metric_name, c)
    term_score = weight * d
    return term_score

def get_dynamic_score(root_dir, test_name, config_name, metric_name):
    score = 0
    perf_filename = root_dir + "work/" + test_name + "/" + config_name + "/" + metric_name + "/perf.out"
    perf_data = read_perf_result_file(perf_filename)
    score += get_term_dynamic_score('perf', 'time', perf_data[0])
    score += get_term_dynamic_score('perf', 'cache_misses', perf_data[1])
    return score

def calculate_score_sub(root_dir, config_name, test_name, metric_name):
    global first_p
    score = 0
    if first_p:
        score += get_dynamic_score(root_dir, test_name, config_name, metric_name)
        first_p = False
    result_dir = root_dir + "result/" + test_name + "/"
    driver.sure_to_exist(result_dir)
    db_fn = config_name + "--" + test_name + "--" + metric_name + ".json"
    db_filename = result_dir + db_fn
    if verbose_p:
        print("use " + db_filename)
    json_data = load_json_file(db_filename)
    title = get_title(json_data)
    if title[0] != 'TITLE':
        driver.data_error('calculate_score_sub 0')
    bar = title[1]
    if verbose_p:
        print(bar)
    data_size = len(bar) - 1
    if json_data[-1][0] != '*SUMMARY*':
        driver.data_error('calculate_score_sub 1')
    for row in json_data[1:-1]:
        bb = row[0]
        data = row[1]
        if len(data) != data_size:
            driver.data_error('calculate_score_sub 1')
        size = data[0]
        depth = data[1]
        if verbose_p:
            print("size=" + size + ",depth=" + depth)
        index = 3
        for d in data[2:]:
            c = bar[index]
            if verbose_p:
                print("index=" + str(index) + ",bar=" + c + ",d=" + d)
            score += get_term_score(metric_name, depth, c, d)
            index += 1
    return score

def calculate_score(root_dir, config_name, test_name, metric_name_list):
    total_score = 0
    for metric_name in metric_name_list:
        score = calculate_score_sub(root_dir, config_name, test_name, metric_name)
        total_score += score
    print('score=' + str(total_score))

def usage():
    driver.user_error('Usage: hcqc-score [--v] config-name test-name metric_name ...')

def hcqc_score_entry(arg_list):
    global verbose_p
    metric_name_list = []
    size = len(arg_list)
    if size < 4:
        usage()
    index = 1
    root_dir = arg_list[index]
    index += 1
    if arg_list[index] == '--v':
        verbose_p = True
        index += 1
    if size < 4:
        usage()
    config_name = arg_list[index]
    index += 1
    test_name = arg_list[index]
    index += 1
    while index < size:
        metric_name = arg_list[index]
        metric_name_list.append(metric_name)
        index += 1
    report_dir = root_dir + "report/" + test_name + "/"
    driver.sure_to_exist(report_dir)
    calculate_score(root_dir, config_name, test_name, metric_name_list)

if __name__ == "__main__":
    hcqc_score_entry(sys.argv)
