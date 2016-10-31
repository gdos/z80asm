//-----------------------------------------------------------------------------
// z80asm
// Copyright (c) Paulo Custodio, 2015-2016
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#include <iostream>

#include "icode.h"
#include "error.h"
#include "source.h"
#include "scanner.h"
#include "filesystem/path.h"
#include "z80asm_config.h"

bool assemble(const std::string& input_filename) {
    // open source
    SourceStack source;
    if (!source.open(input_filename)) {
        fatal_open_file(input_filename);
        return false;
    }

    // create object
    filesystem::path input_path(input_filename);
    filesystem::path obj_path = input_path.replace_extension(OBJ_EXT);
    ObjectFile obj(obj_path.str());

    // create scanner
    Scanner scan(&source, &obj);
    if (!scan.parse())
        return false;

    // assembly OK, generate object file
    obj.write();

    return true;
}

int main(int argc, char* argv[])
{
    bool ok = true;

    cout_version();

    if (argc == 1)
        cout_usage();
    else {
        for (int i = 1; i < argc; ++i) {
            if (!assemble(argv[i]))
                ok= false;
        }
    }
    return ok ? 0 : 1;
}
