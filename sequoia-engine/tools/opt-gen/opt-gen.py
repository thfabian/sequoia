#!/usr/bin/python3
# -*- coding: utf-8 -*-
##===-----------------------------------------------------------------------------*- Python -*-===##
##                         _____                        _       
##                        / ____|                      (_)      
##                       | (___   ___  __ _ _   _  ___  _  __ _ 
##                        \___ \ / _ \/ _` | | | |/ _ \| |/ _` |
##                        ____) |  __/ (_| | |_| | (_) | | (_| |
##                       |_____/ \___|\__, |\__,_|\___/|_|\__,_| - Game Engine
##                                       | |                    
##                                       |_| 
##
## This file is distributed under the MIT License (MIT).
## See LICENSE.txt for details.
##
##===------------------------------------------------------------------------------------------===##
##
## This script parses all the json files and generates a single header *.h and *.inc file of all the
## options.
## 
## The json file needs to have the following format:
##
##  {
##    "Options" : {
##      "StructName" : {
##        "OptionName" : {            # Name of the option (i.e accessed via 
##                                    # Options.StructName.OptionName).
##          "value_type" : "",        # Default value of the option.
##          "value_default" : "",     # Type of the option [int, bool, std::string].
##          "value_allowed" : "",     # [optional] Comma separated list of allowed values. Note that
##                                                 this will implicitly create the check_fun.
##          "doc" : "",               # [optional] Documentation string of the option
##          "check_fun" : "",         # [optional] Lambda function to check if option is valid. 
##                                    #            Default is [](const auto& value) { return true; }
##                                    #            Note that only the body { ... } needs to be 
##                                    #            provided i.e [](const auto& value) will be added 
##                                    #            automatically.
##          "cl" : "",                # [optional] Command-line option (without leading "-" or "--")
##          "cl_metavar" : ""         # [optional] Meta variable used in the help string of the 
##                                    #            command-line target.
##        }
##      }
##    }
##  }
##
## Note that one can reference other arguments (within the same option) with ${...}. 
## For example to reference the type of the option in the doc string:
##          ...
##          "value_type" : "double",
##          "doc" : "This option is of type ${value_type}",
##          ...
##
## Which results in a doc-string "This option is of type double".
##
##===------------------------------------------------------------------------------------------===##

from argparse import ArgumentParser
from sys import exit, stderr, argv
from json import load
from pprint import pprint
from os import getcwd, path
from re import compile, search

# Globals
g_program = None
g_verbose = None

def report_fatal_error(msg):
    """ Issue a fatal error and exit with non-zero exit status """
    print(g_program + ": error: " + msg, file=stderr)
    exit(1)

def report_info(msg):
    """ Report an info message """
    if g_verbose:    
        print("[LOG] " + msg.rstrip())

class Option(object):
    def __init__(self):
        self.name = None            # Name of the option (i.e accessed via Option.struct.name)
        self.value_type = None      # Type of the option [int, bool, std::string]
        self.value_default = None   # Default value of the option
        self.doc = None             # [optional] Documentation string of the option
        self.check_fun = None       # [optional] lambda function to check if option is valid. 
                                    #            Default is [](const auto& value) { return true; }
        self.cl = None              # [optional] Command-line option (without leading "-" or "--")
        self.cl_short = None        # [optional] Short command-line option (single character)
        self.cl_metavar = None      # [optional] Meta variable used in the help string of the 
                                    #            command-line target

