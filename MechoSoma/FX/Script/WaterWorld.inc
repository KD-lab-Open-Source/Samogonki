KeyID	$FXLAB_ID_KEY_FISHWATER_POSITION
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

KeyID	$FXLAB_ID_KEY_FISHWATER_VELOCITY
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
	KeyData	0.0

	KeyName	DataY
	KeyData	0.0

	KeyName	DataZ
	KeyData	0.0

	KeyName	DataA1
	KeyData	0.3

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

KeyID	$FXLAB_ID_KEY_FISHWATER_STREAM
{
	KeyAttribute	"$FXLAB_KEY_ATTRIBUTE_FREEZE | $FXLAB_KEY_ATTRIBUTE_STORAGE"
	
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
	KeyData	$FXLAB_ID_SPRITE_FISHWATER

	KeyName	ImageNoise
	KeyData	0


	KeyName	Rate
	KeyData 1.0

	KeyName	Age
	KeyData 50.0

	KeyName	StartPower
	KeyData 1.0

	KeyName	EndPower
	KeyData 0.0

	KeyName	PositionKeyID
	KeyData $FXLAB_ID_KEY_FISHWATER_POSITION

	KeyName VelocityKeyID
	KeyData $FXLAB_ID_KEY_FISHWATER_VELOCITY


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

KeyID	$FXLAB_ID_KEY_FISHWATER_CLUSTER
{
	KeyAttribute	$FXLAB_KEY_ATTRIBUTE_REPEAT
	
	KeyDefine	Num
	KeyDefine	Rate
	KeyDefine	Radius
	KeyDefine	Psi
	KeyDefine	DeltaPsi
	KeyDefine	Thetta
	KeyDefine	KeyID

	KeyTime	0.0

	KeyName	Num
	KeyData	3.0

	KeyName	Rate
	KeyData	0.0

	KeyName Radius
	KeyData 5.0

	KeyName	Psi
	KeyData	0.0

	KeyName	DeltaPsi
	KeyData	"2.0*3.14"

	KeyName	Thetta
	KeyData "0.1*3.14"

	KeyName	KeyID
	KeyData	$FXLAB_ID_KEY_FISHWATER_STREAM


	KeyTime	60.0

	KeyName	Rate
	KeyData	0.5

	KeyName Radius
	KeyData 5.0

	KeyName	Psi
	KeyData	0.0

	KeyName	DeltaPsi
	KeyData	"2.0*3.14"

	KeyName	Thetta
	KeyData "0.01*3.14"


	KeyTime	110.0

	KeyName	Num
	KeyData	3.0

	KeyName	Rate
	KeyData	1.0

	KeyName Radius
	KeyData 10.0

	KeyName	Psi
	KeyData	"0.5*3.14"

	KeyName	DeltaPsi
	KeyData	"2.0*3.14"

	KeyName	Thetta
	KeyData "0.1*3.14"

	KeyName	KeyID
	KeyData	$FXLAB_ID_KEY_FISHWATER_STREAM
}

KeyID	$FXLAB_ID_KEY_CORE_FISHWATER
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
	KeyData	0.6

	KeyName	Green
	KeyData	0.6

	KeyName Blue
	KeyData	0.6

	KeyName Alpha
	KeyData 0.4

	KeyName	Size
	KeyData	8.0

	KeyName	Angle
	KeyData	0.0

	KeyName	Frame
	KeyData	0.0


	KeyTime	32.0

	KeyName	Red
	KeyData	0.5

	KeyName	Green
	KeyData	0.5

	KeyName Blue
	KeyData	1.0

	KeyName Alpha
	KeyData 0.8

	KeyName	Size
	KeyData	8.0

	KeyName	Angle
	KeyData	0.0

	KeyName	Frame
	KeyData	1.0
}

//-------------------------------------------------

KeyID	$FXLAB_ID_KEY_WATERPUMP_POSITION
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

KeyID	$FXLAB_ID_KEY_WATERPUMP_VELOCITY
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

KeyID	$FXLAB_ID_KEY_WATERPUMP_STREAM
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
	KeyData	$FXLAB_ID_SPRITE_WATERPUMP

	KeyName	ImageNoise
	KeyData	0


	KeyName	Rate
	KeyData 2.0

	KeyName	Age
	KeyData 50.0

	KeyName	StartPower
	KeyData 1.0

	KeyName	EndPower
	KeyData 0.0

	KeyName	PositionKeyID
	KeyData $FXLAB_ID_KEY_WATERPUMP_POSITION

	KeyName VelocityKeyID
	KeyData $FXLAB_ID_KEY_WATERPUMP_VELOCITY


	KeyName ForceX
	KeyData	0

	KeyName	ForceY
	KeyData	0

	KeyName	ForceZ
	KeyData	-0.7

	KeyName ReflectionImpulse
	KeyData	0.7

	KeyName VelocityImpulse
	KeyData	2.0
}

