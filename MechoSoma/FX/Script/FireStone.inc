KeyID	$FXLAB_ID_KEY_CORE_VOLCANIC_STONE_FIRE
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
	KeyData 0.0

	KeyName	Size
	KeyData	16.0

	KeyName	Angle
	KeyData	"3.14*8.0"

	KeyName	Frame
	KeyData	1.0
}

KeyID	$FXLAB_ID_KEY_VOLCANIC_STONE_TAIL_POSITION
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

KeyID	$FXLAB_ID_KEY_VOLCANIC_STONE_TAIL_VELOCITY
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
	KeyData	0.5

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

KeyID	$FXLAB_ID_KEY_VOLCANIC_STONE_TAIL
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

	KeyDefine	PointPower
	KeyDefine	PointEpsilon
	KeyDefine	PointRadius


	KeyTime	0.0

	KeyName	RegenerateTime
	KeyData	15.0

	KeyName	ImageType
	KeyData	$FXLAB_SCRIPT_SECTION_SPRITE

	KeyName	ImageID
	KeyData	$FXLAB_ID_SPRITE_VOLCANIC_STONE_FIRE

	KeyName	ImageNoise
	KeyData	0


	KeyName	Rate
	KeyData 1.0

	KeyName	Age
	KeyData 15.0

	KeyName	StartPower
	KeyData 0.5

	KeyName	EndPower
	KeyData 1.0

	KeyName	PositionKeyID
	KeyData $FXLAB_ID_KEY_VOLCANIC_STONE_TAIL_POSITION

	KeyName VelocityKeyID
	KeyData $FXLAB_ID_KEY_VOLCANIC_STONE_TAIL_VELOCITY


	KeyName	PointPower
	KeyData 100.0

	KeyName	PointEpsilon
	KeyData 2.0

	KeyName	PointRadius
	KeyData 100.0
}

//-------------------------------------------------

KeyID $FXLAB_ID_KEY_SERVER_FIRE_STONE_DAMAGE
{
	KeyAttribute	$FXLAB_KEY_ATTRIBUTE_NONE
	
	KeyDefine	Power
	KeyDefine	Radius
	KeyDefine	DamageType

	KeyTime	0.0

	KeyName	Power
	KeyData	20.0

	KeyName	Radius
	KeyData	15.0

	KeyName	DamageType
	KeyData	$FXLAB_DAMAGE_SPHERE_TYPE_FIRE


	KeyTime	6.0

	KeyName	Power
	KeyData	0.0

	KeyName	Radius
	KeyData	1.0

	KeyName DamageType
	KeyData	$FXLAB_DAMAGE_SPHERE_TYPE_FIRE
}

KeyID	$FXLAB_ID_KEY_SERVER_FIRE_STONE_LINK
{
	KeyAttribute	"$FXLAB_KEY_ATTRIBUTE_REPEAT | $FXLAB_KEY_ATTRIBUTE_STORAGE"
	
	KeyDefine	ExplodeType
	KeyDefine	ExplodeID

	KeyTime	0.0

	KeyName	ExplodeType
	KeyData	$FXLAB_SERVER_PROCESS_DAMAGE_SENSOR

	KeyName	ExplodeID
	KeyData	$FXLAB_ID_KEY_SERVER_FIRE_STONE_DAMAGE
}

KeyID	$FXLAB_ID_KEY_FIRE_STONE_TAIL_LINK
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
//	KeyData	$FXLAB_ID_KEY_FIRE_EXPLOSION_CLIENT_SET
	KeyData -1.0

	KeyName SpeedBorder
	KeyData -1.0
}


KeyID	$FXLAB_ID_KEY_CORE_FIRE_CLOUDS
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
	KeyData	0.0

	KeyName Alpha
	KeyData 0.0

	KeyName	Size
	KeyData	32.0

	KeyName	Angle
	KeyData	0.0

	KeyName	Frame
	KeyData	0.0


	KeyTime	16.0

	KeyName Alpha
	KeyData 0.6


	KeyTime	32.0

	KeyName	Red
	KeyData	0.5

	KeyName	Green
	KeyData	0.0

	KeyName Blue
	KeyData	0.0

	KeyName Alpha
	KeyData 0.0

	KeyName	Size
	KeyData	128.0

	KeyName	Angle
	KeyData	"2.0*3.14"

	KeyName	Frame
	KeyData	1.0
}

