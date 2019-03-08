#ifndef EGALITO_APP_SANDBOX_H
#define EGALITO_APP_SANDBOX_H

#include "conductor/interface.h"

class HardenApp {
private:
    bool quiet;
    EgalitoInterface *egalito;
public:
    void run(int argc, char **argv);
    void parse(const std::string &filename, bool oneToOne);
    void generate(const std::string &filename, bool oneToOne);
    Program *getProgram() const { return egalito->getProgram(); }
private:
    void doCFI();
    void doShadowStack(bool gsMode);
    void doPermuteData();
};

#endif
