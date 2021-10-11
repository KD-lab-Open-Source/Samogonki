KeyID	$FXLAB_ID_KEY_CORE_STONE_BUBBLE_FLUX
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
	KeyData 0.05

	KeyName	Size
	KeyData	4.0

	KeyName	Angle
	KeyData	0.0

	KeyName	Frame
	KeyData	0.0

	
	KeyTime	16.0

	KeyName	Alpha
	KeyData	1.0


	KeyTime	32.0

	KeyName	Red
	KeyData	0.0

	KeyName	Green
	KeyData	1.0

	KeyName Blue
	KeyData	0.0

	KeyName Alpha
	KeyData 0.05

	KeyName	Size
	KeyData	4.0

	KeyName	Angle
	KeyData	0.0

	KeyName	Frame
	KeyData	1.0
}


KeyID	$FXLAB_ID_KEY_STONE_BUBBLE_FLUX_POSITION
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
	KeyData	25.0

	KeyName	DataA1
	KeyData	0.0

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

KeyID	$FXLAB_ID_KEY_STONE_BUBBLE_FLUX_VELOCITY
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
	KeyData	0.0

	KeyName	DataA1
	KeyData	2.0

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

KeyID	$FXLAB_ID_KEY_STONE_BUBBLE_FLUX
{
	KeyAttribute	$FXLAB_KEY_ATTRIBUTE_REPEAT

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
	KeyData	15

	KeyName	ImageType
	KeyData	$FXLAB_SCRIPT_SECTION_SPRITE

	KeyName	ImageID
	KeyData $FXLAB_ID_SPRITE_STONE_BUBBLE_FLUX

	KeyName	ImageNoise
	KeyData	0


	KeyName	Rate
	KeyData	1.5

	KeyName Age
	KeyData	30.0

	KeyName	StartPower
	KeyData	1.0

	KeyName	EndPower
	KeyData	0.0

	KeyName	PositionKeyID
	KeyData $FXLAB_ID_KEY_STONE_BUBBLE_FLUX_POSITION

	KeyName	VelocityKeyID
	KeyData $FXLAB_ID_KEY_STONE_BUBBLE_FLUX_VELOCITY


	KeyName	Speed
	KeyData 5.0

	
	KeyTime	20.0

	KeyName	Rate
	KeyData	0.0

	KeyName	Speed
	KeyData 0.0


	KeyTime	280.0

	KeyName	Rate
	KeyData	0.0

	KeyName	Speed
	KeyData 0.0


	KeyTime	300.0

	KeyName	RegenerateTime
	KeyData	15

	KeyName	ImageType
	KeyData	$FXLAB_SCRIPT_SECTION_SPRITE

	KeyName	ImageID
	KeyData $FXLAB_ID_SPRITE_STONE_BUBBLE_FLUX

	KeyName	ImageNoise
	KeyData	0


	KeyName	Rate
	KeyData	1.5

	KeyName Age
	KeyData	30.0

	KeyName	StartPower
	KeyData	1.0

	KeyName	EndPower
	KeyData	0.0

	KeyName	PositionKeyID
	KeyData $FXLAB_ID_KEY_STONE_BUBBLE_FLUX_POSITION

	KeyName	VelocityKeyID
	KeyData $FXLAB_ID_KEY_STONE_BUBBLE_FLUX_VELOCITY


	KeyName	Speed
	KeyData 5.0
}


KeyID	$FXLAB_ID_KEY_STONE_FIELD_STREAM
{
	KeyAttribute	$FXLAB_KEY_ATTRIBUTE_REPEAT

	KeyDefine	Force
	KeyDefine	Radius

	KeyTime	0.0

	KeyName	Force
	KeyData	300.0

	KeyName	Radius
	KeyData	40.0


	KeyTime	20.0

	KeyName	Force
	KeyData	0.2


	KeyTime	280.0

	KeyName	Force
	KeyData	0.2


	KeyTime	300.0

	KeyName	Force
	KeyData	300.0

	KeyName	Radius
	KeyData	60.0
}

//--------------------------------------


KeyID	$FXLAB_ID_KEY_CORE_ALTAR_DUST
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
	KeyData	0.3

	KeyName Blue
	KeyData	0.0

	KeyName Alpha
	KeyData 0.001

	KeyName	Size
	KeyData	64.0

	KeyName	Angle
	KeyData	0.0

	KeyName	Frame
	KeyData	0.0


	KeyTime	16.0

	KeyName Alpha
	KeyData 0.5

	KeyName	Angle
	KeyData	"3.14*4.0"


	KeyTime	32.0

	KeyName	Red
	KeyData	0.1

	KeyName	Green
	KeyData	0.4

	KeyName Blue
	KeyData	0.1

	KeyName Alpha
	KeyData 0.001

	KeyName	Size
	KeyData	128.0

	KeyName	Angle
	KeyData	"3.14*8.0"

	KeyName	Frame
	KeyData	1.0
}


