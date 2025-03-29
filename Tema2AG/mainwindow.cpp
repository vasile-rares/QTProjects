#include "MainWindow.h"
#include <QPainter>
#include <QColor>
#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), labyrinth(new Labyrinth("labyrinth.txt")) {
    setWindowTitle("Tema 2 AG (Labyrinth)");
    setFixedSize(600, 600);
    labyrinth->findShortestPaths();
}

MainWindow::~MainWindow() {
    delete labyrinth;
}

void MainWindow::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event)
    QPainter painter(this);
    drawLabyrinth(painter);
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Space) {
        showPaths = !showPaths;
        update();
    }
}

void MainWindow::drawLabyrinth(QPainter& painter) {
    const auto& matrix = labyrinth->getLabyrinthMatrix();
    const auto& shortestPaths = labyrinth->getShortestPaths();

    int cellWidth = width() / matrix.size();
    int cellHeight = height() / matrix[0].size();

    for (int i = 0; i < matrix.size(); ++i) {
        for (int j = 0; j < matrix[i].size(); ++j) {
            QRect cellRect(j * cellWidth, i * cellHeight, cellWidth, cellHeight);
            if (matrix[i][j] == 0) {
                painter.fillRect(cellRect, Qt::black);
            } else if (matrix[i][j] == 1) {
                painter.fillRect(cellRect, Qt::white);
            } else if (matrix[i][j] == 3) {
                painter.fillRect(cellRect, Qt::blue);
            } else if (matrix[i][j] == 2) {
                painter.fillRect(cellRect, Qt::red);
            }
            painter.setPen(QColor(25,25,25));
            painter.drawRect(cellRect);
        }
    }

    if (showPaths) {
        for (const auto& [x, y] : shortestPaths) {
            if (matrix[x][y] == 1) {
                QRect cellRect(y * cellWidth, x * cellHeight, cellWidth, cellHeight);
                painter.fillRect(cellRect, Qt::green);
                painter.setPen(Qt::darkGray);
                painter.drawRect(cellRect);
            }
        }
    }
}
