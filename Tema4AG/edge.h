#ifndef EDGE_H
#define EDGE_H

class Edge {
public:
    Edge(int first, int second);
    Edge(int first, int second, int cost);
    ~Edge();

    int getFirst() const;
    int getSecond() const;
    int getCost() const;
    void setFirst(int newFirst);
    void setSecond(int newSecond);
    void setCost(int newCost);

    bool operator==(const Edge& other) const {
        return (m_first == other.m_first && m_second == other.m_second);
    }

private:
    int m_first;
    int m_second;
    int m_cost;
};

#endif // EDGE_H
