/*
	 _____                            ___________ _   __
	/  ___|                          /  ___|  _  \ | / /
	\ `--.  ___  _   _ _ __ ___ ___  \ `--.| | | | |/ /
	 `--. \/ _ \| | | | '__/ __/ _ \  `--. \ | | |    \
	/\__/ / (_) | |_| | | | (_|  __/ /\__/ / |/ /| |\  \
	\____/ \___/ \__,_|_|  \___\___| \____/|___/ \_| \_/

	Designed for Counter-Strike: Global Offensive
	Copyright: ValvE Corporation 2017 (C)
*/

#pragma once
#include "InputEnums.h"

SOURCE_INIT

//-----------------------------------------------------------------------------
// Macro to get at joystick codes
//-----------------------------------------------------------------------------
#define JOYSTICK_AXIS_INTERNAL( _joystick, _axis ) ( JOYSTICK_FIRST_AXIS + ((_joystick) * MAX_JOYSTICK_AXES) + (_axis) )
#define JOYSTICK_AXIS( _joystick, _axis ) ( (AnalogCode_t)JOYSTICK_AXIS_INTERNAL( _joystick, _axis ) )


//-----------------------------------------------------------------------------
// Enumeration for analog input devices. Includes joysticks, mousewheel, mouse
//-----------------------------------------------------------------------------
enum AnalogCode_t
{
	ANALOG_CODE_INVALID = -1,
	MOUSE_X = 0,
	MOUSE_Y,
	MOUSE_XY,		// Invoked when either x or y changes
	MOUSE_WHEEL,

	JOYSTICK_FIRST_AXIS,
	JOYSTICK_LAST_AXIS = JOYSTICK_AXIS_INTERNAL(MAX_JOYSTICKS - 1, MAX_JOYSTICK_AXES - 1),

	ANALOG_CODE_LAST,
};

SOURCE_END