KeyID	$FXLAB_ID_KEY_CORE_WATERPUMP
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
	KeyData	1.0

	KeyName Alpha
	KeyData 0.3

	KeyName	Size
	KeyData	8.0

	KeyName	Angle
	KeyData	0.0

	KeyName	Frame
	KeyData	0.0


	KeyTime	32.0

	KeyName	Red
	KeyData	0.0

	KeyName	Green
	KeyData	0.0

	KeyName Blue
	KeyData	1.0

	KeyName Alpha
	KeyData 0.3

	KeyName	Size
	KeyData	8.0

	KeyName	Angle
	KeyData	0.0

	KeyName	Frame
	KeyData	1.0
}

//---------------------------------------------

KeyID	$FXLAB_ID_KEY_CANNON_DUST_POSITION
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

KeyID	$FXLAB_ID_KEY_CANNON_DUST_VELOCITY
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
	KeyData	2.0

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

KeyID	$FXLAB_ID_KEY_CANNON_DUST
{
	KeyAttribute $FXLAB_KEY_ATTRIBUTE_NONE
	
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


	KeyTime	1.0

	KeyName	RegenerateTime
	KeyData	15.0

	KeyName	ImageType
	KeyData	$FXLAB_SCRIPT_SECTION_SPRITE

	KeyName	ImageID
	KeyData	$FXLAB_ID_SPRITE_CANNON_DUST

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
	KeyData		$FXLAB_ID_KEY_CANNON_DUST_POSITION

	KeyName		VelocityKeyID
	KeyData		$FXLAB_ID_KEY_CANNON_DUST_VELOCITY

	KeyName		Damp
	KeyData		0.9


	KeyTime	5.0

	KeyName	RegenerateTime
	KeyData	15.0

	KeyName	ImageType
	KeyData	$FXLAB_SCRIPT_SECTION_SPRITE

	KeyName	ImageID
	KeyData	$FXLAB_ID_SPRITE_CANNON_DUST

	KeyName	ImageNoise
	KeyData	0


	KeyName		Rate
	KeyData		3.0

	KeyName		Age
	KeyData		30.0

	KeyName		StartPower
	KeyData		0.0

	KeyName		EndPower
	KeyData		1.0

	KeyName		PositionKeyID
	KeyData		$FXLAB_ID_KEY_CANNON_DUST_POSITION

	KeyName		VelocityKeyID
	KeyData		$FXLAB_ID_KEY_CANNON_DUST_VELOCITY

	KeyName		Damp
	KeyData		0.9
}

KeyID	$FXLAB_ID_KEY_CORE_CANNON_DUST
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

//---------------------------------------

KeyID	$FXLAB_ID_KEY_CANNON_FIRE_POSITION
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

KeyID	$FXLAB_ID_KEY_CANNON_FIRE_VELOCITY
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

KeyID	$FXLAB_ID_KEY_CANNON_FIRE
{
	KeyAttribute	$FXLAB_KEY_ATTRIBUTE_STORAGE
	
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


	KeyTime	1.0

	KeyName	RegenerateTime
	KeyData	15.0

	KeyName	ImageType
	KeyData	$FXLAB_SCRIPT_SECTION_SPRITE

	KeyName	ImageID
	KeyData	$FXLAB_ID_SPRITE_DRAGON_FIRE

	KeyName	ImageNoise
	KeyData	0


	KeyName	Rate
	KeyData 5.0

	KeyName	Age
	KeyData 10.0

	KeyName	StartPower
	KeyData 0.5

	KeyName	EndPower
	KeyData 1.0

	KeyName	PositionKeyID
	KeyData $FXLAB_ID_KEY_CANNON_FIRE_POSITION

	KeyName VelocityKeyID
	KeyData $FXLAB_ID_KEY_CANNON_FIRE_VELOCITY


	KeyName	PointPower
	KeyData 200.0

	KeyName	PointEpsilon
	KeyData 100.0

	KeyName	PointRadius
	KeyData 100.0
}

//-----------------CannonBulletTail Data--------------------

KeyID	$FXLAB_ID_KEY_CANNON_BULLET_POSITION
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

KeyID	$FXLAB_ID_KEY_CANNON_BULLET_VELOCITY
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

