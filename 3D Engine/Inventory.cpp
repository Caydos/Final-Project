#include "Inventory.h"
#include "Clock.h"
#include "Blocks.h"

static Clock inputClock;
static bool opened = false;
static std::vector<Blocks::BlockType*> blocks;
static Blocks::BlockType* hotBar[8] = { nullptr };
static int hotBarHovered = 0;

struct GlobalDragState
{
	bool isDragging = false;
	std::string payload;
	void* blockType = nullptr;
};
GlobalDragState draggedItem;

static ImVec2 containerSize(100, 100);
void Container(ImDrawList* _drawList, ImVec2 _position, GLuint _textureID, const char* _itemName, const char* _itemId, void* _pointer, bool _border)
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

	if (_itemId != nullptr)
	{
		ImVec2 textPos = ImVec2(containerPos.x + containerSize.x - 12.5f, containerPos.y + 2.0f);
		_drawList->AddText(textPos, IM_COL32_WHITE, _itemId);
	}

	if (_border)
	{
		ImU32 borderColor = IM_COL32(255, 255, 255, 255); // White border, fully opaque
		float borderWidth = 2.0f; // Border width in pixels
		_drawList->AddRect(rect_min, rect_max, borderColor, 0.0f, 0, borderWidth);
	}

	// Drag source
	if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
	{
		// Update global drag state
		draggedItem.isDragging = true;
		draggedItem.payload = "my_draggable_container";
		draggedItem.blockType = _pointer;

		ImGui::SetDragDropPayload("CONTAINER_PAYLOAD", &draggedItem.payload, sizeof(char) * draggedItem.payload.size());

		ImGui::Text(_itemName);
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
Keys::Keys keys[8] = {
	Keys::NUMBER_ROW_1,
	Keys::NUMBER_ROW_2,
	Keys::NUMBER_ROW_3,
	Keys::NUMBER_ROW_4,
	Keys::NUMBER_ROW_5,
	Keys::NUMBER_ROW_6,
	Keys::NUMBER_ROW_7,
	Keys::NUMBER_ROW_8,
};

void Inventory::Menu(GameData* _gameData)
{
	for (size_t i = 0; i < 8; i++)
	{
		if (_gameData->window.IsKeyPressed(keys[i]) && inputClock.GetElapsedTime() > 125)
		{
			hotBarHovered = i;
			inputClock.Restart();
		}
	}
	if (_gameData->window.IsKeyPressed(Keys::E) && inputClock.GetElapsedTime() > 125)
	{
		if (opened)
		{
			draggedItem.blockType = nullptr;
			draggedItem.isDragging = false;
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
		ImVec2 containerPos = ImVec2(windowDimensions.x - containerSize.x - 10, startY + i * containerSize.y * 1.10);
		ImVec2 containerEndPos = ImVec2(containerPos.x + containerSize.x, containerPos.y + containerSize.y);

		bool isMouseOver = io.MousePos.x >= containerPos.x && io.MousePos.x <= containerEndPos.x &&
			io.MousePos.y >= containerPos.y && io.MousePos.y <= containerEndPos.y;

		// Render the container
		Container(draw_list, containerPos, textureId, name.c_str(), std::to_string(i).c_str(), hotBar[i], (hotBarHovered == i) ? true : false);

		// Start drag
		if (isMouseOver && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
		{
			draggedItem.isDragging = true;
			draggedItem.payload = name;
		}

		// Handle drop (you might need additional logic to determine valid drop targets)
		if (draggedItem.isDragging && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
		{
			if (isMouseOver)
			{
				// Handle drop logic here
				draggedItem.isDragging = false;
				// Other drop logic here
				if (draggedItem.blockType != nullptr)
				{
					hotBar[i] = (Blocks::BlockType*) draggedItem.blockType;
					draggedItem.blockType = nullptr;
				}
			}
		}
	}
	if (opened)
	{
		ImVec2 windowSize(windowDimensions.x * 0.75, windowDimensions.y * 0.75);
		ImGui::SetNextWindowSize(windowSize, ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSizeConstraints(ImVec2(containerSize.x * 1.25f, containerSize.y), windowSize);

		ImGui::Begin("Inventory");
		ImVec2 currentWindowSize = ImGui::GetWindowSize();
		int columns = currentWindowSize.x / (containerSize.x * 1.1);
		if (columns == 0) { return; } // Security when reducing the window

		for (int i = 0; i < blocks.size(); ++i)
		{
			ImGui::PushID(i);

			if (i % columns != 0)
			{
				ImGui::SameLine();
			}

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
			Container(scopeDrawList, rect_min, blocks[i]->GetTexture()->id, blocks[i]->GetName().c_str(), nullptr, blocks[i], false);

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

Blocks::BlockType* Inventory::GetHotBarBlock()
{
	if (blocks.size() <= hotBarHovered) { return nullptr; }
	return hotBar[hotBarHovered];
}
