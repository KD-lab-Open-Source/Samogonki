#include <string.h>

#include "xtool.h"
#include "xmath.h"
#include "arcane_menu.h"
#include "aspect_ratio.h"

AspectRatioInfo createAspectRatio(int sc_width, int sc_height, int cameraOffset) {
    int width = 480 / sc_height * sc_width;

    return AspectRatioInfo {
        .width = width,
        .offset = width - 640,
        .cameraOffset = cameraOffset,
		.ratio = (float)sc_width / sc_height,
    };
}

AspectRatioInfo aspectRatios[] = {
    createAspectRatio(4, 3, 0),
    createAspectRatio(16, 9, 100),
    createAspectRatio(16, 10, 100),
    createAspectRatio(21, 9, 100),
    createAspectRatio(43, 18, 100),
};

AspectRatioInfo *AR_CURRENT = &aspectRatios[0];

void changeAspectRatio(int width, int height) {
    float newRatio = (float) width / height;

    for (auto& ratio : aspectRatios) {
        if (ratio.ratio == newRatio) {
            AR_CURRENT = &ratio;
            break;
        }
	}
}

// Apply offsets for specific screen elements
void applyScreenRatioOffset(mchArcaneScreenElement *el, const char *name) {
	if (AR_CURRENT->offset) {
		// patch arrow horizontal placement
		if (strcmp(name, "arrow") == 0) {
			el->R.x += AR_CURRENT->offset / 2;
		}

		// patch start counter
		if (strcmp(name, "speed_counter") == 0) {
			el->R.x += AR_CURRENT->offset;
		}

		// patch name (wNtN) text position
		if (strcmp(name, "name_str") == 0) {
			el->R.x += AR_CURRENT->offset;
		}

		// patch map position
		if (strcmp(name, "map") == 0) {
			el->R.x += AR_CURRENT->offset;
		}

		// patch avatars position
		if (strcmp(name, "figure") == 0) {
			el->R.x += AR_CURRENT->offset;
		}

		// finish screen changes
		if (
			strcmp(name, "play_again_str") == 0
			|| strcmp(name, "try_again_str") == 0
			|| strcmp(name, "try_again2_str") == 0
			|| strcmp(name, "go_away_str") == 0
			|| strcmp(name, "go_away2_str") == 0
			|| strcmp(name, "lost_str") == 0
			|| strcmp(name, "not_lost_str") == 0
			|| strcmp(name, "won_str0") == 0
			|| strcmp(name, "won_str1") == 0
			|| strcmp(name, "winner_str") == 0
			|| strcmp(name, "continue_str") == 0
			|| strcmp(name, "continue2_str") == 0
			|| strcmp(name, "bonus_part") == 0
			|| strcmp(name, "bonus_part_str") == 0
			|| strcmp(name, "buy_str") == 0
			|| strcmp(name, "price_str") == 0
			|| strcmp(name, "price2_str") == 0
			|| strcmp(name, "buy_it_str") == 0
			|| strcmp(name, "no_thanks_str") == 0
			|| strcmp(name, "not_enough0_str") == 0
			|| strcmp(name, "not_enough1_str") == 0
			|| strcmp(name, "next_time_str") == 0
		) {
			el->R.x += AR_CURRENT->offset;
		}
	}

	// patch race intro black bars
	if (AR_CURRENT->offset) {
		if (
			strcmp(name, "small_rect1") == 0
			|| strcmp(name, "small_rect2") == 0
			|| strcmp(name, "big_rect1") == 0
			|| strcmp(name, "big_rect2") == 0
		) {
			el->SizeX += AR_CURRENT->offset;
		}
	}
}