KeyID	$FXLAB_ID_KEY_CANNON_BULLET
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
	KeyData	$FXLAB_ID_SPRITE_CANNON_DUST

	KeyName	ImageNoise
	KeyData	0


	KeyName	Rate
	KeyData 1.0

	KeyName	Age
	KeyData 15.0

	KeyName	StartPower
	KeyData 0.0

	KeyName	EndPower
	KeyData 1.0

	KeyName	PositionKeyID
	KeyData $FXLAB_ID_KEY_CANNON_BULLET_POSITION

	KeyName VelocityKeyID
	KeyData $FXLAB_ID_KEY_CANNON_BULLET_VELOCITY


	KeyName	PointPower
	KeyData 100.0

	KeyName	PointEpsilon
	KeyData 2.0

	KeyName	PointRadius
	KeyData 100.0
}

KeyID	$FXLAB_ID_KEY_CANNON_BULLET_LINK
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
	KeyData $FXLAB_ID_KEY_CANNON_BULLET

	KeyName	ExplodeType
//	KeyData	$FXLAB_CLIENT_PROCESS_OBJECT_SET
	KeyData -1.0

	KeyName	ExplodeID
//	KeyData	$FXLAB_ID_KEY_BOMB_EXPLOSION_CLIENT_SET
	KeyData -1.0

	KeyName SpeedBorder
	KeyData 0.1
}


KeyID	$FXLAB_ID_KEY_CANNON_OMNI
{
	KeyAttribute	$FXLAB_KEY_ATTRIBUTE_NONE

	KeyDefine	Red
	KeyDefine	Green
	KeyDefine	Blue
	KeyDefine	Radius

	KeyTime	0.0

	KeyName	Red
	KeyData	0.8

	KeyName Green
	KeyData	0.0

	KeyName Blue
	KeyData 0.8

	KeyName Radius
	KeyData	5.0


	KeyTime	3.0

	KeyName	Red
	KeyData	1.0

	KeyName Green
	KeyData	1.0

	KeyName Blue
	KeyData 0.8

	KeyName Radius
	KeyData	25.0


	KeyTime	8.0

	KeyName	Red
	KeyData	0.0

	KeyName Green
	KeyData	0.0

	KeyName Blue
	KeyData 0.0

	KeyName Radius
	KeyData	5.0
}

//----------------------------------------------

KeyID	$FXLAB_ID_KEY_CORE_WATER_BUBBLE_FLUX
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
	KeyData	0.8

	KeyName	Green
	KeyData	0.8

	KeyName Blue
	KeyData	1.0

	KeyName Alpha
	KeyData 0.0

	KeyName	Size
	KeyData	8.0

	KeyName	Angle
	KeyData	0.0

	KeyName	Frame
	KeyData	0.0

	
	KeyTime	16.0

	KeyName	Red
	KeyData	0.8

	KeyName	Green
	KeyData	0.8

	KeyName Blue
	KeyData	1.0

	KeyName Alpha
	KeyData 0.7


	KeyTime	32.0

	KeyName	Red
	KeyData	0.8

	KeyName	Green
	KeyData	0.8

	KeyName Blue
	KeyData	1.0

	KeyName Alpha
	KeyData 0.0

	KeyName	Size
	KeyData	8.0

	KeyName	Angle
	KeyData	0.0

	KeyName	Frame
	KeyData	1.0
}


KeyID	$FXLAB_ID_KEY_WATER_BUBBLE_FLUX_POSITION
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
	KeyData	20.0

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

KeyID	$FXLAB_ID_KEY_WATER_BUBBLE_FLUX_VELOCITY
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
	KeyData	$FXLAB_SWARM_DOMAIN_CIRCLE

	KeyName	DataX
	KeyData	0

	KeyName	DataY
	KeyData	0

	KeyName	DataZ
	KeyData	0.0

	KeyName	DataA1
	KeyData	0.2

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

KeyID	$FXLAB_ID_KEY_WATER_BUBBLE_FLUX
{
	KeyAttribute	"$FXLAB_KEY_ATTRIBUTE_STORAGE | $FXLAB_KEY_ATTRIBUTE_REPEAT"

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
	KeyData	0.0

	KeyName	ImageType
	KeyData	$FXLAB_SCRIPT_SECTION_SPRITE

	KeyName	ImageID
	KeyData $FXLAB_ID_SPRITE_WATER_BUBBLE_FLUX

	KeyName	ImageNoise
	KeyData	0


	KeyName	Rate
	KeyData	0.5

	KeyName Age
	KeyData	30.0

	KeyName	StartPower
	KeyData	1.0

	KeyName	EndPower
	KeyData	0.0

	KeyName	PositionKeyID
	KeyData $FXLAB_ID_KEY_WATER_BUBBLE_FLUX_POSITION

	KeyName	VelocityKeyID
	KeyData $FXLAB_ID_KEY_WATER_BUBBLE_FLUX_VELOCITY


	KeyName	Speed
	KeyData 3.0
}

