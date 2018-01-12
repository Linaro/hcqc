import sys
import driver

def usage():
    driver.user_error('Usage: hcqc [--v] config-name test-name metric_name ...')

def hcqc_entry(arg_list):
    verbose_p = False
    size = len(arg_list)
    if size < 4:
        usage()
    index = 1
    root_dir = arg_list[index]
    index += 1
    if arg_list[index] == '--v':
        verbose_p = True
        index += 1
    if size < 3:
        usage()
    config_name = arg_list[index]
    index += 1
    test_name = arg_list[index]
    index += 1
    while index < size:
        metric_name = arg_list[index]
        driver.hcqc_1(root_dir, verbose_p, config_name, test_name, metric_name)
        index += 1

if __name__ == "__main__":
    hcqc_entry(sys.argv)
