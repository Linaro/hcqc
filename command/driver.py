import subprocess
import sys
import os
import datetime
import re
import json

import config
import cfg

uname_command = '/bin/uname'
diff_command = '/usr/bin/diff'
mv_command = '/bin/mv'
python_command = '/usr/bin/python3'
verbose_p = False

def error_message(message):
    print('Error: ' + message + '\n')
    sys.exit(1)

def show_command_line(command_line_list):
    global verbose_p
    if verbose_p:
        command_line = ' '.join(command_line_list)
        print('run :' + command_line)

def do_execute_command0(command_line_list):
    show_command_line(command_line_list)
    cp = subprocess.run(command_line_list, stdout=subprocess.PIPE)
    if cp.returncode != 0:
        error_message('do_execute_command0: ' + command_line_list)
    lines = cp.stdout.decode('utf-8').splitlines()
    return lines

def do_execute_command1(command_line_list):
    show_command_line(command_line_list)
    cp = subprocess.run(command_line_list)
    return cp.returncode

def do_execute_command1_with_check(command_line_list):
    show_command_line(command_line_list)
    cp = subprocess.run(command_line_list)
    if cp.returncode != 0:
        command_line = ' '.join(command_line_list)
        error_message('run :' + command_line)

def do_execute_command2(command_line_list):
    show_command_line(command_line_list)
    command_line = ' '.join(command_line_list)
    cp = subprocess.run(command_line, shell=True)
    return cp.returncode

def compile_source(xcommand, filename, flag_list, result_filename):
    command_line_list = [xcommand] + flag_list + [filename, '-o', result_filename]
    status = do_execute_command1(command_line_list)
    if status != 0:
        error_message('compile ' + filename)

def make_executable(xcommand, flag_list, filename_list, exec_filename, lib_list):
    command_line_list = [xcommand] + flag_list + filename_list + ['-o', exec_filename] + lib_list
    status = do_execute_command1(command_line_list)
    if status != 0:
        error_message('link ' + exec_filename)

def run_executable(exec_filename, src_dir, in_type, in_filename, work_dir, out_type, out_filename):
    command_line_list = [exec_filename]
    if in_type == "NONE":
        pass
    elif in_type == "STDIN":
        tmp_in_filename = src_dir + in_filename
        command_line_list.extend(['<', tmp_in_filename])
    elif in_type == "FILE":
        tmp_in_filename = src_dir + in_filename
        command_line_list.extend([tmp_in_filename])
    else:
        error_message('run_executable')
    if out_type == "NONE":
        pass
    elif out_type == "STDOUT":
        tmp_out_filename = work_dir + out_filename
        command_line_list.extend(['>', tmp_out_filename])
    elif out_type == "FILE":
        tmp_out_filename = work_dir + out_filename
        command_line_list.extend([tmp_out_filename])
    else:
        error_message('run_executable')
    command_line_list.extend(['2>&1'])
    status = do_execute_command2(command_line_list)
    if status != 0:
        error_message('run ' +  exec_filename)
        
def check_execution_result(out_filename, answer_filename):
    global diff_command
    status = do_execute_command1([diff_command, out_filename, answer_filename])
    if status != 0:
        error_message('check ' + out_filename)

def get_key_value(map, key):
    if not (key in map):
        error_message('map value')
    value = map[key]
    return value
        
def load_config_file(filename):
    global verbose_p
    with open(filename, 'rt') as fin:
        config_data = json.load(fin)
    if verbose_p:        
        print(config_data)
    return config_data

def get_cf_distribution(config_data):
    xdistribution = get_key_value(config_data, 'DISTRIBUTION')
    return xdistribution

def get_cf_arch(config_data):
    xarch = get_key_value(config_data, 'ARCH')
    return xarch

def get_cf_cpu(config_data):
    xcpu = get_key_value(config_data, 'CPU')
    return xcpu

def get_cf_language(config_data):
    xlanguage = get_key_value(config_data, 'LANGUAGE')
    return xlanguage

def get_cf_compiler(config_data):
    xcompiler = get_key_value(config_data, 'COMPILER')
    return xcompiler

def get_cf_command(config_data):
    xcommand = get_key_value(config_data, 'COMMAND')
    return xcommand

def get_cf_version(config_data):
    version = get_key_value(config_data, 'VERSION')
    return version

def get_cf_opt_flags(config_data):
    opt_flags = get_key_value(config_data, 'OPT_FLAGS')
    return opt_flags

def get_cf_asm_flags(config_data):
    asm_flags = get_key_value(config_data, 'ASM_FLAGS')
    return asm_flags

def get_cf_flag_db(config_data):
    flag_db = get_key_value(config_data, 'FLAG_DB')
    return flag_db

