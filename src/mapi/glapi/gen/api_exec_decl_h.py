# Copyright (C) 2012 Intel Corporation
# Copyright (C) 2021 Advanced Micro Devices, Inc.
#
# Permission is hereby granted, free of charge, to any person obtaining a
# copy of this software and associated documentation files (the "Software"),
# to deal in the Software without restriction, including without limitation
# the rights to use, copy, modify, merge, publish, distribute, sublicense,
# and/or sell copies of the Software, and to permit persons to whom the
# Software is furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice (including the next
# paragraph) shall be included in all copies or substantial portions of the
# Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
# THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
# IN THE SOFTWARE.

# This script generates the file api_exec_decl.h, which contains forward
# declarations of all GL functions prefixed with _mesa_*.

import argparse
import collections
import license
import gl_XML
import sys
import apiexec
import textwrap


class PrintCode(gl_XML.gl_print_base):
    def __init__(self):
        super().__init__()

        self.name = 'api_exec_decl_h.py'
        self.license = license.bsd_license_template % (
            'Copyright (C) 2012 Intel Corporation\n'
            'Copyright (C) 2021 Advanced Micro Devices, Inc.',
            'AUTHORS')

    def printBody(self, api):
        print(textwrap.dedent("""\
            #ifndef API_EXEC_DECL_H
            #define API_EXEC_DECL_H

            #include "util/glheader.h"

            #define GL_API GLAPI
            #define GL_APIENTRY GLAPIENTRY
            #include "GLES/gl.h"

            #ifdef __cplusplus
            extern "C" {
            #endif
            """))

        for f in api.functionIterateAll():
            if f.exec_flavor == 'skip':
                continue

            print('{0} GLAPIENTRY _mesa_{1}({2});'.format(
              f.return_type, f.name, f.get_parameter_string()))
            if f.has_no_error_variant:
                print('{0} GLAPIENTRY _mesa_{1}_no_error({2});'.format(
                  f.return_type, f.name, f.get_parameter_string()))
        print('')
        print('#ifdef __cplusplus')
        print('}')
        print('#endif')
        print('')
        print('#endif')


if __name__ == '__main__':
    apiexec.print_glapi_file(PrintCode())
