#pragma once

#ifdef __CLI
    #include "libbgamecli.h"
#else
    #include "libbgamedsk.h"
#endif

namespace s21 {
    void initController();
    void freeController();
}

#endif