def load_program_info_file(filename):
    global verbose_p
    with open(filename, 'rt') as fin:
        program_info = json.load(fin)
    if verbose_p:
        print(program_info)
    return program_info

def get_pi_language(program_info):
    language = get_key_value(program_info, 'LANGUAGE')
    return language

def get_pi_main_flags(program_info):
    main_flags = get_key_value(program_info, 'MAIN_FLAGS')
    return main_flags

def get_pi_kernel_flags(program_info):
    kernel_flags = get_key_value(program_info, 'KERNEL_FLAGS')
    return kernel_flags

def get_pi_link_flags(program_info):
    link_flags = get_key_value(program_info, 'LINK_FLAGS')
    return link_flags

def get_pi_lib_list(program_info):
    lib_list = get_key_value(program_info, 'LIB_LIST')
    return lib_list

def get_pi_main_filename(program_info):
    main_filename = get_key_value(program_info, 'MAIN_FILENAME')
    return main_filename

def get_pi_kernel_filename(program_info):
    kernel_filename = get_key_value(program_info, 'KERNEL_FILENAME')
    return kernel_filename

def get_pi_kernel_function_name(program_info):
    kernel_function_name = get_key_value(program_info, 'KERNEL_FUNCTION_NAME')
    return kernel_function_name

def get_pi_input(program_info):
    input = get_key_value(program_info, 'INPUT')
    size = len(input)
    if size != 2 or not (input[0] in ["NONE", "STDIN", "FILE"]):
        error_message('illegal INPUT : ' + input)
    return (input[0], input[1])

def get_pi_output(program_info):
    output = get_key_value(program_info, 'OUTPUT')
    size = len(output)
    if size != 2 or not (output[0] in ["NONE", "STDOUT", "FILE"]):
        error_message('illegal OUTPUT : ' + output)
    return (output[0], output[1])

def fix_flags(flag_db_map, flag_list):
    fix_flag_list = []
    for flag in flag_list:
        if flag[0] == '?':
            if not (flag in flag_db_map):
                error_message('No flag variable definition: ' + flag)
            value = flag_db_map[flag]
            if value:
                fix_flag_list.extend([value])
        else:
            fix_flag_list.extend([flag])
    return fix_flag_list

def build_and_run_m(config_data, program_info, flag_db_map, m_filename, result_m_filename):
    xcommand = get_cf_command(config_data)
    m_flag_list = fix_flags(flag_db_map, get_pi_main_flags(program_info))
    m_flag_list.extend(["-c"])
    compile_source(xcommand, m_filename, m_flag_list, result_m_filename)

def build_and_run_k(config_data, program_info, flag_db_map, k_filename, result_k_filename):
    xcommand = get_cf_command(config_data)
    k_flag_list = fix_flags(flag_db_map, get_pi_kernel_flags(program_info))
    opt_flags = get_cf_opt_flags(config_data)
    k_flag_list.extend(opt_flags)
    k_flag_list.extend(["-c"])
    compile_source(xcommand, k_filename, k_flag_list, result_k_filename)

def build_and_run_e(config_data, src_dir, work_dir, program_info, flag_db_map, result_m_filename, result_k_filename):
    xcommand = get_cf_command(config_data)
    link_flags = fix_flags(flag_db_map, get_pi_link_flags(program_info))
    filename_list = [result_m_filename, result_k_filename]
    exec_filename = work_dir + "a.out"
    lib_list = fix_flags(flag_db_map, get_pi_lib_list(program_info))
    make_executable(xcommand, link_flags, filename_list, exec_filename, lib_list)
    (in_type, in_filename) = get_pi_input(program_info)
    (out_type, out_filename) = get_pi_output(program_info)
    run_executable(exec_filename, src_dir, in_type, in_filename, work_dir, out_type, out_filename)
    if out_type == "NONE":
        pass
    elif out_type == "STDOUT":
        answer_filename = src_dir + out_filename
        tmp_out_filename = work_dir + out_filename
        check_execution_result(tmp_out_filename, answer_filename)
    elif out_type == "FILE":
        answer_filename = src_dir + out_filename
        tmp_out_filename = work_dir + out_filename
        check_execution_result(tmp_out_filename, answer_filename)
    else:
        error_message('build_and_run_e')

def get_basename(filename):
    index = filename.rfind('/')
    if index == -1:
        xx_filename = filename
    else:
        xx_filename = filename[index+1:]
    index = xx_filename.rfind('.')
    if index <= 0:
        error_message('get_basename: ' + filename)
    return xx_filename[0:index]

