#include "edge.h"

Edge::Edge() {}

Edge::Edge(Node *first, Node *second, int cost = 1)
    : m_first(first), m_second(second), m_cost(cost), highlighted(false) {
}

Node *Edge::getFirst() const {
    return m_first;
}

Node *Edge::getSecond() const {
    return m_second;
}

int Edge::getCost() const {
    return m_cost;
}

void Edge::setCost(int cost) {
    m_cost = cost;
}

void Edge::setHighlight(bool highlight) {
    this->highlighted = highlight;
}

bool Edge::isHighlighted() const {
    return highlighted;
}
