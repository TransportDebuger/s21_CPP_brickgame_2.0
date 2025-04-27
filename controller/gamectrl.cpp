#include "gamectrl.hpp"

s21::GameController::GameController() {}

int s21::GameController::run() {
  this->initialize();
  return this->mainLoop();
}

void s21::GameController::initialize() {
    gameInfo = createGameInfo();
    fsm = fsm_create(&gameStates, NUM_STATES, gameInfo);
    fsm_processTrigger(fsm, TRIGGER_INIT);
}

int s21::GameController::mainLoop() { 
    while (fsm->currentState != STATE_TERMINATE) {
        fsm_update(fsm);
        usleep(1000000 / gameInfo->speed);  // Добавлена задержка
    }

    cleanup();

    return EXIT_SUCCESS;
}
// Обработка сигналов нажатых клавиш. Должно орабатываться также "зажатие клавиши".
void s21::GameController::handleInput() {
    userInput(action, hold); //Вызываем обработчик действий пользователя в модели.
}

void s21::GameController::cleanup() {
    if (fsm) {
        fsm_destroy(fsm);
        fsm = nullptr;
    }
    if (gameInfo) {
        destroyGameInfo(gameInfo);
        gameInfo = nullptr;
    }
}
