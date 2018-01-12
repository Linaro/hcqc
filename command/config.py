import sys
import re

verbose_p = False

def error_message(message):
    print('Error: ' + message + '\n')
    sys.exit(1)

class Config():
    def __init__(self, distribution, arch, cpu, compiler, version):
        self.distribution = distribution
        self.arch = arch
        self.cpu = cpu
        self.compiler = compiler
        self.version = version

    def function_entry_p(self, name, line):
        return re.match('^' + name + ':', line)

    def function_exit_p(self, name, line):
        return re.match('^\t\.size\t' + name, line)

    def bb_label(self, line):
        global verbose_p
        m = re.match('^(\.?\w[^:]*):', line)
        if m:
            label = m.groups()
            if verbose_p:
                print('label: [' + label[0] + ']')
            return label[0]
        else:
            return None

    def get_table_branch_prologue_number(self):
        error_message('get_table_branch_prologue_number')

    def trace_table_branch_prologue(self, region_status, line):
        error_message('trace_table_branch_prologue')

    def get_table_branch_content(self, line):
        error_message('get_table_branch_content')

def strcls(class_name):
    global_symbol_table = globals()
    return global_symbol_table[class_name]

def make_target_config(distribution, arch, cpu, compiler, version):
    global verbose_p
    cls = strcls('C_' + arch + '__' + compiler)
    if verbose_p:
        print(cls)
    target_config = cls(distribution, arch, cpu, compiler, version)
    return target_config

def get_canonical_op(op_dict, op):
    if op in op_dict:
        canonical_op = op_dict[op]
        if canonical_op:
            return canonical_op
        else:
            return op
    else:
        return None

