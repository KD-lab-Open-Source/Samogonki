screen {
	ID	$IW_NAME_ID

	event {
		code	$ACS_STARTUP_KEY

		ev_comm {
			code	$ACS_IWORLD_EVENT
			ev_data $IW_KEYTRAP_EV 1000 0 0
		}
	}

	object {
		ID	1
		type	$ACS_INPUT_FIELD_OBJ

		x	$IW_X1
		y	$IW_Y1

		palette $ACS_MAIN_MENU_PAL0

		str_len 25
		string	$ACS_YourNameStr

		font	1
		space	0

		$iwCOLOR_SET1
		$iwALPHA_SET1
	}
	object {
		ID	2
		type	$ACS_INPUT_FIELD_OBJ

		x	300
		y	"$IW_Y0 + 10"

		str_len 7
		string	"XXXXXXX"

		palette $ACS_MAIN_MENU_PAL0

		font	1
		space	0

		$iwCOLOR_SET0
		$iwALPHA_SET0

		event {
			code	1000

			time 2

			ev_comm {
				code	$ACS_IWORLD_EVENT
				ev_data	$IW_INPUT_STRING_EV 2 0 0 
				start_time 0
			}
			ev_comm {
				code	$ACS_IWORLD_EVENT
				ev_data $IW_CHECK_NAME_EV 0 0 0
				start_time 1
			}
		}
	}
	object {
		ID	10
		type	$ACS_INPUT_FIELD_OBJ

//		flag	0x04

		x	580
		y	"$IW_Y0 + 0"
		size_x	35
		size_y	35

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
				code	$ACS_IWORLD_EVENT
				ev_data $IW_KEYTRAP_EV $VK_RETURN 0 0
			}
		}
	}
}

