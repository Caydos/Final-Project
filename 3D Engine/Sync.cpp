#include "Sync.h"
#include "Network.h"
#include "Peds.h"

//static json syncData;
static std::thread syncThread;
//static Peds::Ped* playerPed = nullptr;

void Synchronization::Loop(void)
{
	while (true)
	{/*always send player data and each X time send entity data as encrypted tags*/
		//syncData.clear();
		//if (playerPed != nullptr)
		//{
		//	Camera* cam = Scene::World::GetCamera();
		//	playerPed->SetPosition(cam->Position);
		//}

		//std::vector<Peds::Ped*>* peds = Peds::Get();
		//json pedArray;
		//std::cout << "Amount of peds : " << peds->size() << std::endl;
		//for (size_t i = 0; i < peds->size(); i++)
		//{
		//	// Actual ped
		//	Peds::Ped* ped = peds->at(i);
		//	if (ped->GetOwnerServerId() == GetServerId())
		//	{
		//		json pedContent;
		//		glm::vec3 position = ped->GetPosition();
		//		pedContent["serverId"] = ped->GetServerId();
		//		pedContent["position"] = {
		//			position.x,
		//			position.y,
		//			position.z
		//		};
		//		pedArray.push_back(pedContent);
		//	}
		//}
		//syncData["peds"] = pedArray;
		//std::string dump = syncData.dump();
		//if (dump.size() > 10)
		//{
		//	Network::Events::TriggerServer("Sync::PlayerUpdate", .0f, .0f, .0f, syncData.dump());
		//}
		std::this_thread::sleep_for(std::chrono::milliseconds(SYNC_TIME));
	}
}


void Synchronization::NodeCallback(char** _args)//callback from server
{
	char* entArray = _args[0];
	//if (entArray != nullptr)
	//{
	//	try {
	//		json syncData = json::parse(entArray);
	//		std::vector<Peds::Ped*>* peds = Peds::Get();
	//		if (syncData.contains("peds") && syncData["peds"].is_array())
	//		{
	//			for (auto& pedData : syncData["peds"])
	//			{
	//				if (pedData.contains("serverId"))
	//				{
	//					bool found = false;
	//					for (size_t i = 0; i < peds->size(); i++)
	//					{
	//						if (peds->at(i)->GetServerId() == pedData["serverId"])
	//						{
	//							found = true;
	//							if (pedData.contains("position"))
	//							{
	//								float x = pedData["position"][0];
	//								float y = pedData["position"][1];
	//								float z = pedData["position"][2];
	//								peds->at(i)->SetPosition(glm::vec3(x,y,z));
	//							}
	//							if (pedData.contains("ownerServerId"))
	//							{
	//								peds->at(i)->SetOwnerServerId(pedData["ownerServerId"]);
	//							}
	//							if (pedData.contains("model"))
	//							{
	//								std::string modelName = pedData["model"];
	//								//peds->at(i)->GetModel()->LoadFromFile(modelName.c_str());
	//							}
	//						}
	//					}
	//					if (!found && pedData.contains("ownerServerId"))
	//					{
	//						std::cout << "Not found so inserting ped." << std::endl;
	//						Peds::Ped* ped = new Peds::Ped;
	//						if (pedData.contains("model"))
	//						{
	//							std::string modelName = pedData["model"];
	//							Models::Model* model = new Models::Model(modelName);
	//							Scene::World::QueueLoading(model, modelName);
	//							ped->SetModel(model);
	//						}
	//						if (pedData.contains("position"))
	//						{
	//							ped->SetPosition(glm::vec3(pedData["position"][0], pedData["position"][1], pedData["position"][2]));
	//						}
	//						ped->SetOwnerServerId(pedData["ownerServerId"]);
	//						ped->SetServerId(pedData["serverId"]);
	//						Peds::Insert(ped);
	//					}
	//				}
	//			}
	//		}
	//		std::cout << "Got server callback : " << entArray << std::endl;
	//		}
	//	catch (json::parse_error& e)
	//	{
	//		std::cerr << "JSON parsing error: " << e.what() << '\n';
	//	}
	//}
	//else
	//{
	//	std::cout << "No data to transfer" << std::endl;
	//}
}
static int serverId = -1;
void Established(char** _args)
{
	char* message = _args[0];
	serverId = ToInt(_args[1]);

	std::cout << message << std::endl;
	std::cout << "My server id is : " << serverId << "." << std::endl;

	Network::Events::TriggerServer("Sync::PlayerJoined", 0.0f, 0.0f, 0.0f, "HAZMAT_01");
	syncThread = std::thread(&Synchronization::Loop);
	syncThread.detach();
}

void Welcome(char** _args)
{
	int serverId = ToInt(_args[0]);
	//playerPed = new Peds::Ped;
	//playerPed->SetPosition(glm::vec3(0.f, 0.f, 0.f));
	//playerPed->SetOwnerServerId(Synchronization::GetServerId());
	//playerPed->SetServerId(serverId);
	//Peds::Insert(playerPed);
}

void Synchronization::PedSyncEvents(void)
{
	Network::Events::Register("Nodes::Callback", &Synchronization::NodeCallback);
	Network::Events::Register("Connections::Established", &Established);
	Network::Events::Register("Welcome", &Welcome);
}
int Synchronization::GetServerId()
{
	return serverId;
}

