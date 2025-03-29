#include "labyrinth.h"
#include <fstream>
#include <map>
#include <queue>
#include <QDebug>

Labyrinth::Labyrinth(const std::string& filePath) {
    loadMatrixFromFile(filePath);
}

void Labyrinth::loadMatrixFromFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        qDebug() << "Eroare la deschiderea fisierului!";
        return;
    }

    int row = 0;
    std::string line;

    while (std::getline(file, line)) {
        std::vector<int> rowValues;
        for (int col = 0; col < line.size(); ++col) {
            int value = line[col] - '0';
            rowValues.push_back(value);
            if (value == 3)
                entrance = { row, col };
            if (value == 2)
                exits.push_back({ row, col });
        }
        matrix.push_back(rowValues);
        ++row;
    }
    file.close();
}

const std::vector<std::vector<int>>& Labyrinth::getLabyrinthMatrix() const {
    return matrix;
}

void Labyrinth::findShortestPaths() {
    bfs();
}

void Labyrinth::bfs() {
    std::queue<std::pair<int, int>> q;
    q.push(entrance);

    std::map<std::pair<int, int>, std::pair<int, int>> parents;
    parents[entrance] = {-1, -1};

    std::vector<std::vector<bool>> visited(matrix.size(), std::vector<bool>(matrix[0].size(), false));
    visited[entrance.first][entrance.second] = true;

    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();

        if (matrix[x][y] == 2) {
            std::pair<int, int> current = {x, y};
            while (current != entrance) {
                shortestPaths.push_back(current);
                current = parents[current];
            }
        }

        std::vector<std::pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
        for (const auto& [dx, dy] : directions) {
            int nx = x + dx;
            int ny = y + dy;
            if (nx >= 0 && ny >= 0 && nx < matrix.size() && ny < matrix[0].size() &&
                !visited[nx][ny] && matrix[nx][ny] != 0) {
                visited[nx][ny] = true;
                q.push({nx, ny});
                parents[{nx, ny}] = {x, y};
            }
        }
    }
}


const std::vector<std::pair<int, int>>& Labyrinth::getShortestPaths() const {
    return shortestPaths;
}
