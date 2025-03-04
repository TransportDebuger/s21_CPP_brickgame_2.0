#include "gamectrl.hpp"

void s21::initController() {
    initPresentation();
    // Draw gameboard layout.
    printFrame();
}

void s21::freeController() {
    freePresentation();
}