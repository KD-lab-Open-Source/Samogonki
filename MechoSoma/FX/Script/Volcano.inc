KeyID	$FXLAB_ID_KEY_CORE_VOLCANIC_FIRE
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
	KeyData	64.0

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
	KeyData	4.0

	KeyName	Angle
	KeyData	"3.14*8.0"

	KeyName	Frame
	KeyData	1.0
}

KeyID	$FXLAB_ID_KEY_CORE_VOLCANIC_DUST
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
	KeyData	0.2

	KeyName	Green
	KeyData	0.2

	KeyName Blue
	KeyData	0.2

	KeyName Alpha
	KeyData 0.8

	KeyName	Size
	KeyData	32.0

	KeyName	Angle
	KeyData	"3.14*4.0"


	KeyTime	32.0

	KeyName	Red
	KeyData	0.0

	KeyName	Green
	KeyData	0.0

	KeyName Blue
	KeyData	0.0

	KeyName Alpha
	KeyData 0.01

	KeyName	Size
	KeyData	64.0

	KeyName	Angle
	KeyData	"3.14*8.0"

	KeyName	Frame
	KeyData	1.0
}

//---------------------------------------------------------

KeyID	$FXLAB_ID_KEY_VOLCANIC_FIRE_POSITION
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

KeyID	$FXLAB_ID_KEY_VOLCANIC_FIRE_VELOCITY
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

KeyID	$FXLAB_ID_KEY_VOLCANIC_FIRE
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
	KeyData	$FXLAB_ID_SPRITE_VOLCANIC_FIRE

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
	KeyData		$FXLAB_ID_KEY_VOLCANIC_FIRE_POSITION

	KeyName		VelocityKeyID
	KeyData		$FXLAB_ID_KEY_VOLCANIC_FIRE_VELOCITY


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


//-----------------------------------------------------


KeyID	$FXLAB_ID_KEY_VOLCANIC_DUST_POSITION
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
	KeyData	15.0

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

KeyID	$FXLAB_ID_KEY_VOLCANIC_DUST_VELOCITY
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
	KeyData	0

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

KeyID	$FXLAB_ID_KEY_VOLCANIC_DUST
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
	
//	KeyDefine	Damp


	KeyTime	0.0

	KeyName		RegenerateTime
	KeyData		15.0

	KeyName	ImageType
	KeyData	$FXLAB_SCRIPT_SECTION_SPRITE

	KeyName	ImageID
	KeyData	$FXLAB_ID_SPRITE_VOLCANIC_DUST

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
	KeyData		$FXLAB_ID_KEY_VOLCANIC_DUST_POSITION

	KeyName		VelocityKeyID
	KeyData		$FXLAB_ID_KEY_VOLCANIC_DUST_VELOCITY

//	KeyName		Damp
//	KeyData		0.95


	KeyTime	300.0

	KeyName		RegenerateTime
	KeyData		15.0

	KeyName	ImageType
	KeyData	$FXLAB_SCRIPT_SECTION_SPRITE

	KeyName	ImageID
	KeyData	$FXLAB_ID_SPRITE_VOLCANIC_DUST

	KeyName	ImageNoise
	KeyData	0


	KeyName		Rate
	KeyData		0.3

	KeyName		Age
	KeyData		25.0

	KeyName		StartPower
	KeyData		0.0

	KeyName		EndPower
	KeyData		1.0

	KeyName		PositionKeyID
	KeyData		$FXLAB_ID_KEY_VOLCANIC_DUST_POSITION

	KeyName		VelocityKeyID
	KeyData		$FXLAB_ID_KEY_VOLCANIC_DUST_VELOCITY

//	KeyName		Damp
//	KeyData		0.95
}

//----------------------------------------------------------------

