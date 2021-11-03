#include "scenemesh.h"

void sNodeObject::Release()							
{ 
	ID=0; 
	name.Release(); 
	parent.Release(); 
	memset(matrix,0,12*sizeof(float)); 
	AnimationVisibility.Release();
	AnimationPosition.Release();
	AnimationRotation.Release();
	AnimationScale.Release();
}
