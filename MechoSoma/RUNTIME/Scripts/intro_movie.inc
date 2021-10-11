
#define IMG_CHANGE action $ia_LOAD {	\
	ID		"$im_SCENE_ + 20"	\
	image	$im_SCENE_	\
	slot	$im_SLOT	\
}	\
action $ia_SET_SCALE {	\
	image	$im_SCENE_	\
	scale	$im_SCALE	\
}	\
action $ia_RND_SCROLL {	\
	image	$im_SCENE_	\
	image	$im_SCENE	\
	\
	time	3000	\
	alpha	3000	\
	\
	$im_SCROLL_COORDS	\
}	\
action $ia_RND_SCROLL {	\
	prev_coords	3	\
	\
	ID		$im_SCENE_	\
	image	$im_SCENE_	\
	\
	time	60000	\
}	\
action $ia_SCALE {	\
	prev_coords	1	\
	ID		"$im_SCENE_ + 10"	\
	\
	image	$im_SCENE_	\
	time	2000	\
	\
	scale	$im_SCALE	\
	scale	1.0	\
}	\
action $ia_DRAW {	\
	image	$im_SCENE_	\
	time	4000	\
}