KeyID	$FXLAB_ID_KEY_ALTAR_DUST_POSITION
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
	KeyData	25.0

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

KeyID	$FXLAB_ID_KEY_ALTAR_DUST_VELOCITY
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
	KeyData	$FXLAB_SWARM_DOMAIN_LINE

	KeyName	DataX
	KeyData	0

	KeyName	DataY
	KeyData	0

	KeyName	DataZ
	KeyData	0.5

	KeyName	DataA1
	KeyData	0.0

	KeyName	DataB1
	KeyData	0

	KeyName	DataC1
	KeyData	10.0

	KeyName	DataA2
	KeyData	0

	KeyName	DataB2
	KeyData	0

	KeyName	DataC2
	KeyData	0
}

KeyID	$FXLAB_ID_KEY_ALTAR_DUST
{
	KeyAttribute	"$FXLAB_KEY_ATTRIBUTE_REPEAT | $FXLAB_KEY_ATTRIBUTE_STORAGE"

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

	KeyName		RegenerateTime
	KeyData		15.0

	KeyName	ImageType
	KeyData	$FXLAB_SCRIPT_SECTION_SPRITE

	KeyName	ImageID
	KeyData	$FXLAB_ID_SPRITE_ALTAR_DUST

	KeyName	ImageNoise
	KeyData	0


	KeyName		Rate
	KeyData		0.5

	KeyName		Age
	KeyData		25.0

	KeyName		StartPower
	KeyData		0.0

	KeyName		EndPower
	KeyData		1.0

	KeyName		PositionKeyID
	KeyData		$FXLAB_ID_KEY_ALTAR_DUST_POSITION

	KeyName		VelocityKeyID
	KeyData		$FXLAB_ID_KEY_ALTAR_DUST_VELOCITY
}

//-------------------------------------------------

KeyID	$FXLAB_ID_KEY_CORE_ALTAR_FIRE
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
	KeyData 0.1

	KeyName	Size
	KeyData	16.0

	KeyName	Angle
	KeyData	0.0

	KeyName	Frame
	KeyData	0.0	


	KeyTime	4.0

	KeyName	Alpha
	KeyData	0.5


	KeyTime	32.0

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

KeyID	$FXLAB_ID_KEY_ALTAR_FIRE_POSITION
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
	KeyData	10.0

	KeyName	DataB1
	KeyData	5.0

	KeyName	DataC1
	KeyData	0

	KeyName	DataA2
	KeyData	0

	KeyName	DataB2
	KeyData	0

	KeyName	DataC2
	KeyData	0
}

KeyID	$FXLAB_ID_KEY_ALTAR_FIRE_VELOCITY
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
	KeyData	1.0

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

KeyID	$FXLAB_ID_KEY_ALTAR_FIRE
{
	KeyAttribute	"$FXLAB_KEY_ATTRIBUTE_REPEAT | $FXLAB_KEY_ATTRIBUTE_STORAGE"

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

	KeyDefine	Power
	KeyDefine	Epsilon
	KeyDefine	DirectionX
	KeyDefine	DirectionY
	KeyDefine	DirectionZ
	KeyDefine	AxisX
	KeyDefine	AxisY
	KeyDefine	AxisZ


	KeyTime	0.0

	KeyName	RegenerateTime
	KeyData	15.0

	KeyName	ImageType
	KeyData	$FXLAB_SCRIPT_SECTION_SPRITE

	KeyName	ImageID
	KeyData	$FXLAB_ID_SPRITE_ALTAR_FIRE

	KeyName	ImageNoise
	KeyData	0


	KeyName		Rate
	KeyData		1.0

	KeyName		Age
	KeyData		25.0

	KeyName		StartPower
	KeyData		0.0

	KeyName		EndPower
	KeyData		1.0

	KeyName		PositionKeyID
	KeyData		$FXLAB_ID_KEY_ALTAR_FIRE_POSITION

	KeyName		VelocityKeyID
	KeyData		$FXLAB_ID_KEY_ALTAR_FIRE_VELOCITY



	KeyName		Power
	KeyData		20.0

	KeyName		Epsilon
	KeyData		80.0

	KeyName		DirectionX
	KeyData		0.0

	KeyName		DirectionY
	KeyData		0.0

	KeyName		DirectionZ
	KeyData		1.0

	KeyName		AxisX
	KeyData		0.0

	KeyName		AxisY
	KeyData		0.0

	KeyName		AxisZ
	KeyData		1.0
}


