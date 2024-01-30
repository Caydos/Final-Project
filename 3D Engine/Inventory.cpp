#include "Inventory.h"
#include "Clock.h"
#include "Blocks.h"

static Clock inputClock;
static bool opened = false;
static std::vector<Blocks::BlockType*> blocks;
static Blocks::BlockType* hotBar[8] = { nullptr };
static ImVec2 containerSize(100, 100);

void Container(ImDrawList* _drawList, ImVec2 _position, GLuint _textureID, const char* _itemName)
{
	ImVec2 rect_min = _position;
	ImVec2 rect_max = ImVec2(_position.x + containerSize.x, _position.y + containerSize.y);

	ImVec4 defaultTextColor = ImGui::GetStyle().Colors[ImGuiCol_Header];

	_drawList->AddRectFilled(rect_min, rect_max, IM_COL32(defaultTextColor.x * 255, defaultTextColor.y * 255, defaultTextColor.z * 255, /*defaultTextColor.w * 255*/ 100)); // color rectangle

	ImVec2 containerPos = _position;

	ImVec2 imageSize = ImVec2(containerSize.x, containerSize.y - 20); // 20px space for the name

	if (_textureID)
	{
		ImVec2 imagePos = containerPos;
		_drawList->AddImage((void*)(intptr_t)_textureID, imagePos, ImVec2(imagePos.x + imageSize.x, imagePos.y + imageSize.y));

		// Draw the name below the image
		ImVec2 textPos = ImVec2(containerPos.x, containerPos.y + imageSize.y);
		_drawList->AddText(textPos, IM_COL32_WHITE, _itemName);
	}

	// Drag source
	if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
	{
		// Set the payload to the container's identifier (could be an index or a pointer)
		ImGui::SetDragDropPayload("CONTAINER_PAYLOAD", "my_draggable_container", sizeof(char) * 24);

		ImGui::Text("Dragging Container");

		ImGui::EndDragDropSource();
	}

	// Drag target (can be the same area for simplicity)
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTAINER_PAYLOAD"))
		{
			// Handle the drop event here
			// e.g., Move the container, process the data, etc.
		}
		ImGui::EndDragDropTarget();
	}
}

void Inventory::Menu(GameData* _gameData)
{
	if (_gameData->window.IsKeyPressed(Keys::E) && inputClock.GetElapsedTime() > 125)
	{
		if (opened)
		{
			_gameData->window.Focus(true);
			opened = false;
		}
		else
		{
			_gameData->window.Focus(false);
			blocks = Blocks::GetAll();
			opened = true;
		}

		inputClock.Restart();
	}
	ImGuiIO& io = ImGui::GetIO();
	ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
	glm::vec2 windowDimensions = _gameData->window.GetDimensions();
	float startY = (windowDimensions.y - 8 * containerSize.y) / 2.0f;

	for (size_t i = 0; i < 8; i++)
	{
		std::string name = "None";
		GLuint textureId = 0;
		if (hotBar[i] != nullptr)
		{
			if (hotBar[i]->GetTexture() != nullptr)
			{
				textureId = hotBar[i]->GetTexture()->id;
			}
			name = hotBar[i]->GetName();
		}
		ImVec2 containerPos = ImVec2(windowDimensions.x - containerSize.x, startY + i * containerSize.y * 1.10);

		Container(draw_list, containerPos, textureId, name.c_str());
	}
	if (opened)
	{
		ImVec2 windowSize(windowDimensions.x * 0.75, windowDimensions.y * 0.75);
		ImGui::SetNextWindowSize(windowSize, ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSizeConstraints(containerSize, windowSize);

		// Start ImGui window
		ImGui::Begin("Inventory");
		ImVec2 currentWindowSize = ImGui::GetWindowSize();
		int columns = currentWindowSize.x / containerSize.x;
		if (columns == 0) { return; } // Security when reducing the window

		// Loop through items and create a button for each
		for (int i = 0; i < blocks.size(); ++i)
		{
			ImGui::PushID(i); // Ensure unique ID for items

			if (i % columns != 0)
			{
				ImGui::SameLine();
			}
			// Create an invisible button to act as our draggable item
			if (ImGui::InvisibleButton("##container", containerSize))
			{//Left click

			}
			//if (ImGui::IsItemClicked(1))
			//{//Right Click
			//    std::cout << "Right click on invisible button detected." << std::endl;
			//}
			ImVec2 rect_min = ImGui::GetItemRectMin();
			ImVec2 rect_max = ImGui::GetItemRectMax();
			ImDrawList* scopeDrawList = ImGui::GetWindowDrawList();
			Container(scopeDrawList, rect_min, blocks[i]->GetTexture()->id, blocks[i]->GetName().c_str());

			ImGui::PopID();
		}

		// End ImGui window
		ImGui::End();
	}
}

bool Inventory::IsActive()
{
	return opened;
}
