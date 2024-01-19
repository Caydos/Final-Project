#ifndef COLORS_H
#define COLORS_H
#include <IMGUI/imgui.h>
#include <vector>
#include <string>
#define TO_RGBA(v) (v / 255.0f)



namespace Colors
{
	class Color
	{
	public:
		Color() { values[0] = .0f; values[1] = .0f; values[2] = .0f; values[3] = .0f; }
		Color(float r, float g, float b, float a = 1.0f, std::string _name = "Default")
			: values{ r,g,b,a }, imguiValues{ r,g,b,a }, name(_name) {}

		float values[4];
		std::string name;
		ImVec4 imguiValues;

	};

	static const Color Red(1.0f, 0.0f, 0.0f, 1.0f, "Red");
	static const Color Green(0.0f, 1.0f, 0.0f, 1.0f, "Green");
	static const Color Blue(0.0f, 0.0f, 1.0f, 1.0f, "Blue");
	static const Color White(1.0f, 1.0f, 1.0f, 1.0f, "White");
	static const Color Black(0.0f, 0.0f, 0.0f, 1.0f, "Black");
	static const Color LimedSpruce(0.2f, 0.3f, 0.3f, 1.0f, "Limed Spruce");

	static const Color DarkGrey(TO_RGBA(60.0f), TO_RGBA(60.0f), TO_RGBA(60.0f), 1.0f, "Dark Grey");
	static const Color Grey(TO_RGBA(120.0f), TO_RGBA(120.0f), TO_RGBA(120.0f), 1.0f, "Grey");
	static const Color BrightGrey(TO_RGBA(180.0f), TO_RGBA(180.0f), TO_RGBA(180.0f), 1.0f, "Bright Grey");

	static const ImVec4 DefaultColors[] =
	{
		Red.imguiValues,
		Green.imguiValues,
		Blue.imguiValues,
		White.imguiValues,
		Black.imguiValues,
		LimedSpruce.imguiValues,
		DarkGrey.imguiValues,
		Grey.imguiValues,
		BrightGrey.imguiValues
	};
}


#endif // !COLORS_H