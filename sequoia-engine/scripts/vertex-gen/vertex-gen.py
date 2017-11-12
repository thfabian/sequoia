#!/usr/bin/python3
# -*- coding: utf-8 -*-
##===-----------------------------------------------------------------------------*- Python -*-===##
##                         _____                        _
##                        / ____|                      (_)
##                       | (___   ___  __ _ _   _  ___  _  __ _
##                        \___ \ / _ \/ _` | | | |/ _ \| |/ _` |
##                        ____) |  __/ (_| | |_| | (_) | | (_| |
##                       |_____/ \___|\__, |\__,_|\___/|_|\__,_| - Game Engine (2016-2017)
##                                       | |
##                                       |_|
##
## This file is distributed under the MIT License (MIT).
## See LICENSE.txt for details.
##
##===------------------------------------------------------------------------------------------===##
##
## This script parses the vertex description file and converts it to C++ code.
## 
## The json file needs to have the following format:
##
## {
##   "Vertices" : {
##     "<VERTEX_NAME>" : [
##       {
##         "<ATTRIBUTE>" : {
##           "Type" : "<TYPE>",
##           "NumElements" : <NUM_ELEMENTS>,
##           "Normalized": <NORMALIZED>
##         }
##       }
##     ]
##   }
## }
##
## with:
##  <VERTEX_NAME>   : string    = Name of the vertex struct
##  <ATTRIBUTE>     : string    = Attribute, one of {"Position", "Normal", "TexCoord", "Color"}
##  <TYPE>          : string    = Type of the data, one of {"Float32", "UInt8"}
##  <NUM_ELEMENTS>  : integer   = Number of elements i.e array size
##  <NORMALIZED>    : boolean   = specifies whether fixed-point data values should be normalized
##                                (true) or converted directly as fixed-point values (false) when
##                                they are accessed.
##
##===------------------------------------------------------------------------------------------===##

from argparse import ArgumentParser
from enum import Enum
from json import load
from os import path
from sys import exit, stderr, argv

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


class CXXType(Enum):
    """ C++ types """
    INVALID = 0

    FLOAT32 = 1 << 1
    UINT8 = 1 << 2


def CXXType_to_string(type):
    if type == CXXType.FLOAT32:
        return "float"
    elif type == CXXType.UINT8:
        return "std::uint8_t"
    else:
        report_fatal_error("cannot convert \"%s\" to C++ type" % type)


def CXXType_to_enum(type):
    if type == CXXType.FLOAT32:
        return "VertexLayout::Float32"
    elif type == CXXType.UINT8:
        return "VertexLayout::UInt8"
    else:
        report_fatal_error("cannot convert \"%s\" to VertexLayout::Type" % type)


def CXXType_sizeof(type):
    if type == CXXType.FLOAT32:
        return 4
    elif type == CXXType.UINT8:
        return 1
    else:
        report_fatal_error("cannot sizeof \"%s\"" % type)


