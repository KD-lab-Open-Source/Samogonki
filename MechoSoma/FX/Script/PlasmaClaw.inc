KeyID	$FXLAB_ID_KEY_CORE_WILD_CLAW
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
	KeyData 0.5

	KeyName	Size
	KeyData	8.0

	KeyName	Angle
	KeyData	0.0

	KeyName	Frame
	KeyData	0.0


	KeyTime	32.0

	KeyName	Red
	KeyData	1.0

	KeyName	Green
	KeyData	0.0

	KeyName Blue
	KeyData	0.0

	KeyName Alpha
	KeyData 0.5

	KeyName	Size
	KeyData	8.0

	KeyName	Angle
	KeyData	0.0

	KeyName	Frame
	KeyData	1.0
}

KeyID	$FXLAB_ID_KEY_CLAW_SNAKE
{
	KeyAttribute	$FXLAB_KEY_ATTRIBUTE_REPEAT
	
	KeyDefine	RegenerateTime

	KeyDefine	ImageType
	KeyDefine	ImageID
	KeyDefine	ImageNoise

	KeyTime	0.0

	KeyName	RegenerateTime
	KeyData	15.0

	KeyName	ImageType
	KeyData	$FXLAB_SCRIPT_SECTION_SPRITE

	KeyName	ImageID
	KeyData	$FXLAB_ID_SPRITE_WILD_CLAW

	KeyName	ImageNoise
	KeyData	0


	KeyTime	10.0

	KeyName	RegenerateTime
	KeyData	15.0

	KeyName	ImageType
	KeyData	$FXLAB_SCRIPT_SECTION_SPRITE

	KeyName	ImageID
	KeyData	$FXLAB_ID_SPRITE_WILD_CLAW

	KeyName	ImageNoise
	KeyData	0
}


KeyID $FXLAB_ID_KEY_WILD_CLAW_CONTROL
{
	KeyAttribute	"$FXLAB_KEY_ATTRIBUTE_REPEAT | $FXLAB_KEY_ATTRIBUTE_STORAGE"
	
	KeyDefine	Damage
	KeyDefine	Type
	KeyDefine	ActiveRadius
	KeyDefine	PassiveRadius

	KeyDefine	ErectionTime

	KeyTime	0.0

	KeyName	Damage
	KeyData	0.5

	KeyName	Type
	KeyData	$FXLAB_DAMAGE_SPHERE_TYPE_GENERIC

	KeyName	ActiveRadius
	KeyData	300.0

	KeyName	PassiveRadius
	KeyData	250.0

	KeyName	ErectionTime
	KeyData 500.0
}


KeyID $FXLAB_ID_KEY_WILD_CLAW
{
	KeyAttribute	$FXLAB_KEY_ATTRIBUTE_REPEAT

	KeyDefine	Radius
	KeyDefine	Angle
	KeyDefine	Radius0
	KeyDefine	Radius1
	KeyDefine	DeltaPhase
	KeyDefine	ErectionTime
	KeyDefine	SplineType
	KeyDefine	SplineID

	KeyTime	0.0

	KeyName	Radius
	KeyData	0.1

	KeyName Angle
	KeyData	0.0

	KeyName	Radius0
	KeyData 0.0

	KeyName Radius1
	KeyData 0.0

	KeyName DeltaPhase
	KeyData "0.01*3.14"

	KeyName ErectionTime
	KeyData	500.0

	KeyName	SplineType
	KeyData	$FXLAB_CLIENT_PROCESS_RUBBER_SNAKE

	KeyName	SplineID
	KeyData $FXLAB_ID_KEY_CLAW_SNAKE


	KeyTime	200.0

	KeyName	Radius
	KeyData	200.0


	KeyTime 1000.0

	KeyName Angle
	KeyData	"3.14*5.0"

	KeyName DeltaPhase
	KeyData	"0.1*3.14"


	KeyTime 1800.0

	KeyName Radius
	KeyData	200.0


	KeyTime	2000.0

	KeyName	Radius
	KeyData	0.1

	KeyName Angle
	KeyData	0.0

	KeyName	Radius0
	KeyData 0.0

	KeyName Radius1
	KeyData 0.0

	KeyName DeltaPhase
	KeyData "0.01*3.14"

	KeyName ErectionTime
	KeyData	500.0

	KeyName	SplineType
	KeyData	$FXLAB_CLIENT_PROCESS_RUBBER_SNAKE

	KeyName	SplineID
	KeyData $FXLAB_ID_KEY_CLAW_SNAKE
}

//--------------------------------------------------------

KeyID $FXLAB_ID_KEY_PLASMA_CLAW_CONTROL
{
	KeyAttribute	"$FXLAB_KEY_ATTRIBUTE_REPEAT | $FXLAB_KEY_ATTRIBUTE_STORAGE"
	
	KeyDefine	Damage
	KeyDefine	Type
	KeyDefine	ActiveRadius
	KeyDefine	PassiveRadius

	KeyDefine	ErectionTime

	KeyTime	0.0

	KeyName	Damage
	KeyData	1.0

	KeyName	Type
	KeyData	$FXLAB_DAMAGE_SPHERE_TYPE_GENERIC

	KeyName	ActiveRadius
	KeyData	300.0

	KeyName	PassiveRadius
	KeyData	250.0

	KeyName	ErectionTime
	KeyData 500.0
}


//----------------------------------------------------

KeyID $FXLAB_ID_KEY_PERESTROYKA_CLAW_CONTROL
{
	KeyAttribute	"$FXLAB_KEY_ATTRIBUTE_REPEAT | $FXLAB_KEY_ATTRIBUTE_STORAGE"
	
	KeyDefine	Damage
	KeyDefine	Type
	KeyDefine	ActiveRadius
	KeyDefine	PassiveRadius

	KeyDefine	ErectionTime

	KeyTime	0.0

	KeyName	Damage
	KeyData	4.0

	KeyName	Type
	KeyData	$FXLAB_DAMAGE_SPHERE_TYPE_GENERIC

	KeyName	ActiveRadius
	KeyData	300.0

	KeyName	PassiveRadius
	KeyData	250.0

	KeyName	ErectionTime
	KeyData 500.0
}
