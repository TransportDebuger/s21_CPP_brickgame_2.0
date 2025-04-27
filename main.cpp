#include "./controller/gamectrl.hpp"
#include <unistd.h>

int main() {
    s21::GameController controller;
    return controller.run();
}