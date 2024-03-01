#include "Set.h"
#include "FrustrumCulling.h"
#include "Scene.h"
#include "Inventory.h"
#include <glm/gtx/matrix_decompose.hpp>

Sets::Set::Set()
{
	this->visible = false;
	this->parent = nullptr;
	this->typesInstances = nullptr;
	this->bone = nullptr;
	this->useParentRendering = false;
}

Sets::Set::~Set()
{
}

void Sets::Set::Initialize()
{
	if (this->bone == nullptr)
	{
		this->bone = new glm::mat4(1.0f);
		this->SetScale(1.0f);
	}
}

void Sets::Set::Erase()
{
	if (this->visible)
	{
		this->visible = false;
		this->AppplyVisibility();
	}
	if (this->bone != nullptr)
	{
		delete this->bone;
	}
	for (size_t i = 0; i < this->blocks.size(); i++)
	{
		this->blocks[i].EraseModel();
	}
}

void Sets::Set::Save()
{//save here
	json resultObject;
	json blocksArray;
	for (size_t i = 0; i < this->blocks.size(); i++)
	{
		json objetAttributes;
		glm::vec3 position = this->blocks[i].GetPosition();
		objetAttributes["position"] = { position.x, position.y, position.z };
		glm::vec3 rotation = this->blocks[i].GetRotation();
		objetAttributes["rotation"] = { rotation.x, rotation.y, rotation.z };
		if (this->blocks[i].GetType() == nullptr) { continue; }
		objetAttributes["type"] = this->blocks[i].GetType()->GetName();
		blocksArray.push_back(objetAttributes);
	}
	resultObject["blocks"] = blocksArray;
	if (this->path.size() > 2)
	{
		Files::Create(this->path.c_str(), this->GetName().c_str(), ".json", resultObject.dump().c_str());
		std::cout << path << std::endl;
		return;
	}
	Files::Create(SETS_DIRECTORY, this->GetName().c_str(), ".json", resultObject.dump().c_str());
}

void Sets::Set::LoadFromJson(json _content)
{//load here
	if (_content.contains("blocks"))
	{
		for (auto& object : _content["blocks"])
		{
			Blocks::Block block;
			block.GenerateModel();
			if (object.contains("type"))
			{
				std::string name = object["type"];
				Blocks::MaterialCheck(&block, name.c_str());
				Blocks::BlockType* type = block.GetType();
				if (type == nullptr)
				{
					block.EraseModel();
					continue;
				}
				block.SetScale(type->GetScale());
			}
			if (object.contains("position"))
			{
				glm::vec3 position(object["position"][0], object["position"][1], object["position"][2]);
				block.SetPosition(position);
			}
			if (object.contains("rotation"))
			{
				glm::vec3 rotation(object["rotation"][0], object["rotation"][1], object["rotation"][2]);
				block.SetRotation(rotation);
			}
			this->InsertBlock(block);
		}
	}
}

std::string Sets::Set::GetName()
{
	return this->name;
}

void Sets::Set::SetName(std::string _name)
{
	this->name = _name;
}

std::string Sets::Set::GetPath()
{
	return this->path;
}

void Sets::Set::SetPath(std::string _path)
{
	this->path = _path;
}

bool Sets::Set::IsVisible()
{
	return this->visible;
}

void Sets::Set::CheckVisibility()
{
	if (!this->blocks.size()) { return; }//Prevent underdered since It has been applied for visibility
	if (FrustrumCulling::IsBoxInFrustum(Scene::World::GetProjection(), Scene::World::GetView(), this->boundingBox.min, this->boundingBox.max))
	{
		if (this->visible) { return; }
		this->visible = true;
		this->AppplyVisibility();
	}
	else
	{
		if (!this->visible) { return; }
		this->visible = false;
		this->AppplyVisibility();
	}
}

void Sets::Set::AppplyVisibility()
{
	if (this->typesInstances != nullptr && !this->useParentRendering)
	{
		for (size_t i = 0; i < this->typesInstances->size(); i++)
		{
			this->typesInstances->at(i)->SetInstanceVisibility(this->typesInstances, this->visible);
		}
	}
}

























