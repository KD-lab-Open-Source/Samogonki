
	object {
		ID	$NAME_ID
		type	$ACS_INPUT_FIELD_OBJ
		$ALPHA_SET0

		obj_index	0	$MECHOS_ID
		obj_index	2	$TYPE_ID
		obj_index	1	$CTL_UP1
		obj_index	3	$CTL_DOWN1

		str_len 7
		string	"Name"
		font	1
		$COLOR_SET0

		palette $ACS_MAIN_MENU_PAL0

		x	$NAME_X
		y	"$CTL_MENU_START_Y + (480 - ($CTL_SY + $CTL_DELTA) * $CTL_MENU_SZ)/2 + ($CTL_SY + $CTL_DELTA ) * $CTL_Y"

		size_x	$NAME_SX
		size_y	$CTL_SY

		flag	0x1000

		event {
			code	$VK_MOUSE_LEFT
			code	$VK_MOUSE_RIGHT

			time	1

			ev_comm {
				code	$ACS_INPUT_STRING
				ev_data $NAME_ID 0 0 0
			}
			ev_comm {
				code	$ACS_EXT_EVENT
				ev_data $ACS_EXT_UPDATE_MM_OPTION $CUR_SCR $NAME_ID 0
				start_time 1
			}
		}
	}
	object {
		ID	$TYPE_ID
		type	$ACS_INPUT_FIELD_OBJ
		$ALPHA_SET0

		obj_index	0	$NAME_ID
		obj_index	2	$MECHOS_ID
		obj_index	1	$CTL_UP
		obj_index	3	$CTL_DOWN

		flag	0x04
		flag	0x1000

		str_len 15

		max_state	2
		cur_state	0
		state_str	$ACS_DISABLED_STR
		state_str	$ACS_HUMAN_STR
		state_str	$ACS_COMPUTER_STR

		font	2
		$COLOR_SET0

		palette $ACS_MAIN_MENU_PAL0

		x	$TYPE_X
		y	"$CTL_MENU_START_Y + (480 - ($CTL_SY + $CTL_DELTA) * $CTL_MENU_SZ)/2 + ($CTL_SY + $CTL_DELTA ) * $CTL_Y"

		size_x	$TYPE_SX
		size_y	$CTL_SY

		event {
			code	$VK_MOUSE_LEFT

			time	1

			ev_comm {
				code	$ACS_EXT_EVENT
				ev_data $ACS_EXT_START_SOUND $EFF_MAIN_MENU_BUTTON 0 0
				start_time 0
			}
			ev_comm {
				code	$ACS_INC_STATE
				ev_data $TYPE_ID 0 0 0
				start_time 0
			}
			ev_comm {
				code	$ACS_EXT_EVENT
				ev_data $ACS_EXT_UPDATE_MM_OPTION $CUR_SCR $TYPE_ID 0
				start_time 1
			}
		}
		event {
			code	$VK_MOUSE_RIGHT

			time	1

			ev_comm {
				code	$ACS_EXT_EVENT
				ev_data $ACS_EXT_START_SOUND $EFF_MAIN_MENU_BUTTON 0 0
				start_time 0
			}
			ev_comm {
				code	$ACS_DEC_STATE
				ev_data $TYPE_ID 0 0 0
				start_time 0
			}
			ev_comm {
				code	$ACS_EXT_EVENT
				ev_data $ACS_EXT_UPDATE_MM_OPTION $CUR_SCR $TYPE_ID 0
				start_time 1
			}
		}
	}
	object {
		ID	$MECHOS_ID
		type	$ACS_INPUT_FIELD_OBJ
		$ALPHA_SET0
		$COLOR_SET0

		obj_index	0	$TYPE_ID
		obj_index	2	$NAME_ID
		obj_index	1	$CTL_UP2
		obj_index	3	$CTL_DOWN2

		str_len 2
		string	" "

		x	$MECHOS_X
		y	"$CTL_MENU_START_Y + (480 - ($CTL_SY + $CTL_DELTA) * $CTL_MENU_SZ)/2 + ($CTL_SY + $CTL_DELTA ) * $CTL_Y"

		size_x	$MECHOS_SX
		size_y	$MECHOS_SY

		flag	0x1000

		event {
			code	$VK_MOUSE_LEFT
			code	$VK_MOUSE_RIGHT

			ev_comm {
				code	$ACS_EXT_EVENT
				ev_data $ACS_EXT_START_SOUND $EFF_MAIN_MENU_BUTTON 0 0
				start_time 0
			}
			ev_comm {
					code	$ACS_EXT_EVENT
					ev_data $ACS_EXT_INIT_SHOP $CTL_Y 0 0
					start_time 0
			}
			ev_comm {
				code	$ACS_CHANGE_SCREEN
				ev_data $ACS_CONSTRUCT_SCR 0 0 0
				start_time 0
			}
		}
	}
