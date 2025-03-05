#include "./gui/cli/cli_presenter.h"
//#include "libbgamecli.h"
#include <unistd.h>

int main() {
    initPresentation();
    sleep(5);
    freePresentation();
    return 0;
}