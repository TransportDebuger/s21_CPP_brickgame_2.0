/**
 * @file gamectrl.hpp
 * @brief Реализация контролера игр Brick Game
 * @author provemet
 * @version 1.0
 * @date Март 2025
 *
 * @details Этот модуль реализует контроллер Brick Game
 * 
 * @todo Интегрировать представление (View);
 */

#pragma once

#ifdef SNAKE
#include "../brick_game/snake/snake.h"
#else
#include "../brick_game/tetris/tetris.h"
#endif

namespace s21 {

    class GameController {
        public:
            int run();            
        private:
            void initialize();
            int mainLoop();
            void handleInput();
            void cleanup();

            FiniteStateMachine* fsm = nullptr;
            GameInfo_t* gameInfo = nullptr;
    }

};

#endif