#include "graph.h"

Graph::Graph() : oriented(false) {}

Graph::~Graph() {
    for (Node* node : nodes) {
        delete node;
    }
    for (Edge* edge : edges) {
        delete edge;
    }
}

void Graph::addNode(const QPoint& pos) {
    nodes.push_back(new Node(nodes.size(), pos));
}

Edge* Graph::addEdge(Node* first, Node* second, bool isOriented, int cost) {
    for (auto edge : edges) {
        if (edge->getFirst() == first && edge->getSecond() == second) {
            return edge;
        }
    }

    Edge* newEdge = new Edge(first, second, cost);
    edges.push_back(newEdge);
    return newEdge;
}

const std::vector<Node *> &Graph::getNodes() const {
    return nodes;
}

const std::vector<Edge*>& Graph::getEdges() const {
    return edges;
}

std::vector<Edge*>& Graph::getEdges() {
    return edges;
}

void Graph::setOriented(bool value) {
    oriented = value;
}

bool Graph::isGraphOriented() const {
    return oriented;
}

Node* Graph::getNodeByValue(int value) {
    for (Node* node : nodes) {
        if (node->getValue() == value) {
            return node;
        }
    }
    return nullptr;
}

bool Graph::bfs(const std::vector<std::vector<int>>& residualGraph, int source, int sink, std::vector<int>& parent) {
    std::vector<bool> visited(residualGraph.size(), false);
    std::queue<int> q;

    q.push(source);
    visited[source] = true;
    parent[source] = -1;

    while (!q.empty()) {
        int current = q.front();
        q.pop();

        for (int next = 0; next < residualGraph.size(); ++next) {
            if (!visited[next] && residualGraph[current][next] > 0) {
                q.push(next);
                parent[next] = current;
                visited[next] = true;

                if (next == sink) {
                    return true;
                }
            }
        }
    }
    return false;
}


int Graph::fordFulkerson(int source, int sink, std::vector<std::vector<std::vector<int>>>& residualGraphs) {
    std::vector<std::vector<int>> residualGraph = adjacencyMatrix;
    std::vector<int> parent(nodes.size());
    int maxFlow = 0;

    while (bfs(residualGraph, source, sink, parent)) {
        int pathFlow = INT_MAX;
        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];
            pathFlow = std::min(pathFlow, residualGraph[u][v]);
        }

        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];
            residualGraph[u][v] -= pathFlow;
            residualGraph[v][u] += pathFlow;
        }

        residualGraphs.push_back(residualGraph);
        maxFlow += pathFlow;
    }

    return maxFlow;
}

void Graph::saveAdjacencyMatrix() {

    int size = nodes.size();
    adjacencyMatrix.assign(size, std::vector<int>(size, 0));

    for (Edge* edge : edges) {
        int firstIndex = edge->getFirst()->getValue();
        int secondIndex = edge->getSecond()->getValue();
        adjacencyMatrix[firstIndex][secondIndex] = edge->getCost();

        if (!oriented) {
            adjacencyMatrix[secondIndex][firstIndex] = edge->getCost();
        }
    }
}

const std::vector<std::vector<int>>& Graph::getAdjacencyMatrix() const {
    return adjacencyMatrix;
}


std::vector<bool> Graph::findReachableNodesInResidualGraph(const std::vector<std::vector<int>>& residualGraph, int source) {
    std::vector<bool> visited(residualGraph.size(), false);
    std::queue<int> queue;

    queue.push(source);
    visited[source] = true;

    while (!queue.empty()) {
        int current = queue.front();
        queue.pop();

        for (int neightbor = 0; neightbor < residualGraph[current].size(); ++neightbor) {
            if (residualGraph[current][neightbor] > 0 && !visited[neightbor]) {
                visited[neightbor] = true;
                queue.push(neightbor);
            }
        }
    }

    return visited;
}