class C_aarch64__(Config):
    load_op_dict = {'ldr': None,
                    'ldrb': None,
                    'ldrsb': None,
                    'ldrh': None,
                    'ldrsh': None,
                    'ldrsw': None,
                    'ldur': None,
                    'ldurb': None,
                    'ldursb': None,
                    'ldurh': None,
                    'ldursh': None,
                    'ldursw': None,
                    'ldp': None,
                    'ldpsw': None,
                    'ldnp': None,
                    'ldtr': None,
                    'ldtrb': None,
                    'ldtrsb': None,
                    'ldtrh': None,
                    'ldtrsh': None,
                    'ldtrsw': None,
                    'ldxr': None,
                    'ldxrb': None,
                    'ldxrh': None,
                    'ldxp': None,
                    'ldar': None,
                    'ldarb': None,
                    'ldarh': None,
                    'ldaxr': None,
                    'ldaxrb': None,
                    'ldaxrh': None,
                    'ldaxp': None,
                    'ld1': None,
                    'ld2': None,
                    'ld3': None,
                    'ld4': None,
                    'ld1r': None,
                    'ld2r': None,
                    'ld3r': None,
                    'ld4r': None}

    store_op_dict = {'str': None,
                     'strb': None,
                     'strh': None,
                     'stur': None,
                     'sturb': None,
                     'sturh': None,
                     'stp': None,
                     'stnp': None,
                     'sttr': None,
                     'sttrb': None,
                     'sttrh': None,
                     'stxr': None,
                     'stxrb': None,
                     'stxrh': None,
                     'stxp': None,
                     'stlr': None,
                     'stlrb': None,
                     'stlrh': None,
                     'stlxr': None,
                     'stlxrb': None,
                     'stlxrh': None,
                     'stlxp': None,
                     'st1': None,
                     'st2': None,
                     'st3': None,
                     'st4': None}

    control_transfer_op_dict = {'beq': 'b.eq',
                                'bne': 'b.ne',
                                'bcs': 'b.cs',
                                'bhs': 'b.hs',
                                'bcc': 'b.cc',
                                'blo': 'b.lo',
                                'bmi': 'b.mi',
                                'bpl': 'b.pl',
                                'bvs': 'b.vs',
                                'bvc': 'b.vc',
                                'bhi': 'b.hi',
                                'bls': 'b.ls',
                                'bge': 'b.ge',
                                'blt': 'b.lt',
                                'bgt': 'b.gt',
                                'ble': 'b.le',
                                'bal': 'b.al',
                                'bnv': 'b.nv',
                                'b.eq': None,
                                'b.ne': None,
                                'b.cs': None,
                                'b.hs': None,
                                'b.cc': None,
                                'b.lo': None,
                                'b.mi': None,
                                'b.pl': None,
                                'b.vs': None,
                                'b.vc': None,
                                'b.hi': None,
                                'b.ls': None,
                                'b.ge': None,
                                'b.lt': None,
                                'b.gt': None,
                                'b.le': None,
                                'b.al': None,
                                'b.nv': None,
                                'cbnz': None,
                                'cbz': None,
                                'tbnz': None,
                                'tbz': None,
                                'b': None,
                                'bl': None,
                                'blr': None,
                                'br': None,
                                'ret': None}

    def get_asm_comment(self, line):
        pattern = '//'
        index = line.find(pattern)
        if index < 0:
            return None
        else:
            return line[index+len(pattern):]

    def bb_branch(self, line):
        branch_op = self.op(line)
        if not self.control_transfer_op_p(branch_op):
            return None
        if branch_op == 'ret':
            return ('ret', None)
        m = re.match('^\t(b)\t([_\w\d]+).*', line)
        if m:
            tmp = m.groups()
            # Tail calls like 'b fputc'.
            return ('b', None)
        m = re.match('^\t(b\.?[\w]*)\t(\.?[_\w\d]+).*', line)
        if m:
            tmp = m.groups()
            return tmp
        m = re.match('^\t(cbn?z)\t[^,]+,\s(\.?[_\w\d]+).*', line)
        if m:
            tmp = m.groups()
            return tmp
        m = re.match('^\t(tbn?z)\t[^,]+,[^,]+,\s(\.?[_\w\d]+).*', line)
        if m:
            tmp = m.groups()
            return tmp
        return None

    def call_p(self, branch_op, branch_target):
        if branch_op in ['bl', 'blr']:
            return True
        else:
            return False

    def tail_call_p(self, branch_op, branch_target):
        if branch_op == 'b' and branch_target and re.match('[_\w]', branch_target[0]):
            return True
        else:
            return False

    def branch_by_register_p(self, branch_op, branch_target):
        if branch_op == 'br' and branch_target and branch_target[0] == 'x':
            return True
        else:
            return False

    def table_branch_p(self, branch_op, branch_target, table_branch_label, line_list):
        if branch_op == 'br' and branch_target and branch_target[0] == 'x':
            pass
        else:
            error_message('table_branch_p')
        if table_branch_label:
            return True
        else:
            return False

    def fall_through_p(self, branch_op):
        if branch_op in ['b', 'ret', 'bal', 'b.al', 'bnv', 'b.nv']:
            return False
        else:
            return True

    def op(self, line):
        m = re.match('^\t(?P<MNEMONIC>\w[\.\w\d]*)($|(\t.*))', line)
        if m:
            return m.group('MNEMONIC')
        else:
            return None

    def load_op_p(self, op):
        return op in self.load_op_dict

    def store_op_p(self, op):
        return op in self.store_op_dict

    def control_transfer_op_p(self, op):
        return op in self.control_transfer_op_dict

    def canonical_op(self, op):
        cop = get_canonical_op(self.load_op_dict, op)
        if cop:
            return cop
        cop = get_canonical_op(self.store_op_dict, op)
        if cop:
            return cop
        cop = get_canonical_op(self.control_transfer_op_dict, op)
        if cop:
            return cop
        return op

