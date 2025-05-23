#include <QMessageBox>
#include <QInputDialog>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "topologicalwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), selectedNode(-1), draggedNode(-1)  {
    ui->setupUi(this);

    setFixedSize(800, 600);
    ui->orientedButton->setChecked(true);

    connect(ui->orientedButton, &QRadioButton::clicked, this, [this]() { graph.setGraphOrientation(true); update(); });
    connect(ui->unorientedButton, &QRadioButton::clicked, this, [this]() { graph.setGraphOrientation(false); update(); });
    connect(ui->toggleComponents, &QCheckBox::checkStateChanged, this, &MainWindow::toggleConnectedComponents);
    connect(ui->topologicalSortButton, &QPushButton::clicked, this, &MainWindow::performTopologicalSort);
    connect(ui->boruvkaButton, &QPushButton::clicked, this, &MainWindow::toggleBoruvkaAlgorithm);
    connect(ui->kruskalButton, &QPushButton::clicked, this, &MainWindow::toggleKruskalAlgorithm);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter paint(this);
    paint.setPen(QPen(Qt::white, 1));

    QVector<Edge> mstEdges;
    if(isDisplayBoruvkaAlgorithm) {
        mstEdges = graph.performBoruvkaAlgorithm();
    }
    if(isDisplayKruskalAlgorithm) {
        mstEdges = graph.performKruskalAlgorithm();
    }

    QSet<QPair<int, int>> uniqueEdges;
    for (const auto& edge : graph.getEdges()) {

        int u = edge.getFirst();
        int v = edge.getSecond();

        if (!graph.isOriented() && u > v)
            std::swap(u, v);

        QPair<int, int> edgePair(u, v);
        if (!graph.isOriented() && uniqueEdges.contains(edgePair)) {
            continue;
        }

        uniqueEdges.insert(edgePair);
        const Node& first = graph.getNodes()[u];
        const Node& second = graph.getNodes()[v];
        QColor edgeColor = mstEdges.contains(edge) ? Qt::green : Qt::white;
        if (isDisplayComponents) {
            int componentIndexFirst = -1;
            int componentIndexSecond = -1;

            for (int i = 0; i < connectedComponents.size(); ++i) {
                if (connectedComponents[i].contains(edge.getFirst())) {
                    componentIndexFirst = i;
                }
                if (connectedComponents[i].contains(edge.getSecond())) {
                    componentIndexSecond = i;
                }
            }

            if (componentIndexFirst == componentIndexSecond && componentIndexFirst != -1) {
                edgeColor = componentColors[componentIndexFirst];
            }
        }

        paint.setPen(QPen(edgeColor, 2));
        paint.drawLine(first.getPos().x(), first.getPos().y(), second.getPos().x(), second.getPos().y());
        if (graph.isOriented()) {
            drawArrow(paint, first, second, edgeColor);
        }

        int midX = (first.getPos().x() + second.getPos().x()) / 2;
        int midY = (first.getPos().y() + second.getPos().y()) / 2;
        paint.setPen(Qt::white);
        paint.setFont(QFont("", 10, QFont::Bold));
        paint.drawText(midX + 5, midY - 5, QString::number(edge.getCost()));
    }

    if(isDisplayComponents)
    {
        for (int i = 0; i < connectedComponents.size(); ++i) {
            QColor componentColor = componentColors[i];
            for (const auto& nodeIndex : connectedComponents[i]) {
                const Node& node = graph.getNodes()[nodeIndex];
                paint.setBrush(componentColor);
                paint.setPen(QPen(Qt::white, 2));
                paint.drawEllipse(node.getPos().x() - 10, node.getPos().y() - 10, 20, 20);
                paint.setFont(QFont("", 0, QFont::Bold));
                QRect textRect(node.getPos().x() - 10, node.getPos().y() - 10, 20, 20);
                paint.drawText(textRect, Qt::AlignCenter | Qt::AlignVCenter, QString::number(node.getIndex() + 1));
            }
        }
    } else {
        for (const auto& node : graph.getNodes()) {
            paint.setBrush(Qt::black);
            paint.setPen(QPen(Qt::white, 2));
            paint.drawEllipse(node.getPos().x() - 10, node.getPos().y() - 10, 20, 20);
            paint.setFont(QFont("", 0, QFont::Bold));
            QRect textRect(node.getPos().x() - 10, node.getPos().y() - 10, 20, 20);
            paint.drawText(textRect, Qt::AlignCenter | Qt::AlignVCenter, QString::number(node.getIndex() + 1));
        }
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::RightButton) {
        graph.addNode(event->pos());
        connectedComponents = graph.findConnectedComponents();
        componentColors.append(QColor::fromHsl(rand() % 360, 200, 127));
        update();
    } else if (event->button() == Qt::LeftButton) {
        int clickedNode = graph.getNodeAtPosition(event->pos());
        if (clickedNode != -1) {
            draggedNode = clickedNode;
            if (event->modifiers() & Qt::ShiftModifier) {
                if (selectedNode == -1) {
                    selectedNode = clickedNode;
                } else if (selectedNode != clickedNode) {
                    bool isValid;
                    int cost = QInputDialog::getInt(this, "Introducere cost muchie", "Costul muchiei:", 1, 1, 1000, 1, &isValid);
                    if (!isValid) {
                        QMessageBox::warning(this, "Eroare", "Costul introdus nu este valid.");
                        return;
                    }

                    graph.addEdge(selectedNode, clickedNode, cost);
                    connectedComponents = graph.findConnectedComponents();
                    componentColors.append(QColor::fromHsl(rand() % 360, 200, 127));
                    selectedNode = -1;
                }
            }
        } else {
            draggedNode = -1;
        }
    }
    update();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    if (draggedNode != -1) {
        graph.getNodes()[draggedNode].setPos(event->pos());
        update();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
    Q_UNUSED(event);
    draggedNode = -1;
    update();
}

