#include "cli_wraper.h"

s21::ConsoleViewWrapper::ConsoleViewWrapper() { initView(this->consoleView); }

s21::ConsoleViewWrapper::~ConsoleViewWrapper() { 
        freeView(this->consoleView);
}

void s21::ConsoleViewWrapper::addElement(int type, int top, int left, int width,
                                         int height, char* label) {
  appendViewElement(this->consoleView, type, top, left, height, width, label);
}

void s21::ConsoleViewWrapper::deleteElement(int index) {
  deleteViewElement(this->consoleView, index);
}

void s21::ConsoleViewWrapper::render() { renderView(this->consoleView); }