#ifndef CROSSHAIR_H
#define CROSSHAIR_H
#include "Colors.h"
#include "Line.h"

#define CROSSHAIR_DIRECTORY "../Crosshairs/"
#define CROSSHAIR_FILE_EXTENSION ".json"

namespace Crosshairs
{
	class Crosshair
	{
	public:
		Crosshair();
		~Crosshair();
		void Initialize();
		std::string GetName();
		void SetName(std::string _name);

		bool IsActive();
		void SetActive(bool _active = true);

		Colors::Color GetColor();
		void SetColor(Colors::Color _color);

		float GetHorizontalSize();
		void SetHorizontalSize(float _size);
		float GetVerticalSize();
		void SetVerticalSize(float _size);

		void LoadFromFile(const char* _fileName);
		void Save();

		void Draw();

	private:
		std::string name;
		bool active;
		Colors::Color color;
		float horizontalSize;
		float verticalSize;
		Line lines[2];
	};
	Crosshair* Get();
	void Draw();
	void Menu(GameData* _gameData);
}

#endif // !CROSSHAIR_H