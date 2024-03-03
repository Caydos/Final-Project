#include "Animation.h"
#include "Clock.h"

static Clock clocking;

void Animation::SubMenu(GameData* _gameData)
{
	if (ImGui::TreeNode("Animation"))
	{
        
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

    ImGui::PushItemWidth(-1);
    ImGui::SliderFloat("##Timebar", &time, 0.0f, 100.0f, "Time: %.3f", ImGuiSliderFlags_AlwaysClamp);
    ImGui::PopItemWidth();

    ImGui::End();
}