void MainWindow::toggleBoruvkaAlgorithm() {
    isDisplayBoruvkaAlgorithm = !isDisplayBoruvkaAlgorithm;
    update();
}

void MainWindow::toggleKruskalAlgorithm() {
    isDisplayKruskalAlgorithm = !isDisplayKruskalAlgorithm;
    update();
}

void MainWindow::toggleConnectedComponents(int state)
{
    if(state == Qt::Checked) {
        for (int i = 0; i < connectedComponents.size(); ++i) {
            qDebug() << "Componenta " << i+1 << ": " << connectedComponents[i];
        }
        isDisplayComponents = true;
    } else {
        isDisplayComponents = false;
    }
    update();
}

void MainWindow::drawArrow(QPainter &paint, const Node &first, const Node &second, QColor arrowColor) {
    double dx = second.getPos().x() - first.getPos().x();
    double dy = second.getPos().y() - first.getPos().y();
    double length = std::sqrt(dx * dx + dy * dy);
    double unitDx = dx / length;
    double unitDy = dy / length;

    int arrowLength = 20;
    int arrowWidth = 5;

    QPoint arrowP1(second.getPos().x() - arrowLength * unitDx + arrowWidth * unitDy,
                   second.getPos().y() - arrowLength * unitDy - arrowWidth * unitDx);
    QPoint arrowP2(second.getPos().x() - arrowLength * unitDx - arrowWidth * unitDy,
                   second.getPos().y() - arrowLength * unitDy + arrowWidth * unitDx);

    QPoint arrowHead[3] = { QPoint(second.getPos().x(), second.getPos().y()), arrowP1, arrowP2 };
    paint.setBrush(Qt::white);
    paint.setPen(QPen(arrowColor, 2));
    paint.drawPolygon(arrowHead, 3);
}

void MainWindow::performTopologicalSort() {
    bool hasCycle = false;
    QVector<int> sortedNodes = graph.topologicalSort(hasCycle);

    if (hasCycle) {
        QMessageBox::warning(this, "Eroare", "Graful contine circuite. Sortarea topologica nu este posibila.");
        return;
    }

    graph.saveTopologicalSortToFile(sortedNodes);
    QMessageBox::information(this, "Sortare Topologica", "Sortarea topologica a fost realizatăa cu succes si salvata in sortare_topologica.txt.");

    TopologicalWindow* topologicalWindow = new TopologicalWindow(sortedNodes, graph.getEdges());
    topologicalWindow->setWindowTitle("Sortarea Topologica (Graf)");
    topologicalWindow->show();
}
