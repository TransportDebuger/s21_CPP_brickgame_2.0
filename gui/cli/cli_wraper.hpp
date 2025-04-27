#pragma once

#include "../view/view.hpp"
#include "cli_presenter.h"
namespace s21 {

class ConsoleViewWrapper : public s21::ViewInterface {
 public:
  ConsoleViewWrapper();
  ~ConsoleViewWrapper() override;

  void addElement(int type, int top, int left, int width, int height,
                  char* label) override;
  void deleteElement(int index) override;
  void refreshElemenet(int index, int datatype, void* data) override;
  void render() override;

 private:
  ConsoleView_t* consoleView;
};

}  // namespace s21