KeyID	$FXLAB_ID_KEY_WATER_FIELD_STREAM
{
	KeyAttribute	"$FXLAB_KEY_ATTRIBUTE_STORAGE | $FXLAB_KEY_ATTRIBUTE_REPEAT"

	KeyDefine	Force
	KeyDefine	Radius

	KeyTime	0.0

	KeyName	Force
	KeyData	120.0

	KeyName	Radius
	KeyData	60.0
}

//-----------------------------------------

KeyID	$FXLAB_ID_KEY_BOSS_WATER_POSITION
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

KeyID	$FXLAB_ID_KEY_BOSS_WATER_VELOCITY
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
	KeyData	0.0

	KeyName	DataY
	KeyData	0.0

	KeyName	DataZ
	KeyData	0.0

	KeyName	DataA1
	KeyData	0.3

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

KeyID	$FXLAB_ID_KEY_BOSS_WATER_STREAM
{
	KeyAttribute	"$FXLAB_KEY_ATTRIBUTE_FREEZE | $FXLAB_KEY_ATTRIBUTE_STORAGE"
	
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
	KeyData	$FXLAB_ID_SPRITE_BOSS_WATER

	KeyName	ImageNoise
	KeyData	0


	KeyName	Rate
	KeyData 1.0

	KeyName	Age
	KeyData 50.0

	KeyName	StartPower
	KeyData 1.0

	KeyName	EndPower
	KeyData 0.0

	KeyName	PositionKeyID
	KeyData $FXLAB_ID_KEY_BOSS_WATER_POSITION

	KeyName VelocityKeyID
	KeyData $FXLAB_ID_KEY_BOSS_WATER_VELOCITY


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

KeyID	$FXLAB_ID_KEY_BOSS_WATER_CLUSTER
{
	KeyAttribute	$FXLAB_KEY_ATTRIBUTE_REPEAT
	
	KeyDefine	Num
	KeyDefine	Rate
	KeyDefine	Radius
	KeyDefine	Psi
	KeyDefine	DeltaPsi
	KeyDefine	Thetta
	KeyDefine	KeyID

	KeyTime	0.0

	KeyName	Num
	KeyData	3.0

	KeyName	Rate
	KeyData	0.0

	KeyName Radius
	KeyData 5.0

	KeyName	Psi
	KeyData	0.0

	KeyName	DeltaPsi
	KeyData	"2.0*3.14"

	KeyName	Thetta
	KeyData "0.1*3.14"

	KeyName	KeyID
	KeyData	$FXLAB_ID_KEY_BOSS_WATER_STREAM


	KeyTime	60.0

	KeyName	Rate
	KeyData	0.5

	KeyName Radius
	KeyData 5.0

	KeyName	Psi
	KeyData	0.0

	KeyName	DeltaPsi
	KeyData	"2.0*3.14"

	KeyName	Thetta
	KeyData "0.01*3.14"


	KeyTime	110.0

	KeyName	Num
	KeyData	3.0

	KeyName	Rate
	KeyData	1.0

	KeyName Radius
	KeyData 10.0

	KeyName	Psi
	KeyData	"0.5*3.14"

	KeyName	DeltaPsi
	KeyData	"2.0*3.14"

	KeyName	Thetta
	KeyData "0.1*3.14"

	KeyName	KeyID
	KeyData	$FXLAB_ID_KEY_BOSS_WATER_STREAM
}

KeyID	$FXLAB_ID_KEY_CORE_BOSS_WATER
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
	KeyData	0.6

	KeyName	Green
	KeyData	0.6

	KeyName Blue
	KeyData	0.6

	KeyName Alpha
	KeyData 0.4

	KeyName	Size
	KeyData	8.0

	KeyName	Angle
	KeyData	0.0

	KeyName	Frame
	KeyData	0.0


	KeyTime	32.0

	KeyName	Red
	KeyData	0.5

	KeyName	Green
	KeyData	0.5

	KeyName Blue
	KeyData	1.0

	KeyName Alpha
	KeyData 0.8

	KeyName	Size
	KeyData	8.0

	KeyName	Angle
	KeyData	0.0

	KeyName	Frame
	KeyData	1.0
}
