#include "LexInterface.h"
#include "SynInterface.h"

int main() {
    setbuf(stdout, nullptr);
    const std::string file_in = DATA_DIR "/demo/demo_parse_1.c-";

    generateSyntax();
    printProductions();

    lexAnalyzer(file_in);
    printSymbolTable();

    everyTokenFirst();
    printFirstSet();

    generateLRTable(productions);
    printLR1Table();

    parse();


    return 0;
}