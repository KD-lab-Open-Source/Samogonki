KeyID	$FXLAB_ID_KEY_CORE_SPIKE_BURST
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
	KeyData	0.6

	KeyName	Green
	KeyData	0.6

	KeyName Blue
	KeyData	1.0

	KeyName Alpha
	KeyData 0.0

	KeyName	Size
	KeyData	4.0

	KeyName	Angle
	KeyData	0.0

	KeyName	Frame
	KeyData	0.0


	KeyTime	8.0

	KeyName Alpha
	KeyData 1.0


	KeyTime	32.0

	KeyName	Red
	KeyData	1.0

	KeyName	Green
	KeyData	1.0

	KeyName Blue
	KeyData	1.0

	KeyName Alpha
	KeyData 0.2

	KeyName	Size
	KeyData	4.0

	KeyName	Angle
	KeyData	"4.0*3.14"

	KeyName	Frame
	KeyData	1.0
}


KeyID	$FXLAB_ID_KEY_SPIKE_BURST_POSITION
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
	KeyData	$FXLAB_SWARM_DOMAIN_POINT

	KeyName	DataX
	KeyData	0

	KeyName	DataY
	KeyData	0

	KeyName	DataZ
	KeyData	0

	KeyName	DataA1
	KeyData	0

	KeyName	DataB1
	KeyData	0

	KeyName	DataC1
	KeyData	0

	KeyName	DataA2
	KeyData	0

	KeyName	DataB2
	KeyData	0

	KeyName	DataC2
	KeyData	0
}


KeyID	$FXLAB_ID_KEY_SPIKE_BURST_VELOCITY
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
	KeyData	$FXLAB_SWARM_DOMAIN_POINT

	KeyName	DataX
	KeyData	0

	KeyName	DataY
	KeyData	0

	KeyName	DataZ
	KeyData	0

	KeyName	DataA1
	KeyData	0

	KeyName	DataB1
	KeyData	0

	KeyName	DataC1
	KeyData	0

	KeyName	DataA2
	KeyData	0

	KeyName	DataB2
	KeyData	0

	KeyName	DataC2
	KeyData	0
}

KeyID	$FXLAB_ID_KEY_SPIKE_BURST
{		
	KeyAttribute	$FXLAB_KEY_ATTRIBUTE_NONE

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


	KeyTime	0.0

	KeyName	RegenerateTime
	KeyData	3.0

	KeyName	ImageType
	KeyData $FXLAB_SCRIPT_SECTION_SPRITE

	KeyName	ImageID
	KeyData	$FXLAB_ID_SPRITE_SPIKE_BURST
	
	KeyName	ImageNoise
	KeyData 0.0

	KeyName	Rate
	KeyData	20.0

	KeyName	Age
	KeyData	15.0

	KeyName StartPower
	KeyData 0.0

	KeyName	EndPower
	KeyData 1.0

	KeyName	PositionKeyID
	KeyData $FXLAB_ID_KEY_SPIKE_BURST_POSITION

	KeyName	VelocityKeyID
	KeyData $FXLAB_ID_KEY_SPIKE_BURST_VELOCITY


	KeyTime	1.0

	KeyName	RegenerateTime
	KeyData	3.0

	KeyName	Rate
	KeyData	20.0

	KeyName	Age
	KeyData	15.0

	KeyName	ImageType
	KeyData $FXLAB_SCRIPT_SECTION_SPRITE

	KeyName	ImageID
	KeyData	$FXLAB_ID_SPRITE_SPIKE_BURST
	
	KeyName	ImageNoise
	KeyData 0.0

	KeyName StartPower
	KeyData 0.0

	KeyName	EndPower
	KeyData 1.0

	KeyName	PositionKeyID
	KeyData $FXLAB_ID_KEY_SPIKE_BURST_POSITION

	KeyName	VelocityKeyID
	KeyData $FXLAB_ID_KEY_SPIKE_BURST_VELOCITY
}
