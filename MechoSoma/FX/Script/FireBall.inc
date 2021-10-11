KeyID	$FXLAB_ID_KEY_CORE_FIREBALL
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

KeyID	$FXLAB_ID_KEY_FIREBALL_TAIL_POSITION
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

KeyID	$FXLAB_ID_KEY_FIREBALL_TAIL_VELOCITY
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

KeyID	$FXLAB_ID_KEY_FIREBALL_TAIL
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
	KeyData	$FXLAB_ID_SPRITE_FIREBALL

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
	KeyData $FXLAB_ID_KEY_FIREBALL_TAIL_POSITION

	KeyName VelocityKeyID
	KeyData $FXLAB_ID_KEY_FIREBALL_TAIL_VELOCITY


	KeyName	PointPower
	KeyData 200.0

	KeyName	PointEpsilon
	KeyData 50.0

	KeyName	PointRadius
	KeyData 100.0
}


KeyID	$FXLAB_ID_KEY_FIREBALL_LIGHT
{
	KeyAttribute	$FXLAB_KEY_ATTRIBUTE_REPEAT

	KeyDefine	Red
	KeyDefine	Green
	KeyDefine	Blue
	KeyDefine	Radius


	KeyTime	0.0

	KeyName	Red
	KeyData	0.5

	KeyName Green
	KeyData	0.1

	KeyName Blue
	KeyData 0.5

	KeyName Radius
	KeyData	15


	KeyTime	5.0

	KeyName	Red
	KeyData	0.5

	KeyName Green
	KeyData	0.3

	KeyName Blue
	KeyData 0.1

	KeyName Radius
	KeyData	15


	KeyTime	10.0

	KeyName	Red
	KeyData	0.5

	KeyName Green
	KeyData	0.1

	KeyName Blue
	KeyData 0.5

	KeyName Radius
	KeyData	15
}

//-------------------------------------


KeyID $FXLAB_ID_KEY_FIREBALL_SET
{
	KeyAttribute	"$FXLAB_KEY_ATTRIBUTE_REPEAT | $FXLAB_KEY_ATTRIBUTE_STORAGE"

	KeyDefine	Mode
	KeyDefine	VelX
	KeyDefine	VelY
	KeyDefine	VelZ
	KeyDefine	Num
	KeyDefine	ObjectType0
	KeyDefine	KeyID0
	KeyDefine	ObjectType1
	KeyDefine	KeyID1

	KeyTime	0.0

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
	KeyData $FXLAB_CLIENT_PROCESS_TAIL

	KeyName	KeyID0
	KeyData $FXLAB_ID_KEY_FIREBALL_TAIL

	KeyName	ObjectType1
	KeyData $FXLAB_CLIENT_PROCESS_CONTROL_OMNI

	KeyName	KeyID1
	KeyData $FXLAB_ID_KEY_FIREBALL_LIGHT
}

//-----------------------------------

KeyID $FXLAB_ID_KEY_FIREBALL_LINK
{
	KeyAttribute	"$FXLAB_KEY_ATTRIBUTE_REPEAT | $FXLAB_KEY_ATTRIBUTE_STORAGE"
	
	KeyDefine	TailType
	KeyDefine	TailID
	KeyDefine	ExplodeType
	KeyDefine	ExplodeID
	KeyDefine	SpeedBorder

	KeyTime	0.0

	KeyName	TailType
	KeyData	$FXLAB_CLIENT_PROCESS_OBJECT_SET

	KeyName	TailID
	KeyData $FXLAB_ID_KEY_FIREBALL_SET

	KeyName	ExplodeType
//	KeyData	$FXLAB_CLIENT_PROCESS_OBJECT_SET
	KeyData	-1.0

	KeyName	ExplodeID
//	KeyData	$FXLAB_ID_KEY_FIRE_EXPLOSION_CLIENT_SET
	KeyData -1.0

	KeyName SpeedBorder
	KeyData -1.0
}

//---------------------------

KeyID $FXLAB_ID_KEY_SERVER_FIREBALL_DAMAGE
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

KeyID	$FXLAB_ID_KEY_SERVER_FIREBALL_LINK
{
	KeyAttribute	"$FXLAB_KEY_ATTRIBUTE_REPEAT | $FXLAB_KEY_ATTRIBUTE_STORAGE"
	
	KeyDefine	ExplodeType
	KeyDefine	ExplodeID

	KeyTime	0.0

	KeyName	ExplodeType
	KeyData	$FXLAB_SERVER_PROCESS_DAMAGE_SENSOR

	KeyName	ExplodeID
	KeyData	$FXLAB_ID_KEY_SERVER_FIREBALL_DAMAGE
}