KeyID $FXLAB_ID_KEY_FIRE_CLOUDS
{
	KeyAttribute	"$FXLAB_KEY_ATTRIBUTE_REPEAT | $FXLAB_KEY_ATTRIBUTE_STORAGE"
	
	KeyDefine	RegenerateTime

	KeyDefine	ImageType
	KeyDefine	ImageID
	KeyDefine	ImageNoise

	KeyDefine	NumBranch
	KeyDefine	NumWing
	KeyDefine	StartHeight
	KeyDefine	EndHeight
	KeyDefine	StartRadius
	KeyDefine   EndRadius
	KeyDefine	StartPower
	KeyDefine	EndPower
	KeyDefine	StartBranchPower
	KeyDefine	EndBranchPower
	KeyDefine	Age
	KeyDefine	Noise
	KeyDefine	EmitterSpeed
	KeyDefine	EmitterAngle


	KeyTime	0.0

	KeyName	RegenerateTime
	KeyData	50.0

	KeyName	ImageType
	KeyData	$FXLAB_SCRIPT_SECTION_SPRITE

	KeyName	ImageID
	KeyData	$FXLAB_ID_SPRITE_FIRE_CLOUDS

	KeyName	ImageNoise
	KeyData	0


	KeyName	NumBranch
	KeyData	3.0

	KeyName	NumWing
	KeyData	1.0

	KeyName	StartHeight
	KeyData	0.0

	KeyName	EndHeight
	KeyData	50.0

	KeyName	StartRadius
	KeyData	5.0

	KeyName	EndRadius
	KeyData	200.0

	KeyName	StartPower
	KeyData	0.0

	KeyName	EndPower
	KeyData	1.0

	KeyName	StartBranchPower
	KeyData	1.0

	KeyName	EndBranchPower
	KeyData	1.0

	KeyName	Age
	KeyData	20.0

	KeyName	Noise
	KeyData	1.0

	KeyName	EmitterSpeed
	KeyData	"-3.14/50.0"

	KeyName	EmitterAngle
	KeyData	3.14
}


KeyID $FXLAB_ID_KEY_FIRE_BULLET_MODEL
{
	KeyAttribute	"$FXLAB_KEY_ATTRIBUTE_FREEZE | $FXLAB_KEY_ATTRIBUTE_STORAGE"

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
	KeyData	76.0

	KeyName	Red
	KeyData	1.0

	KeyName	Green
	KeyData	1.0

	KeyName	Blue
	KeyData	1.0

	KeyName	Alpha
	KeyData	1.0


	KeyName	Radius
	KeyData	15.0

	KeyName	OffsetAngleX
	KeyData	0.0

	KeyName	OffsetAngleY
	KeyData	0.0

	KeyName	OffsetAngleZ
	KeyData	0.0
}

//--------------------------

KeyID	$FXLAB_ID_KEY_CORE_FIRE_BULLET
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
	KeyData 0.0

	KeyName	Size
	KeyData	16.0

	KeyName	Angle
	KeyData	"3.14*8.0"

	KeyName	Frame
	KeyData	1.0
}

KeyID	$FXLAB_ID_KEY_FIRE_BULLET_POSITION
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
	KeyData 4.0

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

KeyID	$FXLAB_ID_KEY_FIRE_BULLET_VELOCITY
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

KeyID	$FXLAB_ID_KEY_FIRE_BULLET
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

	KeyDefine	PointPower
	KeyDefine	PointEpsilon
	KeyDefine	PointRadius	


	KeyTime	0.0

	KeyName	RegenerateTime
	KeyData	15.0


	KeyName	ImageType
	KeyData	$FXLAB_SCRIPT_SECTION_SPRITE

	KeyName	ImageID
	KeyData	$FXLAB_ID_SPRITE_FIRE_BULLET

	KeyName	ImageNoise
	KeyData	0


	KeyName	Rate
	KeyData 3.0

	KeyName	Age
	KeyData 10.0

	KeyName	StartPower
	KeyData 0.5

	KeyName	EndPower
	KeyData 1.0

	KeyName	PositionKeyID
	KeyData $FXLAB_ID_KEY_FIRE_BULLET_POSITION

	KeyName VelocityKeyID
	KeyData $FXLAB_ID_KEY_FIRE_BULLET_VELOCITY


	KeyName	PointPower
	KeyData 200.0

	KeyName	PointEpsilon
	KeyData 50.0

	KeyName	PointRadius
	KeyData 100.0
}
