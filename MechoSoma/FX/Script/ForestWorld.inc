KeyID	$FXLAB_ID_KEY_CORE_FOREST_BUBBLE_FLUX
{
	KeyAttribute	$FXLAB_KEY_ATTRIBUTE_FREEZE

	KeyDefine	Red
	KeyDefine	Green
	KeyDefine	Blue
	KeyDefine	Alpha
	KeyDefine	Size
	KeyDefine	Angle
	KeyDefine	Frame

	KeyTime	0.0

	KeyName	Red
	KeyData	0.9

	KeyName	Green
	KeyData	1.0

	KeyName Blue
	KeyData	0.6

	KeyName Alpha
	KeyData 1.0

	KeyName	Size
	KeyData	8.0

	KeyName	Angle
	KeyData	0.0

	KeyName	Frame
	KeyData	0.0

	
	KeyTime	32.0

	KeyName	Red
	KeyData	0.9

	KeyName	Green
	KeyData	1.0

	KeyName Blue
	KeyData	0.7

	KeyName Alpha
	KeyData 0.0

	KeyName	Size
	KeyData	8.0

	KeyName	Angle
	KeyData	0.0

	KeyName	Frame
	KeyData	1.0
}


KeyID	$FXLAB_ID_KEY_FOREST_BUBBLE_FLUX_POSITION
{
	KeyAttribute	"$FXLAB_KEY_ATTRIBUTE_REPEAT | $FXLAB_KEY_ATTRIBUTE_STORAGE"
	
	KeyDefine	Type
	KeyDefine	DataX
	KeyDefine	DataY
	KeyDefine	DataZ
	KeyDefine	DataA1
	KeyDefine	DataB1
	KeyDefine	DataC1
	KeyDefine	DataA2
	KeyDefine	DataB2
	KeyDefine	DataC2


	KeyTime	0.0

	KeyName	Type
	KeyData	$FXLAB_SWARM_DOMAIN_SPHERE

	KeyName	DataX
	KeyData	0

	KeyName	DataY
	KeyData	0

	KeyName	DataZ
	KeyData	0

	KeyName	DataA1
	KeyData	35.0

	KeyName	DataB1
	KeyData	0.0

	KeyName	DataC1
	KeyData	0

	KeyName	DataA2
	KeyData	0

	KeyName	DataB2
	KeyData	0

	KeyName	DataC2
	KeyData	0
}

KeyID	$FXLAB_ID_KEY_FOREST_BUBBLE_FLUX_VELOCITY
{
	KeyAttribute	"$FXLAB_KEY_ATTRIBUTE_REPEAT | $FXLAB_KEY_ATTRIBUTE_STORAGE"
	
	KeyDefine	Type
	KeyDefine	DataX
	KeyDefine	DataY
	KeyDefine	DataZ
	KeyDefine	DataA1
	KeyDefine	DataB1
	KeyDefine	DataC1
	KeyDefine	DataA2
	KeyDefine	DataB2
	KeyDefine	DataC2


	KeyTime	0.0

	KeyName	Type
	KeyData	$FXLAB_SWARM_DOMAIN_CIRCLE

	KeyName	DataX
	KeyData	0

	KeyName	DataY
	KeyData	0

	KeyName	DataZ
	KeyData	0.0

	KeyName	DataA1
	KeyData	0.2

	KeyName	DataB1
	KeyData	0.0

	KeyName	DataC1
	KeyData	0

	KeyName	DataA2
	KeyData	0

	KeyName	DataB2
	KeyData	0

	KeyName	DataC2
	KeyData	0
}

KeyID	$FXLAB_ID_KEY_FOREST_BUBBLE_FLUX
{
	KeyAttribute	"$FXLAB_KEY_ATTRIBUTE_STORAGE | $FXLAB_KEY_ATTRIBUTE_REPEAT"

	KeyDefine	RegenerateTime

	KeyDefine	ImageType
	KeyDefine	ImageID
	KeyDefine	ImageNoise

	KeyDefine	Rate
	KeyDefine	Age
	KeyDefine	StartPower
	KeyDefine	EndPower
	KeyDefine	PositionKeyID
	KeyDefine	VelocityKeyID

	KeyDefine	Speed

	KeyTime	0.0

	KeyName	RegenerateTime
	KeyData	0.0

	KeyName	ImageType
	KeyData	$FXLAB_SCRIPT_SECTION_SPRITE

	KeyName	ImageID
	KeyData $FXLAB_ID_SPRITE_FOREST_BUBBLE_FLUX

	KeyName	ImageNoise
	KeyData	0


	KeyName	Rate
	KeyData	0.5

	KeyName Age
	KeyData	30.0

	KeyName	StartPower
	KeyData	1.0

	KeyName	EndPower
	KeyData	0.0

	KeyName	PositionKeyID
	KeyData $FXLAB_ID_KEY_FOREST_BUBBLE_FLUX_POSITION

	KeyName	VelocityKeyID
	KeyData $FXLAB_ID_KEY_FOREST_BUBBLE_FLUX_VELOCITY


	KeyName	Speed
	KeyData 4.0
}

KeyID	$FXLAB_ID_KEY_FOREST_FIELD_STREAM
{
	KeyAttribute	"$FXLAB_KEY_ATTRIBUTE_STORAGE | $FXLAB_KEY_ATTRIBUTE_REPEAT"

	KeyDefine	Force
	KeyDefine	Radius

	KeyTime	0.0

	KeyName	Force
	KeyData	120.0

	KeyName	Radius
	KeyData	60.0
}
