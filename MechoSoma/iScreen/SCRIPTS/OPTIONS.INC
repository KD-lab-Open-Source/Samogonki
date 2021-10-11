
// Options screen...

#define OPTIONS_MENU_START_Y	-40

#define OPTIONS_MENU_SIZE  7

#define GRAPHICS_Y	0
#define SOUND_Y 	1
#define CONTROLS_Y	2

#define TB_Y		4
#define SPLIT_Y		5
#define HS_Y		6

/*
object {
	ID	$ACS_NEW_GAME_OPTION
	type	$ACS_INPUT_FIELD_OBJ
	$ALPHA_SET0

	flag	0x04

	str_len 25
	string	$ACS_GAME_OPTIONS_STR
	font	$MAIN_FNT
	$COLOR_SET0

	palette $ACS_MAIN_MENU_PAL0

	x	"(640 - $MAIN_MENU_SX) / 2"
	y	"$OPTIONS_MENU_START_Y + (480 - ($MAIN_MENU_SY + $MAIN_MENU_DELTA) * $OPTIONS_MENU_SIZE)/2 + ($MAIN_MENU_SY + $MAIN_MENU_DELTA ) * $GAME_Y"

	size_x	$MAIN_MENU_SX
	size_y	$MAIN_MENU_SY

	event {
		code	$VK_MOUSE_LEFT
		code	$VK_MOUSE_RIGHT

		time	3

		ev_comm {
			code	$ACS_EXT_EVENT
			ev_data $ACS_EXT_START_SOUND $EFF_MAIN_MENU_BUTTON 0 0
			start_time 0
		}
		ev_comm {
			code	$ACS_CHANGE_SCREEN
			ev_data $ACS_RACE_CFG_SCR 0 0 0
			start_time 0
		}
	}
}
*/

object {
	ID	$ACS_GRAPHICS_OPTION
	type	$ACS_INPUT_FIELD_OBJ
	$ALPHA_SET0

	flag	0x04

	obj_index 1 $ACS_BACK_OPTION

	str_len 25
	string	$ACS_GRAPHICS_STR
	font	$MAIN_FNT
	$COLOR_SET0

	palette $ACS_MAIN_MENU_PAL0

	x	"(640 - $MAIN_MENU_SX) / 2"
	y	"$OPTIONS_MENU_START_Y + (480 - ($MAIN_MENU_SY + $MAIN_MENU_DELTA) * $OPTIONS_MENU_SIZE)/2 + ($MAIN_MENU_SY + $MAIN_MENU_DELTA ) * $GRAPHICS_Y"

	size_x	$MAIN_MENU_SX
	size_y	$MAIN_MENU_SY

	event {
		code	$VK_MOUSE_LEFT
		code	$VK_MOUSE_RIGHT

		time	3

		ev_comm {
			code	$ACS_EXT_EVENT
			ev_data $ACS_EXT_START_SOUND $EFF_MAIN_MENU_BUTTON 0 0
			start_time 0
		}
		ev_comm {
			code	$ACS_CHANGE_SCREEN
			ev_data $ACS_GRAPHICS_SCR 0 0 0
			start_time 0
		}
	}
}

object {
	ID	$ACS_SOUND_OPTION
	type	$ACS_INPUT_FIELD_OBJ
	$ALPHA_SET0

	flag	0x04

	str_len 25
	string	$ACS_SOUND_STR
	font	$MAIN_FNT
	$COLOR_SET0

	palette $ACS_MAIN_MENU_PAL0

	x	"(640 - $MAIN_MENU_SX) / 2"
	y	"$OPTIONS_MENU_START_Y + (480 - ($MAIN_MENU_SY + $MAIN_MENU_DELTA) * $OPTIONS_MENU_SIZE)/2 + ($MAIN_MENU_SY + $MAIN_MENU_DELTA ) * $SOUND_Y"

	size_x	$MAIN_MENU_SX
	size_y	$MAIN_MENU_SY

	event {
		code	$VK_MOUSE_LEFT
		code	$VK_MOUSE_RIGHT

		time	3

		ev_comm {
			code	$ACS_EXT_EVENT
			ev_data $ACS_EXT_START_SOUND $EFF_MAIN_MENU_BUTTON 0 0
			start_time 0
		}
		ev_comm {
			code	$ACS_CHANGE_SCREEN
			ev_data $ACS_SOUND_SCR 0 0 0
			start_time 0
		}
	}
}

object {
	ID	$ACS_CONTROLS_OPTION
	type	$ACS_INPUT_FIELD_OBJ
	$ALPHA_SET0

	flag	0x04

	str_len 25
	string	$ACS_CONTROLS_STR
	font	$MAIN_FNT
	$COLOR_SET0

	palette $ACS_MAIN_MENU_PAL0

	x	"(640 - $MAIN_MENU_SX) / 2"
	y	"$OPTIONS_MENU_START_Y + (480 - ($MAIN_MENU_SY + $MAIN_MENU_DELTA) * $OPTIONS_MENU_SIZE)/2 + ($MAIN_MENU_SY + $MAIN_MENU_DELTA ) * $CONTROLS_Y"

	size_x	$MAIN_MENU_SX
	size_y	$MAIN_MENU_SY

