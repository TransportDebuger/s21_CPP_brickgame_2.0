#pragma once

#include "../view/view.h"
#include <QWidget>

typedef struct QtElement_t {
    int top;
    int left;
    int width;
    int height;
    bool isDirty;
} QtElement_t;

class QtView : public QWidget, public ViewInterface {
    Q_OBJECT

public:
    QtView(QWidget *parent = nullptr);
    void addElement(int top, int left, int width, int height) override;
    void updateElement(int index, int top, int left) override;
    void render() override;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QtElement_t* elements
    std::vector<QtElement> elements;
};