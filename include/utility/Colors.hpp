#ifndef SRC_CORE_UTILITY_COLORS_HPP
#define SRC_CORE_UTILITY_COLORS_HPP

/*!
This file contains definitions of colors for your convenience
*/

#include <glm/glm.hpp>
typedef glm::vec4 Color;
namespace Colors
{
	const Color Transparent =	Color(0.0f, 0.0f, 0.0f, 0.0f);

	const Color White =			Color(1.0f, 1.0f, 1.0f, 1.0f);
	const Color Black =			Color(0.0f, 0.0f, 0.0f, 1.0f);

	// Pink
	const Color Pink =			Color(255/255.0f, 192/255.0f, 203/255.0f, 1.0f);
	const Color HotPink =		Color(255/255.0f, 105/255.0f, 180/255.0f, 1.0f);
	const Color DeepPink =		Color(255/255.0f, 20/255.0f, 147/255.0f, 1.0f);

	// Red
	const Color Salmon =		Color(250/255.0f, 128/255.0f, 114/255.0f, 1.0f);
	const Color Crimson =		Color(220/255.0f, 20/255.0f, 60/255.0f, 1.0f);
	const Color DarkRed =		Color(139/255.0f, 0/255.0f, 0/255.0f, 1.0f);
	const Color Red =			Color(255/255.0f, 0/255.0f, 0/255.0f, 1.0f);

	// Orange
	const Color OrangeRed =		Color(255/255.0f,  69/255.0f, 0/255.0f, 1.0f);
	const Color DarkOrange =	Color(255/255.0f, 140/255.0f, 0/255.0f, 1.0f);
	const Color Orange =		Color(255/255.0f, 165/255.0f, 0/255.0f, 1.0f);
	const Color Gold =			Color(255/255.0f, 215/255.0f, 0/255.0f, 1.0f);

	// Yellow
	const Color Yellow =		Color(255/255.0f, 255/255.0f, 0/255.0f, 1.0f);
	const Color LightYellow =	Color(255/255.0f, 255/255.0f, 22/255.0f, 1.0f);
	const Color PeachPuff =		Color(255/255.0f, 218/255.0f, 185/255.0f, 1.0f);
	const Color Khaki =			Color(240/255.0f, 230/255.0f, 140/255.0f, 1.0f);
	const Color DarkKhaki =		Color(189/255.0f, 183/255.0f, 107/255.0f, 1.0f);

	// Brown
	const Color Wheat =			Color(245/255.0f, 222/255.0f, 179/255.0f, 1.0f);
	const Color RosyBrown =		Color(188/255.0f, 143/255.0f, 143/255.0f, 1.0f);
	const Color Goldenrod =		Color(218/255.0f, 165/255.0f, 32/255.0f, 1.0f);
	const Color Chocolate =		Color(210/255.0f, 105/255.0f, 30/255.0f, 1.0f);
	const Color SaddleBrown =	Color(139/255.0f, 69/255.0f, 19/255.0f, 1.0f);
	const Color Sienna =		Color(160/255.0f, 82/255.0f, 45/255.0f, 1.0f);
	const Color Maroon =		Color(128/255.0f, 0/255.0f, 0/255.0f, 1.0f);

	// Green
	const Color DarkOliveGreen= Color(85/255.0f, 107/255.0f, 47/255.0f, 1.0f);
	const Color Olive =			Color(128/255.0f, 128/255.0f, 0/255.0f, 1.0f);
	const Color YellowGreen =	Color(154/255.0f, 205/255.0f, 50/255.0f, 1.0f);
	const Color LimeGreen =		Color(50/255.0f, 205/255.0f, 50/255.0f, 1.0f);
	const Color Lime =			Color(0/255.0f, 255/255.0f, 0/255.0f, 1.0f);
	const Color MediumSpringGreen = Color(0/255.0f, 250/255.0f, 154/255.0f, 1.0f);
	const Color LightGreen =	Color(144/255.0f, 238/255.0f, 144/255.0f, 1.0f);
	const Color SeaGreen =		Color(46/255.0f, 139/255.0f, 87/255.0f, 1.0f);
	const Color Green =			Color(0/255.0f, 128/255.0f, 0/255.0f, 1.0f);
	const Color DarkGreen =		Color(0/255.0f, 100/255.0f, 0/255.0f, 1.0f);

