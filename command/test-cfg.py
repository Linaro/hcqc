import sys

import config
import driver

def test_cfg(target_config, asm_filename, function_name):
    (bb_list, column_list) = driver.cfg_work(target_config, asm_filename, function_name)
    for column in column_list:
        print(column)

def usage():
    driver.error_message('Usage: python3 test-cfg.py arch compiler version asm-filename kernel-function-name')
        
def call_test_cfg(arg_list):
    size = len(arg_list)
    if size != 6:
        usage()
    arch = arg_list[1]
    compiler = arg_list[2]
    version = arg_list[3]
    asm_filename = arg_list[4]
    kernel_function_name = arg_list[5]
    target_config = config.make_target_config('distribution', arch, 'cpu', compiler, version)
    test_cfg(target_config, asm_filename, kernel_function_name)

if __name__ == "__main__":
    call_test_cfg(sys.argv)
