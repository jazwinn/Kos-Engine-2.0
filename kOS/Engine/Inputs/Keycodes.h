/*!
\file      Keycodes.h
\author    Elijah Teo, teo.e , 2301530
\par       teo.e@digipen.edu
\date      Oct 27, 2024
\brief     This file holds the enums used for queries about if a button is pressed, triggered or released.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#pragma once
#include <cstdint>
#include "Config/pch.h"
using keyCode = uint16_t;

namespace keys {
	enum : keyCode {
		// From glfw3.h
		LMB = 0,
		RMB = 1,
		MMB = 2,
		CONTROLLER_A = LMB,
		CONTROLLER_B = RMB,
		CONTROLLER_X = MMB,
		CONTROLLER_Y = 3,
		CONTROLLER_LBUMP = 4,
		CONTROLLER_RBUMP = 5,
		CONTROLLER_BACK = 6,
		CONTROLLER_START = 7,
		CONTROLLER_GUIDE = 8,
		CONTROLLER_LTHUMB = 9,
		CONTROLLER_RTHUMB = 10,
		CONTROLLER_UP = 11,
		CONTROLLER_RIGHT = 12,
		CONTROLLER_DOWN = 13,
		CONTROLLER_LEFT = 14,
		CONTROLLER_LEFT_TRIGGER = 15,
		CONTROLLER_RIGHT_TRIGGER = 16,
		CONTROLLER_LEFT_AXIS_X = LMB,
		CONTROLLER_LEFT_AXIS_Y = RMB,
		CONTROLLER_RIGHT_AXIS_X = MMB,
		CONTROLLER_RIGHT_AXIS_Y = CONTROLLER_Y,


        SPACE = 32,
		APOSTROPHE = 39, /* ' */
		COMMA = 44, /* , */
		MINUS = 45, /* - */
		PERIOD = 46, /* . */
		SLASH = 47, /* / */

        NUM0 = 48, /* 0 */
        NUM1 = 49, /* 1 */
        NUM2 = 50, /* 2 */
        NUM3 = 51, /* 3 */
        NUM4 = 52, /* 4 */
        NUM5 = 53, /* 5 */
        NUM6 = 54, /* 6 */
        NUM7 = 55, /* 7 */
        NUM8 = 56, /* 8 */
        NUM9 = 57, /* 9 */

		SEMICOLON = 59, /* ; */
		EQUAL = 61, /* = */

		A = 65,
		B = 66,
		C = 67,
		D = 68,
		E = 69,
		F = 70,
		G = 71,
		H = 72,
		I = 73,
		J = 74,
		K = 75,
		L = 76,
		M = 77,
		N = 78,
		O = 79,
		P = 80,
		Q = 81,
		R = 82,
		S = 83,
		T = 84,
		U = 85,
		V = 86,
		W = 87,
		X = 88,
		Y = 89,
		Z = 90,


        ESC = 256,
		ENTER = 257,
		TAB = 258,
		BACKSPACE = 259,
		INSERT = 260,
		DEL = 261,
		RIGHT = 262,
		LEFT = 263,
		DOWN = 264,
		UP = 265,
		F1 = 290,
		F2 = 291,
		F3 = 292,
		F4 = 293,
		F5 = 294,
		F6 = 295,
		F7 = 296,
		F8 = 297,
		F9 = 298,
		F10 = 299,
		F11 = 300,
		F12 = 301,

		LeftShift = 340,
		LeftControl = 341,
		LeftAlt = 342,
		RightShift = 344,
		RightControl = 345,
		RightAlt = 346,

    };
}