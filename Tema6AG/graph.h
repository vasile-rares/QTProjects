#ifndef GRAPH_H
#define GRAPH_H

#include "node.h"
#include "edge.h"
#include <vector>
#include <queue>
#include <fstream>
#include <QDebug>
#include <QPoint>
#include <QTimer>


class MainWindow;
class Graph {
public:
    Graph();
    ~Graph();

    void addNode(const QPoint& pos);
    Edge* addEdge(Node* first, Node* second, bool isOriented, int cost);
    const std::vector<Node*>& getNodes() const;
    const std::vector<Edge*>& getEdges() const;
    std::vector<Edge*>& getEdges();
    void setOriented(bool value);
    bool isGraphOriented() const;
    void saveAdjacencyMatrix();
    const std::vector<std::vector<int>>& getAdjacencyMatrix() const;
    Node* getNodeByValue(int value);


    bool bfs(const std::vector<std::vector<int>>& residualGraph, int source, int sink, std::vector<int>& parent);
    int fordFulkerson(int source, int sink, std::vector<std::vector<std::vector<int>>>& residualGraphs);
    std::vector<bool> findReachableNodesInResidualGraph(const std::vector<std::vector<int>>& residualGraph, int source);
private:
    std::vector<Node*> nodes;
    std::vector<Edge*> edges;
    std::vector<std::vector<int>> adjacencyMatrix;
    bool oriented;
};

#endif // GRAPH_H
