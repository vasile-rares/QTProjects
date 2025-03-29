#ifndef LABYRINTH_H
#define LABYRINTH_H

#include <vector>
#include <string>

class Labyrinth {
public:
    Labyrinth(const std::string& filePath);
    void findShortestPaths();
    const std::vector<std::vector<int>>& getLabyrinthMatrix() const;
    const std::vector<std::pair<int, int>>& getShortestPaths() const;

private:
    std::vector<std::vector<int>> matrix;
    std::vector<std::pair<int, int>> exits;
    std::pair<int, int> entrance;
    std::vector<std::pair<int, int>> shortestPaths;

    void loadMatrixFromFile(const std::string& filePath);
    void bfs();
};

#endif // LABYRINTH_H