KeyID $FXLAB_ID_KEY_ALTAR_DAMAGE
{
	KeyAttribute	"$FXLAB_KEY_ATTRIBUTE_REPEAT | $FXLAB_KEY_ATTRIBUTE_STORAGE"
	
	KeyDefine	Power
	KeyDefine	Radius
	KeyDefine	DamageType

	KeyTime	0.0

	KeyName	Power
	KeyData	100.0

	KeyName	Radius
	KeyData	70.0

	KeyName DamageType
	KeyData	$FXLAB_DAMAGE_SPHERE_TYPE_GENERIC
}

//-------------------------------------------------------

KeyID	$FXLAB_ID_KEY_CORE_HEAD_STREAM
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
	KeyData	0.3

	KeyName Blue
	KeyData	0.0

	KeyName Alpha
	KeyData 0.1

	KeyName	Size
	KeyData	8.0

	KeyName	Angle
	KeyData	0.0

	KeyName	Frame
	KeyData	0.0

	
	KeyTime	16.0

	KeyName	Alpha
	KeyData	0.7


	KeyTime	32.0

	KeyName	Red
	KeyData	0.0

	KeyName	Green
	KeyData	1.0

	KeyName Blue
	KeyData	0.0

	KeyName Alpha
	KeyData 0.05

	KeyName	Size
	KeyData	16.0

	KeyName	Angle
	KeyData	0.0

	KeyName	Frame
	KeyData	1.0
}


KeyID	$FXLAB_ID_KEY_HEAD_STREAM_POSITION
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
	KeyData	2.0

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

KeyID	$FXLAB_ID_KEY_HEAD_STREAM_VELOCITY
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
	KeyData	0.0

	KeyName	DataY
	KeyData	0.0

	KeyName	DataZ
	KeyData	0.0

	KeyName	DataA1
	KeyData	0.1

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

KeyID	$FXLAB_ID_KEY_HEAD_STREAM
{
	KeyAttribute	"$FXLAB_KEY_ATTRIBUTE_REPEAT | $FXLAB_KEY_ATTRIBUTE_STORAGE"
	
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

	KeyDefine	ForceX
	KeyDefine	ForceY
	KeyDefine	ForceZ
	KeyDefine	ReflectionImpulse
	KeyDefine	VelocityImpulse

	KeyTime	0.0

	KeyName	RegenerateTime
	KeyData	50.0

	KeyName	ImageType
	KeyData	$FXLAB_SCRIPT_SECTION_SPRITE

	KeyName	ImageID
	KeyData	$FXLAB_ID_SPRITE_HEAD_STREAM

	KeyName	ImageNoise
	KeyData	0


	KeyName	Rate
	KeyData 1.0

	KeyName	Age
	KeyData 80.0

	KeyName	StartPower
	KeyData 1.0

	KeyName	EndPower
	KeyData 0.0

	KeyName	PositionKeyID
	KeyData $FXLAB_ID_KEY_HEAD_STREAM_POSITION

	KeyName VelocityKeyID
	KeyData $FXLAB_ID_KEY_HEAD_STREAM_VELOCITY


	KeyName ForceX
	KeyData	0

	KeyName	ForceY
	KeyData	0

	KeyName	ForceZ
	KeyData	-0.5

	KeyName ReflectionImpulse
	KeyData	0.5

	KeyName VelocityImpulse
	KeyData	1.0
}

//-----------------

KeyID	$FXLAB_ID_KEY_CORE_STONE_DUST
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
	KeyData	0.35

	KeyName	Green
	KeyData	0.28

	KeyName Blue
	KeyData	0.0

	KeyName Alpha
	KeyData 0.2

	KeyName	Size
	KeyData	4.0

	KeyName	Angle
	KeyData	0.0

	KeyName	Frame
	KeyData	0.0


	KeyTime	1.0

	KeyName Alpha
	KeyData 1.0

	KeyName	Size
	KeyData	16.0


	KeyTime	16.0

	KeyName	Red
	KeyData	0.35

	KeyName	Green
	KeyData	0.28

	KeyName Blue
	KeyData	0.0

	KeyName Alpha
	KeyData 0.0

	KeyName	Size
	KeyData	32.0

	KeyName	Angle
	KeyData	"3.14*8.0"

	KeyName	Frame
	KeyData	1.0
}


KeyID	$FXLAB_ID_KEY_STONE_DUST_POSITION
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
	KeyData	0.0

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