class VertexCodeGen(object):
    def __init__(self):
        self.vertices = {}

    def generate(self, vertices_file, input_header_file, input_cpp_file, output_header_file,
                 output_cpp_file):
        """ Parse the given vertex JSON file, validate it and convert to C++ code """
        with open(vertices_file) as json_file:
            report_info("Parsing file " + vertices_file + " ...")
            json_data = load(json_file)

        for vertex_name, json_attributes in json_data["Vertices"].items():
            vertex_attributes = []
            offset = 0

            for json_attribute in json_attributes:
                for attribute_name, attribute_data in json_attribute.items():
                    cxx_type = CXXType.INVALID
                    type = attribute_data["Type"]
                    if type == "Float32":
                        cxx_type = CXXType.FLOAT32
                    elif type == "UInt8":
                        cxx_type = CXXType.UINT8
                    else:
                        report_fatal_error(
                            "invalid type \"%s\" of vertex attribute %s in vertex %s" % (
                                type,
                                attribute_name,
                                vertex_name))

                    num_elements = attribute_data["NumElements"]
                    normalized = attribute_data["Normalized"]

                    vertex_attribute = {}
                    vertex_attribute["Type"] = cxx_type
                    vertex_attribute["NumElements"] = num_elements
                    vertex_attribute["Offset"] = offset
                    vertex_attribute["Normalized"] = normalized

                    vertex_attributes += [{attribute_name: vertex_attribute}]
                    offset += CXXType_sizeof(cxx_type) * num_elements

                self.vertices[vertex_name] = vertex_attributes

        self.generate_file_impl(True, input_header_file, output_header_file)
        self.generate_file_impl(False, input_cpp_file, output_cpp_file)

    def generate_file_impl(self, is_header, input_file, output_file):
        mode = "header" if is_header else "translation unit"
        replacement = "${SEQUOIA_DECL_HEADER_VERTICES}" if is_header else \
            "${SEQUOIA_DECL_CPP_VERTICES}"
        replacement_str = self.generate_cxx_header_defs() if is_header else \
            self.generate_cxx_cpp_defs()

        if not path.exists(input_file):
            report_fatal_error(mode + " file '" + input_file + "' does not exists")

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

    def generate_cxx_header_defs(self):
        """ Generate the C++ header """
        cxx_header_def = "namespace sequoia {\n\n"
        cxx_header_def += "namespace render {\n\n"

        # Generate vertex sequence
        cxx_header_def += "#define SEQUOIA_VERTEX_SEQ "
        for vertex in self.vertices:
            cxx_header_def += "(%s)" % vertex
        cxx_header_def += "\n\n"

        # Generate vertex sequence
        cxx_header_def += "#define SEQUOIA_VERTEX_LAYOUT_SEQ "
        for vertex in self.vertices:
            cxx_header_def += "(%sLayout)" % vertex
        cxx_header_def += "\n\n"

        for vertex_name, attributes in self.vertices.items():
            # Generate Vertex
            cxx_header_def += "/// @brief Representation of %s\n" % (vertex_name)
            cxx_header_def += "/// @ingroup render\n"
            cxx_header_def += "struct SEQUOIA_API %s {\n" % vertex_name
            for attribute in attributes:
                for attribute_name, attribute_data in attribute.items():
                    cxx_header_def += "  %s %s[%i];\n" % (
                        CXXType_to_string(attribute_data["Type"]),
                        attribute_name, attribute_data["NumElements"])
                    cxx_header_def += "  using %sType = %s;\n" % (
                        attribute_name, CXXType_to_string(attribute_data["Type"]))

            cxx_header_def += "\n"
            cxx_header_def += "  /// @brief Get the layout of the vertex\n"
            cxx_header_def += "  /// @remark Thread-safe\n"
            cxx_header_def += "  static const VertexLayout* getLayout() noexcept;\n"

            cxx_header_def += "\n"
            cxx_header_def += "  /// @brief Convert to string\n"
            cxx_header_def += "  static std::string toString(const %s& vertex);\n" % vertex_name

            cxx_header_def += "};\n\n"

            # Generate VertexLayout
            cxx_header_def += "/// @brief Layout description of %s\n" % vertex_name
            cxx_header_def += "/// @ingroup render\n"
            cxx_header_def += "struct SEQUOIA_API %sLayout : public VertexLayout {\n" % vertex_name
            cxx_header_def += "  %sLayout();\n" % vertex_name
            cxx_header_def += "  virtual ~%sLayout() {}\n" % vertex_name
            cxx_header_def += "  using VertexType = %s;\n" % vertex_name
            cxx_header_def += "  virtual void accept(VertexVisitor& visitor) const override;\n"
            cxx_header_def += "};\n\n"

            # Generate operator== and operator !=
            cxx_header_def += "/// @brief Compare %s\n/// @ingroup render\n" % vertex_name
            cxx_header_def += "inline bool operator==(const %s& lhs, const %s& rhs) noexcept {\n" % (
                vertex_name, vertex_name)
            for attribute in attributes:
                for attribute_name, attribute_data in attribute.items():
                    cxx_header_def += "  for(int i = 0; i < %s; ++i)\n" \
                                      "    if(lhs.%s[i] != rhs.%s[i])\n" \
                                      "      return false;\n" % (
                                          attribute_data["NumElements"], attribute_name,
                                          attribute_name)
            cxx_header_def += "  return true;\n"
            cxx_header_def += "}\n\n"

        # Generate tuple of vertex types
        cxx_header_def += "/// @typedef VertexTypeList\n" \
                          "/// @brief List of available vertices\n" \
                          "/// @ingroup render\n"
        cxx_header_def += "using VertexTypeList = std::tuple<"
        n = 0
        for vertex in self.vertices:
            cxx_header_def += "%s%s" % (vertex, "" if n == len(self.vertices) - 1 else ", ")
            n += 1
        cxx_header_def += ">;\n\n"

        cxx_header_def += "} // namespace render\n\n"
        cxx_header_def += "} // namespace sequoia\n\n"

        # Generate std::hash specialization
        for vertex_name, attributes in self.vertices.items():
            cxx_header_def += "SEQUOIA_DECLARE_STD_HASH(sequoia::render::%s, attrib, " % vertex_name
            n = 0
            for attribute in attributes:
                for attribute_name, attribute_data in attribute.items():
                    cxx_header_def += "attrib.%s%s" % (
                        attribute_name, "" if n == len(attributes) - 1 else ", ")
                n += 1
            cxx_header_def += ")\n"

        return cxx_header_def

    def generate_cxx_cpp_defs(self):
        """ Generate the C++ translation unit """
        cxx_cpp_def = "namespace sequoia {\n\n"
        cxx_cpp_def += "namespace render {\n\n"

        for vertex_name, attributes in self.vertices.items():
            # Definition of Vertex::toString
            cxx_cpp_def += "std::string %s::toString(const %s& vertex) {\n" % (
                vertex_name, vertex_name)
            cxx_cpp_def += "  return core::format(\n"
            cxx_cpp_def += "    \"%s[\\n\"\n" % vertex_name
            for i in range(len(attributes)):
                cxx_cpp_def += "    \"   {:<10} = {}%s\\n\"\n" % (
                    "," if i != len(attributes) - 1 else "")
            cxx_cpp_def += "    \"]\",\n"

            n = 0
            for attribute in attributes:
                for attribute_name, attribute_data in attribute.items():
                    cxx_cpp_def += "  \"%s\", arrayToString(vertex.%s)%s\n" % (
                        attribute_name, attribute_name, "," if n != len(attributes) - 1 else ");")
                n += 1

            cxx_cpp_def += "}\n\n"

            # Definition of Vertex::getLayout
            cxx_cpp_def += "namespace {\n" \
                           "std::unique_ptr<%sLayout> %sLayoutInstance = nullptr;\n" \
                           "std::once_flag %sInitFlag;\n" \
                           "}\n\n" % (vertex_name, vertex_name, vertex_name)
            cxx_cpp_def += "const VertexLayout* %s::getLayout() noexcept {\n" % (vertex_name)

            cxx_cpp_def += "  std::call_once(%sInitFlag, [] {\n" % (vertex_name)
            cxx_cpp_def += "    %sLayoutInstance = std::make_unique<%sLayout>();\n" % (
                vertex_name, vertex_name)
            cxx_cpp_def += "  });\n"
            cxx_cpp_def += "  return %sLayoutInstance.get();\n" % (vertex_name)
            cxx_cpp_def += "}\n\n"

            # Definition of VertexLayout constructor
            cxx_cpp_def += "%sLayout::%sLayout() : VertexLayout() {\n" % (vertex_name, vertex_name)
            for attribute in attributes:
                for attribute_name, attribute_data in attribute.items():
                    cxx_cpp_def += "  %s.%s = %s;\n" % (attribute_name, "Normalized",
                                                        "true" if attribute_data[
                                                            "Normalized"] else "false")

                    cxx_cpp_def += "  %s.%s = %s;\n" % (
                        attribute_name, "Type", CXXType_to_enum(attribute_data["Type"]))

                    cxx_cpp_def += "  %s.%s = %s;\n" % (
                        attribute_name, "NumElements", attribute_data["NumElements"])

                    cxx_cpp_def += "  %s.%s = %s;\n" % (
                        attribute_name, "Offset", attribute_data["Offset"])

                cxx_cpp_def += "\n"

            cxx_cpp_def += "  SizeOf = sizeof(%s);\n" % vertex_name
            cxx_cpp_def += "}\n\n"

            # Definition of VertexLayout constructor
            cxx_cpp_def += "void %sLayout::accept(VertexVisitor& visitor) const {\n" \
                           "  visitor.visit(this);\n" \
                           "}\n\n" % (
                               vertex_name)

        cxx_cpp_def += "} // namespace render\n\n"
        cxx_cpp_def += "} // namespace sequoia\n"

        return cxx_cpp_def


