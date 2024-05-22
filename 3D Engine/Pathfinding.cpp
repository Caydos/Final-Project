#include "Pathfinding.h"




float Pathfinding::euclidean_distance(const Cube* a, const Cube* b) {
	return std::sqrt((a->x - b->x) * (a->x - b->x) + (a->y - b->y) * (a->y - b->y));
}

bool Pathfinding::nearly_equal(float a, float b, float epsilon) {
	return std::fabs(a - b) < epsilon;
}

bool Pathfinding::is_between(const Cube& start, const Cube& middle, const Cube& end) {
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


std::vector<Pathfinding::Cube*> Pathfinding::get_neighbors(Cube* current, const std::vector<Cube>& cubes, const std::vector < Bounds::Box >& obstacles, float neighbor_distance) {

	std::vector<Cube*> neighbors;
	for (const Cube& cube : cubes) {
		if (cube.id != current->id && euclidean_distance(current, &cube) <= neighbor_distance) {
			bool is_obstacle = false;
			//for (const Bounds::Box& obstacle : obstacles)
			//{
			//	glm::vec3 rayOrigin, rayDirection;
			//	//generateRay(glm::vec3(current->x, 0.0, current->y), glm::vec3(cube.x, 0.0, cube.y), rayOrigin, rayDirection);
			//	rayOrigin.x = current->x;
			//	rayOrigin.y = 1.0;
			//	rayOrigin.z = current->y;

			//	rayDirection.x = cube.x;
			//	rayDirection.y = 1.0;
			//	rayDirection.z = cube.y;

			//	float actualSize = glm::distance(obstacle.min, obstacle.max);
			//	float testedSize = neighbor_distance;
			//	glm::vec3 actualMidpoint = (obstacle.min + obstacle.max) / 2.0f;
			//	glm::vec3 testedMidpoint(current->x, 0.0, current->y);

			//	if (glm::distance(actualMidpoint, testedMidpoint) > actualSize + testedSize)
			//	{
			//		continue;
			//	}

			//	float distance = 100.0f;
			//	if (Collisions::IntersectRayWithBox(rayOrigin, rayDirection, obstacle.min, obstacle.max, distance) && distance <= neighbor_distance)
			//	{
			//		//std::cout << "Intersects" << std::endl;
			//		is_obstacle = true;
			//		break;
			//	}
			//}

			if (!is_obstacle) {
				//std::cout << "Got neighbour" << std::endl;
				neighbors.push_back(const_cast<Cube*>(&cube));
			}
		}
	}
	return neighbors;
}

std::vector<Pathfinding::Cube> Pathfinding::reconstruct_path(Node* node) {
	std::vector<Cube> path;
	while (node != nullptr) {
		path.push_back(*(node->cube));
		node = node->parent;
	}
	std::reverse(path.begin(), path.end());
	return path;
}

std::vector<Pathfinding::Cube> Pathfinding::a_star(Cube* start, Cube* goal, const std::vector<Cube>& cubes, std::vector < Bounds::Box > obstacles, float neighbor_distance) {
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

float Pathfinding::distance(float x1, float y1, float x2, float y2) {
	return std::sqrt((x2 - x1) * (x2 - y1) * (x2 - y1));
}

void Pathfinding::move_towards(Sets::Set* entity, const Cube& target, float delta_time) {
	glm::vec3 position = entity->GetPosition();
	float dir_x = target.x - position.x;
	float dir_y = target.y - position.z;
	float length = std::sqrt(dir_x * dir_x + dir_y * dir_y);

	// Normalize the direction vector
	dir_x /= length;
	dir_y /= length;

	// Move the entity towards the target
	position.x += dir_x * speed * delta_time;
	position.z += dir_y * speed * delta_time;

	// If the entity is very close to the target, snap to the target
	if (distance(position.x, position.z, target.x, target.y) < speed * delta_time) {
		//entity.x = target.x;
		//entity.y = target.y;
		entity->SetPosition(glm::vec3(target.x, position.y, target.y));
	}
}



// Function to make the entity follow the path
void Pathfinding::follow_path(Sets::Set* entity, const std::vector<Cube>& path, float delta_time) {
	static size_t target_index = 0;

	if (target_index < path.size()) {
		move_towards(entity, path[target_index], delta_time);

		glm::vec3 position = entity->GetPosition();
		// If the entity has reached the current target, move to the next target
		if (distance(position.x, position.z, path[target_index].x, path[target_index].y) < 0.2f)
		{
			target_index++;
			std::cout << "Got it" << std::endl;
		}
		//if (position.x == path[target_index].x && position.z == path[target_index].y) {
		//    target_index++;
		//}
	}
}

void Pathfinding::CheckForNeighbors(std::vector<Cube>& _array, std::vector<Bounds::Box> _obstacles)
{
	for (size_t cubeId = 0; cubeId < _array.size(); cubeId++)
	{

	}
}
