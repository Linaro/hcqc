import sys
import importlib
import inspect

import config
import driver

def find_class(module):
    for name, member in inspect.getmembers(module):
        if inspect.isclass(member) and issubclass(member, driver.MetricWorker):
            print('MetricWorker : ' + name)
            return member
    return None

def test_metric(target_config, metric_filename, asm_filename, function_name, db_filename):
    (bb_list, column_list) = driver.cfg_work(target_config, asm_filename, function_name)
    m = importlib.import_module(metric_filename)
    cls = find_class(m)
    if cls == None:
        driver.error_message('No metric woker class')
    metric_worker = cls()
    driver.metric_work_body(target_config, db_filename, bb_list, column_list, metric_worker)

def usage():
    driver.error_message('Usage: python3 test-metric.py metric arch compiler version asm-filename kernel-function-name db-filename')

def call_test_metric(arg_list):
    size = len(arg_list)
    if size != 8:
        usage()
    metric_filename = arg_list[1]
    arch = arg_list[2]
    compiler = arg_list[3]
    version = arg_list[4]
    asm_filename = arg_list[5]
    kernel_function_name = arg_list[6]
    db_filename = arg_list[7]
    target_config = config.make_target_config('distribution', arch, 'cpu', compiler, version)
    test_metric(target_config, metric_filename, asm_filename, kernel_function_name, db_filename)

if __name__ == "__main__":
    call_test_metric(sys.argv)
