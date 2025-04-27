#include "cli_wraper.hpp"

s21::ConsoleViewWrapper::ConsoleViewWrapper() {
  this->consoleView = initView();
  locateView(this->consoleView);
}

s21::ConsoleViewWrapper::~ConsoleViewWrapper() {
  locateView(this->consoleView);
  freeView(this->consoleView);
}

void s21::ConsoleViewWrapper::addElement(int type, int top, int left, int width,
                                         int height, char* label) {
  appendViewElement(this->consoleView, type, top, left, height, width, label);
}

void s21::ConsoleViewWrapper::deleteElement(int index) {
  deleteViewElement(this->consoleView, index);
}

void s21::ConsoleViewWrapper::refreshElemenet(int index, int datatype,
                                            void* data) {
  refreshViewElement(this->consoleView, index, datatype, data);
}

void s21::ConsoleViewWrapper::render() { renderView(this->consoleView); }
