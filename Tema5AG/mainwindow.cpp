#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    setFixedSize(900, 700);
    graph.readMap();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    auto scaledNodes = graph.GetScaledNodes();

    if (graph.GetScaledNodes().isEmpty() || graph.GetEdges().isEmpty()) {
        qDebug() << "Harta nu este incarcata sau nu exista date suficiente pentru desenare.";
        return;
    }

    painter.setPen(Qt::magenta);
    for (const auto &edge : graph.GetEdges()) {
        if (edge.first >= 0 && edge.first < scaledNodes.size() &&
            edge.second >= 0 && edge.second < scaledNodes.size()) {
            painter.drawLine(scaledNodes[edge.first], scaledNodes[edge.second]);
        } else {
            qWarning() << "Muchie invalida:" << edge;
        }
    }

    painter.setBrush(Qt::white);
    for (const auto &node : scaledNodes) {
        painter.drawEllipse(node, 1, 1);
    }

    painter.setBrush(Qt::red);
    for (int nodeIndex : selectedNodes) {
        if (nodeIndex >= 0 && nodeIndex < scaledNodes.size()) {
            painter.drawEllipse(scaledNodes[nodeIndex], 2, 2);
        } else {
            qWarning() << "Nod selectat invalid:" << nodeIndex;
        }
    }

    if (!shortestPath.isEmpty()) {
        painter.setPen(QPen(Qt::green, 2));
        for (int i = 0; i < shortestPath.size() - 1; ++i) {
            if (shortestPath[i] >= 0 && shortestPath[i] < scaledNodes.size() &&
                shortestPath[i + 1] >= 0 && shortestPath[i + 1] < scaledNodes.size()) {
                QPointF point1 = scaledNodes[shortestPath[i]];
                QPointF point2 = scaledNodes[shortestPath[i + 1]];
                painter.drawLine(point1, point2);
            } else {
                qWarning() << "Drum minim invalid intre nodurile:" << shortestPath[i] << "si" << shortestPath[i + 1];
            }
        }
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    QPointF clickPosition = event->pos();
    int closestNode = graph.findClosestNode(clickPosition);

    if (closestNode == -1) {
        qDebug() << "Nu exista niciun nod aproape de locatia selectata.";
        return;
    }

    selectedNodes.append(closestNode);
    clickPoints.append(clickPosition);

    QFile minRoad("MinimumRoad.txt");
    if (!minRoad.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning("Nu s-a putut deschide fisierul MinimumRoad.txt pentru scriere!");
        return;
    }
    QTextStream nodeList(&minRoad);

    if (selectedNodes.size() == 2) {
        int source = selectedNodes[0];
        int target = selectedNodes[1];
        
        qDebug() << "Nod start:" << source;
        qDebug() << "Nod destinatie:" << target;
        shortestPath = graph.dijkstra(source, target);

        if (!shortestPath.isEmpty()) {
            nodeList << "Drumul minim intre " << source << " si " << target << ":\n";
            for (int node : shortestPath) {
                nodeList << node << " ";
            }
            nodeList << "\n";
        } else {
            qDebug() << "Nu exista drum intre cele doua noduri selectate.";
        }

        selectedNodes.clear();
        clickPoints.clear();
    }

    update();
}
