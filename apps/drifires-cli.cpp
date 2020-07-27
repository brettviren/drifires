#include "drifires/functions.hpp"

#include "Garfield/Plotting.hh"

#include "TApplication.h"

#include <iostream>
#include <fstream>


int main(int argc, char* argv[])
{
    std::string cfgfile = "/dev/stdin";
    if (argc > 1) {
        cfgfile = argv[1];
        if (cfgfile == "-") {
            cfgfile = "/dev/stdin";
        }
    }
    std::string outfile = "/dev/stdout";
    if (argc > 2) {
        outfile = argv[2];
        if (outfile == "-") {
            outfile = "/dev/stdout";
        }
    }

    std::ifstream fstr(cfgfile);
    if (!fstr) {
        std::cerr << "faile to open: " << cfgfile << std::endl;
        return -1;
    }
    drifires::object cfg;
    fstr >> cfg;

    // TApplication app("app", &argc, argv);
    // Garfield::plottingEngine.SetDefaultStyle();

    auto res = drifires::process(cfg);
    
    std::ofstream out(outfile);
    out << res;
    
    return 0;
}

