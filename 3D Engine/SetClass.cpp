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

	json childsArray;
	for (size_t childId = 0;  childId < this->childs.size();  childId++)
	{
		if (this->childs[childId]->GetName().size() > 3)//Ensure the name is present
		{
			json childObj;
			childObj["name"] = this->childs[childId]->GetName();
			childObj["path"] = this->childs[childId]->GetPath();

			glm::vec3 position = this->childs[childId]->GetPosition();
			childObj["position"] = { position.x, position.y, position.z };
			glm::vec3 rotation = this->childs[childId]->GetRotation();
			childObj["rotation"] = { rotation.x, rotation.y, rotation.z };
			childsArray.push_back(childObj);
		}
	}
	resultObject["childs"] = childsArray;

	if (this->path.size() > 2)
	{
		Files::Create(this->path.c_str(), this->GetName().c_str(), ".json", resultObject.dump().c_str());
		return;
	}
	Files::Create(SETS_DIRECTORY, this->GetName().c_str(), ".json", resultObject.dump().c_str());
}

void Sets::Set::LoadFromJson(json _content, bool _computeTransformation)
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
					Logger::Write("Unable to insert ", name.c_str(), " block in ", this->GetName(), " set.\n");
					block.EraseModel();
					continue;
				}
				block.SetScale(type->GetScale());
			}
			else
			{
				block.EraseModel();
				Logger::Write("Invalid json loading for ", this->GetName(), " set. (Unable to find [type] attribute)\n");
				continue;
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
	else
	{
		Logger::Write("Invalid json loading for ", this->GetName(), " set. (Unable to find [blocks])\n");
		return;
	}
	if (_content.contains("childs"))
	{
		for (auto& child : _content["childs"])
		{
			if (child.contains("name") && child.contains("path"))
			{
				Sets::Set* childSet = Sets::Create();
				childSet->SetParent(this, false);
				childSet->SetRenderingInstance(this->GetRenderingInstance());
				try
				{
					auto parsedContent = json::parse(Files::GetFileContent(std::string(std::string(child["path"]) + std::string(child["name"]) + SETS_FILE_EXTENSION).c_str()));
					childSet->LoadFromJson(parsedContent, false);
				}
				catch (json::parse_error& e)
				{
					std::cerr << "JSON parsing error: " << e.what() << '\n';
					Sets::Erase(childSet);
					continue;
				}
				childSet->SetName(child["name"]);
				childSet->SetPath(child["path"]);
				if (child.contains("position"))
				{
					childSet->SetPosition(glm::vec3(child["position"][0], child["position"][1], child["position"][2]), false);
				}
				if (child.contains("rotation"))
				{
					childSet->SetRotation(glm::vec3(child["rotation"][0], child["rotation"][1], child["rotation"][2]), false);
				}
			}
			else
			{
				Logger::Write("Child from set ", this->GetName(), " has no name or path....\n");
				continue;
			}
		}
	}
	this->ApplyTransformation();
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
	if (FrustrumCulling::IsBoxInFrustum(Scene::World::GetProjection(), Scene::World::GetView(), this->boundingBox.GetBox().min, this->boundingBox.GetBox().max))
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
	if (this->typesInstances != nullptr)
	{
		for (size_t i = 0; i < this->typesInstances->size(); i++)
		{
			this->typesInstances->at(i)->AskForRefresh(this->typesInstances);
		}
	}

	for (size_t childId = 0; childId < this->childs.size(); childId++)
	{
		this->childs[childId]->ApplyTransformation();
	}
	this->CalculateBoundingBox();
}

Sets::Set* Sets::Set::GetParent()
{
	return this->parent;
}

void Sets::Set::SetParent(Set* _parent, bool _boundsCalculation)
{
	if (this->parent != nullptr)
	{
		this->parent->RemoveChild(this);
	}
	this->parent = _parent;
	_parent->AddChild(this, _boundsCalculation);
	this->ApplyTransformation();
}

std::vector<Sets::Set*> Sets::Set::GetChilds()
{
	return this->childs;
}

void Sets::Set::AddChild(Set* _child, bool _boundsCalculation)
{
	if (_child == nullptr)
	{
		Logger::Write("Cannot insert nullptr child in ", this->GetName(), " set.\n");
		return;
	}
	this->childs.push_back(_child);
	if (_boundsCalculation) { this->CalculateBoundingBox(); }
}

void Sets::Set::RemoveChild(Set* _child, bool _boundsCalculation)
{
	if (_child == nullptr)
	{
		Logger::Write("Cannot remove nullptr child from ", this->GetName(), " set.\n");
		return;
	}
	for (size_t childId = 0; childId < this->childs.size(); childId++)
	{
		if (this->childs[childId] == _child)
		{
			this->childs.erase(this->childs.begin() + childId);
			return;
		}
	}
	Logger::Write(_child->GetName(), " does not belong to ", this->GetName(), " set.\n");
	if (_boundsCalculation) { this->CalculateBoundingBox(); }
}

