#ifndef PATHFINDING_H
#define PATHFINDING_H
#include <cmath>
#include <vector>
#include <queue>
#include <unordered_map>
#include "Set.h"

namespace Pathfinding
{
    struct Cube {
        int id; // Unique identifier for the cube
        float x, y;
        std::vector<Cube*> neighbors;
        Cube() {};
        Cube(int id, float x, float y) : id(id), x(x), y(y) {}
    };

    struct Node {
        Cube* cube;
        float g; // Cost from start to current node
        float h; // Heuristic cost from current node to target
        Node* parent;
        Node(Cube* cube, float g, float h, Node* parent = nullptr)
            : cube(cube), g(g), h(h), parent(parent) {}

        float f() const { return g + h; }
    };

    struct CompareNode {
        bool operator()(const Node* lhs, const Node* rhs) const {
            return lhs->f() > rhs->f();
        }
    };

    float euclidean_distance(const Cube* a, const Cube* b);

    // Function to check if two floats are nearly equal
    bool nearly_equal(float a, float b, float epsilon = 1e-6);

    // Function to check if the middle cube is between start and end cubes
    bool is_between(const Cube& start, const Cube& middle, const Cube& end);

    std::vector<Cube*> get_neighbors(Cube* current, const std::vector<Cube>& cubes, const std::vector<Bounds::Box>& obstacles, float neighbor_distance);

    std::vector<Cube> reconstruct_path(Node* node);

    std::vector<Cube> a_star(Cube* start, Cube* goal, const std::vector<Cube>& cubes, std::vector < Bounds::Box > obstacles, float neighbor_distance);


    // Function to calculate the distance between two points
    float distance(float x1, float y1, float x2, float y2);

    const float speed = 1.0f;
    // Function to move the entity towards the next point in the path
    void move_towards(Sets::Set* entity, const Cube& target, float delta_time);

    // Function to make the entity follow the path
    void follow_path(Sets::Set* entity, const std::vector<Cube>& path, float delta_time);

    void CheckForNeighbors(std::vector<Cube>& _array, std::vector<Bounds::Box> _obstacles);
}

#endif // !PATHFINDING_H