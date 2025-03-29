#include "graph.h"

void Graph::readMap() {

    if (isMapLoaded) {
        qDebug() << "Fisierul XML a fost deja citit. Sarim peste aceasta operatiune.";
        return;
    }

    QFile file("Harta_Luxemburg.xml");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning("Nu s-a putut deschide fisierul!");
        return;
    }

    QXmlStreamReader xml(&file);
    double normalizationFactor = 10000.0;

    while (!xml.atEnd()) {
        xml.readNext();
        if (xml.isStartElement()) {
            if (xml.name() == QStringLiteral("node")) {
                double latitude = xml.attributes().value("latitude").toDouble();
                double longitude = xml.attributes().value("longitude").toDouble();
                latitude /= normalizationFactor;
                longitude /= normalizationFactor;
                scaledNodes.append(QPointF(latitude, longitude));
            } else if (xml.name() == QStringLiteral("arc")) {
                int source = xml.attributes().value("from").toInt();
                int target = xml.attributes().value("to").toInt();
                double cost = xml.attributes().value("length").toDouble();
                edges.append(qMakePair(source, target));
                costs.append(cost);
            }
        }
    }

    if (xml.hasError()) {
        qWarning("Eroare la parsarea fisierului XML!");
    } else {
        qDebug() << "Fisier XML citit cu succes!";
    }

    qDebug() << "Numar total de noduri:" << scaledNodes.size();
    qDebug() << "Numar total de muchii:" << edges.size();

    double minLat = 1e9, minLon = 1e9, maxLat = -1e9, maxLon = -1e9;
    for (const auto &node : scaledNodes) {
        minLat = std::min(minLat, node.x());
        minLon = std::min(minLon, node.y());
        maxLat = std::max(maxLat, node.x());
        maxLon = std::max(maxLon, node.y());
    }

    int WINDOW_WIDTH = 900;
    int WINDOW_HEIGHT = 700;

    double scaleX = (WINDOW_WIDTH * 1.0) / (maxLon - minLon);
    double scaleY = (WINDOW_HEIGHT * 1.0) / (maxLat - minLat);
    double scale = std::min(scaleX, scaleY);

    double mapCenterLat = (minLat + maxLat) / 2.0;
    double mapCenterLon = (minLon + maxLon) / 2.0;

    double offsetX = WINDOW_WIDTH / 2.0 - (mapCenterLon - minLon) * scale;
    double offsetY = WINDOW_HEIGHT / 2.0 - (mapCenterLat - minLat) * scale;

    QVector<QPointF> tempScaledNodes;
    for (const auto &node : scaledNodes) {
        tempScaledNodes.append(QPointF((node.y() - minLon) * scale + offsetX, (maxLat - node.x()) * scale + offsetY));
    }
    scaledNodes = tempScaledNodes;
    isMapLoaded = true;
    file.close();
}

int Graph::findClosestNode(const QPointF &clickPosition) {
    int closestNode = -1;
    double minDistanceSquared = std::numeric_limits<double>::max();

    qDebug() << "Pozitia selectata (coordonate):" << clickPosition;
    for (int i = 0; i < scaledNodes.size(); ++i) {
        const QPointF &node = scaledNodes[i];

        double dx = node.x() - clickPosition.x();
        double dy = node.y() - clickPosition.y();
        double distanceSquared = dx * dx + dy * dy;

        if (distanceSquared < minDistanceSquared) {
            minDistanceSquared = distanceSquared;
            closestNode = i;
        }
    }
    if (closestNode == -1) {
        qDebug() << "Nu exista niciun nod aproape de locatia selectata.";
    } else {
        qDebug() << "Cel mai apropiat nod este:" << closestNode << "( distanta patrata:" << minDistanceSquared <<" )";
    }

    return closestNode;
}

QVector<int> Graph::dijkstra(int source, int target) {
    QVector<double> dist(scaledNodes.size(), std::numeric_limits<double>::max());
    QVector<int> parent(scaledNodes.size(), -1);
    dist[source] = 0;

    using Node = QPair<double, int>;
    std::priority_queue<Node, QVector<Node>, std::greater<Node>> priorityQueue;
    priorityQueue.push(qMakePair(0.0, source));

    while (!priorityQueue.empty()) {
        Node current = priorityQueue.top();
        priorityQueue.pop();
        int currentNode = current.second;
        double currentDist = current.first;

        if (currentNode == target)
            break;

        if (currentDist > dist[currentNode])
            continue;

        for (int i = 0; i < edges.size(); ++i) {
            if (edges[i].first == currentNode) {
                int v = edges[i].second;
                double weight = costs[i];

                if (dist[currentNode] + weight < dist[v]) {
                    dist[v] = dist[currentNode] + weight;
                    parent[v] = currentNode;
                    priorityQueue.push(qMakePair(dist[v], v));
                }
            }
        }
    }

    if (dist[target] == std::numeric_limits<double>::max()) {
        return QVector<int>();
    }

    QVector<int> path;
    int v = target;
    while (v != -1) {
        path.prepend(v);
        v = parent[v];
    }

    return path;
}

QVector<QPointF> Graph::GetScaledNodes() const {
    return scaledNodes;
}

QVector<QPair<int, int> > Graph::GetEdges() const {
    return edges;
}

QVector<double> Graph::GetCosts() const {
    return costs;
}
