KeyID	$FXLAB_ID_KEY_CORE_VOODOO_HEAD
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
	KeyData	8.0

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
	KeyData	8.0

	KeyName	Angle
	KeyData	"3.14*8.0"

	KeyName	Frame
	KeyData	1.0
}


KeyID	$FXLAB_ID_KEY_VOODOO_HEAD_TAIL_POSITION
{
	KeyAttribute	"$FXLAB_KEY_ATTRIBUTE_FREEZE | $FXLAB_KEY_ATTRIBUTE_STORAGE"
	
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

KeyID	$FXLAB_ID_KEY_VOODOO_HEAD_TAIL_VELOCITY
{
	KeyAttribute	"$FXLAB_KEY_ATTRIBUTE_FREEZE | $FXLAB_KEY_ATTRIBUTE_STORAGE"
	
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

KeyID	$FXLAB_ID_KEY_VOODOO_HEAD_TAIL
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
	KeyData	$FXLAB_ID_SPRITE_VOODOO_HEAD

	KeyName	ImageNoise
	KeyData	0


	KeyName	Rate
	KeyData 1.0

	KeyName	Age
	KeyData 10.0

	KeyName	StartPower
	KeyData 0.0

	KeyName	EndPower
	KeyData 1.0

	KeyName	PositionKeyID
	KeyData $FXLAB_ID_KEY_VOODOO_HEAD_TAIL_POSITION

	KeyName VelocityKeyID
	KeyData $FXLAB_ID_KEY_VOODOO_HEAD_TAIL_VELOCITY


	KeyName	PointPower
	KeyData 50.0

	KeyName	PointEpsilon
	KeyData 100.0

	KeyName	PointRadius
	KeyData 100.0
}

//-------------------------------

KeyID	$FXLAB_ID_KEY_VOODOO_HEAD_OMNI
{
	KeyAttribute	$FXLAB_KEY_ATTRIBUTE_REPEAT

	KeyDefine	Red
	KeyDefine	Green
	KeyDefine	Blue
	KeyDefine	Radius


	KeyTime	0.0

	KeyName	Red
	KeyData	0.5

	KeyName Blue
	KeyData 0.1

	KeyName Green
	KeyData	0.1

	KeyName Radius
	KeyData	10.0


	KeyTime	10.0

	KeyName	Red
	KeyData	0.1

	KeyName Blue
	KeyData 0.1

	KeyName Green
	KeyData	0.1

	KeyName Radius
	KeyData	10.0


	KeyTime	20.0

	KeyName	Red
	KeyData	0.5

	KeyName Blue
	KeyData 0.1

	KeyName Green
	KeyData	0.1

	KeyName Radius
	KeyData	10.0
}


KeyID	$FXLAB_ID_KEY_VOODOO_MASTER_DAMAGE
{
	KeyAttribute	"$FXLAB_KEY_ATTRIBUTE_REPEAT | $FXLAB_KEY_ATTRIBUTE_STORAGE"

	KeyDefine	Power
	KeyDefine	Radius
//	KeyDefine	Type
//	KeyDefine	MinRadius
//	KeyDefine	Height

	KeyTime	0.0

	KeyName	Power
	KeyData	0.3

	KeyName	Radius
	KeyData	20.0

/*	KeyName	Type
	KeyData	$FXLAB_DAMAGE_SPHERE_TYPE_FIRE

	KeyName	MinRadius
	KeyData 50.0

	KeyName Height
	KeyData 20.0*/
}

KeyID	$FXLAB_ID_KEY_VOODOO_MASTER_BRAIN
{
	KeyAttribute	"$FXLAB_KEY_ATTRIBUTE_REPEAT | $FXLAB_KEY_ATTRIBUTE_STORAGE"

	KeyDefine	Radius
	KeyDefine	DeltaAngle

	KeyTime	0.0

	KeyName	Radius
	KeyData	50.0

	KeyName DeltaAngle
	KeyData "3.14 * 0.01"
}

KeyID	$FXLAB_ID_KEY_VOODOO_HEAD_MODEL
{
	KeyAttribute	$FXLAB_KEY_ATTRIBUTE_FREEZE

	KeyDefine	Model
	KeyDefine	Red
	KeyDefine	Green
	KeyDefine	Blue
	KeyDefine	Alpha

	KeyDefine	Radius
	KeyDefine	AngleX
	KeyDefine	AngleY
	KeyDefine	AngleZ

	KeyTime	0.0

	KeyName	Model
	KeyData	70.0

	KeyName Red
	KeyData	1.0

	KeyName	Green
	KeyData	0.0

	KeyName Blue
	KeyData	0.0

	KeyName Alpha
	KeyData 0.0


	KeyName Radius
	KeyData 10.0

	KeyName AngleX
	KeyData 0.0

	KeyName AngleY
	KeyData	0.0

	KeyName AngleZ
	KeyData 0.0



	KeyTime	20.0

	KeyName	Model
	KeyData	70.0

	KeyName Red
	KeyData	1.0

	KeyName	Green
	KeyData	1.0

	KeyName Blue
	KeyData	1.0

	KeyName Alpha
	KeyData 0.5


	KeyName Radius
	KeyData 10.0

	KeyName AngleX
	KeyData 0.0

	KeyName AngleY
	KeyData	0.0

	KeyName AngleZ
	KeyData 0.0
}

KeyID $FXLAB_ID_KEY_VOODOO_MASTER_LINK
{
	KeyAttribute	"$FXLAB_KEY_ATTRIBUTE_REPEAT | $FXLAB_KEY_ATTRIBUTE_STORAGE"
	
	KeyDefine	TailType
	KeyDefine	TailID
	KeyDefine	ExplodeType
	KeyDefine	ExplodeID
	KeyDefine   SpeedBorder

	KeyTime	0.0

	KeyName	TailType
	KeyData	$FXLAB_CLIENT_PROCESS_VOODOO_MASTER

	KeyName	TailID
	KeyData $FXLAB_ID_KEY_VOODOO_MASTER_BRAIN

	KeyName	ExplodeType
	KeyData	-1.0

	KeyName	ExplodeID
	KeyData	-1.0

	KeyName SpeedBorder
	KeyData -1.0
}