void Sets::Set::ApplyTransformation()
{
	if (this->parent != nullptr)
	{
		*this->bone = parent->GetBone();
	}
	else
	{
		*this->bone = glm::mat4(1.0f);
	}

	*this->bone = glm::translate(*this->bone, this->position);

	*this->bone = glm::rotate(*this->bone, glm::radians(this->rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	*this->bone = glm::rotate(*this->bone, glm::radians(this->rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	*this->bone = glm::rotate(*this->bone, glm::radians(this->rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

	*this->bone = glm::scale(*this->bone, this->scale);

	for (size_t i = 0; i < this->blocks.size(); i++)
	{
		this->blocks[i].ApplyTransformation();
		if (this->typesInstances != nullptr)
		{
			this->blocks[i].GetType()->AskForRefresh(this->typesInstances);
		}
	}
	CalculateBoundingBox();
}

glm::mat4 Sets::Set::GetBone()
{
	return *this->bone;
}


void Sets::Set::InsertBlock(Blocks::Block _block)
{
	if (_block.GetType() == nullptr)
	{
		Logger::Write("Unable to insert a block without type.\n");
		return;
	}
	_block.SetParent(this->bone);
	this->blocks.push_back(_block);
	this->CalculateBoundingBox();

	if (this->typesInstances != nullptr)
	{
		for (size_t instanceId = 0; instanceId < this->typesInstances->size(); instanceId++)
		{
			if (this->typesInstances->at(instanceId) == _block.GetType())
			{
				this->typesInstances->at(instanceId)->InsertInInstance(this->typesInstances, _block.GetModelAddress());
				return;
			}
		}
		//else like
		this->typesInstances->push_back(_block.GetType());
		Blocks::Instance* instance = _block.GetType()->AddInstance(this->typesInstances);
		instance->InsertModel(_block.GetModelAddress());
		instance->SetVisibility(this->visible);
	}
}

void Sets::Set::RemoveBlock(Blocks::Block* _block)
{
	for (size_t blockId = 0; blockId < this->blocks.size(); blockId++)
	{
		if (&this->blocks[blockId] == _block)
		{
			if (this->typesInstances != nullptr)
			{
				for (size_t instanceId = 0; instanceId < this->typesInstances->size(); instanceId++)
				{
					if (this->typesInstances->at(instanceId) == _block->GetType())
					{
						this->typesInstances->at(instanceId)->RemoveFromInstance(this->typesInstances, _block->GetModelAddress());
						break;
					}
				}
			}

			this->blocks[blockId].EraseModel();

			this->blocks.erase(this->blocks.begin() + blockId);
			return;
		}
	}
}

std::vector<Blocks::Block>* Sets::Set::GetBlocks()
{
	return &this->blocks;
}

void Sets::Set::GenerateRenderingInstance()
{
	this->typesInstances = new std::vector<Blocks::BlockType*>;
}

void Sets::Set::EraseRenderingInstance()
{
	if (this->typesInstances != nullptr && !this->useParentRendering)
	{
		delete this->typesInstances;
	}
}

void Sets::Set::SetRenderingInstance(std::vector<Blocks::BlockType*>* _instance)
{
	if (!this->useParentRendering && this->typesInstances != nullptr)
	{
		for (size_t i = 0; i < this->typesInstances->size(); i++)
		{
			this->typesInstances->at(i)->RemoveInstance(this->typesInstances);
		}
		delete this->typesInstances;
	}
	this->typesInstances = _instance;
	this->useParentRendering = true;
}


glm::vec3 Sets::Set::GetPosition() { return this->position; }
void Sets::Set::SetPosition(float _x, float _y, float _z)
{
	this->position = glm::vec3(_x, _y, _z);
	ApplyTransformation();
}
void Sets::Set::SetPosition(glm::vec3 _position)
{
	this->position = _position;
	ApplyTransformation();
}
void Sets::Set::Move(glm::vec3 _position)
{
	this->position += _position;
	ApplyTransformation();
}
void Sets::Set::Move(float _x, float _y, float _z)
{
	this->position.x += _x;
	this->position.y += _y;
	this->position.z += _z;

	ApplyTransformation();
}

glm::vec3 Sets::Set::GetRotation() { return this->rotation; }
void Sets::Set::SetRotation(float _x, float _y, float _z)
{
	this->rotation = glm::vec3(_x, _y, _z);

	ApplyTransformation();
}
void Sets::Set::SetRotation(glm::vec3 _rotation)
{
	this->rotation = _rotation;

	ApplyTransformation();
}
void Sets::Set::Rotate(glm::vec3 _rotation)
{
	this->rotation += _rotation;

	ApplyTransformation();
}
void Sets::Set::Rotate(float _x, float _y, float _z)
{
	this->rotation.x += _x;
	this->rotation.y += _y;
	this->rotation.z += _z;

	ApplyTransformation();
}

glm::vec3 Sets::Set::GetScale() { return this->scale; }
void Sets::Set::SetScale(float _x, float _y, float _z)
{
	this->scale = glm::vec3(_x, _y, _z);

	ApplyTransformation();
}
void Sets::Set::SetScale(float _scale)
{
	this->scale = glm::vec3(_scale, _scale, _scale);

	ApplyTransformation();
}
void Sets::Set::SetScale(glm::vec3 _scale)
{
	this->scale = _scale;

	ApplyTransformation();
}
void Sets::Set::Scale(glm::vec3 _scale)
{
	this->scale += _scale;

	ApplyTransformation();
}
void Sets::Set::Scale(float _x, float _y, float _z)
{
	this->scale.x += _x;
	this->scale.y += _y;
	this->scale.z += _z;

	ApplyTransformation();
}

void Sets::Set::CalculateBoundingBox()
{
	glm::vec3 globalMin = glm::vec3(FLT_MAX);
	glm::vec3 globalMax = glm::vec3(-FLT_MAX);

	for (Blocks::Block& block : blocks)
	{
		glm::vec3 scale;
		glm::quat rotation;
		glm::vec3 translation;
		glm::vec3 skew;
		glm::vec4 perspective;
		glm::decompose(block.GetModel(), scale, rotation, translation, skew, perspective);

		glm::vec3 localMin = translation - scale / 2.0f;
		glm::vec3 localMax = translation + scale / 2.0f;

		globalMin = glm::min(globalMin, localMin);
		globalMax = glm::max(globalMax, localMax);
	}

	this->boundingBox = { globalMin, globalMax };
}

bool Sets::Set::HasOrigin()
{
	for (size_t i = 0; i < this->blocks.size(); i++)
	{
		if (this->blocks[i].GetPosition() == glm::vec3(.0f))
		{
			return true;
		}
	}
	return false;
}

void Sets::Set::SetOriginBlockScale(float _scale)
{
	for (size_t i = 0; i < this->blocks.size(); i++)
	{
		if (this->blocks[i].GetPosition() == glm::vec3(.0f))
		{
			this->blocks[i].SetScale(_scale);
		}
	}
}

void Sets::Set::PlaceOriginBlock(float _scale)
{
	for (size_t i = 0; i < this->blocks.size(); i++)
	{
		if (this->blocks[i].GetPosition() == glm::vec3(.0f))
		{
			return;
		}
	}
	Blocks::Block originBlock;
	originBlock.GenerateModel();
	originBlock.SetScale(_scale);

	Blocks::MaterialCheck(&originBlock, nullptr);

	this->InsertBlock(originBlock);
}

void Sets::Set::MoveOrigin(glm::vec3 _offset)
{
	for (size_t i = 0; i < this->blocks.size(); i++)
	{
		this->blocks[i].Move(_offset);
	}
	CalculateBoundingBox();
}

void Sets::Set::MoveOrigin(float _x, float _y, float _z)
{
	glm::vec3 offset(_x, _y, _z);
	for (size_t i = 0; i < this->blocks.size(); i++)
	{
		this->blocks[i].Move(offset);
	}
	CalculateBoundingBox();
}

Bounds::Box Sets::Set::GetBoundingBox()
{
	return this->boundingBox;
}
