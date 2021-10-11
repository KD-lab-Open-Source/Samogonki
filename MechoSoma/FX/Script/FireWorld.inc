//-----------------Dragon Fire Data------------------

KeyID	$FXLAB_ID_KEY_DRAGON_FIRE_POSITION
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

KeyID	$FXLAB_ID_KEY_DRAGON_FIRE_VELOCITY
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

KeyID	$FXLAB_ID_KEY_DRAGON_FIRE
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

	KeyDefine	Damp


	KeyTime	0.0

	KeyName	RegenerateTime
	KeyData	15.0


	KeyName	ImageType
	KeyData	$FXLAB_SCRIPT_SECTION_SPRITE

	KeyName	ImageID
	KeyData	$FXLAB_ID_SPRITE_DRAGON_FIRE

	KeyName	ImageNoise
	KeyData	0


	KeyName		Rate
	KeyData		1.5

	KeyName		Age
	KeyData		20.0

	KeyName		StartPower
	KeyData		0.0

	KeyName		EndPower
	KeyData		1.0

	KeyName		PositionKeyID
	KeyData		$FXLAB_ID_KEY_DRAGON_FIRE_POSITION

	KeyName		VelocityKeyID
	KeyData		$FXLAB_ID_KEY_DRAGON_FIRE_VELOCITY


	KeyName		Damp
	KeyData		0.9
}

KeyID	$FXLAB_ID_KEY_CORE_DRAGON_FIRE
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
	KeyData	0.2

	KeyName Alpha
	KeyData 0.2

	KeyName	Size
	KeyData	16.0

	KeyName	Angle
	KeyData	0.0

	KeyName	Frame
	KeyData	0.0


	KeyTime 8.0

	KeyName Alpha
	KeyData 0.8


	KeyTime	18.0

	KeyName	Red
	KeyData	1.0

	KeyName	Green
	KeyData	0.5

	KeyName Blue
	KeyData	0.0

	KeyName Alpha
	KeyData 0.8


	KeyTime	32.0

	KeyName	Red
	KeyData	0.1

	KeyName	Green
	KeyData	0.0

	KeyName Blue
	KeyData	0.0

	KeyName Alpha
	KeyData 0.2

	KeyName	Size
	KeyData	24.0

	KeyName	Angle
	KeyData	"3.14*8.0"

	KeyName	Frame
	KeyData	1.0
}

KeyID	$FXLAB_ID_KEY_DRAGON_FIRE_SWITCHER
{
	KeyAttribute	"$FXLAB_KEY_ATTRIBUTE_REPEAT | $FXLAB_KEY_ATTRIBUTE_STORAGE"

	KeyDefine	ObjectType
	KeyDefine	ObjectID

	KeyTime	0.0

	KeyName	ObjectType
	KeyData	$FXLAB_CLIENT_PROCESS_DUST

	KeyName	ObjectID
	KeyData	$FXLAB_ID_KEY_DRAGON_FIRE
}


KeyID	$FXLAB_ID_KEY_DRAGON_FIRE_DAMAGE
{
	KeyAttribute	"$FXLAB_KEY_ATTRIBUTE_REPEAT | $FXLAB_KEY_ATTRIBUTE_STORAGE"

	KeyDefine	Power
	KeyDefine	Radius
	KeyDefine	Type
	KeyDefine	Length
	KeyDefine	StartTime
	KeyDefine	EnableTime
	KeyDefine	DisableTime

	KeyTime	0.0

	KeyName	Power
	KeyData	2.0

	KeyName	Radius
	KeyData	15.0

	KeyName	Type
	KeyData	$FXLAB_DAMAGE_SPHERE_TYPE_FIRE

	KeyName	Length
	KeyData	150.0

	KeyName	StartTime
	KeyData 1000.0

	KeyName	EnableTime
	KeyData	2000.0

	KeyName	DisableTime
	KeyData 5000.0
}

//-----------------------------------------


KeyID	$FXLAB_ID_KEY_HEAD_FIRE_DAMAGE
{
	KeyAttribute	"$FXLAB_KEY_ATTRIBUTE_REPEAT | $FXLAB_KEY_ATTRIBUTE_STORAGE"

	KeyDefine	Power
	KeyDefine	Radius
	KeyDefine	Type
	KeyDefine	Length
	KeyDefine	StartTime
	KeyDefine	EnableTime
	KeyDefine	DisableTime

	KeyTime	0.0

	KeyName	Power
	KeyData	0.0

	KeyName	Radius
	KeyData	20.0

	KeyName	Type
	KeyData	$FXLAB_DAMAGE_SPHERE_TYPE_FIRE

	KeyName	Length
	KeyData	250.0

	KeyName	StartTime
	KeyData 0.0

	KeyName	EnableTime
	KeyData	2000.0

	KeyName	DisableTime
	KeyData 6000.0
}