def build_and_run(config_data, src_dir, work_dir, program_info, flag_db_map, m_filename, k_filename):
    result_m_filename = work_dir + get_basename(m_filename) + ".o"
    build_and_run_m(config_data, program_info, flag_db_map, m_filename, result_m_filename)
    result_k_filename = work_dir + get_basename(k_filename) + ".o"
    build_and_run_k(config_data, program_info, flag_db_map, k_filename, result_k_filename)
    build_and_run_e(config_data, src_dir, work_dir, program_info, flag_db_map, result_m_filename, result_k_filename)

def gen_asm_file(config_data, work_dir, program_info, flag_db_map, k_filename):
    xcommand = get_cf_command(config_data)
    k_flag_list = fix_flags(flag_db_map, get_pi_kernel_flags(program_info))
    opt_flags = get_cf_opt_flags(config_data)
    k_flag_list.extend(opt_flags)
    asm_flags = get_cf_asm_flags(config_data)
    k_flag_list.extend(asm_flags)
    asm_filename = work_dir + get_basename(k_filename) + ".s"
    compile_source(xcommand, k_filename, k_flag_list, asm_filename)
    return asm_filename

def sure_to_exist(dir):
    if not os.path.exists(dir):
        os.mkdir(dir)

def make_flag_db_map(config_data):
    flag_db_map = {}
    flag_db = get_cf_flag_db(config_data)
    for pair in flag_db:
        size = len(pair)
        if size != 2:
            error_message('illegal FLAG_DB : ' + pair)
        key = pair[0]
        value = pair[1]
        flag_db_map[key] = value
    return flag_db_map

def work_by_compiler(root_dir, config_name, test_name, metric_name):
    filename = root_dir + 'config/' + config_name + '.json'
    config_data= load_config_file(filename)
    arch = get_cf_arch(config_data)
    check_arch(arch)
    flag_db_map = make_flag_db_map(config_data)
    src_dir = root_dir + "test-program/" + test_name + "/"
    work_dir0 = root_dir + "work/" + test_name + "/"
    sure_to_exist(work_dir0)
    work_dir1 = work_dir0 + config_name + "/"
    sure_to_exist(work_dir1)
    work_dir = work_dir1 + metric_name + "/"
    sure_to_exist(work_dir)
    pfilename = src_dir + 'program-info.json'
    program_info = load_program_info_file(pfilename)
    check_language(config_data, program_info)
    main_filename = get_pi_main_filename(program_info)
    m_filename = src_dir + main_filename
    kernel_filename = get_pi_kernel_filename(program_info)
    k_filename = src_dir + kernel_filename
    kernel_function_name = get_pi_kernel_function_name(program_info)
    build_and_run(config_data, src_dir, work_dir, program_info, flag_db_map, m_filename, k_filename)
    asm_filename = gen_asm_file(config_data, work_dir, program_info, flag_db_map, k_filename)
    return (kernel_function_name, asm_filename)

def run_metric_command(metric_command, root_dir, config_name, test_name, function_name, asm_filename, db_filename):
    global verbose_p
    global python_command
    command_line_list = [python_command, metric_command, root_dir, config_name, test_name, function_name, asm_filename, db_filename]
    status = do_execute_command1(command_line_list)
    if status != 0:
        if verbose_p:
            print('Warning: metric not ok ' + metric_command + '\n')
    return status

def hcqc_1(root_dir, arg_verbose_p, config_name, test_name, metric_name):
    global verbose_p
    verbose_p = arg_verbose_p
    (function_name, asm_filename) = work_by_compiler(root_dir, config_name, test_name, metric_name)
    result_dir = root_dir + "result/" + test_name + "/"
    sure_to_exist(result_dir)
    db_fn = config_name + "--" + test_name + "--" + metric_name + ".json"
    db_filename = result_dir + db_fn
    metric_dir = root_dir + "command/metric/" + metric_name
    file_list = os.listdir(metric_dir)
    file_list.sort()
    if verbose_p:
        print(file_list) 
    for f in file_list:
        m = re.search(metric_name + '.*py$', f)
        if m:
            metric_command = metric_dir + "/" + f
            if verbose_p:
                print(metric_command)
            status = run_metric_command(metric_command, root_dir, config_name, test_name, function_name, asm_filename, db_filename)
            if status == 0:
                if verbose_p:
                    print('Warning: metric ok ' + metric_command + '\n')
                return
    error_message('no metric: ' + metric_name)

def usage():
    error_message('Usage: metric-command root-dir config-name test-name asm-filename db-filename')
    
def check_arch(arch):
    lines = do_execute_command0([uname_command, "-m"])
    if lines[0] != arch:
        error_message("arch mismatch: `" + arch + "' in config : `" + lines[0] + "' by uname")

