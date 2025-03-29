#ifndef TOPOLOGICALWINDOW_H
#define TOPOLOGICALWINDOW_H

#include <QWidget>
#include "edge.h"

class TopologicalWindow : public QWidget {
    Q_OBJECT

public:
    explicit TopologicalWindow(const QVector<int>& sortedNodes, const QVector<Edge>& edges, QWidget* parent = nullptr);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QVector<int> sortedNodes;
    QVector<Edge> edges;
};

#endif // TOPOLOGICALWINDOW_H
