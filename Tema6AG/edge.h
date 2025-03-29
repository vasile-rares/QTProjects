#ifndef EDGE_H
#define EDGE_H

#include "node.h"

class Edge {
public:
    Edge();
    Edge(Node* first, Node* second, int cost);
    Node* getFirst() const;
    Node* getSecond() const;
    int getCost() const;
    void setCost(int cost);
    void setHighlight(bool highlight);

    bool isHighlighted() const;
private:
    Node* m_first;
    Node* m_second;
    int m_cost;
    bool highlighted;
};

#endif // EDGE_H
