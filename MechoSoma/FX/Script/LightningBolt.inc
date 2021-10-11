KeyID	$FXLAB_ID_KEY_LIGHTNING_BOLT_BOW
{
	KeyAttribute	"$FXLAB_KEY_ATTRIBUTE_REPEAT | $FXLAB_KEY_ATTRIBUTE_STORAGE"

	KeyDefine Stop
	KeyDefine Nascency
	KeyDefine xP
	KeyDefine yP
	KeyDefine xLevel
	KeyDefine yLevel

	KeyDefine Radius

	KeyDefine Red
	KeyDefine Green
	KeyDefine Blue

	KeyDefine Num

	KeyDefine OmniID

	KeyTime	0.0

	KeyName	Stop
	KeyData	0.1

	KeyName Nascency
	KeyData 0.5

	KeyName	xP
	KeyData 2.0

	KeyName	yP
	KeyData 2.0

	KeyName	xLevel
	KeyData 0.2

	KeyName	yLevel
	KeyData 0.2


	KeyName	Radius
	KeyData 20.0


	KeyName	Red
	KeyData 0.4

	KeyName	Green
	KeyData 0.4

	KeyName	Blue
	KeyData 1.0


	KeyName Num
	KeyData	3.0


	KeyName OmniID
	KeyData $FXLAB_ID_KEY_LIGHTNING_BOLT_LIGHT
}


KeyID	$FXLAB_ID_KEY_LIGHTNING_BOLT_LIGHT
{
	KeyAttribute	$FXLAB_KEY_ATTRIBUTE_REPEAT

	KeyDefine	Red
	KeyDefine	Green
	KeyDefine	Blue
	KeyDefine	Radius

	KeyTime	0.0

	KeyName	Red
	KeyData	0.1

	KeyName Green
	KeyData	0.1

	KeyName Blue
	KeyData 0.1

	KeyName Radius
	KeyData	10.0


	KeyTime	1.0

	KeyName	Red
	KeyData	0.3

	KeyName Green
	KeyData	0.3

	KeyName Blue
	KeyData 0.4

	KeyName Radius
	KeyData	20.0


	KeyTime	4.0

	KeyName	Red
	KeyData	0.0

	KeyName Green
	KeyData	0.0

	KeyName Blue
	KeyData 0.0

	KeyName Radius
	KeyData	10.0
}


KeyID $FXLAB_ID_KEY_LIGHTNING_BOLT_LINK
{
	KeyAttribute	"$FXLAB_KEY_ATTRIBUTE_REPEAT | $FXLAB_KEY_ATTRIBUTE_STORAGE"
	
	KeyDefine	TailType
	KeyDefine	TailID
	KeyDefine	ExplodeType
	KeyDefine	ExplodeID
	KeyDefine	SpeedBorder

	KeyTime	0.0

	KeyName	TailType
	KeyData	$FXLAB_CLIENT_PROCESS_LIGHTNING_SPHERE

	KeyName	TailID
	KeyData $FXLAB_ID_KEY_LIGHTNING_BOLT_BOW

	KeyName	ExplodeType
	KeyData	-1.0
//	KeyData	$FXLAB_CLIENT_PROCESS_FEAR_STREAM

	KeyName	ExplodeID
	KeyData	-1.0
//	KeyData	$FXLAB_ID_KEY_LIGHTNING_BOLT_SPARK

	KeyName SpeedBorder
	KeyData -1.0
}

//----------------------------------------------------

KeyID	$FXLAB_ID_KEY_CORE_LIGHTNING_BOLT_SPARK
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
	KeyData 1.0

	KeyName	Size
	KeyData	4.0

	KeyName	Angle
	KeyData	0.0

	KeyName	Frame
	KeyData	0.0


	KeyTime	25.0

	KeyName Alpha
	KeyData 1.0


	KeyTime	32.0

	KeyName	Red
	KeyData	0.5

	KeyName	Green
	KeyData	0.5

	KeyName Blue
	KeyData	1.0

	KeyName Alpha
	KeyData 0.0

	KeyName	Size
	KeyData	4.0

	KeyName	Angle
	KeyData	"3.14*2.0"

	KeyName	Frame
	KeyData	1.0
}

KeyID	$FXLAB_ID_KEY_LIGHTNING_BOLT_SPARK_POSITION
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
	KeyData	3.0

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

KeyID	$FXLAB_ID_KEY_LIGHTNING_BOLT_SPARK_VELOCITY
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
	KeyData	5.0

	KeyName	DataA1
	KeyData	3.0

	KeyName	DataB1
	KeyData	0.0

	KeyName	DataC1
	KeyData	0.0

	KeyName	DataA2
	KeyData	0.0

	KeyName	DataB2
	KeyData	0.0

	KeyName	DataC2
	KeyData	0.0
}

KeyID	$FXLAB_ID_KEY_LIGHTNING_BOLT_SPARK
{
	KeyAttribute	"$FXLAB_KEY_ATTRIBUTE_NONE | $FXLAB_KEY_ATTRIBUTE_STORAGE"
	
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
	KeyData	25.0

	KeyName	ImageType
	KeyData	$FXLAB_SCRIPT_SECTION_SPRITE

	KeyName	ImageID
	KeyData	$FXLAB_ID_SPRITE_LIGHTNING_BOLT_SPARK

	KeyName	ImageNoise
	KeyData	0


	KeyName	Rate
	KeyData 50.0

	KeyName	Age
	KeyData 25.0

	KeyName	StartPower
	KeyData 0.0

	KeyName	EndPower
	KeyData 1.0

	KeyName	PositionKeyID
	KeyData $FXLAB_ID_KEY_LIGHTNING_BOLT_SPARK_POSITION

	KeyName VelocityKeyID
	KeyData $FXLAB_ID_KEY_LIGHTNING_BOLT_SPARK_VELOCITY


	KeyName ForceX
	KeyData	0

	KeyName	ForceY
	KeyData	0

	KeyName	ForceZ
	KeyData	-1.0

	KeyName ReflectionImpulse
	KeyData	0.8

	KeyName VelocityImpulse
	KeyData	1.0
}


//-----------------------------------

KeyID $FXLAB_ID_KEY_SERVER_LIGHTNING_BOLT_DAMAGE
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
	KeyData	$FXLAB_DAMAGE_SPHERE_TYPE_LIGHTNING


	KeyTime	6.0

	KeyName	Power
	KeyData	0.0

	KeyName	Radius
	KeyData	1.0

	KeyName DamageType
	KeyData	$FXLAB_DAMAGE_SPHERE_TYPE_LIGHTNING
}

KeyID	$FXLAB_ID_KEY_SERVER_LIGHTNING_BOLT_LINK
{
	KeyAttribute	"$FXLAB_KEY_ATTRIBUTE_REPEAT | $FXLAB_KEY_ATTRIBUTE_STORAGE"
	
	KeyDefine	ExplodeType
	KeyDefine	ExplodeID

	KeyTime	0.0

	KeyName	ExplodeType
	KeyData	$FXLAB_SERVER_PROCESS_DAMAGE_SENSOR

	KeyName	ExplodeID
	KeyData	$FXLAB_ID_KEY_SERVER_LIGHTNING_BOLT_DAMAGE
}

