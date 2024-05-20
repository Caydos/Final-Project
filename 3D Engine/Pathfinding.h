#ifndef PATHFINDING_H
#define PATHFINDING_H
#include <cmath>
#include <vector>
#include <queue>
#include <unordered_map>


namespace Pathfinding
{
    struct Cube {
        int id; // Unique identifier for the cube
        float x, y;
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

    float euclidean_distance(const Cube* a, const Cube* b) {
        return std::sqrt((a->x - b->x) * (a->x - b->x) + (a->y - b->y) * (a->y - b->y));
    }

    // Function to check if two floats are nearly equal
    bool nearly_equal(float a, float b, float epsilon = 1e-6) {
        return std::fabs(a - b) < epsilon;
    }

    // Function to check if the middle cube is between start and end cubes
    bool is_between(const Cube& start, const Cube& middle, const Cube& end) {
        // Check if middle is collinear with start and end
        float cross_product = (middle.y - start.y) * (end.x - start.x) - (middle.x - start.x) * (end.y - start.y);
        if (!nearly_equal(cross_product, 0)) {
            return false;
        }

        // Check if middle is within the bounding box of start and end
        bool within_x_bounds = std::min(start.x, end.x) <= middle.x && middle.x <= std::max(start.x, end.x);
        bool within_y_bounds = std::min(start.y, end.y) <= middle.y && middle.y <= std::max(start.y, end.y);

        return within_x_bounds && within_y_bounds;
    }

    std::vector<Cube*> get_neighbors(Cube* current, const std::vector<Cube>& cubes, const std::vector<Cube>& obstacles, float neighbor_distance) {
        std::vector<Cube*> neighbors;
        for (const Cube& cube : cubes) {
            if (cube.id != current->id && euclidean_distance(current, &cube) <= neighbor_distance) {
                bool is_obstacle = false;
                for (const Cube& obstacle : obstacles) {
                    if (cube.id == obstacle.id) {
                        is_obstacle = true;
                        break;
                    }
                }
                if (!is_obstacle) {
                    neighbors.push_back(const_cast<Cube*>(&cube));
                }
            }
        }
        return neighbors;
    }

    std::vector<Cube> reconstruct_path(Node* node) {
        std::vector<Cube> path;
        while (node != nullptr) {
            path.push_back(*(node->cube));
            node = node->parent;
        }
        std::reverse(path.begin(), path.end());
        return path;
    }

    std::vector<Cube> a_star(Cube* start, Cube* goal, const std::vector<Cube>& cubes, const std::vector<Cube>& obstacles, float neighbor_distance) {
        std::priority_queue<Node*, std::vector<Node*>, CompareNode> open_set;
        std::unordered_map<int, Node*> all_nodes; // To keep track of all created nodes

        Node* start_node = new Node(start, 0, euclidean_distance(start, goal));
        open_set.push(start_node);
        all_nodes[start->id] = start_node;

        while (!open_set.empty()) {
            Node* current = open_set.top();
            open_set.pop();

            if (current->cube->id == goal->id) {
                std::vector<Cube> path = reconstruct_path(current);
                for (auto& pair : all_nodes) {
                    delete pair.second; // Clean up
                }
                return path;
            }

            std::vector<Cube*> neighbors = get_neighbors(current->cube, cubes, obstacles, neighbor_distance);
            for (Cube* neighbor : neighbors) {
                float tentative_g = current->g + euclidean_distance(current->cube, neighbor);
                if (all_nodes.find(neighbor->id) == all_nodes.end() || tentative_g < all_nodes[neighbor->id]->g) {
                    Node* neighbor_node = new Node(neighbor, tentative_g, euclidean_distance(neighbor, goal), current);
                    open_set.push(neighbor_node);
                    all_nodes[neighbor->id] = neighbor_node;
                }
            }
        }

        for (auto& pair : all_nodes) {
            delete pair.second; // Clean up
        }

        return {}; // No path found
    }
}

#endif // !PATHFINDING_H