	// Cyan
	const Color Cyan =			Color(0/255.0f, 255/255.0f, 255/255.0f, 1.0f);
	const Color LightCyan =		Color(224/255.0f, 255/255.0f, 255/255.0f, 1.0f);
	const Color PaleTurquoise =	Color(175/255.0f, 238/255.0f, 238/255.0f, 1.0f);
	const Color Aquamarine =	Color(127/255.0f, 255/255.0f, 212/255.0f, 1.0f);
	const Color Turquoise =		Color(64/255.0f, 224/255.0f, 208/255.0f, 1.0f);
	const Color LightSeaGreen =	Color(32/255.0f, 178/255.0f, 170/255.0f, 1.0f);
	const Color CadetBlue =		Color(95/255.0f, 158/255.0f, 160/255.0f, 1.0f);
	const Color Teal =			Color(0/255.0f, 128/255.0f, 128/255.0f, 1.0f);

	// Blue
	const Color LightSteelBlue =Color(176/255.0f, 196/255.0f, 222/255.0f, 1.0f);
	const Color PowderBlue =	Color(176/255.0f, 224/255.0f, 230/255.0f, 1.0f);
	const Color LightBlue =		Color(173/255.0f, 216/255.0f, 230/255.0f, 1.0f);
	const Color SkyBlue =		Color(135/255.0f, 206/255.0f, 235/255.0f, 1.0f);
	const Color DeepSkyBlue =	Color(0/255.0f, 191/255.0f, 255/255.0f, 1.0f);
	const Color DodgerBlue =	Color(30/255.0f, 144/255.0f, 255/255.0f, 1.0f);
	const Color CornflowerBlue =Color(100/255.0f, 149/255.0f, 237/255.0f, 1.0f);
	const Color SteelBlue =		Color(70/255.0f, 130/255.0f, 180/255.0f, 1.0f);
	const Color Blue =			Color(0/255.0f, 0/255.0f, 255/255.0f, 1.0f);
	const Color DarkBlue =		Color(0/255.0f, 0/255.0f, 139/255.0f, 1.0f);
	const Color MidnightBlue =	Color(25/255.0f, 25/255.0f, 112/255.0f, 1.0f);

	// Purple
	const Color Lavender =		Color(230/255.0f, 230/255.0f, 250/255.0f, 1.0f);
	const Color Violet =		Color(238/255.0f, 130/255.0f, 238/255.0f, 1.0f);
	const Color Magenta =		Color(255/255.0f, 0/255.0f, 255/255.0f, 1.0f);
	const Color MediumOrchid =	Color(186/255.0f, 85/255.0f, 211/255.0f, 1.0f);
	const Color MediumPurple =	Color(147/255.0f, 112/255.0f, 219/255.0f, 1.0f);
	const Color BlueViolet =	Color(138/255.0f, 43/255.0f, 226/255.0f, 1.0f);
	const Color Purple =		Color(128/255.0f, 0/255.0f, 128/255.0f, 1.0f);
	const Color Indigo =		Color(75/255.0f, 0/255.0f, 130/255.0f, 1.0f);
	const Color DarkSlateBlue =	Color(72/255.0f, 61/255.0f, 139/255.0f, 1.0f);
	const Color MediumSlateBlue=Color(123/255.0f, 104/255.0f, 238/255.0f, 1.0f);

	// Gray
	const Color LightGray =		Color(211/255.0f, 211/255.0f, 211/255.0f, 1.0f);
	const Color Silver =		Color(192/255.0f, 192/255.0f, 192/255.0f, 1.0f);
	const Color DarkGray =		Color(169/255.0f, 169/255.0f, 169/255.0f, 1.0f);
	const Color Gray =			Color(128/255.0f, 128/255.0f, 128/255.0f, 1.0f);
	const Color DimGray =		Color(105/255.0f, 105/255.0f, 105/255.0f, 1.0f);
	const Color SlateGray =		Color(112/255.0f, 128/255.0f, 144/255.0f, 1.0f);
	const Color DarkSlateGray =	Color(47/255.0f,  79/255.0f,  79/255.0f, 1.0f);

}

#endif
