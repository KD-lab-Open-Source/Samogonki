KeyID	$FXLAB_ID_KEY_BOW_MACHINE_ACTIVE_LIGHTNING
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
	KeyData	0.05

	KeyName Nascency
	KeyData 0.01

	KeyName xP
	KeyData 3.0

	KeyName yP
	KeyData 3.0

	KeyName xLevel
	KeyData 0.07

	KeyName yLevel
	KeyData 0.07


	KeyName Radius
	KeyData 50.0


	KeyName Red
	KeyData 0.4

	KeyName	Green
	KeyData 0.4

	KeyName Blue
	KeyData 1.0


	KeyName Num
	KeyData 5.0

	KeyName OmniID
	KeyData $FXLAB_ID_KEY_BOW_MACHINE_ACTIVE_OMNI
}


KeyID	$FXLAB_ID_KEY_BOW_MACHINE_PASSIVE_LIGHTNING
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
	KeyData 0.07

	KeyName xP
	KeyData 3.0

	KeyName yP
	KeyData 3.0

	KeyName xLevel
	KeyData 0.07

	KeyName yLevel
	KeyData 0.07


	KeyName Radius
	KeyData 50.0


	KeyName Red
	KeyData 0.4

	KeyName	Green
	KeyData 0.4

	KeyName Blue
	KeyData 1.0


	KeyName Num
	KeyData 5.0

	KeyName OmniID
	KeyData $FXLAB_ID_KEY_BOW_MACHINE_PASSIVE_OMNI
}

KeyID	$FXLAB_ID_KEY_BOW_MACHINE_PASSIVE_OMNI
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
	KeyData 0.3

	KeyName Radius
	KeyData	10.0


	KeyTime	10.0

	KeyName	Red
	KeyData	0.2

	KeyName Green
	KeyData	0.2

	KeyName Blue
	KeyData 0.4

	KeyName Radius
	KeyData	15.0


	KeyTime	20.0

	KeyName	Red
	KeyData	0.1

	KeyName Green
	KeyData	0.1

	KeyName Blue
	KeyData 0.3

	KeyName Radius
	KeyData	10.0
}

KeyID	$FXLAB_ID_KEY_BOW_MACHINE_ACTIVE_OMNI
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
	KeyData	0.3

	KeyName Blue
	KeyData 0.6

	KeyName Radius
	KeyData	12.0


	KeyTime	3.0

	KeyName	Red
	KeyData	0.5

	KeyName Green
	KeyData	0.5

	KeyName Blue
	KeyData 0.6

	KeyName Radius
	KeyData	50.0


	KeyTime	6.0

	KeyName	Red
	KeyData	0.3

	KeyName Green
	KeyData	0.3

	KeyName Blue
	KeyData 0.6

	KeyName Radius
	KeyData	12.0
}


KeyID $FXLAB_ID_KEY_BOW_MACHINE_DAMAGE
{
	KeyAttribute	"$FXLAB_KEY_ATTRIBUTE_REPEAT | $FXLAB_KEY_ATTRIBUTE_STORAGE"
	
	KeyDefine	Damage
	KeyDefine	Type
	KeyDefine	ActiveRadius
	KeyDefine	PassiveRadius

	KeyTime	0.0

	KeyName	Damage
	KeyData	"200.0/100.0"

	KeyName	Type
	KeyData	$FXLAB_DAMAGE_SPHERE_TYPE_LIGHTNING

	KeyName	ActiveRadius
	KeyData	200.0

	KeyName	PassiveRadius
	KeyData	150.0
}

KeyID $FXLAB_ID_KEY_BOW_MACHINE
{
	KeyAttribute	"$FXLAB_KEY_ATTRIBUTE_REPEAT | $FXLAB_KEY_ATTRIBUTE_STORAGE"

	KeyDefine	ActiveType
	KeyDefine	ActiveID
	KeyDefine	PassiveType
	KeyDefine	PassiveID

	KeyTime	0.0

	KeyName	ActiveType
	KeyData	$FXLAB_CLIENT_PROCESS_LIGHTNING_RADIUS

	KeyName	ActiveID
	KeyData	$FXLAB_ID_KEY_BOW_MACHINE_ACTIVE_LIGHTNING

	KeyName	PassiveType
	KeyData	$FXLAB_CLIENT_PROCESS_LIGHTNING_SPHERE

	KeyName	PassiveID
	KeyData	$FXLAB_ID_KEY_BOW_MACHINE_PASSIVE_LIGHTNING
}


//--------------------------------------------------------

KeyID	$FXLAB_ID_KEY_LIGHTNING_SPARK_POSITION
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
	KeyData	20.0

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

KeyID	$FXLAB_ID_KEY_LIGHTNING_SPARK_VELOCITY
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