class OptionList(object):
    """ List of all options grouped by struct """
    def __init__(self):
        self.options = {}
        self.regex_substitute = compile("(\$\{[^}]*\})")

    def add_option(self, struct, option):
        if not struct in self.options:
            self.options[struct] = []
        self.options[struct] += [option]


    def substitute_and_setattr(self, opt, key, option_dict):
        value = option_dict[key]        
        
        match = search(self.regex_substitute, str(value))
        while match:
            match_str = match.group(1)
            key_name = match_str[2:-1] # ${X} -> X

            # Check if argument exists
            if not key_name in option_dict:            
                raise KeyError("bad substitution: '" + key_name + "'")
            
            # Recursivly resolve the option
            self.substitute_and_setattr(opt, key_name, option_dict)
            substitution_value = option_dict[key_name]

            # Replace the option
            value = value.replace(match_str, substitution_value)

            # Continue substituting
            match = search(self.regex_substitute, str(value))
        
        setattr(opt, key, value)

    def parse_json_file(self, path):
        """ Parse the given JSON file, validate it and convert it to a list of 'Option's """
        with open(path) as json_file:
            report_info("Parsing file " + path + " ...")
            json_data = load(json_file)

        optional = ["value_allowed"]

        try:
            for struct_name, options in json_data["Options"].items():
                report_info("Parsing struct '" + struct_name + "' ...")
                
                for option_name, option in options.items():
                    report_info("Parsing option '" + struct_name + "." + option_name + "' ...")
                    opt = Option()
                    opt.name = option_name

                    for key, value in option.items():
                        report_info("Parsing '" + key + "' = '" + str(value) + "'")
                        if not hasattr(opt, key) and not key in optional:
                            raise KeyError("invalid key: '" + key + "' of option '" + \
                                           option_name + "'")                        
                        self.substitute_and_setattr(opt, key, option)

                    # Check the default value if we have a range of given options
                    if hasattr(opt, "value_allowed"):
                        if not opt.value_default in opt.value_allowed:
                            raise RuntimeError("option '" + option_name + "': inconsistent default"\
                                               + " value '" + opt.value_default + "' not in " + \
                                               "allowed values '" + str(opt.value_allowed) + "'")

                        if opt.cl and not opt.cl_metavar:
                            opt.cl_metavar = opt.value_allowed.replace(",", "|")

                    # Add the check_fun
                    prefix = "[](const auto& value) -> bool "
                    if not opt.check_fun and not hasattr(opt, "value_allowed"):
                        opt.check_fun = prefix + "{ (void)value; return true; }"
                    else:
                        # Check if the option is in the range of the allowed values
                        if hasattr(opt, "value_allowed"):
                            opt.check_fun = prefix + "{ std::vector<" + opt.value_type+ "> allowed{" 
                            for v in opt.value_allowed.split(","):
                                opt.check_fun += self.get_value_str(opt.value_type, v) + ", "
                            opt.check_fun = opt.check_fun[:-2]
                            opt.check_fun += "}; return (std::find(allowed.begin(), allowed.end(),"\
                                             " value) != allowed.end()); }"
                        else:
                            opt.check_fun = prefix + "{" + opt.check_fun + "}"

                    self.add_option(struct_name, opt)
                    report_info("Done parsing option '" + struct_name + "." + option_name + "' ...")

                report_info("Done parsing struct '" + struct_name + "'")

        except (KeyError, RuntimeError) as e :
            report_fatal_error("invalid JSON file " + path + ": " + str(e))

        report_info("Done parsing " + path)
    
    def get_value_str(self, value_type, value):
        """ Get a valid string interpretation of the value """
        if value_type == "std::string":
            return "\"%s\"" % str(value)
        if value_type == "bool":
            return str(value).lower()
        else:
            return str(value)

    def generate_cxx_header_option_def(self):
        """ Generate the C++ header containing the options """
        cxx_header_option_def = ""
        for struct_name, options in self.options.items():  
            cxx_header_option_def += "  /// @brief " + struct_name + " specific options\n"      
            cxx_header_option_def += "  struct " + struct_name + "Type {\n"
            for opt in options:
                cxx_header_option_def += "\n"
                if opt.doc:
                    cxx_header_option_def += "    /// @brief " + opt.doc + "\n"
                cxx_header_option_def += "    " + opt.value_type + " " + opt.name + " = " + \
                                         self.get_value_str(opt.value_type, opt.value_default) + \
                                         ";\n"
            cxx_header_option_def += "  } " + struct_name + ";\n\n"
        return cxx_header_option_def

    def generate_cxx_include_option_def(self):
        cxx_inc_option_def = ""

        empty_str = "\"\""
        signature = "(Structure, Name, Type, DefaultValue, CheckFun, Doc, CommandLine, " \
                    "CommandLineShort, CommandLineMetaVar)"

        for struct_name, options in self.options.items(): 
            cxx_inc_option_def += "#ifndef OPT_" + struct_name.upper() + "\n"
            cxx_inc_option_def += "#define OPT_" + struct_name.upper() + signature + " OPT" + \
                                  signature + "\n"
            cxx_inc_option_def += "#endif \n\n"

        for struct_name, options in self.options.items(): 
            for opt in options:
                cxx_inc_option_def += "OPT_" + struct_name.upper() + "(" + struct_name + ", " + \
                                       opt.name + ", " + opt.value_type + ", " + \
                                       self.get_value_str(opt.value_type, opt.value_default) + \
                                       ", (" + opt.check_fun + ")"
                cxx_inc_option_def += ", " + (empty_str if not opt.doc else "\"" + opt.doc + "\"")
                cxx_inc_option_def += ", " + (empty_str if not opt.cl else "\"" + opt.cl + "\"")
                cxx_inc_option_def += ", " + (empty_str if not opt.cl_short else "\"" + \
                                      opt.cl_short + "\"")
                cxx_inc_option_def += ", " + (empty_str if not opt.cl_metavar else "\"" + \
                                      opt.cl_metavar + "\"")
                cxx_inc_option_def += ")\n"
            cxx_inc_option_def += "\n"

        return cxx_inc_option_def

    def generate_file_impl(self, is_header, input_file, output_file):
        mode = "header" if is_header else "include"
        replacement = "${SEQUOIA_DECL_HEADER_OPTIONS}" if is_header else \
                      "${SEQUOIA_DECL_INCLUDE_OPTIONS}"
        replacement_str = self.generate_cxx_header_option_def() if is_header else \
                          self.generate_cxx_include_option_def()

        if not path.exists(input_file):
            report_fatal_error(mode + "file '" + input_file + "' does not exists")

        replaced = False
        new_content = "" 
        with open(input_file, 'r') as input_header:
            report_info("Reading input " + mode + " file: '" + input_file + "'")
            linenumber = 1      
            for line in input_header.readlines():
                if replacement in line:
                    report_info("Replacing line " + str(linenumber))
                    new_content += replacement_str
                    replaced = True
                else:
                    new_content += line
                linenumber += 1
        
        if not replaced:
            report_fatal_error("couldn't find " + replacement + " in '" + input_file + "'")        

        with open(output_file, 'w') as new_file:
            report_info("Writing output " + mode + " file: '" + output_file + "'")
            new_file.write(new_content)
        report_info("Done with " + mode + " file")

    def generate_files(self, input_header_file, input_include_file, 
                             output_header_file, output_include_file):
        self.generate_file_impl(True, input_header_file, output_header_file)
        self.generate_file_impl(False, input_include_file, output_include_file)
        