KeyID	$FXLAB_ID_KEY_HEAD_ROSE_DAMAGE
{
	KeyAttribute	"$FXLAB_KEY_ATTRIBUTE_REPEAT | $FXLAB_KEY_ATTRIBUTE_STORAGE"

	KeyDefine	Power
	KeyDefine	Radius
	KeyDefine	Type
	KeyDefine	StartTime
	KeyDefine	EnableTime
	KeyDefine	DisableTime

	KeyTime	0.0

	KeyName	Power
	KeyData	0.0

	KeyName	Radius
	KeyData	20.0

	KeyName	Type
	KeyData	$FXLAB_DAMAGE_SPHERE_TYPE_FIRE

	KeyName	StartTime
	KeyData 1000.0

	KeyName	EnableTime
	KeyData	2000.0

	KeyName	DisableTime
	KeyData 6000.0
}

//-------------------------------------------------------

KeyID	$FXLAB_ID_KEY_CORE_HEAD_BREATH
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
	KeyData	0.5

	KeyName Blue
	KeyData	0.5

	KeyName Alpha
	KeyData 0.2

	KeyName	Size
	KeyData	16.0

	KeyName	Angle
	KeyData	0.0

	KeyName	Frame
	KeyData	0.0


	KeyTime	16.0

	KeyName	Red
	KeyData	0.5

	KeyName	Green
	KeyData	0.5

	KeyName Blue
	KeyData	0.5

	KeyName Alpha
	KeyData 0.8

	KeyName	Size
	KeyData	16.0

	KeyName	Angle
	KeyData	"3.14*4.0"


	KeyTime	32.0

	KeyName	Red
	KeyData	0.5

	KeyName	Green
	KeyData	0.5

	KeyName Blue
	KeyData	0.5

	KeyName Alpha
	KeyData 0.2

	KeyName	Size
	KeyData	16.0

	KeyName	Angle
	KeyData	"3.14*8.0"

	KeyName	Frame
	KeyData	1.0
}

KeyID	$FXLAB_ID_KEY_HEAD_BREATH_POSITION
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

KeyID	$FXLAB_ID_KEY_HEAD_BREATH_VELOCITY
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

KeyID	$FXLAB_ID_KEY_HEAD_BREATH
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

	KeyDefine	Damp	

	KeyTime	0.0

	KeyName	RegenerateTime
	KeyData	15.0

	KeyName	ImageType
	KeyData	$FXLAB_SCRIPT_SECTION_SPRITE

	KeyName	ImageID
	KeyData	$FXLAB_ID_SPRITE_HEAD_BREATH

	KeyName	ImageNoise
	KeyData	0


	KeyName		Rate
	KeyData		1.5

	KeyName		Age
	KeyData		20.0

	KeyName		StartPower
	KeyData		0.0

	KeyName		EndPower
	KeyData		1.0

	KeyName		PositionKeyID
	KeyData		$FXLAB_ID_KEY_HEAD_BREATH_POSITION

	KeyName		VelocityKeyID
	KeyData		$FXLAB_ID_KEY_HEAD_BREATH_VELOCITY


	KeyName		Damp
	KeyData		0.9


	KeyTime	50.0

	KeyName	RegenerateTime
	KeyData	15.0

	KeyName	ImageType
	KeyData	$FXLAB_SCRIPT_SECTION_SPRITE

	KeyName	ImageID
	KeyData	$FXLAB_ID_SPRITE_HEAD_BREATH

	KeyName	ImageNoise
	KeyData	0


	KeyName		Rate
	KeyData		1.0

	KeyName		Age
	KeyData		20.0

	KeyName		StartPower
	KeyData		0.0

	KeyName		EndPower
	KeyData		1.0

	KeyName		PositionKeyID
	KeyData		$FXLAB_ID_KEY_HEAD_BREATH_POSITION

	KeyName		VelocityKeyID
	KeyData		$FXLAB_ID_KEY_HEAD_BREATH_VELOCITY


	KeyName		Damp
	KeyData		0.9
}


