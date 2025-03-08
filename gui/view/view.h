#pragma once

#define FRAME_NAME_1 NULL
#define FRAME_NAME_2 "NEXT"
#define FRAME_NAME_3 "SCORE"
#define FRAME_NAME_4 "TOP"
#define FRAME_NAME_5 "LEVEL"
#define FRAME_NAME_6 "SPEED"
#define FRAME_NAME_7 "HINTS"
#define GET_MNAME(i) FRAME_NAME_##i

enum layoutElements {
  GameFrame,
  NextFigureFrame,
  ScoreFrame,
  TopScoreFrame,
  LevelFrame,
  SpeedFrame,
  PauseStatusFrame
};

namespace s21 {

/*!
    \brief Абстрактный класс графического представления игр BrickGame
    \details Данный класс необходим для обеспечения взаимодействия с
   контроллером игры. Класс обеспечивает независимость от реализации конкретного
   представления и предоставляет возможность не реализовывать условную
   компиляцию и другие методы при сборке программы, независимо от вида
   представления
*/
class ViewInterface {
 public:
  virtual ~ViewInterface() = default;
  virtual void addElement(int top, int left, int width, int height) = 0;
  virtual void updateElement(int index, int top, int left) = 0;
  virtual void render() = 0;
};

}  // namespace s21