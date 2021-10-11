object {
	ID	$IW_ID

	x	"20 + 52 * $IW_X"
	y	"$IW_Y0 + 0"

	align_x 0

	size_x	50
	size_y	50

	$iwCOLOR_SET0
	$iwALPHA_SET0

	event {
		code	$VK_MOUSE_LEFT
		code	$VK_MOUSE_RIGHT

		ev_comm {
			code	$ACS_IWORLD_EVENT
			ev_data $IW_SELECT_PART_TYPE_EV $IW_ID 0 0
		}
	}
}
