#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <cmath>
#include <queue>
#include <QQueue>
#include <QStack>
#include "Graph.h"

Graph::Graph() {}

Graph::~Graph() {}

QVector<Node>& Graph::getNodes() {
    return nodes;
}

const QVector<Node>& Graph::getNodes() const {
    return nodes;
}

const QVector<Edge>& Graph::getEdges() const {
    return edges;
}

bool Graph::isOriented() const {
    return oriented;
}

bool Graph::isNodeOverlapping(const QPoint& pos) const {
    for (const auto& node : nodes) {
        int dx = node.getPos().x() - pos.x();
        int dy = node.getPos().y() - pos.y();
        if (std::sqrt(dx * dx + dy * dy) < 20) {
            return true;
        }
    }
    return false;
}

bool Graph::isDuplicateEdge(const Edge& newEdge) const {
    for (const auto& edge : edges) {
        if (edge.getFirst() == newEdge.getFirst() && edge.getSecond() == newEdge.getSecond()) {
            return true;
        }
    }
    return false;
}

void Graph::addNode(QPoint pos) {
    if (isNodeOverlapping(pos)) {
        QMessageBox::warning(nullptr, "Eroare", "Nodul se suprapune cu un alt nod existent.");
        return;
    }
    nodes.append(Node(pos, nodes.size()));
    saveAdjacencyMatrix();
}

void Graph::addEdge(int first, int second) {
    Edge newEdge(first, second);
    if (!isDuplicateEdge(newEdge)) {
        edges.append(newEdge);
        if (!oriented) {
            edges.append(Edge(second, first));
        }
        saveAdjacencyMatrix();
    } else {
        QMessageBox::warning(nullptr, "Eroare", "Exista deja acest arc.");
        return;
    }
}

int Graph::getNodeAtPosition(const QPoint &pos) const {
    for (int i = 0; i < nodes.size(); ++i) {
        int dx = nodes[i].getPos().x() - pos.x();
        int dy = nodes[i].getPos().y() - pos.y();
        if (std::sqrt(dx * dx + dy * dy) <= 10) {
            return i;
        }
    }
    return -1;
}

void Graph::setGraphOrientation(bool oriented) {
    this->oriented = oriented;
    saveAdjacencyMatrix();
}

void Graph::saveAdjacencyMatrix() {
    int numNodes = nodes.size();
    QVector<QVector<int>> adjacencyMatrix(numNodes, QVector<int>(numNodes, 0));

    for (const auto& edge : edges) {
        adjacencyMatrix[edge.getFirst()][edge.getSecond()] = 1;
        if (!oriented) {
            adjacencyMatrix[edge.getSecond()][edge.getFirst()] = 1;
        }
    }

    QFile file("matrice_adiacenta.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << numNodes << "\n";
        for (int i = 0; i < numNodes; ++i) {
            for (int j = 0; j < numNodes; ++j) {
                out << adjacencyMatrix[i][j] << " ";
            }
            out << "\n";
        }
        file.close();
    }
}

QVector<QVector<int>> Graph::findConnectedComponents() {
    QVector<bool> visited(nodes.size(), false);
    QVector<QVector<int>> components;

    for (int i = 0; i < nodes.size(); ++i) {
        if (!visited[i]) {
            QVector<int> component;
            std::queue<int> q;
            q.push(i);
            visited[i] = true;

            while (!q.empty()) {
                int current = q.front();
                q.pop();
                component.append(current);

                for (const auto& edge : edges) {
                    int neighbor = -1;
                    if (edge.getFirst() == current) {
                        neighbor = edge.getSecond();
                    } else if (edge.getSecond() == current) {
                        neighbor = edge.getFirst();
                    }

                    if (neighbor != -1 && !visited[neighbor]) {
                        q.push(neighbor);
                        visited[neighbor] = true;
                    }
                }
            }

            components.append(component);
        }
    }
    return components;
}

QVector<int> Graph::topologicalSort(bool& hasCycle) {
    QVector<int> sortedNodes;              // Rezultatul sortării topologice
    QVector<bool> visited(nodes.size(), false); // Marcaj pentru noduri vizitate
    QVector<bool> onStack(nodes.size(), false); // Marcaj pentru noduri aflate pe stivă (detectarea ciclurilor)
    QStack<int> stack;                     // Stivă pentru simulat DFS
    QStack<int> tempStack;                 // Stivă temporară pentru procesare

    hasCycle = false;

    for (int startNode = 0; startNode < nodes.size(); ++startNode) {
        if (!visited[startNode]) {
            stack.push(startNode);

            while (!stack.isEmpty()) {
                int currentNode = stack.top();

                if (!visited[currentNode]) {
                    visited[currentNode] = true;
                    onStack[currentNode] = true;

                    // Adăugăm vecinii în stivă
                    for (const auto& edge : edges) {
                        if (edge.getFirst() == currentNode) {
                            int neighbor = edge.getSecond();

                            if (!visited[neighbor]) {
                                stack.push(neighbor);
                            } else if (onStack[neighbor]) {
                                // Detectăm un ciclu
                                hasCycle = true;
                                return {};
                            }
                        }
                    }
                } else {
                    // Dacă nodul a fost deja procesat, îl scoatem de pe stivă
                    stack.pop();
                    onStack[currentNode] = false;
                    tempStack.push(currentNode); // Adăugăm nodul finalizat în stiva temporară
                }
            }
        }
    }

    // Transferăm nodurile din stiva temporară în rezultatul sortării (în ordine inversă)
    while (!tempStack.isEmpty()) {
        sortedNodes.append(tempStack.pop());
    }

    return sortedNodes;
}

void Graph::saveTopologicalSortToFile(const QVector<int>& sortedNodes) {
    QFile file("sortare_topologica.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        for (int i = 0; i < sortedNodes.size(); ++i) {
            out << sortedNodes[i] + 1;
            if (i < sortedNodes.size() - 1) {
                out << ", ";
            }
        }
        file.close();
    }
}
