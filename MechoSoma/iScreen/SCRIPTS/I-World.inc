
#define iwALPHA_SET0	 alpha0  32 \
alpha1 32 \
alpha2 128 \
d_alpha 16

#define iwALPHA_SET1	 alpha0  32 \
alpha1 32 \
alpha2 32 \
d_alpha 16

#define iwCOLOR_SET0   col0    2 \
	col1    2 \
	col2    2

#define iwCOLOR_SET1   col0    1 \
	col1    2 \
	col2    2

screen {
	ID	$IW_TELEPORT2_SCR

	event {
		opt_code	$MCH_KEY_TURN_LEFT

		ev_comm {
			code	$ACS_IWORLD_EVENT
			ev_data $IW_TOGGLE_TURNBASED_EV 0 0 0
		}
	}
	event {
		opt_code	$MCH_KEY_TURN_RIGHT

		ev_comm {
			code	$ACS_IWORLD_EVENT
			ev_data $IW_TOGGLE_TURNBASED_EV 1 0 0
		}
	}
	event {
		opt_code	$MCH_KEY_OK

		ev_comm {
			code	$ACS_IWORLD_EVENT
			ev_data $IW_TELEPORT_EV 0 0 0
		}
	}
	object {
		ID	1
		type	$ACS_INPUT_FIELD_OBJ

//		flag 0x04

		x	50
		y	"$IW_Y0 + 5"
		size_x	200
		size_y	30

		str_len 25
		string	$ACS_RT_MODE_STR

		font	1
		space	0

		$iwCOLOR_SET1
		$iwALPHA_SET1

		palette $ACS_MAIN_MENU_PAL0

		event {
			code	$VK_MOUSE_LEFT
			code	$VK_MOUSE_RIGHT

			time	1

			ev_comm {
				code	$ACS_IWORLD_EVENT
				ev_data $IW_SET_TURNBASED_EV 0 0 0
			}
			ev_comm {
				code	$ACS_IWORLD_EVENT
				ev_data $IW_TELEPORT_EV 0 0 0
				start_time 1
			}
		}
	}
	object {
		ID	2
		type	$ACS_INPUT_FIELD_OBJ

//		flag 0x04

		x	300
		y	"$IW_Y0 + 5"
		size_x	200
		size_y	30

		str_len 25
		string	$ACS_TB_MODE_STR

		font	1
		space	0

		$iwCOLOR_SET1
		$iwALPHA_SET1

		palette $ACS_MAIN_MENU_PAL0

		event {
			code	$VK_MOUSE_LEFT
			code	$VK_MOUSE_RIGHT

			time	1

			ev_comm {
				code	$ACS_IWORLD_EVENT
				ev_data $IW_SET_TURNBASED_EV 1 0 0
			}
			ev_comm {
				code	$ACS_IWORLD_EVENT
				ev_data $IW_TELEPORT_EV 0 0 0
				start_time 1
			}
		}
	}
	object {
		ID	3
		type	$ACS_INPUT_FIELD_OBJ

//		flag 0x04

		x	580
		y	"$IW_Y0 + 5"
		size_x	50
		size_y	30

		align_x	1

		str_len 25
		string	$ACS_BACK_STR

		font	1
		space	0

		$iwCOLOR_SET0
		$iwALPHA_SET0

		palette $ACS_MAIN_MENU_PAL0

		event {
			code		$VK_MOUSE_LEFT
			code		$VK_MOUSE_RIGHT
			opt_code	$MCH_KEY_CANCEL

			ev_comm {
				code	$ACS_IWORLD_EVENT
				ev_data $IW_SET_TURNBASED_EV 2 0 0
			}
			ev_comm {
				code	$ACS_IWORLD_EVENT
				ev_data $IW_TELEPORT_EV 0 0 0
				start_time 1
			}
/*
			ev_comm {
				code	$ACS_IWORLD_EVENT
				ev_data $IW_TELEPORT_CANCEL_EV 0 0 0
			}
*/
		}
	}
}

screen {
	ID	$IW_TELEPORT_SCR

	object {
		ID	1
		type	$ACS_INPUT_FIELD_OBJ

//		flag	0x04

		x	560
		y	"$IW_Y0 + 0"

		size_x	70
		size_y	30

		align_x		1

		str_len 25
		string	$ACS_START_STR

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
				ev_data $IW_TELEPORT_GO_EV 0 0 0
			}
		}
	}
	object {
		ID	4
		type	$ACS_INPUT_FIELD_OBJ

//		flag	0x04

		x	560
		y	"$IW_Y0 + 0"
		size_x	70
		size_y	30

		align_x		1

		str_len 25
		string	$ACS_BUY_STR

		font	0
		space	1

		$iwCOLOR_SET0
		$iwALPHA_SET0

		palette $ACS_MAIN_MENU_PAL0

		event {
			code	$VK_MOUSE_LEFT
			code	$VK_MOUSE_RIGHT
			opt_code	$MCH_KEY_OK

			ev_comm {
				code	$ACS_IWORLD_EVENT
				ev_data $IW_BUY_TRACK_EV 0 0 0
			}
		}
	}
}

