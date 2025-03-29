#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "labyrinth.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Labyrinth* labyrinth;
    void keyPressEvent(QKeyEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void drawLabyrinth(QPainter& painter);
    bool showPaths = false;
};

#endif // MAINWINDOW_H
