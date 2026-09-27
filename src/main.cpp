#include "common/cli.hpp"

int main(int argc, char** argv) {
    securebootx::CLI cli;
    return cli.run(argc, argv);
}
