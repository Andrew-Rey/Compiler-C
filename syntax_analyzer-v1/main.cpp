#include "LexInterface.h"
#include "SynInterface.h"

int main() {
    setbuf(stdout, nullptr);
    const std::string file_in = DATA_DIR "/demo/demo_parse_1.c-";
    const std::string lr1_file = LR_DIR "/output/LR1Table.txt";
    const std::string first_file = LR_DIR "/output/FirstSet.txt";

    generateSyntax();
    printProductions();

    lexAnalyzer(file_in);
    printSymbolTable();

    /**
     * the first run needed to do:
     * generate first set and lr1_table
     */
    everyTokenFirst();
    printFirstSet();
    generateLRTable();
    printLR1Table();

    /**
     * if there is already a lr1_table in MAIN MEMORY
     * then use this parser
     */
//    parse();

    /**
     * if there is already a lr1_table in DISK (FILE)
     * then use this parser
     */
//    parseOffLine(lr1_file);

    return 0;
}