#include "pch.h"
#include "Instruction.h"
#include "Archive.h"

void PrintHelp() {
    printf(
        "Format: COMMAND ARGS...\n"
        "   COMMAND can be:\n"
        "       l ARCHIVE OUTPUT - Decompiles the archive.\n"
        //"       x \n"
    );
}

int wmain(int argc, wchar_t* argv[]) {
    if (argc < 2) {
        PrintHelp();
        return 0;
    }

    switch (*argv[1]) {
    case L'l':
    {
        if (argc != 4) {
            PrintHelp();
            break;
        }

        InstructionDataManager* instrListManager = new InstructionDataManager;

        Archive anm;
        anm.Load(argv[2]);
        anm.Write(argv[3]);

        delete instrListManager;
        break;
    }
    default:
        PrintHelp();
    }

    return 0;
}