KeyID	$FXLAB_ID_KEY_HEAD_BREATH_LAUNCHER
{
	KeyAttribute	"$FXLAB_KEY_ATTRIBUTE_REPEAT | $FXLAB_KEY_ATTRIBUTE_STORAGE"

	KeyDefine	Type
	KeyDefine	KeyID
	KeyDefine	StartTime
	KeyDefine	DisableTime

	KeyTime	0.0

	KeyName	Type
	KeyData	$FXLAB_CLIENT_PROCESS_DUST

	KeyName	KeyID
	KeyData	$FXLAB_ID_KEY_HEAD_BREATH

	KeyName	StartTime
	KeyData	0.0

	KeyName	DisableTime
	KeyData 6000.0
}

//-----------------------------------------------------

KeyID	$FXLAB_ID_KEY_CORE_LAVA_WIND
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
	KeyData	0.1

	KeyName	Green
	KeyData	0.0

	KeyName Blue
	KeyData	0.0

	KeyName Alpha
	KeyData 0.2

	KeyName	Size
	KeyData	16.0

	KeyName	Angle
	KeyData	0.0

	KeyName	Frame
	KeyData	0.0


	KeyTime	16.0

	KeyName	Red
	KeyData	0.1

	KeyName	Green
	KeyData	0.1

	KeyName Blue
	KeyData	0.0

	KeyName Alpha
	KeyData 0.8

	KeyName	Size
	KeyData	16.0

	KeyName	Angle
	KeyData	"3.14*4.0"


	KeyTime	32.0

	KeyName	Red
	KeyData	0.1

	KeyName	Green
	KeyData	0.0

	KeyName Blue
	KeyData	0.0

	KeyName Alpha
	KeyData 0.2

	KeyName	Size
	KeyData	16.0

	KeyName	Angle
	KeyData	"3.14*8.0"

	KeyName	Frame
	KeyData	1.0
}

KeyID	$FXLAB_ID_KEY_LAVA_WIND_POSITION
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

KeyID	$FXLAB_ID_KEY_LAVA_WIND_VELOCITY
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
	KeyData	20.0

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

KeyID	$FXLAB_ID_KEY_LAVA_WIND
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

	KeyDefine	Damp


	KeyTime	0.0

	KeyName	RegenerateTime
	KeyData	15.0


	KeyName	ImageType
	KeyData	$FXLAB_SCRIPT_SECTION_SPRITE

	KeyName	ImageID
	KeyData	$FXLAB_ID_SPRITE_LAVA_WIND

	KeyName	ImageNoise
	KeyData	0


	KeyName		Rate
	KeyData		1.0

	KeyName		Age
	KeyData		20.0

	KeyName		StartPower
	KeyData		0.0

	KeyName		EndPower
	KeyData		1.0

	KeyName		PositionKeyID
	KeyData		$FXLAB_ID_KEY_LAVA_WIND_POSITION

	KeyName		VelocityKeyID
	KeyData		$FXLAB_ID_KEY_LAVA_WIND_VELOCITY


	KeyName		Damp
	KeyData		0.9


	KeyTime	5.0

	KeyName	RegenerateTime
	KeyData	15.0

	KeyName	ImageType
	KeyData	$FXLAB_SCRIPT_SECTION_SPRITE

	KeyName	ImageID
	KeyData	$FXLAB_ID_SPRITE_LAVA_WIND

	KeyName	ImageNoise
	KeyData	0


	KeyName		Rate
	KeyData		1.5

	KeyName		Age
	KeyData		20.0

	KeyName		StartPower
	KeyData		0.0

	KeyName		EndPower
	KeyData		1.0

	KeyName		PositionKeyID
	KeyData		$FXLAB_ID_KEY_LAVA_WIND_POSITION

	KeyName		VelocityKeyID
	KeyData		$FXLAB_ID_KEY_LAVA_WIND_VELOCITY

	KeyName		Damp
	KeyData		0.9
}


KeyID	$FXLAB_ID_KEY_LAVA_WIND_LAUNCHER
{
	KeyAttribute	"$FXLAB_KEY_ATTRIBUTE_REPEAT | $FXLAB_KEY_ATTRIBUTE_STORAGE"

	KeyDefine	Type
	KeyDefine	KeyID
	KeyDefine	StartTime
	KeyDefine	DisableTime

	KeyTime	0.0

	KeyName	Type
	KeyData	$FXLAB_CLIENT_PROCESS_DUST

	KeyName	KeyID
	KeyData	$FXLAB_ID_KEY_LAVA_WIND

	KeyName	StartTime
	KeyData	0.0

	KeyName	DisableTime
	KeyData 6000.0
}

//------------------------------------------------

