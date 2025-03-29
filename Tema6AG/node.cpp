#include "node.h"

Node::Node() {}

Node::Node(int value, const QPoint &pos)
    : m_value(value), m_pos(pos) {
}

int Node::getValue() const {
    return m_value;
}

QPoint Node::getPos() const {
    return m_pos;
}

void Node::setPos(const QPoint &pos) {
    m_pos = pos;
}