	event {
		code	$VK_MOUSE_LEFT
		code	$VK_MOUSE_RIGHT

		time	3

		ev_comm {
			code	$ACS_EXT_EVENT
			ev_data $ACS_EXT_START_SOUND $EFF_MAIN_MENU_BUTTON 0 0
			start_time 0
		}
		ev_comm {
			code	$ACS_CHANGE_SCREEN
			ev_data $ACS_CONTROLS_SCR 0 0 0
			start_time 0
		}
	}
}

object {
	ID	1 // SPLIT-SCREEN
	type	$ACS_INPUT_FIELD_OBJ
	$ALPHA_SET0

	flag	0x04

	str_len 25
	string	$ACS_SPLITSCREEN_STR
	font	$MAIN_FNT
	$COLOR_SET0

	palette $ACS_MAIN_MENU_PAL0

	x	"(640 - $MAIN_MENU_SX) / 2"
	y	"$OPTIONS_MENU_START_Y + (480 - ($MAIN_MENU_SY + $MAIN_MENU_DELTA) * $OPTIONS_MENU_SIZE)/2 + ($MAIN_MENU_SY + $MAIN_MENU_DELTA ) * $SPLIT_Y"

	size_x	$MAIN_MENU_SX
	size_y	$MAIN_MENU_SY

	event {
		code	$VK_MOUSE_LEFT
		code	$VK_MOUSE_RIGHT

		time	3

		ev_comm {
			code	$ACS_EXT_EVENT
			ev_data $ACS_EXT_START_SOUND $EFF_MAIN_MENU_BUTTON 0 0
			start_time 0
		}
		ev_comm {
			code	$ACS_CHANGE_SCREEN
			ev_data $ACS_SPLIT_CFG_SCR 0 0 0
			start_time 0
		}
	}
}

object {
	ID	2 // Turn-Based
	type	$ACS_INPUT_FIELD_OBJ
	$ALPHA_SET0

	flag	0x04

	str_len 25
	string	$ACS_TB_OPTIONS_STR
	font	$MAIN_FNT
	$COLOR_SET0

	palette $ACS_MAIN_MENU_PAL0

	x	"(640 - $MAIN_MENU_SX) / 2"
	y	"$OPTIONS_MENU_START_Y + (480 - ($MAIN_MENU_SY + $MAIN_MENU_DELTA) * $OPTIONS_MENU_SIZE)/2 + ($MAIN_MENU_SY + $MAIN_MENU_DELTA ) * $TB_Y"

	size_x	$MAIN_MENU_SX
	size_y	$MAIN_MENU_SY

	event {
		code	$VK_MOUSE_LEFT
		code	$VK_MOUSE_RIGHT

		time	3

		ev_comm {
			code	$ACS_EXT_EVENT
			ev_data $ACS_EXT_START_SOUND $EFF_MAIN_MENU_BUTTON 0 0
			start_time 0
		}
		ev_comm {
			code	$ACS_CHANGE_SCREEN
			ev_data $ACS_TB_CFG_SCR 0 0 0
			start_time 0
		}
	}
}

object {
	ID	3 // Hotseat
	type	$ACS_INPUT_FIELD_OBJ
	$ALPHA_SET0

	flag	0x04

	str_len 25
	string	$ACS_HS_OPTIONS_STR
	font	$MAIN_FNT
	$COLOR_SET0

	palette $ACS_MAIN_MENU_PAL0

	x	"(640 - $MAIN_MENU_SX) / 2"
	y	"$OPTIONS_MENU_START_Y + (480 - ($MAIN_MENU_SY + $MAIN_MENU_DELTA) * $OPTIONS_MENU_SIZE)/2 + ($MAIN_MENU_SY + $MAIN_MENU_DELTA ) * $HS_Y"

	size_x	$MAIN_MENU_SX
	size_y	$MAIN_MENU_SY

	event {
		code	$VK_MOUSE_LEFT
		code	$VK_MOUSE_RIGHT

		time	3

		ev_comm {
			code	$ACS_EXT_EVENT
			ev_data $ACS_EXT_START_SOUND $EFF_MAIN_MENU_BUTTON 0 0
			start_time 0
		}
		ev_comm {
			code	$ACS_CHANGE_SCREEN
			ev_data $ACS_HOTSEAT_CFG_SCR 0 0 0
			start_time 0
		}
	}
}

object {
	ID	$ACS_BACK_OPTION
	type	$ACS_INPUT_FIELD_OBJ
	$ALPHA_SET0

	flag	0x04

	obj_index 1 3
	obj_index 3 $ACS_GRAPHICS_OPTION

	str_len 25
	string	$ACS_BACK_STR
	font	$MAIN_FNT
	$COLOR_SET0

	palette $ACS_MAIN_MENU_PAL0

	$BACK_COORDS
//	x	90

	event {
		code	$VK_MOUSE_LEFT
		code	$VK_MOUSE_RIGHT
		code	$VK_ESCAPE

		time 1

		ev_comm {
			code	$ACS_EXT_EVENT
			ev_data $ACS_EXT_START_SOUND $EFF_MAIN_MENU_BUTTON 0 0
			start_time 0
		}
		ev_comm {
			code	$ACS_EXT_EVENT
			ev_data $ACS_EXT_RESTORE_SCREEN 0 0 0
			start_time 0
		}
	}
}

#define OPTIONS_MENU_START_Y	0