def main():
    parser = ArgumentParser("vertex-gen.py input",
                            description="Generate a C++ header (*.h) and translation unit (*.cpp) file"
                                        " from the given vertex specification (given in JSON). "
                                        "The input header and translation unit need to exist and"
                                        " contain a ${SEQUOIA_DECL_HEADER_VERTICES} and"
                                        " ${SEQUOIA_DECL_CPP_VERTICES} entry points respectively."
                                        " Note that neither of them should be declared inside a "
                                        "namespace, the sequoia namespace will be added automatically.")
    parser.add_argument("-v", "--verbose", dest="verbose", action='store_true',
                        help="enable verbose logging")
    parser.add_argument("--vertices", dest="vertices_file",
                        help="path to the vertex specification (*.json)",
                        metavar="PATH")
    parser.add_argument("--input-header", dest="input_header_file",
                        help="path to the input header file (*.h)",
                        metavar="PATH")
    parser.add_argument("--input-cpp", dest="input_cpp_file",
                        help="path to the input translation unit (*.cpp)",
                        metavar="PATH")
    parser.add_argument("--output-header", dest="output_header_file",
                        help="path to the output header file (*.h)",
                        metavar="PATH")
    parser.add_argument("--output-cpp", dest="output_cpp_file",
                        help="path to the output translation unit (*.cpp)",
                        metavar="PATH")
    args = parser.parse_args()

    global g_program
    g_program = argv[0]
    global g_verbose
    g_verbose = args.verbose

    has_arg = lambda attr, option: None if getattr(args, attr) else \
        report_fatal_error("argument '%s' is required" % option)

    has_arg("vertices_file", "--vertices")
    has_arg("input_header_file", "--input-header")
    has_arg("input_cpp_file", "--input-cpp")
    has_arg("output_header_file", "--output-header")
    has_arg("output_cpp_file", "--output-cpp")

    cg = VertexCodeGen()
    cg.generate(args.vertices_file, args.input_header_file, args.input_cpp_file,
                args.output_header_file, args.output_cpp_file)


if __name__ == '__main__':
    main()
