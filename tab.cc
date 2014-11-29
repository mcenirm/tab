
#include "tab.h"

std::istream& file_or_stdin(const std::string& file) {

    if (file.empty())
        return std::cin;

    static std::ifstream ret;

    ret.open(file);

    if (!ret)
        throw std::runtime_error("Could not open input file: " + file);

    return ret;
}        

int main(int argc, char** argv) {

    try {

        if (argc < 2) {
            std::cerr << "Usage: " << argv[0] << " <expression>" << std::endl;
            return 1;
        }

        unsigned int debuglevel = 0;
        std::string program;
        std::string infile;

        for (int i = 1; i < argc; ++i) {
            std::string arg(argv[i]);

            if (arg == "-v") {
                debuglevel = 1;

            } else if (arg == "-vv") {
                debuglevel = 2;

            } else if (arg == "-vvv") {
                debuglevel = 3;

            } else if (arg == "-f") {

                if (i == argc - 1)
                    throw std::runtime_error("The '-f' command line argument expects a filename argument.");

                ++i;
                infile = argv[i];
                
            } else {

                if (program.size() > 0) {
                    program += ' ';
                }

                program += arg;
            }
        }
        
        register_functions();

        std::vector<Command> commands;
        TypeRuntime typer;

        Type finaltype = parse(program.begin(), program.end(), typer, commands, debuglevel);

        execute(commands, finaltype, typer.num_vars(), file_or_stdin(infile));
        
    } catch (std::exception& e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
        return 1;

    } catch (...) {
        std::cerr << "UNKNOWN ERROR." << std::endl;
        return 1;
    }

    return 0;
}