KeyID	$FXLAB_ID_KEY_STONE_DUST_VELOCITY
{
	KeyAttribute	$FXLAB_KEY_ATTRIBUTE_FREEZE
	
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
	KeyData	$FXLAB_SWARM_DOMAIN_LINE

	KeyName	DataX
	KeyData	0

	KeyName	DataY
	KeyData	0

	KeyName	DataZ
	KeyData	0.0

	KeyName	DataA1
	KeyData	0.0

	KeyName	DataB1
	KeyData	0.0

	KeyName	DataC1
	KeyData	1.0

	KeyName	DataA2
	KeyData	0

	KeyName	DataB2
	KeyData	0

	KeyName	DataC2
	KeyData	0


	KeyTime	5.0

	KeyName	Type
	KeyData	$FXLAB_SWARM_DOMAIN_SPHERE

	KeyName	DataX
	KeyData	0

	KeyName	DataY
	KeyData	0

	KeyName	DataZ
	KeyData	1.0

	KeyName	DataA1
	KeyData	0.0

	KeyName	DataB1
	KeyData	0.0

	KeyName	DataC1
	KeyData	5.0

	KeyName	DataA2
	KeyData	0

	KeyName	DataB2
	KeyData	0

	KeyName	DataC2
	KeyData	0
}

KeyID	$FXLAB_ID_KEY_STONE_DUST
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
	KeyData	$FXLAB_ID_SPRITE_STONE_DUST

	KeyName	ImageNoise
	KeyData	0


	KeyName	Rate
	KeyData 10.0

	KeyName	Age
	KeyData 20.0

	KeyName	StartPower
	KeyData 0.0

	KeyName	EndPower
	KeyData 1.0

	KeyName	PositionKeyID
	KeyData	$FXLAB_ID_KEY_STONE_DUST_POSITION

	KeyName	VelocityKeyID
	KeyData	$FXLAB_ID_KEY_STONE_DUST_VELOCITY


	KeyTime	5.0

	KeyName	RegenerateTime
	KeyData	15.0

	KeyName	ImageType
	KeyData	$FXLAB_SCRIPT_SECTION_SPRITE

	KeyName	ImageID
	KeyData	$FXLAB_ID_SPRITE_STONE_DUST

	KeyName	ImageNoise
	KeyData	0



	KeyName	Rate
	KeyData 1.0

	KeyName	Age
	KeyData 40.0

	KeyName	StartPower
	KeyData 0.0

	KeyName	EndPower
	KeyData 1.0

	KeyName	PositionKeyID
	KeyData	$FXLAB_ID_KEY_STONE_DUST_POSITION

	KeyName	VelocityKeyID
	KeyData	$FXLAB_ID_KEY_STONE_DUST_VELOCITY
}

//-----------------------------------------------

KeyID	$FXLAB_ID_KEY_CORE_WHITE_STONE_DUST
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
	KeyData	1.0

	KeyName Alpha
	KeyData 0.2

	KeyName	Size
	KeyData	4.0

	KeyName	Angle
	KeyData	0.0

	KeyName	Frame
	KeyData	0.0


	KeyTime	1.0

	KeyName Alpha
	KeyData 1.0

	KeyName	Size
	KeyData	16.0


	KeyTime	16.0

	KeyName	Red
	KeyData	1.0

	KeyName	Green
	KeyData	1.0

	KeyName Blue
	KeyData	1.0

	KeyName Alpha
	KeyData 0.0

	KeyName	Size
	KeyData	32.0

	KeyName	Angle
	KeyData	"3.14*8.0"

	KeyName	Frame
	KeyData	1.0
}


KeyID	$FXLAB_ID_KEY_STONE_WHITE_DUST
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
	KeyData	$FXLAB_ID_SPRITE_WHITE_STONE_DUST

	KeyName	ImageNoise
	KeyData	0


	KeyName	Rate
	KeyData 10.0

	KeyName	Age
	KeyData 20.0

	KeyName	StartPower
	KeyData 0.0

	KeyName	EndPower
	KeyData 1.0

	KeyName	PositionKeyID
	KeyData	$FXLAB_ID_KEY_STONE_DUST_POSITION

	KeyName	VelocityKeyID
	KeyData	$FXLAB_ID_KEY_STONE_DUST_VELOCITY


	KeyTime	5.0

	KeyName	RegenerateTime
	KeyData	15.0

	KeyName	ImageType
	KeyData	$FXLAB_SCRIPT_SECTION_SPRITE

	KeyName	ImageID
	KeyData	$FXLAB_ID_SPRITE_WHITE_STONE_DUST

	KeyName	ImageNoise
	KeyData	0



	KeyName	Rate
	KeyData 1.0

	KeyName	Age
	KeyData 40.0

	KeyName	StartPower
	KeyData 0.0

	KeyName	EndPower
	KeyData 1.0

	KeyName	PositionKeyID
	KeyData	$FXLAB_ID_KEY_STONE_DUST_POSITION

	KeyName	VelocityKeyID
	KeyData	$FXLAB_ID_KEY_STONE_DUST_VELOCITY
}
