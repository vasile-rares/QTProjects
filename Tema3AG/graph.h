#ifndef GRAPH_H
#define GRAPH_H

#include <QVector>
#include "node.h"
#include "edge.h"

class Graph {
public:
    Graph();
    ~Graph();

    void addNode(QPoint pos);
    void addEdge(int first, int second);
    void setGraphOrientation(bool oriented);
    bool isDuplicateEdge(const Edge& newEdge) const;
    bool isNodeOverlapping(const QPoint& pos) const;
    void saveAdjacencyMatrix();
    int getNodeAtPosition(const QPoint &pos) const;
    QVector<Node>& getNodes();
    const QVector<Node>& getNodes() const;
    const QVector<Edge>& getEdges() const;
    QVector<QVector<int>> findConnectedComponents();
    bool isOriented() const;
    QVector<int> topologicalSort(bool& hasCycle);
    void saveTopologicalSortToFile(const QVector<int>& sortedNodes);

private:
    QVector<Node> nodes;
    QVector<Edge> edges;
    bool oriented = true;
};

#endif // GRAPH_H
