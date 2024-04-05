#include "Lighting.h"
#include "Scene.h"

float Lighting::CalculateLightRange(float constant, float linear, float quadratic, float threshold)
{
	// Assuming the light intensity falls to 5% of its initial value at d_max
	// The equation derived from the attenuation formula:
	// threshold = 1 / (constant + linear * d_max + quadratic * d_max^2)
	// Solve for d_max using the quadratic formula: ax^2 + bx + c = 0
	// where a = quadratic, b = linear, and c = constant - (1/threshold)

	float a = quadratic;
	float b = linear;
	float c = constant - (1.0f / threshold);

	// Calculate the discriminant
	float discriminant = b * b - 4 * a * c;

	// Ensure the discriminant is non-negative
	if (discriminant < 0) {
		return -1.0f; // Return an error value, as no real solution exists
	}

	// Calculate the two possible solutions for d_max
	float d_max1 = (-b + sqrt(discriminant)) / (2 * a);
	float d_max2 = (-b - sqrt(discriminant)) / (2 * a);

	// Return the positive d_max value, which is the meaningful solution
	return (d_max1 > 0) ? d_max1 : (d_max2 > 0) ? d_max2 : -1.0f;
}


//Lighting::Light::Light()
//{
//	this->visible = false;
//	this->active = false;
//};
//Lighting::Light::~Light() {};
//
//std::string Lighting::Light::GetName() { return this->name; }
//void Lighting::Light::SetName(std::string _name) { this->name = _name; }
//
//
//bool Lighting::Light::IsVisible() { return this->visible; }
//void Lighting::Light::CheckVisibility()
//{
//	//this->visible = FrustumCulling::IsBoxInFrustum(Scene::World::GetProjection(), Scene::World::GetView(), this->position - this->range, this->position + this->range);
//	
//	this->visible = true;
//	//if (this->type == Lighting::LightType::SPOT && glm::distance(this->position, GetGameData()->camera->Position) > LIGHT_VISIBILITY_RANGE)
//	//{
//	//	this->visible = false;
//	//	return;
//	//}
//}
//void Lighting::Light::CalculateRange()
//{
//	this->range = CalculateLightRange(this->constant, this->linear, this->quadratic);
//}
//
//bool Lighting::Light::IsActive() { return this->active; }
//void Lighting::Light::SetActive(bool _status)
//{
//	this->active = _status;
//}
//
//Lighting::LightType Lighting::Light::GetType() { return this->type; }
//void Lighting::Light::SetType(LightType _type) { this->type = _type; }
//
//glm::vec3 Lighting::Light::GetPosition() { return this->position; }
//void Lighting::Light::SetPosition(glm::vec3 _position) { this->position = _position; }
//glm::vec3 Lighting::Light::GetDirection() { return this->direction; }
//void Lighting::Light::SetDirection(glm::vec3 _direction) { this->direction = _direction; }
//
//float Lighting::Light::GetCutOff() { return this->cutOff; }
//void Lighting::Light::SetCutOff(float _cutOff) { this->cutOff = _cutOff; }
//float Lighting::Light::GetOuterCutOff() { return this->outerCutOff; }
//void Lighting::Light::SetOuterCutOff(float _outerCutOff) { this->outerCutOff = _outerCutOff; }
//
//float Lighting::Light::GetConstant() { return this->constant; }
//void Lighting::Light::SetConstant(float _constant) { this->constant = _constant; this->CalculateRange(); }
//float Lighting::Light::GetLinear() { return this->linear; }
//void Lighting::Light::SetLinear(float _linear) { this->linear = _linear; this->CalculateRange(); }
//float Lighting::Light::GetQuadratic() { return this->quadratic; }
//void Lighting::Light::SetQuadratic(float _quadratic) { this->quadratic = _quadratic; this->CalculateRange(); }
//
//glm::vec3 Lighting::Light::GetAmbient() { return this->ambient; }
//void Lighting::Light::SetAmbient(glm::vec3 _ambient) { this->ambient = _ambient; }
//glm::vec3 Lighting::Light::GetDiffuse() { return this->diffuse; }
//void Lighting::Light::SetDiffuse(glm::vec3 _diffuse) { this->diffuse = _diffuse; }
//glm::vec3 Lighting::Light::GetSpecular() { return this->specular; }
//void Lighting::Light::SetSpecular(glm::vec3 _specular) { this->specular = _specular; }
//
//json Lighting::Light::Encode()
//{
//	json object;
//
//	object["name"] = name;
//	object["active"] = active;
//	object["type"] = static_cast<int>(type);
//
//	object["position"] = { position.x, position.y, position.z };
//	object["direction"] = { direction.x, direction.y, direction.z };
//
//	object["cutOff"] = cutOff;
//	object["outerCutOff"] = outerCutOff;
//
//	object["constant"] = constant;
//	object["linear"] = linear;
//	object["quadratic"] = quadratic;
//
//	object["ambient"] = { ambient.x, ambient.y, ambient.z };
//	object["diffuse"] = { diffuse.x, diffuse.y, diffuse.z };
//	object["specular"] = { specular.x, specular.y, specular.z };
//
//	return object;
//}
//
//void Lighting::Light::LoadFromJson(json _content)
//{
//	if (_content.contains("name"))
//	{
//		this->SetName(_content["name"]);
//	}
//	if (_content.contains("active"))
//	{
//		this->SetActive(_content["active"]);
//	}
//	if (_content.contains("type"))
//	{
//		this->SetType(_content["type"]);
//	}
//
//	if (_content.contains("position"))
//	{
//		this->SetPosition(glm::vec3(_content["position"][0], _content["position"][1], _content["position"][2]));
//	}
//	if (_content.contains("direction"))
//	{
//		this->SetDirection(glm::vec3(_content["direction"][0], _content["direction"][1], _content["direction"][2]));
//	}
//
//	if (_content.contains("cutOff"))
//	{
//		this->SetCutOff(_content["cutOff"]);
//	}
//	if (_content.contains("outerCutOff"))
//	{
//		this->SetOuterCutOff(_content["outerCutOff"]);
//	}
//
//	if (_content.contains("constant"))
//	{
//		this->SetConstant(_content["constant"]);
//	}
//	if (_content.contains("linear"))
//	{
//		this->SetLinear(_content["linear"]);
//	}
//	if (_content.contains("quadratic"))
//	{
//		this->SetQuadratic(_content["quadratic"]);
//	}
//
//	if (_content.contains("ambient"))
//	{
//		this->SetAmbient(glm::vec3(_content["ambient"][0], _content["ambient"][1], _content["ambient"][2]));
//	}
//	if (_content.contains("diffuse"))
//	{
//		this->SetDiffuse(glm::vec3(_content["diffuse"][0], _content["diffuse"][1], _content["diffuse"][2]));
//	}
//	if (_content.contains("specular"))
//	{
//		this->SetSpecular(glm::vec3(_content["specular"][0], _content["specular"][1], _content["specular"][2]));
//	}
//}
//
