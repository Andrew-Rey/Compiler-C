
#include "LexicalAnalyzer.h"
//#include "Analyzer_demo.h"

int main() {
    setbuf(stdout, 0);
    const std::string file_in = DATA_DIR "/demo/demo0.c-";
    const std::string file_out = DATA_DIR "/demo/result.txt";

    lexAnalyzer(file_in, file_out);

//    getNext();
    return 0;
}
