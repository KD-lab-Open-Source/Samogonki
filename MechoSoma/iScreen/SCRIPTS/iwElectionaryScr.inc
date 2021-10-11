
screen {
	ID	$IW_ELECTIONARY_ID

	object {
		ID	1
		type	$ACS_INPUT_FIELD_OBJ

		x	"$IW_EL_BASE + 15"
		y	"$IW_Y0 + 10"

		size_y	30
		size_x	100

//		flag	0x04
		flag	0x1000

		str_len 25
		string	"  "

		align_x		0

		font	1
		space	0

		$iwCOLOR_SET0
		$iwALPHA_SET0

		palette $ACS_MAIN_MENU_PAL0
	}

	event {
		opt_code	$MCH_KEY_CANCEL

		ev_comm {
			code	$ACS_IWORLD_EVENT
			ev_data $IW_CANCEL_ELECTIONARY_EV 0 0 0
		}
	}

	object {
		ID	10
		type	$ACS_INPUT_FIELD_OBJ

//		flag	0x04

		x	580
		y	"$IW_Y0 + 0"
		size_x	35
		size_y	30

		align_x		1

		str_len 25
		string	$ACS_OK_STR

		font	0
		space	0

		$iwCOLOR_SET0
		$iwALPHA_SET0

		palette $ACS_MAIN_MENU_PAL0

		event {
			code	$VK_MOUSE_LEFT
			code	$VK_MOUSE_RIGHT
			opt_code	$MCH_KEY_OK

			ev_comm {
				code	$ACS_EXT_EVENT
				ev_data $ACS_EXT_START_SOUND $EFF_TUB_BEEP 0 0
				start_time 0
			}
			ev_comm {
				code	$ACS_IWORLD_EVENT
				ev_data $IW_SELECT_FIGURE_EV 0 0 0
			}
		}
	}

	object {
		ID	2
		type	$ACS_INPUT_FIELD_OBJ

		x	"$IW_EL_BASE - 15"
		y	"$IW_Y0 + 10"
		size_x	30
		size_y	30

		str_len 25
//		string	$ACS_PREV_STR
		string	" "

		align_x		0

		flag	0x1000

		font	1
		space	0

		$iwCOLOR_SET0
		$iwALPHA_SET0

		palette $ACS_MAIN_MENU_PAL0

		event {
			code	$VK_MOUSE_LEFT
			code	$VK_MOUSE_RIGHT
			opt_code	$MCH_KEY_TURN_LEFT

			ev_comm {
				code	$ACS_IWORLD_EVENT
				ev_data $IW_PREV_FIGURE_EV 0 0 0
			}
		}
	}
	object {
		ID	3
		type	$ACS_INPUT_FIELD_OBJ

		x	"$IW_EL_BASE + 115"
		y	"$IW_Y0 + 10"
		size_x	30
		size_y	30

		flag	0x1000

		align_x		0

		str_len 25
//		string	$ACS_NEXT_STR
		string	" "

		font	1
		space	0

		$iwCOLOR_SET0
		$iwALPHA_SET0

		palette $ACS_MAIN_MENU_PAL0

		event {
			code	$VK_MOUSE_LEFT
			code	$VK_MOUSE_RIGHT
			opt_code	$MCH_KEY_TURN_RIGHT

			ev_comm {
				code	$ACS_IWORLD_EVENT
				ev_data $IW_NEXT_FIGURE_EV 0 0 0
			}
		}
	}
}

