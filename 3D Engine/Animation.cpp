#include "Animation.h"
#include "Clock.h"

static Clock clocking;
static std::vector<std::string> sequences; //Stocker le nom des séquences
static int sequenceCounter = 0; //Compteur pour générer des noms
static bool renamingSequence = false; // Indicateur de renommage en cours
static std::string newSequenceName; // Nouveau nom de la séquence en cours de renommage
int renamingSequenceIndex = -1;
bool loopTimer = false;

void Animation::SubMenu(GameData* _gameData)
{
    if (ImGui::TreeNode("Animation"))
    {
        if (ImGui::TreeNode("Sequence"))
        {
            if (ImGui::Button("Create Sequence"))
            {
                sequences.push_back("Sequence " + std::to_string(sequenceCounter++));
            }

            for (int i = 0; i < sequences.size(); i++)
            {
                if (ImGui::TreeNode(sequences[i].c_str()))
                {
                    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
                    {
                        ImGui::SetDragDropPayload("SEQUENCE", &i, sizeof(int));
                        ImGui::Text("Move %s", sequences[i].c_str());
                        ImGui::EndDragDropSource();
                    }

                    if (ImGui::BeginDragDropTarget())
                    {
                        const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SEQUENCE");
                        if (payload)
                        {
                            int sourceIndex = *(const int*)payload->Data;
                            std::swap(sequences[i], sequences[sourceIndex]);
                        }
                        ImGui::EndDragDropTarget();
                    }

                    if (ImGui::Button(("Rename##" + std::to_string(i)).c_str()))
                    {
                        renamingSequenceIndex = i;
                        renamingSequence = true;
                        newSequenceName = sequences[i];
                    }

                    ImGui::TreePop();
                }
            }

            ImGui::TreePop();
        }

        ImGui::TreePop();
    }
}

void Animation::Menu(GameData* _gameData)
{
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

    if (ImGui::Checkbox("Loop Timer", &loopTimer))
    {
        // Si la checkbox est cochée, le timer se relance en boucle.
        if (loopTimer)
        {
            //Relancer le timer du debut quand il atteint sa valeur max

        }
        else //Sinon
        {
            // Si la checkbox n'est pas cochée, le timer s'arrete bien quand il atteint sa valeur max

        }
    }

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