def check_language(config_data, program_info):
    clanguage = get_cf_language(config_data)
    planguage = get_pi_language(program_info)
    if clanguage != planguage:
        error_message("language mismatch: `" + clanguage + "' in config : `" + planguage + "' in program-info")
        
def check_compiler_version(command, version):
    lines = do_execute_command0([command, "--version"])
    index = lines[0].find(' ' + version)
    if index == -1:
        error_message("version mismatch: `" + version + "' in config : `" + lines[0] + "' by " + command)

def cfg_work(target_config, asm_filename, function_name):
    bb_list = cfg.get_bb_list(target_config, asm_filename, function_name)
    column_list = cfg.dump_column(bb_list)
    cfg_filename = asm_filename + '.dot'
    with open(cfg_filename, 'wt') as fout:
        cfg.draw_by_dot(fout, bb_list)
    return (bb_list, column_list)
        
def run_metric_work_if_match_p(arg_list, metric_worker):
    size = len(arg_list)
    if size != 7:
        usage()
    root_dir = arg_list[1]
    config_name = arg_list[2]
    test_name = arg_list[3]
    function_name = arg_list[4]
    asm_filename = arg_list[5]
    db_filename = arg_list[6]
    a_filename = root_dir + 'config/' + config_name + '.json'
    a_config_data= load_config_file(a_filename)
    a_distribution = get_cf_distribution(a_config_data)
    a_arch = get_cf_arch(a_config_data)
    check_arch(a_arch)
    a_cpu = get_cf_cpu(a_config_data)
    a_xcompiler = get_cf_compiler(a_config_data)
    a_version = get_cf_version(a_config_data)
    a_command = get_cf_command(a_config_data)
    check_compiler_version(a_command, a_version)
    target_config = config.make_target_config(a_distribution, a_arch, a_cpu, a_xcompiler, a_version)
    if not metric_worker.match_p(target_config, test_name):
        sys.exit(1)
    (bb_list, column_list) = cfg_work(target_config, asm_filename, function_name)
    metric_work_body(target_config, db_filename, bb_list, column_list, metric_worker)
    sys.exit(0)

def print_bar(fout, column_name_list):
    print('    [ "TITLE", ["CFG", "DEPTH"', file=fout, end='')
    for column_name in column_name_list:
        print(', "' + column_name + '"', file=fout, end='')
    print(']],', file=fout)

def print_one_row(fout, bb_field, depth_field, column_data_list):
    print('    [ "' + bb_field + '", [ "' + depth_field + '"', file=fout, end='')
    for column_data in column_data_list:
        print(', "' + column_data + '"', file=fout, end='')
    print(']]', file=fout, end='')

class MetricWorker():
    def match_p(self, target_config, test_name):
        error_message('match_p')

    def set_up_before_getting_data(self, target_config, bb_list):
        error_message('set_up_before_getting_data')
        
    def get_column_name_list(self):
        error_message('get_column_name_list')

    def get_data_list(self, target_config, bb):
        error_message('get_data_list')

    def get_summary_list(self, target_config):
        error_message('get_summary_list')

def metric_work_body_core(target_config, db_filename, bb_list, column_list, metric_worker):
    metric_worker.set_up_before_getting_data(target_config, bb_list)
    with open(db_filename, 'wt') as fout:
        print('[', file=fout)
        column_name_list = metric_worker.get_column_name_list()
        print_bar(fout, column_name_list)
        size = len(bb_list)
        for index in range(0, size):
            bb = bb_list[index] 
            column = column_list[index]
            bb_field = column[0]
            depth_field = column[1]
            data_list = metric_worker.get_data_list(target_config, bb)
            print_one_row(fout, bb_field, depth_field, data_list)
            print(',', file=fout)
        summary_list = metric_worker.get_summary_list(target_config)
        print_one_row(fout, '*SUMMARY*', "-", summary_list)
        print(']', file=fout)
    
def metric_work_body(target_config, db_filename, bb_list, column_list, metric_worker):
    new_db_filename = db_filename + '.new'
    metric_work_body_core(target_config, new_db_filename, bb_list, column_list, metric_worker)
    if os.path.exists(db_filename):
        status = do_execute_command1([diff_command, db_filename, new_db_filename])
        if status == 0:
            print('Warning: The contents of the two files were the same: ' + new_db_filename + ' ' + db_filename + '\n')
            os.remove(new_db_filename)
        else:
            dt = datetime.datetime.utcnow()
            tail = dt.strftime('%Y-%m-%d-%H-%M-%S')
            do_execute_command1_with_check([mv_command, db_filename, db_filename + '.' + tail])
            do_execute_command1_with_check([mv_command, new_db_filename, db_filename])
    else:
        do_execute_command1_with_check([mv_command, new_db_filename, db_filename])