class C_aarch64__ClangLLVM(C_aarch64__):
    def function_exit_p(self, name, line):
        return re.match('^\.Lfunc_end\d+:$', line)

    def get_table_branch_prologue_number(self):
        # .section .rodata,"a",@progbits
	# .p2align 3
        # .LJTI?_?:
	# --> .xword .LBB?_?
        return 3

    def trace_table_branch_prologue(self, region_status, line):
        if region_status == 0 and re.match('^\t\.section\t\.rodata,"a",@progbits$', line):
            return (True, None)
        elif region_status == 1 and re.match("^\t\.p2align\t3$", line):
            return (True, None)
        elif region_status == 2:
            m = re.match('^(?P<TBLABEL>\.LJTI\d+_\d+):$', line)
            if m:
                tb_label = m.group('TBLABEL')
                return (True, tb_label)
            else:
                return (False, None)
        else:
            return (False, None)

    def get_table_branch_content(self, line):
        m = re.match('^\t\.xword\t(\.L.*)$', line)
        if m:
            label = m.groups()
            return label[0]
        else:
            return None

class C_aarch64__GCC(C_aarch64__):
    def get_table_branch_prologue_number(self):
        # .Lrtx?:
	# .section .rodata
	# .align 0
	# .align 2
        # .L?:
	# --> .word (.L? - .Lrtx?) / 4
        return 5

    def trace_table_branch_prologue(self, region_status, line):
        if region_status == 0 and re.match('^\.Lrtx\d+:$', line):
            return (True, None)
        elif region_status == 1 and re.match("^\t\.section\t\.rodata$", line):
            return (True, None)
        elif region_status == 2 and re.match("^\t\.align\t0$", line):
            return (True, None)
        elif region_status == 3 and re.match("^\t\.align\t2$", line):
            return (True, None)
        elif region_status == 4:
            m = re.match('^(?P<TBLABEL>\.L\d+):$', line)
            if m:
                tb_label = m.group('TBLABEL')
                return (True, tb_label)
            else:
                return (False, None)
        else:
            return (False, None)

    def get_table_branch_content(self, line):
        m = re.match('^\t\.word\t\((\.L\d+) - \.Lrtx\d+\) / 4$', line)
        if m:
            label = m.groups()
            return label[0]
        return None

# TODO
class C_x86_64__(Config):
    load_op_dict = {}

    store_op_dict = {}

    control_transfer_op_dict = {'ja': None,
                                'jnbe': None,
                                'jae': None,
                                'jnb': None,
                                'jb': None,
                                'jnae': None,
                                'jbe': None,
                                'jna': None,
                                'je': None,
                                'jz': None,
                                'jne': None,
                                'jnz': None,
                                'jg': None,
                                'jnle': None,
                                'jge': None,
                                'jnl': None,
                                'jl': None,
                                'jnge': None,
                                'jle': None,
                                'jng': None,
                                'js': None,
                                'jns': None,
                                'jc': None,
                                'jnc': None,
                                'jo': None,
                                'jno': None,
                                'jp': None,
                                'jpe': None,
                                'jnp': None,
                                'jpo': None,
                                'jmp': None,
                                'jmpq': None,
                                'call': None,
                                'callq': None,
                                'ret': None,
                                'retq': None}

    def get_asm_comment(self, line):
        pattern = '#'
        index = line.find(pattern)
        if index < 0:
            return None
        else:
            return line[index+len(pattern):]

    def bb_branch(self, line):
        branch_op = self.op(line)
        if not self.control_transfer_op_p(branch_op):
            return None
        if branch_op in ['retq', 'ret']:
            return (branch_op, None)
        m = re.match('^\t(jmp)\t([_\w\d]+).*', line)
        if m:
            tmp = m.groups()
            # tail calls
            return tmp
        m = re.match('^\t(j[\w]*)\t(\.?[_\w\d]+).*', line)
        if m:
            tmp = m.groups()
            return tmp
        m = re.match('^\t(jmpq)\t(\*[^\s]+).*', line)
        if m:
            tmp = m.groups()
            return tmp
        m = re.match('^\t(jmp)\t(\*[^\s]+).*', line)
        if m:
            tmp = m.groups()
            return tmp
        return None

    def call_p(self, branch_op, branch_target):
        if branch_op in ['callq', 'call']:
            return True
        else:
            return False

    def tail_call_p(self, branch_op, branch_target):
        if branch_op == 'jmp' and branch_target and re.match('[_\w]', branch_target[0]):
            return True
        else:
            return False

    def branch_by_register_p(self, branch_op, branch_target):
        if branch_op in ['jmp', 'jmpq'] and branch_target and branch_target[0] == '*':
            return True
        else:
            return False

    def table_branch_p(self, branch_op, branch_target, table_branch_label, line_list):
        if branch_op in ['jmp', 'jmpq'] and branch_target and branch_target[0] == '*':
            pass
        else:
           error_message('table_branch_p')
        if table_branch_label:
            return True
        else:
            return False

    def fall_through_p(self, branch_op):
        if branch_op in ['jmp', 'retq', 'ret']:
            return False
        else:
            return True

    def op(self, line):
        m = re.match('^\t(?P<MNEMONIC>\w[\.\w\d]*)($|(\t.*))', line)
        if m:
            return m.group('MNEMONIC')
        else:
            return None

    def load_op_p(self, op):
        return op in self.load_op_dict

    def store_op_p(self, op):
        return op in self.store_op_dict

    def control_transfer_op_p(self, op):
        return op in self.control_transfer_op_dict

    def canonical_op(self, op):
        return op

