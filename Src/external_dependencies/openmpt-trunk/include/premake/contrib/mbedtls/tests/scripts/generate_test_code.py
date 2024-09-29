													#!/usr/bin/env python3
													# Test suites code generator.
													#
													# Copyright The Mbed TLS Contributors
													# SPDX-License-Identifier: Apache-2.0
													#
													# Licensed under the Apache License, Version 2.0 (the "License"); you may
													# not use this file except in compliance with the License.
													# You may obtain a copy of the License at
													#
													# http://www.apache.org/licenses/LICENSE-2.0
													#
													# Unless required by applicable law or agreed to in writing, software
													# distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
													# WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
													# See the License for the specific language governing permissions and
													# limitations under the License.
													
													"""
													This script is a key part of Mbed TLS test suites framework. For
													understanding the script it is important to understand the
													framework. This doc string contains a summary of the framework
													and explains the function of this script.
													
													Mbed TLS test suites:
													=====================
													Scope:
													------
													The test suites focus on unit testing the crypto primitives and also
													include x509 parser tests. Tests can be added to test any Mbed TLS
													module. However, the framework is not capable of testing SSL
													protocol, since that requires full stack execution and that is best
													tested as part of the system test.
													
													Test case definition:
													---------------------
													Tests are defined in a test_suite_<module>[.<optional sub module>].data
													file. A test definition contains:
                                                    test name
                                                    optional build macro dependencies
                                                    test function
                                                    test parameters
													
													Test dependencies are build macros that can be specified to indicate
													the build config in which the test is valid. For example if a test
													depends on a feature that is only enabled by defining a macro. Then
													that macro should be specified as a dependency of the test.
													
													Test function is the function that implements the test steps. This
													function is specified for different tests that perform same steps
													with different parameters.
													
													Test parameters are specified in string form separated by ':'.
													Parameters can be of type string, binary data specified as hex
													string and integer constants specified as integer, macro or
													as an expression. Following is an example test definition:
													
                                                    AES 128 GCM Encrypt and decrypt 8 bytes
                                                    depends_on:MBEDTLS_AES_C:MBEDTLS_GCM_C
                                                    enc_dec_buf:MBEDTLS_CIPHER_AES_128_GCM:"AES-128-GCM":128:8:-1
													
													Test functions:
													---------------
													Test functions are coded in C in test_suite_<module>.function files.
													Functions file is itself not compilable and contains special
													format patterns to specify test suite dependencies, start and end
													of functions and function dependencies. Check any existing functions
													file for example.
													
													Execution:
													----------
													Tests are executed in 3 steps:
													- Generating test_suite_<module>[.<optional sub module>].c file
                                                    for each corresponding .data file.
													- Building each source file into executables.
													- Running each executable and printing report.
													
													Generating C test source requires more than just the test functions.
													Following extras are required:
													- Process main()
													- Reading .data file and dispatching test cases.
													- Platform specific test case execution
													- Dependency checking
													- Integer expression evaluation
													- Test function dispatch
													
													Build dependencies and integer expressions (in the test parameters)
													are specified as strings in the .data file. Their run time value is
													not known at the generation stage. Hence, they need to be translated
													into run time evaluations. This script generates the run time checks
													for dependencies and integer expressions.
													
													Similarly, function names have to be translated into function calls.
													This script also generates code for function dispatch.
													
													The extra code mentioned here is either generated by this script
													or it comes from the input files: helpers file, platform file and
													the template file.
													
													Helper file:
													------------
													Helpers file contains common helper/utility functions and data.
													
													Platform file:
													--------------
													Platform file contains platform specific setup code and test case
													dispatch code. For example, host_test.function reads test data
													file from host's file system and dispatches tests.
													In case of on-target target_test.function tests are not dispatched
													on target. Target code is kept minimum and only test functions are
													dispatched. Test case dispatch is done on the host using tools like
													Greentea.
													
													Template file:
													---------
													Template file for example main_test.function is a template C file in
													which generated code and code from input files is substituted to
													generate a compilable C file. It also contains skeleton functions for
													dependency checks, expression evaluation and function dispatch. These
													functions are populated with checks and return codes by this script.
													
													Template file contains "replacement" fields that are formatted
													strings processed by Python string.Template.substitute() method.
													
													This script:
													============
													Core function of this script is to fill the template file with
													code that is generated or read from helpers and platform files.
													
													This script replaces following fields in the template and generates
													the test source file:
													
													$test_common_helpers        <-- All common code from helpers.function
                    is substituted here.
													$functions_code             <-- Test functions are substituted here
                    from the input test_suit_xyz.function
                    file. C preprocessor checks are generated
                    for the build dependencies specified
                    in the input file. This script also
                    generates wrappers for the test
                    functions with code to expand the
                    string parameters read from the data
                    file.
													$expression_code            <-- This script enumerates the
                    expressions in the .data file and
                    generates code to handle enumerated
                    expression Ids and return the values.
													$dep_check_code             <-- This script enumerates all
                    build dependencies and generate
                    code to handle enumerated build
                    dependency Id and return status: if
                    the dependency is defined or not.
													$dispatch_code              <-- This script enumerates the functions
                    specified in the input test data file
                    and generates the initializer for the
                    function table in the template
                    file.
													$platform_code              <-- Platform specific setup and test
                    dispatch code.
													
													"""
													
													
													import io
													import os
													import re
													import sys
													import string
													import argparse
													
													
													BEGIN_HEADER_REGEX = r'/\*\s*BEGIN_HEADER\s*\*/'
													END_HEADER_REGEX = r'/\*\s*END_HEADER\s*\*/'
													
													BEGIN_SUITE_HELPERS_REGEX = r'/\*\s*BEGIN_SUITE_HELPERS\s*\*/'
													END_SUITE_HELPERS_REGEX = r'/\*\s*END_SUITE_HELPERS\s*\*/'
													
													BEGIN_DEP_REGEX = r'BEGIN_DEPENDENCIES'
													END_DEP_REGEX = r'END_DEPENDENCIES'
													
													BEGIN_CASE_REGEX = r'/\*\s*BEGIN_CASE\s*(?P<depends_on>.*?)\s*\*/'
													END_CASE_REGEX = r'/\*\s*END_CASE\s*\*/'
													
													DEPENDENCY_REGEX = r'depends_on:(?P<dependencies>.*)'
													C_IDENTIFIER_REGEX = r'!?[a-z_][a-z0-9_]*'
													CONDITION_OPERATOR_REGEX = r'[!=]=|[<>]=?'
													# forbid 0ddd which might be accidentally octal or accidentally decimal
													CONDITION_VALUE_REGEX = r'[-+]?(0x[0-9a-f]+|0|[1-9][0-9]*)'
													CONDITION_REGEX = r'({})(?:\s*({})\s*({}))?$'.format(C_IDENTIFIER_REGEX,
CONDITION_OPERATOR_REGEX,
CONDITION_VALUE_REGEX)
													TEST_FUNCTION_VALIDATION_REGEX = r'\s*void\s+(?P<func_name>\w+)\s*\('
													INT_CHECK_REGEX = r'int\s+.*'
													CHAR_CHECK_REGEX = r'char\s*\*\s*.*'
													DATA_T_CHECK_REGEX = r'data_t\s*\*\s*.*'
													FUNCTION_ARG_LIST_END_REGEX = r'.*\)'
													EXIT_LABEL_REGEX = r'^exit:'
													
													
													class GeneratorInputError(Exception):
                                                """
                                                Exception to indicate error in the input files to this script.
                                                This includes missing patterns, test function names and other
                                                parsing errors.
                                                """
                                                pass
													
													
													class FileWrapper(io.FileIO):
                                                """
                                                This class extends built-in io.FileIO class with attribute line_no,
                                                that indicates line number for the line that is read.
                                                """
													
                                                def __init__(self, file_name):
                                            """
                                            Instantiate the base class and initialize the line number to 0.
													
                                            :param file_name: File path to open.
                                            """
                                            super(FileWrapper, self).__init__(file_name, 'r')
                                            self._line_no = 0
													
                                                def next(self):
                                            """
                                            Python 2 iterator method. This method overrides base class's
                                            next method and extends the next method to count the line
                                            numbers as each line is read.
													
                                            It works for both Python 2 and Python 3 by checking iterator
                                            method name in the base iterator object.
													
                                            :return: Line read from file.
                                            """
                                            parent = super(FileWrapper, self)
                                            if hasattr(parent, '__next__'):
                                        line = parent.__next__()  # Python 3
                                            else:
                                        line = parent.next()  # Python 2 # pylint: disable=no-member
                                            if line is not None:
                                        self._line_no += 1
                                        # Convert byte array to string with correct encoding and
                                        # strip any whitespaces added in the decoding process.
                                        return line.decode(sys.getdefaultencoding()).rstrip() + '\n'
                                            return None
													
                                                # Python 3 iterator method
                                                __next__ = next
													
                                                def get_line_no(self):
                                            """
                                            Gives current line number.
                                            """
                                            return self._line_no
													
                                                line_no = property(get_line_no)
													
													
													def split_dep(dep):
                                                """
                                                Split NOT character '!' from dependency. Used by gen_dependencies()
													
                                                :param dep: Dependency list
                                                :return: string tuple. Ex: ('!', MACRO) for !MACRO and ('', MACRO) for
                                        MACRO.
                                                """
                                                return ('!', dep[1:]) if dep[0] == '!' else ('', dep)
													
													
													def gen_dependencies(dependencies):
                                                """
                                                Test suite data and functions specifies compile time dependencies.
                                                This function generates C preprocessor code from the input
                                                dependency list. Caller uses the generated preprocessor code to
                                                wrap dependent code.
                                                A dependency in the input list can have a leading '!' character
                                                to negate a condition. '!' is separated from the dependency using
                                                function split_dep() and proper preprocessor check is generated
                                                accordingly.
													
                                                :param dependencies: List of dependencies.
                                                :return: if defined and endif code with macro annotations for
                                        readability.
                                                """
                                                dep_start = ''.join(['#if %sdefined(%s)\n' % (x, y) for x, y in
                            map(split_dep, dependencies)])
                                                dep_end = ''.join(['#endif /* %s */\n' %
                                x for x in reversed(dependencies)])
													
                                                return dep_start, dep_end
													
													
													def gen_dependencies_one_line(dependencies):
                                                """
                                                Similar to gen_dependencies() but generates dependency checks in one line.
                                                Useful for generating code with #else block.
													
                                                :param dependencies: List of dependencies.
                                                :return: Preprocessor check code
                                                """
                                                defines = '#if ' if dependencies else ''
                                                defines += ' && '.join(['%sdefined(%s)' % (x, y) for x, y in map(
                                            split_dep, dependencies)])
                                                return defines
													
													
													def gen_function_wrapper(name, local_vars, args_dispatch):
                                                """
                                                Creates test function wrapper code. A wrapper has the code to
                                                unpack parameters from parameters[] array.
													
                                                :param name: Test function name
                                                :param local_vars: Local variables declaration code
                                                :param args_dispatch: List of dispatch arguments.
                                            Ex: ['(char *)params[0]', '*((int *)params[1])']
                                                :return: Test function wrapper.
                                                """
                                                # Then create the wrapper
                                                wrapper = '''
													void {name}_wrapper( void ** params )
													{{
													{unused_params}{locals}
                                                {name}( {args} );
													}}
													'''.format(name=name,
                                            unused_params='' if args_dispatch else '    (void)params;\n',
                                            args=', '.join(args_dispatch),
                                            locals=local_vars)
                                                return wrapper
													
													
													def gen_dispatch(name, dependencies):
                                                """
                                                Test suite code template main_test.function defines a C function
                                                array to contain test case functions. This function generates an
                                                initializer entry for a function in that array. The entry is
                                                composed of a compile time check for the test function
                                                dependencies. At compile time the test function is assigned when
                                                dependencies are met, else NULL is assigned.
													
                                                :param name: Test function name
                                                :param dependencies: List of dependencies
                                                :return: Dispatch code.
                                                """
                                                if dependencies:
                                            preprocessor_check = gen_dependencies_one_line(dependencies)
                                            dispatch_code = '''
													{preprocessor_check}
                                                {name}_wrapper,
													#else
                                                NULL,
													#endif
													'''.format(preprocessor_check=preprocessor_check, name=name)
                                                else:
                                            dispatch_code = '''
                                                {name}_wrapper,
													'''.format(name=name)
													
                                                return dispatch_code
													
													
													def parse_until_pattern(funcs_f, end_regex):
                                                """
                                                Matches pattern end_regex to the lines read from the file object.
                                                Returns the lines read until end pattern is matched.
													
                                                :param funcs_f: file object for .function file
                                                :param end_regex: Pattern to stop parsing
                                                :return: Lines read before the end pattern
                                                """
                                                headers = '#line %d "%s"\n' % (funcs_f.line_no + 1, funcs_f.name)
                                                for line in funcs_f:
                                            if re.search(end_regex, line):
                                        break
                                            headers += line
                                                else:
                                            raise GeneratorInputError("file: %s - end pattern [%s] not found!" %
                    (funcs_f.name, end_regex))
													
                                                return headers
													
													
													def validate_dependency(dependency):
                                                """
                                                Validates a C macro and raises GeneratorInputError on invalid input.
                                                :param dependency: Input macro dependency
                                                :return: input dependency stripped of leading & trailing white spaces.
                                                """
                                                dependency = dependency.strip()
                                                if not re.match(CONDITION_REGEX, dependency, re.I):
                                            raise GeneratorInputError('Invalid dependency %s' % dependency)
                                                return dependency
													
													
													def parse_dependencies(inp_str):
                                                """
                                                Parses dependencies out of inp_str, validates them and returns a
                                                list of macros.
													
                                                :param inp_str: Input string with macros delimited by ':'.
                                                :return: list of dependencies
                                                """
                                                dependencies = list(map(validate_dependency, inp_str.split(':')))
                                                return dependencies
													
													
													def parse_suite_dependencies(funcs_f):
                                                """
                                                Parses test suite dependencies specified at the top of a
                                                .function file, that starts with pattern BEGIN_DEPENDENCIES
                                                and end with END_DEPENDENCIES. Dependencies are specified
                                                after pattern 'depends_on:' and are delimited by ':'.
													
                                                :param funcs_f: file object for .function file
                                                :return: List of test suite dependencies.
                                                """
                                                dependencies = []
                                                for line in funcs_f:
                                            match = re.search(DEPENDENCY_REGEX, line.strip())
                                            if match:
                                        try:
                                    dependencies = parse_dependencies(match.group('dependencies'))
                                        except GeneratorInputError as error:
                                    raise GeneratorInputError(
                                str(error) + " - %s:%d" % (funcs_f.name, funcs_f.line_no))
                                            if re.search(END_DEP_REGEX, line):
                                        break
                                                else:
                                            raise GeneratorInputError("file: %s - end dependency pattern [%s]"
                    " not found!" % (funcs_f.name,
    END_DEP_REGEX))
													
                                                return dependencies
													
													
													def parse_function_dependencies(line):
                                                """
                                                Parses function dependencies, that are in the same line as
                                                comment BEGIN_CASE. Dependencies are specified after pattern
                                                'depends_on:' and are delimited by ':'.
													
                                                :param line: Line from .function file that has dependencies.
                                                :return: List of dependencies.
                                                """
                                                dependencies = []
                                                match = re.search(BEGIN_CASE_REGEX, line)
                                                dep_str = match.group('depends_on')
                                                if dep_str:
                                            match = re.search(DEPENDENCY_REGEX, dep_str)
                                            if match:
                                        dependencies += parse_dependencies(match.group('dependencies'))
													
                                                return dependencies
													
													
													def parse_function_arguments(line):
                                                """
                                                Parses test function signature for validation and generates
                                                a dispatch wrapper function that translates input test vectors
                                                read from the data file into test function arguments.
													
                                                :param line: Line from .function file that has a function
                                    signature.
                                                :return: argument list, local variables for
                                        wrapper function and argument dispatch code.
                                                """
                                                args = []
                                                local_vars = ''
                                                args_dispatch = []
                                                arg_idx = 0
                                                # Remove characters before arguments
                                                line = line[line.find('(') + 1:]
                                                # Process arguments, ex: <type> arg1, <type> arg2 )
                                                # This script assumes that the argument list is terminated by ')'
                                                # i.e. the test functions will not have a function pointer
                                                # argument.
                                                for arg in line[:line.find(')')].split(','):
                                            arg = arg.strip()
                                            if arg == '':
                                        continue
                                            if re.search(INT_CHECK_REGEX, arg.strip()):
                                        args.append('int')
                                        args_dispatch.append('*( (int *) params[%d] )' % arg_idx)
                                            elif re.search(CHAR_CHECK_REGEX, arg.strip()):
                                        args.append('char*')
                                        args_dispatch.append('(char *) params[%d]' % arg_idx)
                                            elif re.search(DATA_T_CHECK_REGEX, arg.strip()):
                                        args.append('hex')
                                        # create a structure
                                        pointer_initializer = '(uint8_t *) params[%d]' % arg_idx
                                        len_initializer = '*( (uint32_t *) params[%d] )' % (arg_idx+1)
                                        local_vars += """    data_t data%d = {%s, %s};
													""" % (arg_idx, pointer_initializer, len_initializer)
													
                                        args_dispatch.append('&data%d' % arg_idx)
                                        arg_idx += 1
                                            else:
                                        raise ValueError("Test function arguments can only be 'int', "
                        "'char *' or 'data_t'\n%s" % line)
                                            arg_idx += 1
													
                                                return args, local_vars, args_dispatch
													
													
													def generate_function_code(name, code, local_vars, args_dispatch,
                            dependencies):
                                                """
                                                Generate function code with preprocessor checks and parameter dispatch
                                                wrapper.
													
                                                :param name: Function name
                                                :param code: Function code
                                                :param local_vars: Local variables for function wrapper
                                                :param args_dispatch: Argument dispatch code
                                                :param dependencies: Preprocessor dependencies list
                                                :return: Final function code
                                                """
                                                # Add exit label if not present
                                                if code.find('exit:') == -1:
                                            split_code = code.rsplit('}', 1)
                                            if len(split_code) == 2:
                                        code = """exit:
                                                ;
													}""".join(split_code)
													
                                                code += gen_function_wrapper(name, local_vars, args_dispatch)
                                                preprocessor_check_start, preprocessor_check_end = \
                                            gen_dependencies(dependencies)
                                                return preprocessor_check_start + code + preprocessor_check_end
													
													
													def parse_function_code(funcs_f, dependencies, suite_dependencies):
                                                """
                                                Parses out a function from function file object and generates
                                                function and dispatch code.
													
                                                :param funcs_f: file object of the functions file.
                                                :param dependencies: List of dependencies
                                                :param suite_dependencies: List of test suite dependencies
                                                :return: Function name, arguments, function code and dispatch code.
                                                """
                                                line_directive = '#line %d "%s"\n' % (funcs_f.line_no + 1, funcs_f.name)
                                                code = ''
                                                has_exit_label = False
                                                for line in funcs_f:
                                            # Check function signature. Function signature may be split
                                            # across multiple lines. Here we try to find the start of
                                            # arguments list, then remove '\n's and apply the regex to
                                            # detect function start.
                                            up_to_arg_list_start = code + line[:line.find('(') + 1]
                                            match = re.match(TEST_FUNCTION_VALIDATION_REGEX,
                            up_to_arg_list_start.replace('\n', ' '), re.I)
                                            if match:
                                        # check if we have full signature i.e. split in more lines
                                        name = match.group('func_name')
                                        if not re.match(FUNCTION_ARG_LIST_END_REGEX, line):
                                    for lin in funcs_f:
                                line += lin
                                if re.search(FUNCTION_ARG_LIST_END_REGEX, line):
                            break
                                        args, local_vars, args_dispatch = parse_function_arguments(
                                    line)
                                        code += line
                                        break
                                            code += line
                                                else:
                                            raise GeneratorInputError("file: %s - Test functions not found!" %
                    funcs_f.name)
													
                                                # Prefix test function name with 'test_'
                                                code = code.replace(name, 'test_' + name, 1)
                                                name = 'test_' + name
													
                                                for line in funcs_f:
                                            if re.search(END_CASE_REGEX, line):
                                        break
                                            if not has_exit_label:
                                        has_exit_label = \
                                    re.search(EXIT_LABEL_REGEX, line.strip()) is not None
                                            code += line
                                                else:
                                            raise GeneratorInputError("file: %s - end case pattern [%s] not "
                    "found!" % (funcs_f.name, END_CASE_REGEX))
													
                                                code = line_directive + code
                                                code = generate_function_code(name, code, local_vars, args_dispatch,
                    dependencies)
                                                dispatch_code = gen_dispatch(name, suite_dependencies + dependencies)
                                                return (name, args, code, dispatch_code)
													
													
													def parse_functions(funcs_f):
                                                """
                                                Parses a test_suite_xxx.function file and returns information
                                                for generating a C source file for the test suite.
													
                                                :param funcs_f: file object of the functions file.
                                                :return: List of test suite dependencies, test function dispatch
                                        code, function code and a dict with function identifiers
                                        and arguments info.
                                                """
                                                suite_helpers = ''
                                                suite_dependencies = []
                                                suite_functions = ''
                                                func_info = {}
                                                function_idx = 0
                                                dispatch_code = ''
                                                for line in funcs_f:
                                            if re.search(BEGIN_HEADER_REGEX, line):
                                        suite_helpers += parse_until_pattern(funcs_f, END_HEADER_REGEX)
                                            elif re.search(BEGIN_SUITE_HELPERS_REGEX, line):
                                        suite_helpers += parse_until_pattern(funcs_f,
    END_SUITE_HELPERS_REGEX)
                                            elif re.search(BEGIN_DEP_REGEX, line):
                                        suite_dependencies += parse_suite_dependencies(funcs_f)
                                            elif re.search(BEGIN_CASE_REGEX, line):
                                        try:
                                    dependencies = parse_function_dependencies(line)
                                        except GeneratorInputError as error:
                                    raise GeneratorInputError(
                                "%s:%d: %s" % (funcs_f.name, funcs_f.line_no,
                    str(error)))
                                        func_name, args, func_code, func_dispatch =\
                                    parse_function_code(funcs_f, dependencies, suite_dependencies)
                                        suite_functions += func_code
                                        # Generate dispatch code and enumeration info
                                        if func_name in func_info:
                                    raise GeneratorInputError(
                                "file: %s - function %s re-declared at line %d" %
                                (funcs_f.name, func_name, funcs_f.line_no))
                                        func_info[func_name] = (function_idx, args)
                                        dispatch_code += '/* Function Id: %d */\n' % function_idx
                                        dispatch_code += func_dispatch
                                        function_idx += 1
													
                                                func_code = (suite_helpers +
                                    suite_functions).join(gen_dependencies(suite_dependencies))
                                                return suite_dependencies, dispatch_code, func_code, func_info
													
													
													def escaped_split(inp_str, split_char):
                                                """
                                                Split inp_str on character split_char but ignore if escaped.
                                                Since, return value is used to write back to the intermediate
                                                data file, any escape characters in the input are retained in the
                                                output.
													
                                                :param inp_str: String to split
                                                :param split_char: Split character
                                                :return: List of splits
                                                """
                                                if len(split_char) > 1:
                                            raise ValueError('Expected split character. Found string!')
                                                out = re.sub(r'(\\.)|' + split_char,
                                    lambda m: m.group(1) or '\n', inp_str,
                                    len(inp_str)).split('\n')
                                                out = [x for x in out if x]
                                                return out
													
													
													def parse_test_data(data_f):
                                                """
                                                Parses .data file for each test case name, test function name,
                                                test dependencies and test arguments. This information is
                                                correlated with the test functions file for generating an
                                                intermediate data file replacing the strings for test function
                                                names, dependencies and integer constant expressions with
                                                identifiers. Mainly for optimising space for on-target
                                                execution.
													
                                                :param data_f: file object of the data file.
                                                :return: Generator that yields test name, function name,
                                        dependency list and function argument list.
                                                """
                                                __state_read_name = 0
                                                __state_read_args = 1
                                                state = __state_read_name
                                                dependencies = []
                                                name = ''
                                                for line in data_f:
                                            line = line.strip()
                                            # Skip comments
                                            if line.startswith('#'):
                                        continue
													
                                            # Blank line indicates end of test
                                            if not line:
                                        if state == __state_read_args:
                                    raise GeneratorInputError("[%s:%d] Newline before arguments. "
            "Test function and arguments "
            "missing for %s" %
            (data_f.name, data_f.line_no, name))
                                        continue
													
                                            if state == __state_read_name:
                                        # Read test name
                                        name = line
                                        state = __state_read_args
                                            elif state == __state_read_args:
                                        # Check dependencies
                                        match = re.search(DEPENDENCY_REGEX, line)
                                        if match:
                                    try:
                                dependencies = parse_dependencies(
                            match.group('dependencies'))
                                    except GeneratorInputError as error:
                                raise GeneratorInputError(
                            str(error) + " - %s:%d" %
                            (data_f.name, data_f.line_no))
                                        else:
                                    # Read test vectors
                                    parts = escaped_split(line, ':')
                                    test_function = parts[0]
                                    args = parts[1:]
                                    yield name, test_function, dependencies, args
                                    dependencies = []
                                    state = __state_read_name
                                                if state == __state_read_args:
                                            raise GeneratorInputError("[%s:%d] Newline before arguments. "
                    "Test function and arguments missing for "
                    "%s" % (data_f.name, data_f.line_no, name))
													
													
													def gen_dep_check(dep_id, dep):
                                                """
                                                Generate code for checking dependency with the associated
                                                identifier.
													
                                                :param dep_id: Dependency identifier
                                                :param dep: Dependency macro
                                                :return: Dependency check code
                                                """
                                                if dep_id < 0:
                                            raise GeneratorInputError("Dependency Id should be a positive "
                    "integer.")
                                                _not, dep = ('!', dep[1:]) if dep[0] == '!' else ('', dep)
                                                if not dep:
                                            raise GeneratorInputError("Dependency should not be an empty string.")
													
                                                dependency = re.match(CONDITION_REGEX, dep, re.I)
                                                if not dependency:
                                            raise GeneratorInputError('Invalid dependency %s' % dep)
													
                                                _defined = '' if dependency.group(2) else 'defined'
                                                _cond = dependency.group(2) if dependency.group(2) else ''
                                                _value = dependency.group(3) if dependency.group(3) else ''
													
                                                dep_check = '''
                                            case {id}:
                                        {{
													#if {_not}{_defined}({macro}{_cond}{_value})
                                    ret = DEPENDENCY_SUPPORTED;
													#else
                                    ret = DEPENDENCY_NOT_SUPPORTED;
													#endif
                                        }}
                                        break;'''.format(_not=_not, _defined=_defined,
                        macro=dependency.group(1), id=dep_id,
                        _cond=_cond, _value=_value)
                                                return dep_check
													
													
													def gen_expression_check(exp_id, exp):
                                                """
                                                Generates code for evaluating an integer expression using
                                                associated expression Id.
													
                                                :param exp_id: Expression Identifier
                                                :param exp: Expression/Macro
                                                :return: Expression check code
                                                """
                                                if exp_id < 0:
                                            raise GeneratorInputError("Expression Id should be a positive "
                    "integer.")
                                                if not exp:
                                            raise GeneratorInputError("Expression should not be an empty string.")
                                                exp_code = '''
                                            case {exp_id}:
                                        {{
                                    *out_value = {expression};
                                        }}
                                        break;'''.format(exp_id=exp_id, expression=exp)
                                                return exp_code
													
													
													def write_dependencies(out_data_f, test_dependencies, unique_dependencies):
                                                """
                                                Write dependencies to intermediate test data file, replacing
                                                the string form with identifiers. Also, generates dependency
                                                check code.
													
                                                :param out_data_f: Output intermediate data file
                                                :param test_dependencies: Dependencies
                                                :param unique_dependencies: Mutable list to track unique dependencies
                                            that are global to this re-entrant function.
                                                :return: returns dependency check code.
                                                """
                                                dep_check_code = ''
                                                if test_dependencies:
                                            out_data_f.write('depends_on')
                                            for dep in test_dependencies:
                                        if dep not in unique_dependencies:
                                    unique_dependencies.append(dep)
                                    dep_id = unique_dependencies.index(dep)
                                    dep_check_code += gen_dep_check(dep_id, dep)
                                        else:
                                    dep_id = unique_dependencies.index(dep)
                                        out_data_f.write(':' + str(dep_id))
                                            out_data_f.write('\n')
                                                return dep_check_code
													
													
													def write_parameters(out_data_f, test_args, func_args, unique_expressions):
                                                """
                                                Writes test parameters to the intermediate data file, replacing
                                                the string form with identifiers. Also, generates expression
                                                check code.
													
                                                :param out_data_f: Output intermediate data file
                                                :param test_args: Test parameters
                                                :param func_args: Function arguments
                                                :param unique_expressions: Mutable list to track unique
                                            expressions that are global to this re-entrant function.
                                                :return: Returns expression check code.
                                                """
                                                expression_code = ''
                                                for i, _ in enumerate(test_args):
                                            typ = func_args[i]
                                            val = test_args[i]
													
                                            # check if val is a non literal int val (i.e. an expression)
                                            if typ == 'int' and not re.match(r'(\d+|0x[0-9a-f]+)$',
            val, re.I):
                                        typ = 'exp'
                                        if val not in unique_expressions:
                                    unique_expressions.append(val)
                                    # exp_id can be derived from len(). But for
                                    # readability and consistency with case of existing
                                    # let's use index().
                                    exp_id = unique_expressions.index(val)
                                    expression_code += gen_expression_check(exp_id, val)
                                    val = exp_id
                                        else:
                                    val = unique_expressions.index(val)
                                            out_data_f.write(':' + typ + ':' + str(val))
                                                out_data_f.write('\n')
                                                return expression_code
													
													
													def gen_suite_dep_checks(suite_dependencies, dep_check_code, expression_code):
                                                """
                                                Generates preprocessor checks for test suite dependencies.
													
                                                :param suite_dependencies: Test suite dependencies read from the
                                        .function file.
                                                :param dep_check_code: Dependency check code
                                                :param expression_code: Expression check code
                                                :return: Dependency and expression code guarded by test suite
                                        dependencies.
                                                """
                                                if suite_dependencies:
                                            preprocessor_check = gen_dependencies_one_line(suite_dependencies)
                                            dep_check_code = '''
													{preprocessor_check}
													{code}
													#endif
													'''.format(preprocessor_check=preprocessor_check, code=dep_check_code)
                                            expression_code = '''
													{preprocessor_check}
													{code}
													#endif
													'''.format(preprocessor_check=preprocessor_check, code=expression_code)
                                                return dep_check_code, expression_code
													
													
													def gen_from_test_data(data_f, out_data_f, func_info, suite_dependencies):
                                                """
                                                This function reads test case name, dependencies and test vectors
                                                from the .data file. This information is correlated with the test
                                                functions file for generating an intermediate data file replacing
                                                the strings for test function names, dependencies and integer
                                                constant expressions with identifiers. Mainly for optimising
                                                space for on-target execution.
                                                It also generates test case dependency check code and expression
                                                evaluation code.
													
                                                :param data_f: Data file object
                                                :param out_data_f: Output intermediate data file
                                                :param func_info: Dict keyed by function and with function id
                                            and arguments info
                                                :param suite_dependencies: Test suite dependencies
                                                :return: Returns dependency and expression check code
                                                """
                                                unique_dependencies = []
                                                unique_expressions = []
                                                dep_check_code = ''
                                                expression_code = ''
                                                for test_name, function_name, test_dependencies, test_args in \
                                        parse_test_data(data_f):
                                            out_data_f.write(test_name + '\n')
													
                                            # Write dependencies
                                            dep_check_code += write_dependencies(out_data_f, test_dependencies,
        unique_dependencies)
													
                                            # Write test function name
                                            test_function_name = 'test_' + function_name
                                            if test_function_name not in func_info:
                                        raise GeneratorInputError("Function %s not found!" %
                test_function_name)
                                            func_id, func_args = func_info[test_function_name]
                                            out_data_f.write(str(func_id))
													
                                            # Write parameters
                                            if len(test_args) != len(func_args):
                                        raise GeneratorInputError("Invalid number of arguments in test "
                "%s. See function %s signature." %
                (test_name, function_name))
                                            expression_code += write_parameters(out_data_f, test_args, func_args,
        unique_expressions)
													
                                            # Write a newline as test case separator
                                            out_data_f.write('\n')
													
                                                dep_check_code, expression_code = gen_suite_dep_checks(
                                            suite_dependencies, dep_check_code, expression_code)
                                                return dep_check_code, expression_code
													
													
													def add_input_info(funcs_file, data_file, template_file,
                                    c_file, snippets):
                                                """
                                                Add generator input info in snippets.
													
                                                :param funcs_file: Functions file object
                                                :param data_file: Data file object
                                                :param template_file: Template file object
                                                :param c_file: Output C file object
                                                :param snippets: Dictionary to contain code pieces to be
                                substituted in the template.
                                                :return:
                                                """
                                                snippets['test_file'] = c_file
                                                snippets['test_main_file'] = template_file
                                                snippets['test_case_file'] = funcs_file
                                                snippets['test_case_data_file'] = data_file
													
													
													def read_code_from_input_files(platform_file, helpers_file,
                        out_data_file, snippets):
                                                """
                                                Read code from input files and create substitutions for replacement
                                                strings in the template file.
													
                                                :param platform_file: Platform file object
                                                :param helpers_file: Helper functions file object
                                                :param out_data_file: Output intermediate data file object
                                                :param snippets: Dictionary to contain code pieces to be
                                substituted in the template.
                                                :return:
                                                """
                                                # Read helpers
                                                with open(helpers_file, 'r') as help_f, open(platform_file, 'r') as \
                                        platform_f:
                                            snippets['test_common_helper_file'] = helpers_file
                                            snippets['test_common_helpers'] = help_f.read()
                                            snippets['test_platform_file'] = platform_file
                                            snippets['platform_code'] = platform_f.read().replace(
                                        'DATA_FILE', out_data_file.replace('\\', '\\\\'))  # escape '\'
													
													
													def write_test_source_file(template_file, c_file, snippets):
                                                """
                                                Write output source file with generated source code.
													
                                                :param template_file: Template file name
                                                :param c_file: Output source file
                                                :param snippets: Generated and code snippets
                                                :return:
                                                """
                                                with open(template_file, 'r') as template_f, open(c_file, 'w') as c_f:
                                            for line_no, line in enumerate(template_f.readlines(), 1):
                                        # Update line number. +1 as #line directive sets next line number
                                        snippets['line_no'] = line_no + 1
                                        code = string.Template(line).substitute(**snippets)
                                        c_f.write(code)
													
													
													def parse_function_file(funcs_file, snippets):
                                                """
                                                Parse function file and generate function dispatch code.
													
                                                :param funcs_file: Functions file name
                                                :param snippets: Dictionary to contain code pieces to be
                                substituted in the template.
                                                :return:
                                                """
                                                with FileWrapper(funcs_file) as funcs_f:
                                            suite_dependencies, dispatch_code, func_code, func_info = \
                                        parse_functions(funcs_f)
                                            snippets['functions_code'] = func_code
                                            snippets['dispatch_code'] = dispatch_code
                                            return suite_dependencies, func_info
													
													
													def generate_intermediate_data_file(data_file, out_data_file,
                suite_dependencies, func_info, snippets):
                                                """
                                                Generates intermediate data file from input data file and
                                                information read from functions file.
													
                                                :param data_file: Data file name
                                                :param out_data_file: Output/Intermediate data file
                                                :param suite_dependencies: List of suite dependencies.
                                                :param func_info: Function info parsed from functions file.
                                                :param snippets: Dictionary to contain code pieces to be
                                substituted in the template.
                                                :return:
                                                """
                                                with FileWrapper(data_file) as data_f, \
                                        open(out_data_file, 'w') as out_data_f:
                                            dep_check_code, expression_code = gen_from_test_data(
                                        data_f, out_data_f, func_info, suite_dependencies)
                                            snippets['dep_check_code'] = dep_check_code
                                            snippets['expression_code'] = expression_code
													
													
													def generate_code(**input_info):
                                                """
                                                Generates C source code from test suite file, data file, common
                                                helpers file and platform file.
													
                                                input_info expands to following parameters:
                                                funcs_file: Functions file object
                                                data_file: Data file object
                                                template_file: Template file object
                                                platform_file: Platform file object
                                                helpers_file: Helper functions file object
                                                suites_dir: Test suites dir
                                                c_file: Output C file object
                                                out_data_file: Output intermediate data file object
                                                :return:
                                                """
                                                funcs_file = input_info['funcs_file']
                                                data_file = input_info['data_file']
                                                template_file = input_info['template_file']
                                                platform_file = input_info['platform_file']
                                                helpers_file = input_info['helpers_file']
                                                suites_dir = input_info['suites_dir']
                                                c_file = input_info['c_file']
                                                out_data_file = input_info['out_data_file']
                                                for name, path in [('Functions file', funcs_file),
                                ('Data file', data_file),
                                ('Template file', template_file),
                                ('Platform file', platform_file),
                                ('Helpers code file', helpers_file),
                                ('Suites dir', suites_dir)]:
                                            if not os.path.exists(path):
                                        raise IOError("ERROR: %s [%s] not found!" % (name, path))
													
                                                snippets = {'generator_script': os.path.basename(__file__)}
                                                read_code_from_input_files(platform_file, helpers_file,
                        out_data_file, snippets)
                                                add_input_info(funcs_file, data_file, template_file,
                                    c_file, snippets)
                                                suite_dependencies, func_info = parse_function_file(funcs_file, snippets)
                                                generate_intermediate_data_file(data_file, out_data_file,
                suite_dependencies, func_info, snippets)
                                                write_test_source_file(template_file, c_file, snippets)
													
													
													def main():
                                                """
                                                Command line parser.
													
                                                :return:
                                                """
                                                parser = argparse.ArgumentParser(
                                            description='Dynamically generate test suite code.')
													
                                                parser.add_argument("-f", "--functions-file",
                            dest="funcs_file",
                            help="Functions file",
                            metavar="FUNCTIONS_FILE",
                            required=True)
													
                                                parser.add_argument("-d", "--data-file",
                            dest="data_file",
                            help="Data file",
                            metavar="DATA_FILE",
                            required=True)
													
                                                parser.add_argument("-t", "--template-file",
                            dest="template_file",
                            help="Template file",
                            metavar="TEMPLATE_FILE",
                            required=True)
													
                                                parser.add_argument("-s", "--suites-dir",
                            dest="suites_dir",
                            help="Suites dir",
                            metavar="SUITES_DIR",
                            required=True)
													
                                                parser.add_argument("--helpers-file",
                            dest="helpers_file",
                            help="Helpers file",
                            metavar="HELPERS_FILE",
                            required=True)
													
                                                parser.add_argument("-p", "--platform-file",
                            dest="platform_file",
                            help="Platform code file",
                            metavar="PLATFORM_FILE",
                            required=True)
													
                                                parser.add_argument("-o", "--out-dir",
                            dest="out_dir",
                            help="Dir where generated code and scripts are copied",
                            metavar="OUT_DIR",
                            required=True)
													
                                                args = parser.parse_args()
													
                                                data_file_name = os.path.basename(args.data_file)
                                                data_name = os.path.splitext(data_file_name)[0]
													
                                                out_c_file = os.path.join(args.out_dir, data_name + '.c')
                                                out_data_file = os.path.join(args.out_dir, data_name + '.datax')
													
                                                out_c_file_dir = os.path.dirname(out_c_file)
                                                out_data_file_dir = os.path.dirname(out_data_file)
                                                for directory in [out_c_file_dir, out_data_file_dir]:
                                            if not os.path.exists(directory):
                                        os.makedirs(directory)
													
                                                generate_code(funcs_file=args.funcs_file, data_file=args.data_file,
                                    template_file=args.template_file,
                                    platform_file=args.platform_file,
                                    helpers_file=args.helpers_file, suites_dir=args.suites_dir,
                                    c_file=out_c_file, out_data_file=out_data_file)
													
													
													if __name__ == "__main__":
                                                try:
                                            main()
                                                except GeneratorInputError as err:
                                            sys.exit("%s: input error: %s" %
                                    (os.path.basename(sys.argv[0]), str(err)))
													