KeyID	$FXLAB_ID_KEY_CORE_LAVA_ALTAR_FIRE
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
	KeyData	0.2

	KeyName Alpha
	KeyData 0.2

	KeyName	Size
	KeyData	16.0

	KeyName	Angle
	KeyData	0.0

	KeyName	Frame
	KeyData	0.0


	KeyTime 8.0

	KeyName Alpha
	KeyData 0.8


	KeyTime	18.0

	KeyName	Red
	KeyData	1.0

	KeyName	Green
	KeyData	0.5

	KeyName Blue
	KeyData	0.0

	KeyName Alpha
	KeyData 0.8


	KeyTime	32.0

	KeyName	Red
	KeyData	0.1

	KeyName	Green
	KeyData	0.0

	KeyName Blue
	KeyData	0.0

	KeyName Alpha
	KeyData 0.2

	KeyName	Size
	KeyData	16.0

	KeyName	Angle
	KeyData	"3.14*8.0"

	KeyName	Frame
	KeyData	1.0
}

KeyID	$FXLAB_ID_KEY_LAVA_ALTAR_FIRE_POSITION
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
	KeyData	8.0

	KeyName	DataB1
	KeyData	8.0

	KeyName	DataC1
	KeyData	0

	KeyName	DataA2
	KeyData	0

	KeyName	DataB2
	KeyData	0

	KeyName	DataC2
	KeyData	0
}

KeyID	$FXLAB_ID_KEY_LAVA_ALTAR_FIRE_VELOCITY
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

KeyID	$FXLAB_ID_KEY_LAVA_ALTAR_FIRE
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

	KeyName		RegenerateTime
	KeyData		15.0

	KeyName	ImageType
	KeyData	$FXLAB_SCRIPT_SECTION_SPRITE

	KeyName	ImageID
	KeyData	$FXLAB_ID_SPRITE_LAVA_ALTAR_FIRE

	KeyName	ImageNoise
	KeyData	0


	KeyName		Rate
	KeyData		1.0

	KeyName		Age
	KeyData		25.0

	KeyName		StartPower
	KeyData		0.5

	KeyName		EndPower
	KeyData		1.0

	KeyName		PositionKeyID
	KeyData		$FXLAB_ID_KEY_LAVA_ALTAR_FIRE_POSITION

	KeyName		VelocityKeyID
	KeyData		$FXLAB_ID_KEY_LAVA_ALTAR_FIRE_VELOCITY



	KeyName		Power
	KeyData		30.0

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

KeyID	$FXLAB_ID_KEY_LAVA_ALTAR_OMNI
{
	KeyAttribute	$FXLAB_KEY_ATTRIBUTE_REPEAT

	KeyDefine	Red
	KeyDefine	Green
	KeyDefine	Blue
	KeyDefine	Radius

	KeyTime	0.0

	KeyName	Red
	KeyData	0.3

	KeyName Green
	KeyData	0.1

	KeyName Blue
	KeyData 0.0

	KeyName Radius
	KeyData	10.0


	KeyTime	15.0

	KeyName	Red
	KeyData	0.5

	KeyName Green
	KeyData	0.2

	KeyName Blue
	KeyData 0.0

	KeyName Radius
	KeyData	25.0


	KeyTime	30.0

	KeyName	Red
	KeyData	0.3

	KeyName Green
	KeyData	0.1

	KeyName Blue
	KeyData 0.0

	KeyName Radius
	KeyData	10.0
}


KeyID $FXLAB_ID_KEY_LAVA_ALTAR_DAMAGE
{
	KeyAttribute	"$FXLAB_KEY_ATTRIBUTE_REPEAT | $FXLAB_KEY_ATTRIBUTE_STORAGE"
	
	KeyDefine	Power
	KeyDefine	Radius
	KeyDefine	DamageType

	KeyTime	0.0

	KeyName	Power
	KeyData	1.0

	KeyName	Radius
	KeyData	40.0

	KeyName	DamageType
	KeyData	$FXLAB_DAMAGE_SPHERE_TYPE_FIRE
}

//-------------------------------------------

KeyID	$FXLAB_ID_KEY_CORE_DRAGON_ROSE
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
	KeyData	0.2

	KeyName Alpha
	KeyData 0.2

	KeyName	Size
	KeyData	16.0

	KeyName	Angle
	KeyData	0.0

	KeyName	Frame
	KeyData	0.0


	KeyTime 8.0

	KeyName Alpha
	KeyData 0.8


	KeyTime	18.0

	KeyName	Red
	KeyData	1.0

	KeyName	Green
	KeyData	0.5

	KeyName Blue
	KeyData	0.0

	KeyName Alpha
	KeyData 0.8


	KeyTime	32.0

	KeyName	Red
	KeyData	0.1

	KeyName	Green
	KeyData	0.0

	KeyName Blue
	KeyData	0.0

	KeyName Alpha
	KeyData 0.2

	KeyName	Size
	KeyData	16.0

	KeyName	Angle
	KeyData	"3.14*8.0"

	KeyName	Frame
	KeyData	1.0
}

