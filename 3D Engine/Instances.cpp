#include "Instances.h"


std::vector<Instances::Cube> Instances::Generate(std::vector<::Cube>* _objects)
{
	std::vector<Instances::Cube> instances;
	for (size_t objIndex = 0; objIndex < _objects->size(); objIndex++)
	{
		bool found = false;
		for (size_t instanceIndex = 0; instanceIndex < instances.size(); instanceIndex++)
		{
			if (_objects->at(objIndex).GetMaterial() == instances[instanceIndex].material
				&& _objects->at(objIndex).GetShader() == instances[instanceIndex].shader
				&& _objects->at(objIndex).GetLightDependency() == instances[instanceIndex].lightDependent)
			{
				_objects->at(objIndex).Update();
				instances[instanceIndex].Add(&_objects->at(objIndex));
				found = true;
			}
		}
		if (!found)
		{
			Instances::Cube instance;
			instance.material = _objects->at(objIndex).GetMaterial();
			instance.shader = _objects->at(objIndex).GetShader();
			instance.lightDependent = _objects->at(objIndex).GetLightDependency();
			_objects->at(objIndex).Update();
			instance.Add(&_objects->at(objIndex));
			instances.push_back(instance);
		}
	}

	for (size_t instanceIndex = 0; instanceIndex < instances.size(); instanceIndex++)
	{
		instances[instanceIndex].Initialize();
	}
	return instances;
}