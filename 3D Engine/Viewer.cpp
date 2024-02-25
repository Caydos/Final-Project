//#include "Viewer.h"
//#include "Cube.h"
//#include "Quad.h"
//#include "Line.h"
//#include "DefaultVertices.h"
//#include "RayCasting.h"
//#include "Keys.h"
//#include "Files.h"
//#include "TexturePicker.h"
//#include "Clock.h"
//#include <algorithm>
//
//static bool initialized = false;
//std::vector<Cube> objects;
//std::vector<Texture> textures;
//Line line[2];
//float editorScale = 1.0f;
//Cube* selectedObject;
//Cube ghostObject;
//unsigned int defaultObjectVAO;
//RayCasting::Face hittedFace;
//static Clock inputClock;
//
//// lighting
//glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
//
//void Viewer::Initialize(GameData* _gameData)
//{
//	_gameData->shaders[Shaders::WORLD_OBJECT]->use();
//	_gameData->shaders[Shaders::WORLD_OBJECT]->setInt("material.diffuse", 0);
//	_gameData->shaders[Shaders::WORLD_OBJECT]->setInt("material.specular", 1);
//
//
//	inputClock.Restart();
//
//	std::vector<std::string> paths = Files::GetAllAtPath("../Textures/Blocks/");
//	textures.resize(paths.size());
//
//	for (int index = 0; index < paths.size(); index++)
//	{
//		std::string path = "../Textures/Blocks/";
//		path += paths[index];
//		textures[index].LoadFromFile(path.c_str());
//		textures[index].name = paths[index];
//	}
//	//TexturePicker::Initialize(_gameData, &textures);
//
//	line[0].Initialize();
//	line[0].BindShader(_gameData->shaders[Shaders::UI]);
//	line[0].SetScale(SCREEN_WIDTH * 0.01f, SCREEN_WIDTH * 0.01f, .0f);
//	line[0].SetPosition(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, .0f);
//	line[1].Initialize();
//	line[1].BindShader(_gameData->shaders[Shaders::UI]);
//	line[1].SetScale(SCREEN_WIDTH * 0.01f, SCREEN_WIDTH * 0.01f, .0f);
//	line[1].SetPosition(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, .0f);
//	//line[1].SetRotation(90.0f);
//
//	Cube defaultObj;
//	defaultObj.Initialize();
//	defaultObj.scale = { editorScale,editorScale,editorScale };
//	defaultObj.position = { 0.0,0.0,-5.0 };
//	defaultObj.BindShader(_gameData->shaders[Shaders::WORLD_OBJECT]);
//	defaultObj.SetTexture(&textures[5]);
//	objects.push_back(defaultObj);
//	defaultObjectVAO = objects[0].VAO;
//
//	ghostObject.Initialize();
//	ghostObject.scale = { editorScale,editorScale,editorScale };
//	ghostObject.BindShader(_gameData->shaders[Shaders::WORLD_OBJECT]);
//	ghostObject.SetColor(Colors::White);
//	ghostObject.opacity = 0.5f;
//	ghostObject.SetLightDependency(false);
//
//	initialized = true;
//}
//
//void Viewer::Inputs(GameData* _gameData)
//{
//	if (_gameData->window.IsKeyPressed(Keys::ESCAPE))
//	{
//		_gameData->window.Close(true);
//	}
//	if (!TexturePicker::IsActive())
//	{
//		if (_gameData->window.IsKeyPressed(Keys::W))
//		{
//			_gameData->camera.ProcessKeyboard(FORWARD, _gameData->dt);
//		}
//		if (_gameData->window.IsKeyPressed(Keys::S))
//		{
//			_gameData->camera.ProcessKeyboard(BACKWARD, _gameData->dt);
//		}
//		if (_gameData->window.IsKeyPressed(Keys::A))
//		{
//			_gameData->camera.ProcessKeyboard(LEFT, _gameData->dt);
//		}
//		if (_gameData->window.IsKeyPressed(Keys::D))
//		{
//			_gameData->camera.ProcessKeyboard(RIGHT, _gameData->dt);
//		}
//		if (_gameData->window.IsKeyPressed(Keys::SPACE))
//		{
//			_gameData->camera.ProcessKeyboard(UP, _gameData->dt);
//		}
//		if (_gameData->window.IsKeyPressed(Keys::LEFT_CONTROL))
//		{
//			_gameData->camera.ProcessKeyboard(DOWN, _gameData->dt);
//		}
//	}
//
//	if (inputClock.GetElapsedTime() > 125)
//	{
//		if (_gameData->window.IsKeyPressed(Keys::MouseButtons::MOUSE_BUTTON_LEFT))
//		{
//			Cube* inHand = TexturePicker::GetHotBarItem();
//			if (inHand->texture != nullptr)
//			{
//				if (!TexturePicker::IsActive() && selectedObject != nullptr)
//				{
//					Cube defaultObj;
//					defaultObj.Initialize();
//					defaultObj.scale = { editorScale,editorScale,editorScale };
//					defaultObj.position = { selectedObject->position.x,selectedObject->position.y,selectedObject->position.z };
//					switch (hittedFace) {
//					case RayCasting::FRONT: {defaultObj.position.z += editorScale; break; }
//					case RayCasting::BACK: {defaultObj.position.z -= editorScale; break; }
//					case RayCasting::LEFT: {defaultObj.position.x -= editorScale; break; }
//					case RayCasting::RIGHT: {defaultObj.position.x += editorScale; break; }
//					case RayCasting::TOP: {defaultObj.position.y += editorScale; break; }
//					case RayCasting::BOTTOM: {defaultObj.position.y -= editorScale; break; }
//					default: break;
//					}
//					defaultObj.BindShader(_gameData->shaders[Shaders::WORLD_OBJECT]);
//					defaultObj.SetTexture(inHand->texture);
//					objects.push_back(defaultObj);
//				}
//			}
//			inputClock.Restart();
//		}
//		if (_gameData->window.IsKeyPressed(Keys::MouseButtons::MOUSE_BUTTON_RIGHT))
//		{
//			if (!TexturePicker::IsActive() && selectedObject != nullptr)
//			{
//				if (objects.size() - 1 > 0 && selectedObject->VAO != defaultObjectVAO)
//				{
//					if (selectedObject >= objects.data() && selectedObject < objects.data() + objects.size()) {
//						// Calculate the index
//						auto index = selectedObject - objects.data();
//
//						// Erase the element
//						objects.erase(objects.begin() + index);
//					}
//					else {
//						std::cout << "Pointer does not point to an element in the vector." << std::endl;
//					}
//				}
//			}
//			inputClock.Restart();
//		}
//	}
//}
//
//void Viewer::Tick(GameData* _gameData)
//{
//	if (!initialized) { Initialize(_gameData); }
//	Inputs(_gameData);
//	TexturePicker::Inputs(_gameData);
//
//	selectedObject = nullptr;
//	_gameData->window.Clear();
//	if (!TexturePicker::IsActive())
//	{
//		_gameData->camera.ProcessMouseMovement(_gameData->window.xoffset, _gameData->window.yoffset, true, _gameData->dt);
//		_gameData->window.xoffset = .0f;
//		_gameData->window.yoffset = .0f;
//	}
//#pragma region 3D Objects
//
//	{
//		_gameData->shaders[Shaders::WORLD_OBJECT]->use();
//		_gameData->shaders[Shaders::WORLD_OBJECT]->setVec3("viewPos", _gameData->camera.Position);
//		_gameData->shaders[Shaders::WORLD_OBJECT]->setFloat("material.shininess", 32.0f);
//
//		Colors::Color lightColor(0.4f, 0.4f, 0.4f);
//
//		// directional light
//		_gameData->shaders[Shaders::WORLD_OBJECT]->setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
//		_gameData->shaders[Shaders::WORLD_OBJECT]->setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
//		_gameData->shaders[Shaders::WORLD_OBJECT]->setVec3("dirLight.diffuse", lightColor.values[0], lightColor.values[1], lightColor.values[2]);
//		_gameData->shaders[Shaders::WORLD_OBJECT]->setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
//
//		// spotLight
//		_gameData->shaders[Shaders::WORLD_OBJECT]->setVec3("spotLight.position", _gameData->camera.Position);
//		_gameData->shaders[Shaders::WORLD_OBJECT]->setVec3("spotLight.direction", _gameData->camera.Front);
//		_gameData->shaders[Shaders::WORLD_OBJECT]->setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
//		_gameData->shaders[Shaders::WORLD_OBJECT]->setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
//		_gameData->shaders[Shaders::WORLD_OBJECT]->setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
//		_gameData->shaders[Shaders::WORLD_OBJECT]->setFloat("spotLight.constant", 1.0f);
//		_gameData->shaders[Shaders::WORLD_OBJECT]->setFloat("spotLight.linear", 0.09f);
//		_gameData->shaders[Shaders::WORLD_OBJECT]->setFloat("spotLight.quadratic", 0.032f);
//		_gameData->shaders[Shaders::WORLD_OBJECT]->setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
//		_gameData->shaders[Shaders::WORLD_OBJECT]->setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
//
//		// pass projection matrix to shader (note that in this case it could change every frame)
//		glm::mat4 projection = glm::perspective(glm::radians(_gameData->camera.Fov), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
//		_gameData->shaders[Shaders::WORLD_OBJECT]->setMat4("projection", projection);
//		// camera/view transformation
//		glm::mat4 view = _gameData->camera.GetViewMatrix();
//		_gameData->shaders[Shaders::WORLD_OBJECT]->setMat4("view", view);
//
//		// Assuming your crosshair is at the center of the screen
//		glm::vec4 ray_clip = glm::vec4(0.0, 0.0, -1.0, 1.0);
//
//		// Convert to Eye Space
//		glm::mat4 inv_projection = glm::inverse(projection);
//		glm::vec4 ray_eye = inv_projection * ray_clip;
//		ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0); // Homogenize
//
//		// Convert to World Space
//		glm::mat4 inv_view = glm::inverse(view);
//		glm::vec4 ray_wor = inv_view * ray_eye;
//		// Don't forget to normalize the ray direction
//		glm::vec3 ray_world_direction = glm::normalize(glm::vec3(ray_wor));
//
//		RayCasting::Ray ray;
//		ray.origin = _gameData->camera.Position;
//		ray.direction = ray_world_direction;
//
//		glm::vec3 position = _gameData->camera.Position;
//		std::sort(objects.begin(), objects.end(), [position](const Cube& a, const Cube& b) {
//			float distA = glm::length(a.position - position);
//			float distB = glm::length(b.position - position);
//			return distA < distB;
//			});
//
//		float closest = 100.0f;
//		for (auto it = objects.begin(); it != objects.end(); ++it)
//		{
//			glm::vec3 boxPosition = it->position;
//			float halfSize = 0.5; // Since scale is 1.0, half-size on each axis is 0.5
//
//			RayCasting::AABB boxAABB;
//			boxAABB.min = boxPosition - glm::vec3(halfSize, halfSize, halfSize);
//			boxAABB.max = boxPosition + glm::vec3(halfSize, halfSize, halfSize);
//			float rslt = RayCasting::Intersect(ray, boxAABB);
//			if (rslt > .0f && rslt < closest)
//			{
//				//Face plane Intersection
//				RayCasting::Cube cube = { it->position, it->scale.x };
//				hittedFace = IntersectedFace(ray, cube);
//				closest = rslt;
//				it->Draw();
//				selectedObject = &(*it);
//				continue;
//			}
//			it->Draw();
//		}
//		if (selectedObject != nullptr && TexturePicker::GetHotBarItem()->texture != nullptr)
//		{
//			ghostObject.position = selectedObject->position;
//			switch (hittedFace) {
//			case RayCasting::FRONT: {ghostObject.position.z += editorScale; break; }
//			case RayCasting::BACK: {ghostObject.position.z -= editorScale; break; }
//			case RayCasting::LEFT: {ghostObject.position.x -= editorScale; break; }
//			case RayCasting::RIGHT: {ghostObject.position.x += editorScale; break; }
//			case RayCasting::TOP: {ghostObject.position.y += editorScale; break; }
//			case RayCasting::BOTTOM: {ghostObject.position.y -= editorScale; break; }
//			default: break;
//			}
//			ghostObject.Draw();
//		}
//	}
//#pragma endregion
//	//glDisable(GL_DEPTH_TEST);
//	TexturePicker::Render(_gameData);
//	if (!TexturePicker::IsActive())
//	{
//		line[0].Draw();
//		line[1].Draw();
//	}
//	//glEnable(GL_DEPTH_TEST);
//	_gameData->window.Events();
//}
//
//void Viewer::CleanUp()
//{
//}