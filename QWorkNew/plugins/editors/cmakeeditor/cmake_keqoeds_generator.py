import subprocess
import shlex

cmake_path = 'C:\\CMake\\bin'
src_path = 'c:\\Alex\\cmake_gen.cpp'

def cmake_var_list(src):
    cmd = 'cmake.exe --help-variable-list'
    args = shlex.split(cmd)
    p = subprocess.Popen(args, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, cwd=cmake_path)
    result = p.communicate()[0]
    var_list = result.split('\r\n')

    src.write('static const char *const variableKeywords[] = {\n')
    for var in var_list[1:]:
        if var.find('_<CONFIG>') != -1:
            src.write('    "{0}",\n'.format(var.replace('_<CONFIG>', '_DEBUG')))
            src.write('    "{0}",\n'.format(var.replace('_<CONFIG>', '_RELEASE')))
            src.write('    "{0}",\n'.format(var.replace('_<CONFIG>', '_MINSIZEREL')))
            src.write('    "{0}",\n'.format(var.replace('_<CONFIG>', '_RELWITHDEBINFO')))
        elif var.find('_<LANG>') != -1:
            src.write('    "{0}",\n'.format(var.replace('_<LANG>', '_C')))
            src.write('    "{0}",\n'.format(var.replace('_<LANG>', '_CXX')))
        elif var.find('>_') == -1:
            src.write('    "{0}",\n'.format(var))
    src.write('    0\n};\n\n')

def cmake_command_list(src):
    cmd = 'cmake.exe --help-command-list'
    args = shlex.split(cmd)
    p = subprocess.Popen(args, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, cwd='C:\\CMake\\bin')
    result = p.communicate()[0]
    var_list = result.split('\r\n')

    src.write('static const char *const functionKeywords[] = {\n')
    for var in var_list[1:]:
        src.write('    "{0}",\n'.format(var))
    src.write('    0\n};\n\n')

with open(src_path, 'w') as src:
    cmake_var_list(src)
    cmake_command_list(src)