class C_x86_64__ClangLLVM(C_x86_64__):
    def function_exit_p(self, name, line):
        return re.match('^\.Lfunc_end\d+:$', line)

    def get_table_branch_prologue_number(self):
        # .section .rodata,"a",@progbits
        # .p2align 3
        # .LJTI?_?:
	# --> .quad .LBB?_?
        return 3

    def trace_table_branch_prologue(self, region_status, line):
        if region_status == 0 and re.match('^\t\.section\t\.rodata,"a",@progbits$', line):
            return (True, None)
        elif region_status == 1 and re.match("^\t\.p2align\t3$", line):
            return (True, None)
        elif region_status == 2:
            m = re.match('^(?P<TBLABEL>\.LJTI\d+_\d+):$', line)
            if m:
                tb_label = m.group('TBLABEL')
                return (True, tb_label)
            else:
                return (False, None)
        else:
            return (False, None)

    def get_table_branch_content(self, line):
        m = re.match('^\t\.quad\t(\.L.*)$', line)
        if m:
            label = m.groups()
            return label[0]
        return None

class C_x86_64__GCC(C_x86_64__):
    def function_exit_p(self, name, line):
        return re.match('^\t\.cfi_endproc', line)

    def get_table_branch_prologue_number(self):
        # jmp *%r?x
	# .section .rodata
	# .align 4
	# .align 4
        # .L?:
	# --> .long .L?-.L?
        return 5

    def trace_table_branch_prologue(self, region_status, line):
        if region_status == 0 and re.match('^\tjmp\t\*%r\wx.*$', line):
            return (True, None)
        elif region_status == 1 and re.match("^\t\.section\t\.rodata$", line):
            return (True, None)
        elif region_status == 2 and re.match("^\t\.align 4$", line):
            return (True, None)
        elif region_status == 3 and re.match("^\t\.align 4$", line):
            return (True, None)
        elif region_status == 4:
            m = re.match('^(?P<TBLABEL>\.L\d+):$', line)
            if m:
                tb_label = m.group('TBLABEL')
                return (True, tb_label)
            else:
                return (False, None)
        else:
            return (False, None)

    def get_table_branch_content(self, line):
        m = re.match('^\t\.long\t(\.L\d+)-\.L\d+', line)
        if m:
            label = m.groups()
            return label[0]
        return None
