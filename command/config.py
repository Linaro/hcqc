import sys
import re

verbose_p = False

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
        return re.match('^\t.size\t' + name, line)

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

def strcls(class_name):
    global_symbol_table = globals()
    return global_symbol_table[class_name]

def make_target_config(distribution, arch, cpu, xcompiler, version):
    global verbose_p
    cls = strcls('C_' + arch + '__' + xcompiler)
    if verbose_p:
        print(cls)
    target_config = cls(distribution, arch, cpu, xcompiler, version)
    return target_config

class C_aarch64__(Config):
    load_op_dict = {'ldr': '',
                    'ldrb': '',
                    'ldrsb': '',
                    'ldrh': '',
                    'ldrsh': '',
                    'ldrsw': '',
                    'ldur': '',
                    'ldurb': '',
                    'ldursb': '',
                    'ldurh': '',
                    'ldursh': '',
                    'ldursw': '',
                    'ldp': '',
                    'ldpsw': '',
                    'ldnp': '',
                    'ldtr': '',
                    'ldtrb': '',
                    'ldtrsb': '',
                    'ldtrh': '',
                    'ldtrsh': '',
                    'ldtrsw': '',
                    'ldxr': '',
                    'ldxrb': '',
                    'ldxrh': '',
                    'ldxp': '',
                    'ldar': '',
                    'ldarb': '',
                    'ldarh': '',
                    'ldaxr': '',
                    'ldaxrb': '',
                    'ldaxrh': '',
                    'ldaxp': '',
                    'ld1': '',
                    'ld2': '',
                    'ld3': '',
                    'ld4': '',
                    'ld1r': '',
                    'ld2r': '',
                    'ld3r': '',
                    'ld4r': ''}

    store_op_dict = {'str': '',
                     'strb': '',
                     'strh': '',
                     'stur': '',
                     'sturb': '',
                     'sturh': '',
                     'stp': '',
                     'stnp': '',
                     'sttr': '',
                     'sttrb': '',
                     'sttrh': '',
                     'stxr': '',
                     'stxrb': '',
                     'stxrh': '',
                     'stxp': '',
                     'stlr': '',
                     'stlrb': '',
                     'stlrh': '',
                     'stlxr': '',
                     'stlxrb': '',
                     'stlxrh': '',
                     'stlxp': '',
                     'st1': '',
                     'st2': '',
                     'st3': '',
                     'st4': ''}

    control_transfer_op_dict = {'beq': '',
                                'bne': '',
                                'bcs': '',
                                'bhs': '',
                                'bcc': '',
                                'blo': '',
                                'bmi': '',
                                'bpl': '',
                                'bvs': '',
                                'bvc': '',
                                'bhi': '',
                                'bls': '',
                                'bge': '',
                                'blt': '',
                                'bgt': '',
                                'ble': '',
                                'bal': '',
                                'bnv': '',
                                'b.eq': '',
                                'b.ne': '',
                                'b.cs': '',
                                'b.hs': '',
                                'b.cc': '',
                                'b.lo': '',
                                'b.mi': '',
                                'b.pl': '',
                                'b.vs': '',
                                'b.vc': '',
                                'b.hi': '',
                                'b.ls': '',
                                'b.ge': '',
                                'b.lt': '',
                                'b.gt': '',
                                'b.le': '',
                                'b.al': '',
                                'b.nv': '',
                                'cbnz': '',
                                'cbz': '',
                                'tbnz': '',
                                'tbz': '',
                                'b': '',
                                'bl': '',
                                'blr': '',
                                'br': '',
                                'ret': ''}

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

    def table_branch_p(self, branch_op, branch_target):
        if branch_op in ['br'] and branch_target and branch_target[0] == 'x':
            return True
        else:
            return False

    def get_table_branch_prologue_number(self):
        print('Error: get_table_branch_prologue_number')
        sys.exit(1)

    def trace_table_branch_prologue(self, region_status, line):
        print('Error: trace_table_branch_prologue')
        sys.exit(1)

    def get_table_branch_content(self, line):
        print('Error: get_table_branch_content')
        sys.exit(1)
        
    def fall_through_p(self, branch_op):
        if branch_op in ['b', 'ret']:
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

class C_aarch64__ClangLLVM(C_aarch64__):
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
    
    control_transfer_op_dict = {'ja': '',
                                'jnbe': '',
                                'jae': '',
                                'jnb': '',
                                'jb': '',
                                'jnae': '',
                                'jbe': '',
                                'jna': '',
                                'je': '',
                                'jz': '',
                                'jne': '',
                                'jnz': '',
                                'jg': '',
                                'jnle': '',
                                'jge': '',
                                'jnl': '',
                                'jl': '',
                                'jnge': '',
                                'jle': '',
                                'jng': '',
                                'js': '',
                                'jns': '',
                                'jc': '',
                                'jnc': '',
                                'jo': '',
                                'jno': '',
                                'jp': '',
                                'jpe': '',
                                'jnp': '',
                                'jpo': '',
                                'jmp': '',
                                'jmpq': '',
                                'call': '',
                                'callq': '',
                                'ret': '',
                                'retq': ''}

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
            # Tail calls like 'jmp fputc'.
            return ('jmp', None)
        m = re.match('^\t(j[\w]*)\t(\.?[_\w\d]+).*', line)
        if m:
            tmp = m.groups()
            return tmp
        m = re.match('^\t(jmpq)\t(\*%\w+).*', line)
        if m:
            tmp = m.groups()
            return tmp
        m = re.match('^\t(jmp)\t(\*%\w+).*', line)
        if m:
            tmp = m.groups()
            return tmp
        return None

    def call_p(self, branch_op, branch_target):
        if branch_op in ['callq', 'call']:
            return True
        else:
            return False

    def table_branch_p(self, branch_op, branch_target):
        print('Error: table_branch_p')
        sys.exit(1)

    def get_table_branch_prologue_number(self):
        print('Error: get_table_branch_prologue_number')
        sys.exit(1)

    def trace_table_branch_prologue(self, region_status, line):
        print('Error: trace_table_branch_prologue')
        sys.exit(1)

    def get_table_branch_content(self, line):
        print('Error: get_table_branch_content')
        sys.exit(1)
        
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

class C_x86_64__ClangLLVM(C_x86_64__):
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

    def table_branch_p(self, branch_op, branch_target):
        if branch_op in ['jmpq'] and branch_target and branch_target[0] == '*':
            return True
        else:
            return False        

    def get_table_branch_content(self, line):
        m = re.match('^\t\.quad\t(\.L.*)$', line)
        if m:
            label = m.groups()
            return label[0]
        return None

class C_x86_64__GCC(C_x86_64__):
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

    def table_branch_p(self, branch_op, branch_target):
        if branch_op in ['jmp'] and branch_target and branch_target[0] == '*':
            return True
        else:
            return False        
    
    def get_table_branch_content(self, line):
        m = re.match('^\t\.long\t(\.L\d+)-\.L\d+', line)
        if m:
            label = m.groups()
            return label[0]
        return None
