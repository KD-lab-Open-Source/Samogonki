KeyID $FXLAB_ID_KEY_KINETIC_HIT_WAVE
{
	KeyAttribute	$FXLAB_KEY_ATTRIBUTE_NONE

	KeyDefine	Model
	KeyDefine	Red
	KeyDefine	Green
	KeyDefine	Blue
	KeyDefine	Alpha

	KeyDefine	Radius
	KeyDefine   OffsetAngleX
	KeyDefine   OffsetAngleY
	KeyDefine   OffsetAngleZ

	KeyTime	0.0

	KeyName	Model
	KeyData	32.0

	KeyName	Red
	KeyData	1.0

	KeyName	Green
	KeyData	1.0

	KeyName	Blue
	KeyData	0.0

	KeyName	Alpha
	KeyData	0.5


	KeyName	Radius
	KeyData	20.0

	KeyName	OffsetAngleX
	KeyData	0.0

	KeyName	OffsetAngleY
	KeyData	0.0

	KeyName	OffsetAngleZ
	KeyData	0.0


	KeyTime	6.0

	KeyName	Alpha
	KeyData	0.5


	KeyTime	9.0

	KeyName	Model
	KeyData	32.0

	KeyName	Red
	KeyData	1.0

	KeyName	Green
	KeyData	0.0

	KeyName	Blue
	KeyData	0.0

	KeyName	Alpha
	KeyData	0.01


	KeyName	Radius
	KeyData	300.0

	KeyName	OffsetAngleX
	KeyData	0.0

	KeyName	OffsetAngleY
	KeyData	0.0

	KeyName	OffsetAngleZ
	KeyData	0.0
}


KeyID	$FXLAB_ID_KEY_KINETIC_HIT_DEFORMATION
{
	KeyAttribute	$FXLAB_KEY_ATTRIBUTE_NONE
	
	KeyDefine	Amplitude
	KeyDefine	Radius
	KeyDefine	Delta

	KeyTime	0.0

	KeyName		Amplitude
	KeyData		32.0
	
	KeyName		Radius
	KeyData		128.0
	
	KeyName		Delta
	KeyData		8.0


	KeyTime	15.0

	KeyName		Amplitude
	KeyData		0.0
	
	KeyName		Radius
	KeyData		128.0
	
	KeyName		Delta
	KeyData		128.0
}


KeyID	$FXLAB_ID_KEY_KINETIC_HIT_DAMAGE
{
	KeyAttribute	"$FXLAB_KEY_ATTRIBUTE_NONE | $FXLAB_KEY_ATTRIBUTE_STORAGE"

	KeyDefine	Power
	KeyDefine	Radius
	KeyDefine	Type

	KeyTime	1.0

	KeyName	Power
	KeyData	0.0

	KeyName	Radius
	KeyData	200.0

	KeyName	Type
	KeyData	$FXLAB_DAMAGE_SPHERE_TYPE_KINETIC
}

