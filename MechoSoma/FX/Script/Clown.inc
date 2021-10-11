KeyID	$FXLAB_ID_KEY_CLOWN_POSITION
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
	KeyData	50.0

	KeyName	DataB1
	KeyData	20.0

	KeyName	DataC1
	KeyData	0

	KeyName	DataA2
	KeyData	0

	KeyName	DataB2
	KeyData	0

	KeyName	DataC2
	KeyData	0
}

KeyID	$FXLAB_ID_KEY_CLOWN_VELOCITY
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
	KeyData	5.0

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

KeyID	$FXLAB_ID_KEY_CLOWN
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
	KeyData	15.0

	KeyName	ImageType
	KeyData	$FXLAB_SCRIPT_SECTION_SPRITE

	KeyName	ImageID
	KeyData	$FXLAB_ID_SPRITE_CLOWN_RED

	KeyName	ImageNoise
	KeyData	7.0


	KeyName	Rate
	KeyData 50.0

	KeyName	Age
	KeyData 15.0

	KeyName	StartPower
	KeyData 0.5

	KeyName	EndPower
	KeyData 1.0

	KeyName	PositionKeyID
	KeyData	$FXLAB_ID_KEY_CLOWN_POSITION

	KeyName	VelocityKeyID
	KeyData	$FXLAB_ID_KEY_CLOWN_VELOCITY


	KeyTime	5.0

	KeyName	RegenerateTime
	KeyData	15.0

	KeyName	ImageType
	KeyData	$FXLAB_SCRIPT_SECTION_SPRITE

	KeyName	ImageID
	KeyData	$FXLAB_ID_SPRITE_CLOWN_RED

	KeyName	ImageNoise
	KeyData	7.0


	KeyName	Rate
	KeyData 0.0

	KeyName	Age
	KeyData 15.0

	KeyName	StartPower
	KeyData 0.5

	KeyName	EndPower
	KeyData 1.0

	KeyName	PositionKeyID
	KeyData	$FXLAB_ID_KEY_CLOWN_POSITION

	KeyName	VelocityKeyID
	KeyData	$FXLAB_ID_KEY_CLOWN_VELOCITY
}


KeyID	$FXLAB_ID_KEY_CORE_CLOWN_RED
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
	KeyData	1.0

	KeyName	Green
	KeyData	0.0

	KeyName Blue
	KeyData	0.0

	KeyName Alpha
	KeyData 0.5

	KeyName	Size
	KeyData	16.0

	KeyName	Angle
	KeyData	0.0

	KeyName	Frame
	KeyData	0.0

	
	KeyTime	2.0

	KeyName	Red
	KeyData	1.0

	KeyName	Green
	KeyData	0.0

	KeyName Blue
	KeyData	0.0

	KeyName Alpha
	KeyData 1.0


	KeyTime	8.0

	KeyName	Red
	KeyData	1.0

	KeyName	Green
	KeyData	0.0

	KeyName Blue
	KeyData	0.0

	KeyName Alpha
	KeyData 0.0

	KeyName	Size
	KeyData	16.0

	KeyName	Angle
	KeyData	0.0

	KeyName	Frame
	KeyData	1.0
}

KeyID	$FXLAB_ID_KEY_CORE_CLOWN_ORANGE
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
	KeyData	1.0

	KeyName	Green
	KeyData	0.5

	KeyName Blue
	KeyData	0.2

	KeyName Alpha
	KeyData 0.5

	KeyName	Size
	KeyData	16.0

	KeyName	Angle
	KeyData	0.0

	KeyName	Frame
	KeyData	0.0

	
	KeyTime	2.0

	KeyName	Red
	KeyData	1.0

	KeyName	Green
	KeyData	0.5

	KeyName Blue
	KeyData	0.2

	KeyName Alpha
	KeyData 1.0


	KeyTime	8.0

	KeyName	Red
	KeyData	1.0

	KeyName	Green
	KeyData	0.5

	KeyName Blue
	KeyData	0.2

	KeyName Alpha
	KeyData 0.0

	KeyName	Size
	KeyData	16.0

	KeyName	Angle
	KeyData	0.0

	KeyName	Frame
	KeyData	1.0
}

