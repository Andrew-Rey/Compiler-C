#include "LexInterface.h"
#include "SynInterface.h"

int main() {
    setbuf(stdout, 0);
    const std::string file_in = DATA_DIR "/demo/demo_parse_1.c-";
    generateSyntax();
    lexAnalyzer(file_in);
    printProductions();
    printSymbolTable();

//    int i = 0;
//    std::vector<int> temp = {1, 2, 3, 4, 5, 6};
//    for (auto it: temp) {
//        i += 1;
//        temp.push_back(0);
//        cout << it << " ";
//        if (i == 6) { break; }
//    }
//    cout << endl;
    return 0;
}