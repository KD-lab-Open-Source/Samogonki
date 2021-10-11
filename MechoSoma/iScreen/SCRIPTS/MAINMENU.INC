resID	   $ACS_MAIN_MENU_BACK

object {
	ID	$ACS_NEW_GAME_OPTION
	type	$ACS_INPUT_FIELD_OBJ

	$ALPHA_SET0

	flag	0x04

	obj_index	1 $NG_UP
	obj_index	0 0 
	obj_index	2 0 

	x	"(640 - $MAIN_MENU_SX) / 2 + $MENU_DX"
	y	"$MAIN_MENU_START_Y + (480 - ($MAIN_MENU_SY + $MAIN_MENU_DELTA) * $MAIN_MENU_SIZE)/2 + ($MAIN_MENU_SY + $MAIN_MENU_DELTA ) * $NEW_GAME_Y"

	size_x	$MAIN_MENU_SX
	size_y	$MAIN_MENU_SY

	str_len 25
	string	$ACS_NG_STR
	font	$MAIN_FNT
	$COLOR_SET0

	palette $ACS_MAIN_MENU_PAL0

	$NEW_GAME_EV
}

object {
	ID	$ACS_OPTIONS_OPTION
	type	$ACS_INPUT_FIELD_OBJ

	$ALPHA_SET0

	flag	0x04

	obj_index	0 0 
	obj_index	2 0 

	str_len 25
	string	$ACS_OPTIONS_STR
	font	$MAIN_FNT
	$COLOR_SET0

	def_resource	$ACS_OPTIONS_OPTION
	palette $ACS_MAIN_MENU_PAL0

	x	"(640 - $MAIN_MENU_SX) / 2 + $MENU_DX"
	y	"$MAIN_MENU_START_Y + (480 - ($MAIN_MENU_SY + $MAIN_MENU_DELTA) * $MAIN_MENU_SIZE)/2 + ($MAIN_MENU_SY + $MAIN_MENU_DELTA ) * $OPTIONS_Y"

	size_x	$MAIN_MENU_SX
	size_y	$MAIN_MENU_SY

	event {
		code	$VK_MOUSE_LEFT
		code	$VK_MOUSE_RIGHT

		time	1

		ev_comm {
			code	$ACS_EXT_EVENT
			ev_data $ACS_EXT_START_SOUND $EFF_MAIN_MENU_BUTTON 0 0
			start_time 0
		}
		ev_comm {
			code	$ACS_EXT_EVENT
			ev_data $ACS_EXT_SAVE_SCREEN 0 0 0
			start_time 0
		}
		ev_comm {
			code	$ACS_CHANGE_SCREEN
			ev_data $ACS_OPTIONS_SCR 0 0 0
			start_time 0
		}
	}
}


