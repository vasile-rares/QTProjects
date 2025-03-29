#ifndef GRAPH_H
#define GRAPH_H

#include <QVector>
#include <QList>
#include <QPair>
#include <QPointF>
#include <QFile>
#include <QXmlStreamReader>
#include <QDebug>
#include <QSet>
#include <queue>

class Graph
{
public:
    Graph() = default;

    void readMap();
    QVector<int> dijkstra(int source, int target);
    int findClosestNode(const QPointF &clickPosition);

    QVector<QPointF> GetScaledNodes() const;
    QVector<QPair<int, int>> GetEdges() const;
    QVector<double> GetCosts() const;

private:
    QVector<QPointF> scaledNodes;
    QVector<QPair<int, int>> edges;
    QVector<double> costs;
    bool isMapLoaded = false;
};

#endif // GRAPH_H
