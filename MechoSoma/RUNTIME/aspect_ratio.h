#ifndef __ASPECT_RATIO_H__
#define __ASPECT_RATIO_H__

struct AspectRatioInfo {
    // virtual width for interface render
	int width;
    // offset for right-aligned elements
    int offset;
	// camera offset
	int cameraOffset;
	float ratio;
};

extern AspectRatioInfo *AR_CURRENT;

void changeAspectRatio(int width, int height);

void applyScreenRatioOffset(mchArcaneScreenElement *el, const char *name);

#endif // __ASPECT_RATIO_H__