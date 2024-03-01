#include "Animation.h"
#include "Clock.h"

static Clock clocking;

void Animation::Menu(GameData* _gameData)
{
	if (ImGui::TreeNode("Animation"))
	{
		float time = clocking.GetElapsedTime() / 1000;


		if (ImGui::Button("Pause"))
		{
			clocking.Pause();
		}
		ImGui::SameLine();
		if (ImGui::Button("Play"))
		{
			clocking.Play();
		}
		ImGui::SameLine();
		if (ImGui::Button("Restart"))
		{
			clocking.Restart();
			time = 0;
		}

		if (ImGui::SliderFloat("TimerBar", &time, 0.0f, 60.0f))
		{
			
		}


		ImGui::TreePop();
	}
}
