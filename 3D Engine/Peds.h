#ifndef PEDS_H
#define PEDS_H
#include "Common.h"
#include "Files.h"
#include "Model.h"

namespace Peds
{
	class Ped
	{
	private:
		std::shared_mutex mutex;
		int serverId;
		int ownerServerId;
		Models::Model* model;
		glm::vec3 position;

	public:
		Ped()
		{
			this->model = nullptr;
		}
		~Ped() {}

		int GetServerId()
		{
			//std::shared_lock<std::shared_mutex> lock(this->mutex);
			return this->serverId;
		}
		void SetServerId(int _serverId)
		{
			std::unique_lock<std::shared_mutex> lock(this->mutex);
			this->serverId = _serverId;
		}

		int GetOwnerServerId()
		{
			//std::shared_lock<std::shared_mutex> lock(this->mutex);
			return this->ownerServerId;
		}
		// LOCAL CHANGE ONLY !!!
		void SetOwnerServerId(int _ownerServerId)
		{
			std::unique_lock<std::shared_mutex> lock(this->mutex);
			this->ownerServerId = _ownerServerId;
		}

		Models::Model* GetModel()
		{
			//std::shared_lock<std::shared_mutex> lock(this->mutex);
			return this->model;
		}
		void SetModel(Models::Model* _model)
		{
			std::unique_lock<std::shared_mutex> lock(this->mutex);
			this->model = _model;
		}

		glm::vec3 GetPosition()
		{
			//std::shared_lock<std::shared_mutex> lock(this->mutex);
			return this->position;
		}
		void SetPosition(glm::vec3 _position)
		{
			std::unique_lock<std::shared_mutex> lock(this->mutex);
			this->position = _position;
			if (this->GetModel() != nullptr)
			{
				std::cout << "offsetSetting " << _position.x << " " << _position.y << " " << _position.z << std::endl;
				//this->GetModel()->SetOffsetUsage(true);
				//this->GetModel()->ApplyOffset(this->position);
			}
		}
	};

	void Insert(Ped* _ped);
	void Remove(int _serverId);
	std::vector<Ped*>* Get();
}

#endif // !PEDS_H