KeyID	$FXLAB_ID_KEY_CORE_CLOWN_YELLOW
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
	KeyData	1.0

	KeyName	Green
	KeyData	1.0

	KeyName Blue
	KeyData	0.0

	KeyName Alpha
	KeyData 0.5

	KeyName	Size
	KeyData	16.0

	KeyName	Angle
	KeyData	0.0

	KeyName	Frame
	KeyData	0.0

	
	KeyTime	2.0

	KeyName	Red
	KeyData	1.0

	KeyName	Green
	KeyData	1.0

	KeyName Blue
	KeyData	0.0

	KeyName Alpha
	KeyData 1.0


	KeyTime	8.0

	KeyName	Red
	KeyData	1.0

	KeyName	Green
	KeyData	1.0

	KeyName Blue
	KeyData	0.0

	KeyName Alpha
	KeyData 0.0

	KeyName	Size
	KeyData	16.0

	KeyName	Angle
	KeyData	0.0

	KeyName	Frame
	KeyData	1.0
}

KeyID	$FXLAB_ID_KEY_CORE_CLOWN_GREEN
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
	KeyData	0.0

	KeyName	Green
	KeyData	1.0

	KeyName Blue
	KeyData	0.0

	KeyName Alpha
	KeyData 0.5

	KeyName	Size
	KeyData	16.0

	KeyName	Angle
	KeyData	0.0

	KeyName	Frame
	KeyData	0.0

	
	KeyTime	2.0

	KeyName	Red
	KeyData	0.0

	KeyName	Green
	KeyData	1.0

	KeyName Blue
	KeyData	0.0

	KeyName Alpha
	KeyData 1.0


	KeyTime	8.0

	KeyName	Red
	KeyData	0.0

	KeyName	Green
	KeyData	1.0

	KeyName Blue
	KeyData	0.0

	KeyName Alpha
	KeyData 0.0

	KeyName	Size
	KeyData	16.0

	KeyName	Angle
	KeyData	0.0

	KeyName	Frame
	KeyData	1.0
}

KeyID	$FXLAB_ID_KEY_CORE_CLOWN_LIGHT_BLUE
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
	KeyData	0.2

	KeyName	Green
	KeyData	0.5

	KeyName Blue
	KeyData	1.0

	KeyName Alpha
	KeyData 0.5

	KeyName	Size
	KeyData	16.0

	KeyName	Angle
	KeyData	0.0

	KeyName	Frame
	KeyData	0.0

	
	KeyTime	2.0

	KeyName	Red
	KeyData	0.2

	KeyName	Green
	KeyData	0.5

	KeyName Blue
	KeyData	1.0

	KeyName Alpha
	KeyData 1.0


	KeyTime	8.0

	KeyName	Red
	KeyData	0.2

	KeyName	Green
	KeyData	0.5

	KeyName Blue
	KeyData	1.0

	KeyName Alpha
	KeyData 0.0

	KeyName	Size
	KeyData	16.0

	KeyName	Angle
	KeyData	0.0

	KeyName	Frame
	KeyData	1.0
}

KeyID	$FXLAB_ID_KEY_CORE_CLOWN_BLUE
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
	KeyData	0.0

	KeyName	Green
	KeyData	0.0

	KeyName Blue
	KeyData	1.0

	KeyName Alpha
	KeyData 0.5

	KeyName	Size
	KeyData	16.0

	KeyName	Angle
	KeyData	0.0

	KeyName	Frame
	KeyData	0.0

	
	KeyTime	2.0

	KeyName	Red
	KeyData	0.0

	KeyName	Green
	KeyData	0.0

	KeyName Blue
	KeyData	1.0

	KeyName Alpha
	KeyData 1.0


	KeyTime	8.0

	KeyName	Red
	KeyData	0.0

	KeyName	Green
	KeyData	0.0

	KeyName Blue
	KeyData	1.0

	KeyName Alpha
	KeyData 0.0

	KeyName	Size
	KeyData	16.0

	KeyName	Angle
	KeyData	0.0

	KeyName	Frame
	KeyData	1.0
}

KeyID	$FXLAB_ID_KEY_CORE_CLOWN_MAGENTA
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
	KeyData	0.5

	KeyName	Green
	KeyData	0.2

	KeyName Blue
	KeyData	1.0

	KeyName Alpha
	KeyData 0.5

	KeyName	Size
	KeyData	16.0

	KeyName	Angle
	KeyData	0.0

	KeyName	Frame
	KeyData	0.0

	
	KeyTime	2.0

	KeyName	Red
	KeyData	0.5

	KeyName	Green
	KeyData	0.2

	KeyName Blue
	KeyData	1.0

	KeyName Alpha
	KeyData 1.0


	KeyTime	8.0

	KeyName	Red
	KeyData	0.5

	KeyName	Green
	KeyData	0.2

	KeyName Blue
	KeyData	1.0

	KeyName Alpha
	KeyData 0.0

	KeyName	Size
	KeyData	16.0

	KeyName	Angle
	KeyData	0.0

	KeyName	Frame
	KeyData	1.0
}
