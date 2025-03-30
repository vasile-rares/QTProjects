# QTProjects 🔍

[![C++](https://img.shields.io/badge/Language-C++-purple)](https://isocpp.org/)
[![License](https://img.shields.io/badge/License-MIT-blue.svg)](https://opensource.org/licenses/MIT)

This repository contains a collection of graph algorithms and data structures implemented using Qt Framework. Each project focuses on different graph theory concepts and provides interactive visualizations.

## 📋 Project Descriptions

### 📊 Graph Editor (Basic Graph Operations)

A simple graph editor that allows users to:

-   Create directed or undirected graphs
-   Add nodes with right-click
-   Connect nodes by selecting them with Shift+click
-   Drag nodes to reposition them
-   Visualize arrows for directed edges

### 🧩 Labyrinth Solver (Breadth-First Search)

A visualization tool for finding the shortest path in a labyrinth:

-   Reads a labyrinth from a text file
-   Uses graph algorithms to find the shortest path between start and end points
-   Visualizes the labyrinth and path using different colors
-   Pressing spacebar toggles the display of the shortest path

### 🔗 Connected Components & Topological Sorting (Depth-First Search)

Extends the graph editor with:

-   Visualization of connected components with different colors
-   Topological sorting for directed acyclic graphs (DAGs)
-   Detection of cycles in directed graphs
-   Saving the topological sort result to a file
-   Displaying the result in a new window

### 🌲 Minimum Spanning Trees (Borůvka & Kruskal)

Implements minimum spanning tree algorithms:

-   Borůvka's algorithm for finding a minimum spanning tree
-   Kruskal's algorithm for finding a minimum spanning tree
-   Support for edge weights/costs
-   Visual representation of the resulting minimum spanning tree

### 🛣️ Dijkstra's Shortest Path (Dijkstra's Algorithm)

Implementation of Dijkstra's algorithm for finding the shortest path:

-   Loads map data from XML files (example: Luxembourg map)
-   Scales and visualizes the map
-   Finds the closest node to a clicked position
-   Calculates and displays the shortest path between two selected points

### 🌊 Maximum Flow (Ford-Fulkerson)

Implementation of network flow algorithms:

-   Ford-Fulkerson algorithm for finding maximum flow
-   Visualization of residual graphs at each step
-   Interactive creation of flow networks
-   Display of the final maximum flow value

## 🔧 Technical Details

-   Built with Qt Framework
-   Uses C++ for implementing graph algorithms
-   Provides interactive GUI for all applications
-   Visualizes the algorithms' results in real-time