KeyID	$FXLAB_ID_KEY_DRAGON_ROSE_POSITION
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

KeyID	$FXLAB_ID_KEY_DRAGON_ROSE_VELOCITY
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
	KeyData	4.0

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

KeyID	$FXLAB_ID_KEY_DRAGON_ROSE
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

	KeyDefine	Damp


	KeyTime	0.0

	KeyName	RegenerateTime
	KeyData	15.0

	KeyName	ImageType
	KeyData	$FXLAB_SCRIPT_SECTION_SPRITE

	KeyName	ImageID
	KeyData	$FXLAB_ID_SPRITE_DRAGON_ROSE

	KeyName	ImageNoise
	KeyData	0


	KeyName		Rate
	KeyData		1.0

	KeyName		Age
	KeyData		15.0

	KeyName		StartPower
	KeyData		0.5

	KeyName		EndPower
	KeyData		1.0

	KeyName		PositionKeyID
	KeyData		$FXLAB_ID_KEY_DRAGON_ROSE_POSITION

	KeyName		VelocityKeyID
	KeyData		$FXLAB_ID_KEY_DRAGON_ROSE_VELOCITY


	KeyName		Damp
	KeyData		0.95
}

KeyID	$FXLAB_ID_KEY_DRAGON_ROSE_OMNI
{
	KeyAttribute	$FXLAB_KEY_ATTRIBUTE_REPEAT

	KeyDefine	Red
	KeyDefine	Green
	KeyDefine	Blue
	KeyDefine	Radius

	KeyTime	0.0

	KeyName	Red
	KeyData	0.3

	KeyName Green
	KeyData	0.1

	KeyName Blue
	KeyData 0.0

	KeyName Radius
	KeyData	10.0


	KeyTime	15.0

	KeyName	Red
	KeyData	0.5

	KeyName Green
	KeyData	0.2

	KeyName Blue
	KeyData 0.0

	KeyName Radius
	KeyData	25.0


	KeyTime	30.0

	KeyName	Red
	KeyData	0.3

	KeyName Green
	KeyData	0.1

	KeyName Blue
	KeyData 0.0

	KeyName Radius
	KeyData	10.0
}


KeyID $FXLAB_ID_KEY_DRAGON_ROSE_DAMAGE
{
	KeyAttribute	"$FXLAB_KEY_ATTRIBUTE_NONE | $FXLAB_KEY_ATTRIBUTE_STORAGE"
	
	KeyDefine	Power
	KeyDefine	Radius
	KeyDefine	DamageType

	KeyTime	50.0

	KeyName	Power
	KeyData	0.01

	KeyName	Radius
	KeyData	40.0

	KeyName	DamageType
	KeyData	$FXLAB_DAMAGE_SPHERE_TYPE_FIRE
}

KeyID	$FXLAB_ID_KEY_DRAGON_ROSE_SET
{
	KeyAttribute	"$FXLAB_KEY_ATTRIBUTE_NONE | $FXLAB_KEY_ATTRIBUTE_STORAGE"

	KeyDefine	Mode
	KeyDefine	VelX
	KeyDefine	VelY
	KeyDefine	VelZ
	KeyDefine	Num
	KeyDefine	ObjectType0
	KeyDefine	KeyID0
	KeyDefine	ObjectType1
	KeyDefine	KeyID1

	KeyTime	50.0

	KeyName	Mode
	KeyData	$FXLAB_CLIENT_SET_MODE_DESTROY

	KeyName	VelX
	KeyData	0.0

	KeyName	VelY
	KeyData	0.0

	KeyName	VelZ
	KeyData	0.0

	KeyName	Num
	KeyData	2


	KeyName	ObjectType0
	KeyData	$FXLAB_CLIENT_PROCESS_DUST

	KeyName	KeyID0
	KeyData $FXLAB_ID_KEY_DRAGON_ROSE

	KeyName	ObjectType1
	KeyData $FXLAB_CLIENT_PROCESS_CONTROL_OMNI

	KeyName	KeyID1
	KeyData $FXLAB_ID_KEY_DRAGON_ROSE_OMNI
}