KeyID	$FXLAB_ID_KEY_LIGHTNING_SPARK
{
	KeyAttribute $FXLAB_KEY_ATTRIBUTE_REPEAT
	
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
	KeyData	$FXLAB_ID_SPRITE_LIGHTNING_SPARK

	KeyName	ImageNoise
	KeyData	0


	KeyName	Rate
	KeyData 0.0

	KeyName	Age
	KeyData 25.0

	KeyName	StartPower
	KeyData 0.0

	KeyName	EndPower
	KeyData 1.0

	KeyName	PositionKeyID
	KeyData $FXLAB_ID_KEY_LIGHTNING_SPARK_POSITION

	KeyName VelocityKeyID
	KeyData $FXLAB_ID_KEY_LIGHTNING_SPARK_VELOCITY


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


	KeyTime 24.0
	
	KeyName	Rate
	KeyData 0.0


	KeyTime 25.0

	KeyName Rate
	KeyData	10.0


	KeyTime	26.0

	KeyName	Rate
	KeyData	0.0


	KeyTime	50.0

	KeyName	RegenerateTime
	KeyData	25.0

	KeyName	ImageType
	KeyData	$FXLAB_SCRIPT_SECTION_SPRITE

	KeyName	ImageID
	KeyData	$FXLAB_ID_SPRITE_LIGHTNING_SPARK

	KeyName	ImageNoise
	KeyData	0


	KeyName	Rate
	KeyData 0.0

	KeyName	Age
	KeyData 25.0

	KeyName	StartPower
	KeyData 0.0

	KeyName	EndPower
	KeyData 1.0

	KeyName	PositionKeyID
	KeyData $FXLAB_ID_KEY_LIGHTNING_SPARK_POSITION

	KeyName VelocityKeyID
	KeyData $FXLAB_ID_KEY_LIGHTNING_SPARK_VELOCITY


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

KeyID	$FXLAB_ID_KEY_CORE_LIGHTNING_SPARK
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

//-----------------------------------

KeyID	$FXLAB_ID_KEY_LIGHTNING_SEEDS_ACTIVE_LIGHTNING
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
	KeyData	0.05

	KeyName Nascency
	KeyData 0.01

	KeyName xP
	KeyData 3.0

	KeyName yP
	KeyData 3.0

	KeyName xLevel
	KeyData 0.07

	KeyName yLevel
	KeyData 0.07


	KeyName Radius
	KeyData 50.0


	KeyName Red
	KeyData 0.4

	KeyName	Green
	KeyData 0.4

	KeyName Blue
	KeyData 1.0


	KeyName Num
	KeyData 5.0

	KeyName OmniID
	KeyData $FXLAB_ID_KEY_LIGHTNING_SEEDS_ACTIVE_OMNI
}


KeyID	$FXLAB_ID_KEY_LIGHTNING_SEEDS_PASSIVE_LIGHTNING
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
	KeyData 0.07

	KeyName xP
	KeyData 3.0

	KeyName yP
	KeyData 3.0

	KeyName xLevel
	KeyData 0.07

	KeyName yLevel
	KeyData 0.07


	KeyName Radius
	KeyData 30.0


	KeyName Red
	KeyData 0.4

	KeyName	Green
	KeyData 0.4

	KeyName Blue
	KeyData 1.0


	KeyName Num
	KeyData 5.0

	KeyName OmniID
	KeyData $FXLAB_ID_KEY_LIGHTNING_SEEDS_PASSIVE_OMNI
}

KeyID	$FXLAB_ID_KEY_LIGHTNING_SEEDS_PASSIVE_OMNI
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
	KeyData 0.3

	KeyName Radius
	KeyData	10.0


	KeyTime	10.0

	KeyName	Red
	KeyData	0.2

	KeyName Green
	KeyData	0.2

	KeyName Blue
	KeyData 0.4

	KeyName Radius
	KeyData	15.0


	KeyTime	20.0

	KeyName	Red
	KeyData	0.1

	KeyName Green
	KeyData	0.1

	KeyName Blue
	KeyData 0.3

	KeyName Radius
	KeyData	10.0
}

KeyID	$FXLAB_ID_KEY_LIGHTNING_SEEDS_ACTIVE_OMNI
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
	KeyData	0.3

	KeyName Blue
	KeyData 0.6

	KeyName Radius
	KeyData	12.0


	KeyTime	3.0

	KeyName	Red
	KeyData	0.5

	KeyName Green
	KeyData	0.5

	KeyName Blue
	KeyData 0.6

	KeyName Radius
	KeyData	30.0


	KeyTime	6.0

	KeyName	Red
	KeyData	0.3

	KeyName Green
	KeyData	0.3

	KeyName Blue
	KeyData 0.6

	KeyName Radius
	KeyData	12.0
}


KeyID $FXLAB_ID_KEY_LIGHTNING_SEEDS_DAMAGE
{
	KeyAttribute	"$FXLAB_KEY_ATTRIBUTE_REPEAT | $FXLAB_KEY_ATTRIBUTE_STORAGE"
	
	KeyDefine	Damage
	KeyDefine	Type
	KeyDefine	ActiveRadius
	KeyDefine	PassiveRadius

	KeyTime	0.0

	KeyName	Damage
	KeyData	1.6

	KeyName	Type
	KeyData	$FXLAB_DAMAGE_SPHERE_TYPE_LIGHTNING

	KeyName	ActiveRadius
	KeyData	80.0

	KeyName	PassiveRadius
	KeyData	60.0
}

KeyID $FXLAB_ID_KEY_LIGHTNING_SEEDS
{
	KeyAttribute	"$FXLAB_KEY_ATTRIBUTE_REPEAT | $FXLAB_KEY_ATTRIBUTE_STORAGE"

	KeyDefine	ActiveType
	KeyDefine	ActiveID
	KeyDefine	PassiveType
	KeyDefine	PassiveID

	KeyTime	0.0

	KeyName	ActiveType
	KeyData	$FXLAB_CLIENT_PROCESS_LIGHTNING_RADIUS

	KeyName	ActiveID
	KeyData	$FXLAB_ID_KEY_LIGHTNING_SEEDS_ACTIVE_LIGHTNING

	KeyName	PassiveType
	KeyData	$FXLAB_CLIENT_PROCESS_LIGHTNING_SPHERE

	KeyName	PassiveID
	KeyData	$FXLAB_ID_KEY_LIGHTNING_SEEDS_PASSIVE_LIGHTNING
}