screen {
	ID	$IW_ASSEMBLY_SCR

	event {
//		code		$VK_ESCAPE
		opt_code	$MCH_KEY_CANCEL

		ev_comm {
			code	$ACS_IWORLD_EVENT
			ev_data $IW_CANCEL_ASSEMBLY_EV 0 0 0
		}
	}

	event {
		opt_code	$MCH_KEY_TURN_LEFT

		ev_comm {
			code	$ACS_IWORLD_EVENT
			ev_data $IW_PREV_PART_EV 0 0 0
		}
	}
	event {
		opt_code	$MCH_KEY_TURN_RIGHT

		ev_comm {
			code	$ACS_IWORLD_EVENT
			ev_data $IW_NEXT_PART_EV 0 0 0
		}
	}

	event {
		opt_code	$MCH_KEY_MOVE_BACKWARD

		ev_comm {
			code	$ACS_IWORLD_EVENT
			ev_data $IW_NEXT_PART_TYPE_EV 0 0 0
		}
	}
	event {
		opt_code	$MCH_KEY_MOVE_FORWARD

		ev_comm {
			code	$ACS_IWORLD_EVENT
			ev_data $IW_PREV_PART_TYPE_EV 0 0 0
		}
	}

	object {
		ID	10
		type	$ACS_INPUT_FIELD_OBJ

//		flag	0x04

		x	600
		y	"$IW_Y0 + 0"
		size_x	30
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
				code	$ACS_IWORLD_EVENT
				ev_data $IW_FINIT_ASSEMBLY_EV 0 0 0
			}
		}
	}
/*
	#define IW_ID 0
	#define IW_X 1
	#include "iScreen\Scripts\iwAssemblyObj.inc"

	#define IW_ID 1
	#define IW_X 0
	#include "iScreen\Scripts\iwAssemblyObj.inc"

	#define IW_ID 2
	#define IW_X 2
	#include "iScreen\Scripts\iwAssemblyObj.inc"
*/
	object {
		ID	11

		x	"10"
		y	"$IW_Y0 + 0"

		size_x	50
		size_y	50

		$iwCOLOR_SET0
		$iwALPHA_SET0
	}

	#define IW_EL_BASE		520
	#define IW_EL_BASE2		520

	object {
		ID	12
		type	$ACS_INPUT_FIELD_OBJ

		x	"$IW_EL_BASE - 25"
		y	"$IW_Y0 + 5"
		size_x	20
		size_y	30

		str_len 25
//		string	$ACS_PREV_STR
		string	" "

		align_x		1

		flag	0x1000

		font	1
		space	0

		$iwCOLOR_SET0
		$iwALPHA_SET0

		palette $ACS_MAIN_MENU_PAL0

		event {
			code	$VK_MOUSE_LEFT
			code	$VK_MOUSE_RIGHT

			ev_comm {
				code	$ACS_IWORLD_EVENT
				ev_data $IW_PREV_PART_EV 0 0 0
			}
		}
	}
	object {
		ID	13
		type	$ACS_INPUT_FIELD_OBJ

		x	"$IW_EL_BASE + 25"
		y	"$IW_Y0 + 5"
		size_x	20
		size_y	30

		flag	0x1000

		align_x		1

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

			ev_comm {
				code	$ACS_IWORLD_EVENT
				ev_data $IW_NEXT_PART_EV 0 0 0
			}
		}
	}
	object {
		ID	14
		type	$ACS_INPUT_FIELD_OBJ

		x	$IW_EL_BASE2
		y	"$IW_Y0 + 3"
		size_x	20
		size_y	20

		str_len 25
//		string	$ACS_PREV_STR
		string	" "

		align_x		1

		flag	0x1000

		font	1
		space	0

		$iwCOLOR_SET0
		$iwALPHA_SET0

		palette $ACS_MAIN_MENU_PAL0

		event {
			code	$VK_MOUSE_LEFT
			code	$VK_MOUSE_RIGHT

			ev_comm {
				code	$ACS_IWORLD_EVENT
				ev_data $IW_PREV_PART_TYPE_EV 0 0 0
			}
		}
	}
	object {
		ID	15
		type	$ACS_INPUT_FIELD_OBJ

		x	"$IW_EL_BASE2 + 1"
		y	"$IW_Y0 + 22"
		size_x	20
		size_y	20

		flag	0x1000

		align_x		1

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

			ev_comm {
				code	$ACS_IWORLD_EVENT
				ev_data $IW_NEXT_PART_TYPE_EV 0 0 0
			}
		}
	}
}