KeyID $FXLAB_ID_KEY_VOLCANIC_OMNI
{
	KeyAttribute	$FXLAB_KEY_ATTRIBUTE_REPEAT

	KeyDefine	Red
	KeyDefine	Green
	KeyDefine	Blue
	KeyDefine	Radius

	KeyTime	0.0

	KeyName	Red
	KeyData	1.0

	KeyName Green
	KeyData	0.5

	KeyName Blue
	KeyData 0.3

	KeyName Radius
	KeyData	50.0


	KeyTime	20.0

	KeyName	Red
	KeyData	0.1

	KeyName Green
	KeyData	0.1

	KeyName Blue
	KeyData 0.1


	KeyTime 40.0

	KeyName	Red
	KeyData	1.0

	KeyName Green
	KeyData	0.5

	KeyName Blue
	KeyData 0.3

	KeyName Radius
	KeyData	50.0
}

//--------------------------------------------------

KeyID	$FXLAB_ID_KEY_VOLCANIC_STONE_TAIL_LINK
{
	KeyAttribute	"$FXLAB_KEY_ATTRIBUTE_REPEAT | $FXLAB_KEY_ATTRIBUTE_STORAGE"
	
	KeyDefine	TailType
	KeyDefine	TailID
	KeyDefine	ExplodeType
	KeyDefine	ExplodeID
	KeyDefine   SpeedBorder

	KeyTime	0.0

	KeyName	TailType
	KeyData	$FXLAB_CLIENT_PROCESS_TAIL

	KeyName	TailID
	KeyData $FXLAB_ID_KEY_VOLCANIC_STONE_TAIL

	KeyName	ExplodeType
//	KeyData	$FXLAB_CLIENT_PROCESS_OBJECT_SET
	KeyData -1.0

	KeyName	ExplodeID
//	KeyData	$FXLAB_ID_KEY_STONE_EXPLOSION_CLIENT_SET
	KeyData -1.0

	KeyName SpeedBorder
	KeyData -1.0
}


KeyID	$FXLAB_ID_KEY_VOLCANIC_LAUNCHER
{
	KeyAttribute	$FXLAB_KEY_ATTRIBUTE_REPEAT
	
	KeyDefine	Rate
	KeyDefine	Speed
	KeyDefine	DeltaSpeed
	KeyDefine	Thetta
	KeyDefine   DeltaThetta

	KeyTime	0.0

	KeyName	Rate
	KeyData	0.02

	KeyName Speed
	KeyData 50.0

	KeyName DeltaSpeed
	KeyData 20.0

	KeyName Thetta
	KeyData "3.14*0.05f"

	KeyName DeltaThetta
	KeyData "3.14*0.05f"


	KeyTime	300.0

	KeyName	Rate
	KeyData	0.005

	KeyName Speed
	KeyData 50.0

	KeyName DeltaSpeed
	KeyData 20.0

	KeyName Thetta
	KeyData "3.14*0.05f"

	KeyName DeltaThetta
	KeyData "3.14*0.05f"
}

KeyID	$FXLAB_ID_KEY_HEAD_LAUNCHER
{
	KeyAttribute	$FXLAB_KEY_ATTRIBUTE_REPEAT
	
	KeyDefine	Rate
	KeyDefine	Speed
	KeyDefine	DeltaSpeed
	KeyDefine	Thetta
	KeyDefine   DeltaThetta

	KeyTime	0.0

	KeyName	Rate
	KeyData	0.05

	KeyName Speed
	KeyData 50.0

	KeyName DeltaSpeed
	KeyData 20.0

	KeyName Thetta
	KeyData "3.14*0.05f"

	KeyName DeltaThetta
	KeyData "3.14*0.1f"


	KeyTime	300.0

	KeyName	Rate
	KeyData	0.005

	KeyName Speed
	KeyData 50.0

	KeyName DeltaSpeed
	KeyData 20.0

	KeyName Thetta
	KeyData "3.14*0.05f"

	KeyName DeltaThetta
	KeyData "3.14*0.1f"
}
