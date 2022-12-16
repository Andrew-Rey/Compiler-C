#include "LexInterface.h"
#include "SynInterface.h"

int main() {
    setbuf(stdout, 0);
    const std::string file_in = DATA_DIR "/demo/demo_parse_1.c-";
    generateSyntax();
    lexAnalyzer(file_in);
    printProductions();
    printSymbolTable();
    generateLRTable(productions);
    printLR1Table();
    return 0;
}