glm::mat4 Sets::Set::GetBone()
{
	return *this->bone;
}


void Sets::Set::InsertBlock(Blocks::Block _block, bool _boundsCalculation)
{
	if (_block.GetType() == nullptr)
	{
		Logger::Write("Unable to insert a block without type.\n");
		return;
	}
	_block.SetParent(this->bone);
	this->blocks.push_back(_block);
	if (_boundsCalculation)
	{
		this->CalculateBoundingBox();
	}

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
			this->CalculateBoundingBox();
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

std::vector<Blocks::BlockType*>* Sets::Set::GetRenderingInstance()
{
	return this->typesInstances;
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
void Sets::Set::SetPosition(float _x, float _y, float _z, bool _computeTransformation)
{
	this->position = glm::vec3(_x, _y, _z);
	if (_computeTransformation)
	{
		ApplyTransformation();
	}
}
void Sets::Set::SetPosition(glm::vec3 _position, bool _computeTransformation)
{
	this->position = _position;
	if (_computeTransformation)
	{
		ApplyTransformation();
	}
}
void Sets::Set::Move(glm::vec3 _position, bool _computeTransformation)
{
	this->position += _position;
	if (_computeTransformation)
	{
		ApplyTransformation();
	}
}
void Sets::Set::Move(float _x, float _y, float _z, bool _computeTransformation)
{
	this->position.x += _x;
	this->position.y += _y;
	this->position.z += _z;

	if (_computeTransformation)
	{
		ApplyTransformation();
	}
}

glm::vec3 Sets::Set::GetRotation() { return this->rotation; }
void Sets::Set::SetRotation(float _x, float _y, float _z, bool _computeTransformation)
{
	this->rotation = glm::vec3(_x, _y, _z);

	if (_computeTransformation)
	{
		ApplyTransformation();
	}
}
void Sets::Set::SetRotation(glm::vec3 _rotation, bool _computeTransformation)
{
	this->rotation = _rotation;

	if (_computeTransformation)
	{
		ApplyTransformation();
	}
}
void Sets::Set::Rotate(glm::vec3 _rotation, bool _computeTransformation)
{
	this->rotation += _rotation;

	if (_computeTransformation)
	{
		ApplyTransformation();
	}
}
void Sets::Set::Rotate(float _x, float _y, float _z, bool _computeTransformation)
{
	this->rotation.x += _x;
	this->rotation.y += _y;
	this->rotation.z += _z;

	if (_computeTransformation)
	{
		ApplyTransformation();
	}
}

glm::vec3 Sets::Set::GetScale() { return this->scale; }
void Sets::Set::SetScale(float _x, float _y, float _z, bool _computeTransformation)
{
	this->scale = glm::vec3(_x, _y, _z);

	if (_computeTransformation)
	{
		ApplyTransformation();
	}
}
void Sets::Set::SetScale(float _scale, bool _computeTransformation)
{
	this->scale = glm::vec3(_scale, _scale, _scale);

	if (_computeTransformation)
	{
		ApplyTransformation();
	}
}
void Sets::Set::SetScale(glm::vec3 _scale, bool _computeTransformation)
{
	this->scale = _scale;

	if (_computeTransformation)
	{
		ApplyTransformation();
	}
}
void Sets::Set::Scale(glm::vec3 _scale, bool _computeTransformation)
{
	this->scale += _scale;

	if (_computeTransformation)
	{
		ApplyTransformation();
	}
}
void Sets::Set::Scale(float _x, float _y, float _z, bool _computeTransformation)
{
	this->scale.x += _x;
	this->scale.y += _y;
	this->scale.z += _z;

	if (_computeTransformation)
	{
		ApplyTransformation();
	}
}

void Sets::Set::CalculateBoundingBox()
{
	Bounds::Box box;
	{
		glm::vec3 globalMin = glm::vec3(FLT_MAX);
		glm::vec3 globalMax = glm::vec3(-FLT_MAX);

		for (Blocks::Block& block : this->blocks)
		{
			block.CalculateBoundingBox();
			Bounds::Box box = block.GetBoundingBox();

			globalMin = glm::min(globalMin, box.min);
			globalMax = glm::max(globalMax, box.max);
		}

		box.min = globalMin;
		box.max = globalMax;
	}
	this->boundingBox.SetBox(box);
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

	if (this->typesInstances != nullptr)
	{
		for (size_t i = 0; i < this->typesInstances->size(); i++)
		{
			this->typesInstances->at(i)->AskForRefresh(this->typesInstances);
		}
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
	return this->boundingBox.GetBox();
}

void Sets::Set::DrawBoundingBox()
{
	this->boundingBox.Draw();
}
