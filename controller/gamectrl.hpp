#pragma once

#ifdef __CLI
    #include "cli_presenter.h"
#else

namespace s21 {

    void initController();
    void freeController();

}

#endif