def main():
    parser = ArgumentParser("opt-gen.py input", 
                            description="Generate a C++ header (*.h) and include (*.inc) file"
                                        " from the given input options (specified in JSON). "
                                        "The input header and include file need to exist and" 
                                        " contain a ${SEQUOIA_DECL_HEADER_OPTIONS} and "
                                        " ${SEQUOIA_DECL_INCLUDE_OPTIONS} entry point respectivly.")
    parser.add_argument("-v", "--verbose", dest="verbose", action='store_true',
                        help="enable verbose logging")
    parser.add_argument('option_file', nargs='+', help="input option file (*.json)")
    parser.add_argument("--input-header", dest="input_header_file",
                        help="path to the input header file", 
                        metavar="PATH")
    parser.add_argument("--input-include", dest="input_include_file",
                        help="path to the input include file", 
                        metavar="PATH")
    parser.add_argument("--output-header", dest="output_header_file",
                        help="path to the output header file (*.h)", 
                        metavar="PATH")
    parser.add_argument("--output-include", dest="output_include_file",
                        help="path to the output include file (*.inc)", 
                        metavar="PATH")
    args = parser.parse_args()

    global g_program 
    g_program = argv[0]
    global g_verbose 
    g_verbose = args.verbose
    
    has_arg = lambda attr, option: None if getattr(args, attr) else \
                                   report_fatal_error("argument '%s' is required" % option) 
    has_arg("input_header_file", "--input-header")
    has_arg("input_include_file", "--input-include")
    has_arg("output_header_file", "--output-header")
    has_arg("output_include_file", "--output-include")

    opt_list = OptionList()
    for opt_file in args.option_file:
        opt_list.parse_json_file(opt_file)
    opt_list.generate_files(args.input_header_file, 
                            args.input_include_file,
                            args.output_header_file,
                            args.output_include_file)

if __name__ == '__main__':
    main()