screen {
	ID	$IW_NAME2_SCR

	object {
		ID	1
		type	$ACS_INPUT_FIELD_OBJ

		x	20
		y	"$IW_Y0 + 10"

		palette $ACS_MAIN_MENU_PAL0

		str_len 50
		string	$ACS_OVERWRITE_STR

		font	1
		space	0

		$iwCOLOR_SET0
		$iwALPHA_SET0
	}
	object {
		ID	2
		type	$ACS_INPUT_FIELD_OBJ

		x	500
		y	"$IW_Y0 + 10"
		size_x 30
		size_y 30

		str_len 7
		string	$ACS_YES_STR

		palette $ACS_MAIN_MENU_PAL0

		font	1
		space	0

		$iwCOLOR_SET0
		$iwALPHA_SET0
		event {
			code	$VK_MOUSE_LEFT
			code	$VK_MOUSE_RIGHT
			code	$VK_Y
			code	$VK_RETURN
			code	$VK_SPACE

			ev_comm {
				code	$ACS_IWORLD_EVENT
				ev_data $IW_CHOOSE_FIGURE_EV 0 0 0
			}
			ev_comm {
				code	$ACS_IWORLD_EVENT
				ev_data $IW_FINIT_ELECTIONARY_EV 0 0 0
			}
		}
	}
	object {
		ID	3
		type	$ACS_INPUT_FIELD_OBJ

		x	547
		y	"$IW_Y0 + 10"

		str_len 7
		string	$ACS_SLASH_STR

		palette $ACS_MAIN_MENU_PAL0

		font	1
		space	0

		$iwCOLOR_SET0
		$iwALPHA_SET0
	}
	object {
		ID	4
		type	$ACS_INPUT_FIELD_OBJ

		x	560
		y	"$IW_Y0 + 10"
		size_x 30
		size_y 30

		str_len 7
		string	$ACS_NO_STR

		palette $ACS_MAIN_MENU_PAL0

		font	1
		space	0

		$iwCOLOR_SET0
		$iwALPHA_SET0

		event {
			code	$VK_MOUSE_LEFT
			code	$VK_MOUSE_RIGHT
			code	$VK_N
			opt_code	$MCH_KEY_CANCEL

			ev_comm {
				code	$ACS_IWORLD_EVENT
				ev_data $IW_INIT_NAME_EV $IW_NAME_SCR 0 0
			}
			ev_comm {
				code	$ACS_IWORLD_EVENT
				ev_data $IW_SET_SCREEN_EV $IW_NAME_SCR 0 0
			}
		}
	}
}

#define IW_NAME_ID	$IW_NAME_SCR
#define IW_X1	150
#define IW_Y1	"$IW_Y0 + 10"

#include "iScreen\Scripts\iwNameScr.inc"

#define IW_NAME_ID	$IW_NAME_HSPLIT_SCR
#define IW_X1	150
#define IW_Y1	"$IW_Y0 + 10"

#include "iScreen\Scripts\iwNameScr.inc"

#define IW_NAME_ID	$IW_NAME_VSPLIT1_SCR
#define IW_X1	20
#define IW_Y1	"$IW_Y0 + 10"

#include "iScreen\Scripts\iwNameScr.inc"

#define IW_NAME_ID	$IW_NAME_VSPLIT2_SCR
#define IW_X1	20
#define IW_Y1	"$IW_Y0 + 10"

#include "iScreen\Scripts\iwNameScr.inc"

#define IW_ELECTIONARY_ID	$IW_ELECTIONARY_SCR
#define IW_EL_BASE		250
#include "iScreen\Scripts\iwElectionaryScr.inc"

#define IW_ELECTIONARY_ID	$IW_ELECTIONARY2_SCR
#define IW_EL_BASE		75
#include "iScreen\Scripts\iwElectionaryScr.inc"

screen {
	ID	$IW_DOGGY_SCR

	object {
		ID	10
		type	$ACS_INPUT_FIELD_OBJ

		flag	0x04

		x	580
		y	"$IW_Y0 + 0"
		size_x	35
		size_y	30

		align_x		1

		str_len 25
		string	"  "

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
				ev_data $IW_DOGGY_EV 0 0 0
			}
		}
	}
}
