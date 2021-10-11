KeyID	$FXLAB_ID_KEY_ELECTRIC_STORM_BOW
{
	KeyAttribute "$FXLAB_KEY_ATTRIBUTE_NONE | $FXLAB_KEY_ATTRIBUTE_STORAGE"

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


	KeyTime	1.0

	KeyName	Stop
	KeyData	0.01

	KeyName Nascency
	KeyData 0.001

	KeyName	xP
	KeyData 3.0

	KeyName	yP
	KeyData 3.0

	KeyName	xLevel
	KeyData 0.01

	KeyName	yLevel
	KeyData 0.01


	KeyName	Radius
	KeyData 100.0


	KeyName	Red
	KeyData 0.4

	KeyName	Green
	KeyData 0.4

	KeyName	Blue
	KeyData 1.0


	KeyName Num
	KeyData	2.0

	KeyName OmniID
	KeyData -1.0
}


KeyID	$FXLAB_ID_KEY_ELECTRIC_STORM_LIGHT
{
	KeyAttribute	$FXLAB_KEY_ATTRIBUTE_NONE

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
	KeyData	20.0


	KeyTime	1.0

	KeyName	Red
	KeyData	0.3

	KeyName Green
	KeyData	0.3

	KeyName Blue
	KeyData 0.4

	KeyName Radius
	KeyData	80.0


	KeyTime	4.0

	KeyName	Red
	KeyData	0.0

	KeyName Green
	KeyData	0.0

	KeyName Blue
	KeyData 0.0

	KeyName Radius
	KeyData	30.0
}

KeyID $FXLAB_ID_KEY_ELECTRIC_STORM_DAMAGE
{
	KeyAttribute	$FXLAB_KEY_ATTRIBUTE_NONE
	
	KeyDefine	Power
	KeyDefine	Radius
	KeyDefine	DamageType

	KeyTime	0.0

	KeyName	Power
	KeyData	100.0

	KeyName	Radius
	KeyData	30.0

	KeyName	DamageType
	KeyData	$FXLAB_DAMAGE_SPHERE_TYPE_LIGHTNING


	KeyTime	3.0

	KeyName	Power
	KeyData	10.0

	KeyName	Radius
	KeyData	30.0

	KeyName	DamageType
	KeyData	$FXLAB_DAMAGE_SPHERE_TYPE_LIGHTNING
}

//-----------------------------------------

KeyID	$FXLAB_ID_KEY_CORE_STORM_CLOUDS
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
	KeyData	0.1

	KeyName Blue
	KeyData	0.1

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
	KeyData 0.5


	KeyTime	32.0

	KeyName	Red
	KeyData	0.1

	KeyName	Green
	KeyData	0.1

	KeyName Blue
	KeyData	0.1

	KeyName Alpha
	KeyData 0.0

	KeyName	Size
	KeyData	128.0

	KeyName	Angle
	KeyData	"2.0*3.14"

	KeyName	Frame
	KeyData	1.0
}

KeyID $FXLAB_ID_KEY_STORM_CLOUDS
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
	KeyData	$FXLAB_ID_SPRITE_STORM_CLOUDS

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


//----------------------------

KeyID	$FXLAB_ID_KEY_ELECTRIC_STORM_MARKER_OMNI
{
	KeyAttribute	$FXLAB_KEY_ATTRIBUTE_REPEAT

	KeyDefine	Red
	KeyDefine	Green
	KeyDefine	Blue
	KeyDefine	Radius

	KeyTime	0.0

	KeyName	Red
	KeyData	0.8

	KeyName Green
	KeyData	0.8

	KeyName Blue
	KeyData 1.0

	KeyName Radius
	KeyData	5.0


	KeyTime	10.0

	KeyName	Red
	KeyData	0.3

	KeyName Green
	KeyData	0.3

	KeyName Blue
	KeyData 0.5

	KeyName Radius
	KeyData	25.0


	KeyTime	20.0

	KeyName	Red
	KeyData	0.8

	KeyName Green
	KeyData	0.8

	KeyName Blue
	KeyData 1.0

	KeyName Radius
	KeyData	5.0
}


//---------------------------------

KeyID	$FXLAB_ID_KEY_CORE_STORM_SPARK
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

KeyID	$FXLAB_ID_KEY_STORM_SPARK_POSITION
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

KeyID	$FXLAB_ID_KEY_STORM_SPARK_VELOCITY
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
	KeyData	10.0

	KeyName	DataA1
	KeyData	5.0

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

KeyID	$FXLAB_ID_KEY_STORM_SPARK
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
	KeyData	$FXLAB_ID_SPRITE_STORM_SPARK

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
	KeyData $FXLAB_ID_KEY_STORM_SPARK_POSITION

	KeyName VelocityKeyID
	KeyData $FXLAB_ID_KEY_STORM_SPARK_VELOCITY


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

