#include "topologicalwindow.h"
#include <QPainter>

TopologicalWindow::TopologicalWindow(const QVector<int>& sortedNodes, const QVector<Edge>& edges, QWidget* parent)
    : QWidget(parent), sortedNodes(sortedNodes), edges(edges) {
    setFixedSize(800, 200);
}

void TopologicalWindow::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    QPainter paint(this);
    paint.setRenderHint(QPainter::Antialiasing);

    QVector<QPoint> positions;

    int nodeRadius = 20;
    int spacing = 100;
    int startX = 50;
    int centerY = height() / 2;

    for (int i = 0; i < sortedNodes.size(); ++i) {
        int x = startX + i * spacing;
        int y = centerY;
        positions.append(QPoint(x, y));

        paint.setBrush(Qt::black);
        paint.setPen(QPen(Qt::white, 2));
        paint.drawEllipse(x - nodeRadius, y - nodeRadius, 2 * nodeRadius, 2 * nodeRadius);

        QRect textRect(x - nodeRadius, y - nodeRadius, 2 * nodeRadius, 2 * nodeRadius);
        paint.drawText(textRect, Qt::AlignCenter, QString::number(sortedNodes[i] + 1));
    }

    paint.setPen(QPen(Qt::white, 2));
    for (const auto& edge : edges) {
        int firstIndex = sortedNodes.indexOf(edge.getFirst());
        int secondIndex = sortedNodes.indexOf(edge.getSecond());

        if (firstIndex != -1 && secondIndex != -1) {
            QPoint start = positions[firstIndex];
            QPoint end = positions[secondIndex];
            paint.drawLine(start, end);
        }
    }
}
