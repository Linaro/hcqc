import sys
import driver
import json
import csv

verbose_p = False

result_title = [ ]
result_table = [ ]

first_p = True

def load_json_file(filename):
    with open(filename, 'rt') as fin:
        json_data = json.load(fin)
    return json_data

def get_title(json_data):
    title = json_data[0]
    return title

def report_c1_t1_mx_sub(root_dir, config_name, test_name, metric_name):
    global result_title
    global result_table
    global first_p
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
    if first_p:
        result_title.extend(bar)
    else:
        result_title.extend(bar[2:])
    data_size = len(bar) - 1
    index = 0
    for row in json_data[1:]:
        bb = row[0]
        data = row[1]
        if len(data) != data_size:
            driver.error_message('report_c1_t1_mx_sub 1')
        if first_p:
            data.insert(0, bb)
            result_table.append(data)
        else:
            result_table[index].extend(data[1:])
        index += 1
    first_p = False

def gen_csv_file(csv_filename, result_table):
    with open(csv_filename, 'wt') as fout:
        csvout = csv.writer(fout)
        csvout.writerows(result_table)

def report_c1_t1_mx(root_dir, config_name, test_name, metric_name_list, csv_filename):
    global result_title
    global result_table
    global first_p
    result_title = []
    result_table = []
    first_p = True
    for metric_name in metric_name_list:
        report_c1_t1_mx_sub(root_dir, config_name, test_name, metric_name)
    result_table.insert(0, result_title)
    gen_csv_file(csv_filename, result_table)
    if verbose_p:
        print("generate " + csv_filename)

def usage():
    driver.error_message('Usage: hcqc-report [--v] config-name test-name report-id metric_name ...')

def report1_entry(arg_list):
    global verbose_p
    metric_name_list = []
    size = len(arg_list)
    if size < 5:
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
    report_id = arg_list[index]
    index += 1
    while index < size:
        metric_name = arg_list[index]
        metric_name_list.append(metric_name)
        index += 1
    report_dir = root_dir + "report/" + test_name + "/"
    driver.sure_to_exist(report_dir)
    csv_fn = config_name + '--' + test_name + '--' + report_id + '.csv'
    csv_filename = report_dir + csv_fn
    report_c1_t1_mx(root_dir, config_name, test_name, metric_name_list, csv_filename)

if __name__ == "__main__":
    report1_entry(sys.argv)
