#include "Animation.h"
#include "Clock.h"
#include "Set.h"
#include "Editor.h"
#define bzero(b,len) (memset((b), '\0', (len)), (void) 0)  

static bool loopTimer = false;
static std::vector<Animations::Animation>* animationCreation = nullptr;
static Animations::Animation* animationRename = nullptr;
static std::vector<Animations::Sequence>* sequenceCreation = nullptr;
static Animations::Sequence* sequenceRename = nullptr;
static int selectedAnimationIndex = -1;

static char nameArray[255] = { NULL };

void Animations::UI::MenuPopUps(GameData* _gameData)
{
	if (animationCreation != nullptr)
	{
		ImGui::OpenPopup("Enter Animation name");
		if (ImGui::BeginPopupModal("Enter Animation name", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			Editor::SetClosureLock(true);
			if (ImGui::IsWindowAppearing())
			{
				ImGui::SetKeyboardFocusHere();
			}
			ImGui::InputText("Animation name", nameArray, IM_ARRAYSIZE(nameArray));
			if (ImGui::Button("OK")
				|| _gameData->window.IsKeyPressed(Keys::ENTER)
				|| _gameData->window.IsKeyPressed(Keys::KP_ENTER))
			{
				Animation newAnimation;
				newAnimation.SetName(nameArray);
				animationCreation->push_back(newAnimation);
				animationCreation = nullptr;
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("Cancel")
				|| _gameData->window.IsKeyPressed(Keys::ESCAPE))
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
			Editor::SetClosureLock(true);
			if (ImGui::IsWindowAppearing())
			{
				ImGui::SetKeyboardFocusHere();
			}
			ImGui::InputText("Animation new name", nameArray, IM_ARRAYSIZE(nameArray));
			if (ImGui::Button("OK")
				|| _gameData->window.IsKeyPressed(Keys::ENTER)
				|| _gameData->window.IsKeyPressed(Keys::KP_ENTER))
			{
				animationRename->SetName(nameArray);
				animationRename = nullptr;
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("Cancel")
				|| _gameData->window.IsKeyPressed(Keys::ESCAPE))
			{
				animationRename = nullptr;
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
	}
	else if (sequenceCreation != nullptr)
	{
		ImGui::OpenPopup("Enter Sequence name");
		if (ImGui::BeginPopupModal("Enter Sequence name", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			Editor::SetClosureLock(true);
			if (ImGui::IsWindowAppearing())
			{
				ImGui::SetKeyboardFocusHere();
			}
			ImGui::InputText("Sequence name", nameArray, IM_ARRAYSIZE(nameArray));
			if (ImGui::Button("OK")
				|| _gameData->window.IsKeyPressed(Keys::ENTER)
				|| _gameData->window.IsKeyPressed(Keys::KP_ENTER))
			{
				Sequence newSequence;
				newSequence.SetName(nameArray);
				sequenceCreation->push_back(newSequence);
				sequenceCreation = nullptr;
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("Cancel")
				|| _gameData->window.IsKeyPressed(Keys::ESCAPE))
			{
				sequenceCreation = nullptr;
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
	}
	else if (sequenceRename != nullptr)
	{
		ImGui::OpenPopup("Enter Sequence new name");
		if (ImGui::BeginPopupModal("Enter Sequence new name", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			Editor::SetClosureLock(true);
			if (ImGui::IsWindowAppearing())
			{
				ImGui::SetKeyboardFocusHere();
			}
			ImGui::InputText("Sequence new name", nameArray, IM_ARRAYSIZE(nameArray));
			if (ImGui::Button("OK")
				|| _gameData->window.IsKeyPressed(Keys::ENTER)
				|| _gameData->window.IsKeyPressed(Keys::KP_ENTER))
			{
				sequenceRename->SetName(nameArray);
				sequenceRename = nullptr;
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("Cancel")
				|| _gameData->window.IsKeyPressed(Keys::ESCAPE))
			{
				sequenceRename = nullptr;
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
	}
}

void Animations::UI::Menu(GameData* _gameData)
{
	Sets::Set* editedSet = Sets::GetEditedSet();
	if (editedSet == nullptr) { selectedAnimationIndex = -1; return; }
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
				Animation& actualAnimation = animations->at(animationId);
				std::string subAnimName("##" + actualAnimation.GetName() + std::to_string(animationId));
				float duration = actualAnimation.GetDuration();
				if (ImGui::TreeNode((actualAnimation.GetName() + subAnimName).c_str()))
				{
					bool selected = (selectedAnimationIndex == animationId) ? true: false;
					if (ImGui::Checkbox(("Select" + subAnimName).c_str(), &selected))
					{
						selectedAnimationIndex = animationId;
					}
					ImGui::Text("Total Duration : %f seconds", duration);
					if (ImGui::Button(("Rename" + subAnimName).c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 0)))
					{
						bzero(nameArray, sizeof(nameArray));
						std::string oldName = actualAnimation.GetName();
						std::memcpy(nameArray, oldName.c_str(), oldName.size() * sizeof(char));
						animationRename = &actualAnimation;
						ImGui::TreePop();
						break;
					}

					// Sequences management
					if (ImGui::TreeNode(("Sequences" + subAnimName).c_str()))
					{
						std::vector<Sequence>* sequences = actualAnimation.GetSequences();
						if (ImGui::Button("Create Sequence", ImVec2(ImGui::GetContentRegionAvail().x, 0)))
						{
							sequenceCreation = sequences;
							bzero(nameArray, sizeof(nameArray));
						}

						for (int sequenceId = 0; sequenceId < sequences->size(); sequenceId++)
						{
							Sequence& actualSequence = sequences->at(sequenceId);
							std::string subSequenceName(subAnimName + actualSequence.GetName() + std::to_string(sequenceId));
							if (ImGui::TreeNode((actualSequence.GetName() + subSequenceName).c_str()))
							{
								if (ImGui::Button(("Rename" + subSequenceName).c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 0)))
								{
									bzero(nameArray, sizeof(nameArray));
									std::string oldName = actualSequence.GetName();
									std::memcpy(nameArray, oldName.c_str(), oldName.size() * sizeof(char));
									sequenceRename = &actualSequence;
									ImGui::TreePop();
									break;
								}

								float duration = actualSequence.GetDuration();
								ImGui::Text("Duration : %f seconds", duration);
								if (ImGui::SliderFloat(("##Duration" + subSequenceName).c_str(), &duration, 0.1f, 2.0f))
								{
									actualSequence.SetDuration(duration);
									actualAnimation.CalculateDuration();
								}

								ImGui::TreePop();
							}
							// Drag And Drop thing
							if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
							{
								ImGui::SetDragDropPayload(subAnimName.c_str(), &sequenceId, sizeof(int));
								ImGui::Text("Move %s", actualSequence.GetName().c_str());
								ImGui::EndDragDropSource();
							}
							if (ImGui::BeginDragDropTarget())
							{
								const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(subAnimName.c_str());
								if (payload)
								{
									int sourceIndex = *(const int*)payload->Data;
									std::swap(actualSequence, sequences->at(sourceIndex));
								}
								ImGui::EndDragDropTarget();
							}
						}
						ImGui::TreePop();
					}

					ImGui::TreePop();
				}
			}
		}
		ImGui::TreePop();
	}

	MenuPopUps(_gameData);
	Bar(_gameData);
}

void Animations::UI::Bar(GameData* _gameData)
{
	Sets::Set* editedSet = Sets::GetEditedSet();
	if (editedSet == nullptr || selectedAnimationIndex == -1) { return; }
	std::vector<Animations::Animation>* animationList = editedSet->GetAnimations();
	Animations::Animation& actualAnimation = animationList->at(selectedAnimationIndex);

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

	Clock* clock = actualAnimation.AccessClock();
	if (clock->GetElapsedTime() >= actualAnimation.GetDuration() * 1000)
	{
		if (loopTimer)
		{
			clock->Restart();
		}
		else
		{
			clock->Pause();
		}
	}
	float time = clock->GetElapsedTime() / 1000;
	if (ImGui::Button("Play", ImVec2(buttonSize, 0)))
	{
		clock->Play();
	}
	ImGui::SameLine();

	if (ImGui::Button("Pause", ImVec2(buttonSize, 0)))
	{
		clock->Pause();
	}
	ImGui::SameLine();

	if (ImGui::Button("Restart", ImVec2(buttonSize, 0)))
	{
		clock->Restart();
		time = 0;
	}
	ImGui::SameLine();

	if (ImGui::Checkbox("Loop Timer", &loopTimer));

	ImGui::PushItemWidth(-1);
	if (ImGui::SliderFloat("##Timebar", &time, 0.0f, actualAnimation.GetDuration(), "Time: %.3f", ImGuiSliderFlags_AlwaysClamp))
	{
		clock->SetElapsedTime(time * 1000);
	}
	ImGui::PopItemWidth();

	ImGui::End();
	actualAnimation.Update();
}