#include "Animation.h"
#include "Clock.h"
#include "Set.h"
#define bzero(b,len) (memset((b), '\0', (len)), (void) 0)  

static Clock clocking;
static std::vector<std::string> sequences; //Stocker le nom des séquences
static int sequenceCounter = 0; //Compteur pour générer des noms
static bool renamingSequence = false; // Indicateur de renommage en cours
static std::string newSequenceName; // Nouveau nom de la séquence en cours de renommage
static int renamingSequenceIndex = -1;
static bool loopTimer = false;
static std::vector<Animations::Animation>* animationCreation = nullptr;
static Animations::Animation* animationRename = nullptr;

static char nameArray[255] = { NULL };

void Animations::UI::Menu(GameData* _gameData)
{
	Sets::Set* editedSet = Sets::GetEditedSet();
	if (editedSet == nullptr) { return; }
	if (ImGui::TreeNode("Animations"))
	{
		std::vector<Animation>* animations = editedSet->GetAnimations();
		if (ImGui::Button("Create Animation", ImVec2(ImGui::GetContentRegionAvail().x, 0)))
		{
			animationCreation = animations;
			bzero(nameArray, sizeof(nameArray));
		}
		else
		{
			for (size_t animationId = 0; animationId < animations->size(); animationId++)
			{
				std::string subName("##" + animations->at(animationId).GetName() + std::to_string(animationId));
				if (ImGui::TreeNode((animations->at(animationId).GetName() + subName).c_str()))
				{
					if (ImGui::Button(("Rename" + subName).c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 0)))
					{
						bzero(nameArray, sizeof(nameArray));
						std::string oldName = animations->at(animationId).GetName();
						std::memcpy(nameArray, oldName.c_str(), oldName.size() * sizeof(char));
						animationRename = &animations->at(animationId);
						ImGui::TreePop();
						break;
					}
					ImGui::TreePop();
				}
			}
		}
		//if (ImGui::TreeNode("Sequence"))
		//{
		//    if (ImGui::Button("Create Sequence"))
		//    {
		//        sequences.push_back("Sequence " + std::to_string(sequenceCounter++));
		//    }

		//    for (int i = 0; i < sequences.size(); i++)
		//    {
		//        if (ImGui::TreeNode(sequences[i].c_str()))
		//        {
		//            if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
		//            {
		//                ImGui::SetDragDropPayload("SEQUENCE", &i, sizeof(int));
		//                ImGui::Text("Move %s", sequences[i].c_str());
		//                ImGui::EndDragDropSource();
		//            }

		//            if (ImGui::BeginDragDropTarget())
		//            {
		//                const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SEQUENCE");
		//                if (payload)
		//                {
		//                    int sourceIndex = *(const int*)payload->Data;
		//                    std::swap(sequences[i], sequences[sourceIndex]);
		//                }
		//                ImGui::EndDragDropTarget();
		//            }

		//            if (ImGui::Button(("Rename##" + std::to_string(i)).c_str()))
		//            {
		//                renamingSequenceIndex = i;
		//                renamingSequence = true;
		//                newSequenceName = sequences[i];
		//            }

		//            ImGui::TreePop();
		//        }
		//    }

		//    ImGui::TreePop();
		//}

		ImGui::TreePop();
	}

	if (animationCreation != nullptr)
	{
		ImGui::OpenPopup("Enter Animation name");
		if (ImGui::BeginPopupModal("Enter Animation name", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::InputText("Animation name", nameArray, IM_ARRAYSIZE(nameArray));
			if (ImGui::Button("OK"))
			{
				Animation newAnimation;
				newAnimation.SetName(nameArray);
				animationCreation->push_back(newAnimation);
				animationCreation = nullptr;
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("Cancel"))
			{
				animationCreation = nullptr;
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
	}
	else if (animationRename != nullptr)
	{
		ImGui::OpenPopup("Enter Animation new name");
		if (ImGui::BeginPopupModal("Enter Animation new name", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::InputText("Animation new name", nameArray, IM_ARRAYSIZE(nameArray));
			if (ImGui::Button("OK"))
			{
				animationRename->SetName(nameArray);
				animationRename = nullptr;
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("Cancel"))
			{
				animationRename = nullptr;
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
	}
	Bar(_gameData);
}

void Animations::UI::Bar(GameData* _gameData)
{
	if (loopTimer)
	{

	}
	ImVec2 window_pos, window_pos_pivot;
	ImGuiIO& io = ImGui::GetIO();

	ImVec2 windowSize(io.DisplaySize.x / 4, _gameData->resolution[0] * 0.03);
	ImGui::SetNextWindowSize(windowSize);
	ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x / 2 - (windowSize.x) / 2, io.DisplaySize.y - windowSize.y));
	ImGui::SetNextWindowBgAlpha(0.5f);

	ImGui::Begin("Playback Controls", nullptr,
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoSavedSettings);

	float buttonSize = windowSize.x * 0.4 / 3;
	float totalButtonsWidth = 3 * buttonSize;
	ImGui::SetCursorPosX(windowSize.x / 2 - totalButtonsWidth / 2);


	float time = clocking.GetElapsedTime() / 1000;
	if (ImGui::Button("Play", ImVec2(buttonSize, 0)))
	{
		clocking.Play();
	}
	ImGui::SameLine();

	if (ImGui::Button("Pause", ImVec2(buttonSize, 0)))
	{
		clocking.Pause();
	}
	ImGui::SameLine();

	if (ImGui::Button("Restart", ImVec2(buttonSize, 0)))
	{
		clocking.Restart();
		time = 0;
	}
	ImGui::SameLine();

	if (ImGui::Checkbox("Loop Timer", &loopTimer));

	ImGui::PushItemWidth(-1);
	if (ImGui::SliderFloat("##Timebar", &time, 0.0f, 100.0f, "Time: %.3f", ImGuiSliderFlags_AlwaysClamp))
	{
		clocking.SetElapsedTime(time * 1000);
	}
	ImGui::PopItemWidth();

	if (renamingSequence)
	{
		ImGui::OpenPopup("Rename Sequence");
		renamingSequence = false;
	}

	if (ImGui::BeginPopupModal("Rename Sequence"))
	{
		ImGui::Text("New name:");
		ImGui::InputText("##newSequenceName", &newSequenceName[0], newSequenceName.size() + 1);
		if (ImGui::Button("OK", ImVec2(120, 0)))
		{
			// Mettre à jour le nom de la séquence
			sequences[renamingSequenceIndex] = newSequenceName;
			newSequenceName.clear();
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0)))
		{
			newSequenceName.clear();
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	ImGui::End();
}