#include "Collisions.h"
#include "Scene.h"
#include "Decor.h"
#include "Crosshair.h"

bool Collisions::PointCollidingBox(glm::vec3 _position, Bounds::Box _box)
{
	bool collisionX = _position.x >= _box.min.x && _position.x <= _box.max.x;
	bool collisionY = _position.y >= _box.min.y && _position.y <= _box.max.y;
	bool collisionZ = _position.z >= _box.min.z && _position.z <= _box.max.z;

	return collisionX && collisionY && collisionZ;
	//return false;
}

bool Collisions::BoxColliding(Bounds::Box _firstBox, Bounds::Box _secondBox)
{
	bool collisionX = _firstBox.min.x <= _secondBox.max.x && _firstBox.max.x >= _secondBox.min.x;
	bool collisionY = _firstBox.min.y <= _secondBox.max.y && _firstBox.max.y >= _secondBox.min.y;
	bool collisionZ = _firstBox.min.z <= _secondBox.max.z && _firstBox.max.z >= _secondBox.min.z;

	return collisionX && collisionY && collisionZ;
	//return false;
}

static bool initialized = false;
Decors::Decor* objects[2];
void Collisions::Tick()
{
	if (!initialized)
	{
		objects[0] = new Decors::Decor("FirstBox");
		objects[0]->LoadFromFile("FirstBox");
		objects[0]->CalculateBoundingBox();
		objects[0]->ShowBounds(true);
		Scene::World::InsertComponent(objects[0]);

		objects[1] = new Decors::Decor("SecondBox");
		objects[1]->LoadFromFile("SecondBox");
		objects[1]->ApplyOffset(glm::vec3(1.0f, .0f, .0f));
		objects[1]->CalculateBoundingBox();
		objects[1]->ShowBounds(true);
		Scene::World::InsertComponent(objects[1]);
		initialized = true;
	}

	//if (BoxColliding(objects[0]->GetBoundingBox(), objects[1]->GetBoundingBox()))
	//{
	//	Crosshairs::Get()->SetColor(Colors::Red);
	//}
	//else
	//{
	//	Crosshairs::Get()->SetColor(Colors::Green);
	//


	Camera* cam = Scene::World::GetCamera();
	if (PointCollidingBox(cam->Position, objects[1]->GetBoundingBox()))
	{
		Crosshairs::Get()->SetColor(Colors::Red);
	}
	else
	{
		Crosshairs::Get()->SetColor(Colors::Green);
	}

}