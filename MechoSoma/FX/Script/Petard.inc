KeyID $FXLAB_ID_KEY_PETARD_MODEL
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
	KeyData	49.0

	KeyName	Red
	KeyData	1.0

	KeyName	Green
	KeyData	1.0

	KeyName	Blue
	KeyData	1.0

	KeyName	Alpha
	KeyData	1.0


	KeyName	Radius
	KeyData	30.0

	KeyName	OffsetAngleX
	KeyData	0.0

	KeyName	OffsetAngleY
	KeyData	0.0

	KeyName	OffsetAngleZ
	KeyData	0.0
}

//---------------------------------------

KeyID	$FXLAB_ID_KEY_PETARD_TAIL_POSITION
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

KeyID	$FXLAB_ID_KEY_PETARD_TAIL_VELOCITY
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

KeyID	$FXLAB_ID_KEY_PETARD_TAIL
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
	KeyData	$FXLAB_ID_SPRITE_CLOWN_RED

	KeyName	ImageNoise
	KeyData	7.0


	KeyName	Rate
	KeyData 1.5

	KeyName	Age
	KeyData 10.0

	KeyName	StartPower
	KeyData 0.0

	KeyName	EndPower
	KeyData 1.0

	KeyName	PositionKeyID
	KeyData $FXLAB_ID_KEY_PETARD_TAIL_POSITION

	KeyName VelocityKeyID
	KeyData $FXLAB_ID_KEY_PETARD_TAIL_VELOCITY


	KeyName	PointPower
	KeyData 100.0

	KeyName	PointEpsilon
	KeyData 2.0

	KeyName	PointRadius
	KeyData 100.0
}

//-------------------------------

KeyID $FXLAB_ID_KEY_PETARD_BURN_MODEL
{
	KeyAttribute	$FXLAB_KEY_ATTRIBUTE_NONE

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
	KeyData	32.0

	KeyName	Red
	KeyData	1.0

	KeyName	Green
	KeyData	1.0

	KeyName	Blue
	KeyData	0.0

	KeyName	Alpha
	KeyData	1.0


	KeyName	Radius
	KeyData	20.0

	KeyName	OffsetAngleX
	KeyData	0.0

	KeyName	OffsetAngleY
	KeyData	0.0

	KeyName	OffsetAngleZ
	KeyData	0.0


	KeyTime 8.0

	KeyName	Red
	KeyData	1.0

	KeyName	Green
	KeyData	1.0

	KeyName	Blue
	KeyData	0.0

	KeyName Alpha
	KeyData 0.2


	KeyTime	12.0

	KeyName	Model
	KeyData	32.0

	KeyName	Red
	KeyData	1.0

	KeyName	Green
	KeyData	0.0

	KeyName	Blue
	KeyData	0.0

	KeyName	Alpha
	KeyData	0.01


	KeyName	Radius
	KeyData	200.0

	KeyName	OffsetAngleX
	KeyData	0.0

	KeyName	OffsetAngleY
	KeyData	0.0

	KeyName	OffsetAngleZ
	KeyData	0.0
}

KeyID $FXLAB_ID_KEY_PETARD_BURN_OMNI
{
	KeyAttribute	$FXLAB_KEY_ATTRIBUTE_NONE

	KeyDefine	Red
	KeyDefine	Green
	KeyDefine	Blue
	KeyDefine	Radius

	KeyTime	0.0

	KeyName	Red
	KeyData	0.9

	KeyName Green
	KeyData	0.5

	KeyName Blue
	KeyData 0.1

	KeyName Radius
	KeyData	40.0


	KeyTime	6.0

	KeyName	Red
	KeyData	0.1

	KeyName Green
	KeyData	0.1

	KeyName Blue
	KeyData 0.0

	KeyName Radius
	KeyData	15.0
}


KeyID	$FXLAB_ID_KEY_PETARD_BURN_POSITION
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
	KeyData	10.0

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

KeyID	$FXLAB_ID_KEY_PETARD_BURN_VELOCITY
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
	KeyData	4.0

	KeyName	DataC1
	KeyData	0

	KeyName	DataA2
	KeyData	0

	KeyName	DataB2
	KeyData	0

	KeyName	DataC2
	KeyData	0
}

KeyID	$FXLAB_ID_KEY_PETARD_BURN
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


	KeyTime	0.0

	KeyName	RegenerateTime
	KeyData	15.0

	KeyName	ImageType
	KeyData	$FXLAB_SCRIPT_SECTION_SPRITE

	KeyName	ImageID
	KeyData	$FXLAB_ID_SPRITE_CLOWN_RED

	KeyName	ImageNoise
	KeyData	7.0


	KeyName	Rate
	KeyData 20.0

	KeyName	Age
	KeyData 10.0

	KeyName	StartPower
	KeyData 0.0

	KeyName	EndPower
	KeyData 1.0

	KeyName	PositionKeyID
	KeyData $FXLAB_ID_KEY_PETARD_BURN_POSITION

	KeyName VelocityKeyID
	KeyData $FXLAB_ID_KEY_PETARD_BURN_VELOCITY


	KeyTime	3.0

	KeyName	RegenerateTime
	KeyData	15.0

	KeyName	ImageType
	KeyData	$FXLAB_SCRIPT_SECTION_SPRITE

	KeyName	ImageID
	KeyData	$FXLAB_ID_SPRITE_CLOWN_RED

	KeyName	ImageNoise
	KeyData	7.0


	KeyName	Rate
	KeyData 5.0

	KeyName	Age
	KeyData 10.0

	KeyName	StartPower
	KeyData 0.0

	KeyName	EndPower
	KeyData 1.0

	KeyName	PositionKeyID
	KeyData $FXLAB_ID_KEY_PETARD_BURN_POSITION

	KeyName VelocityKeyID
	KeyData $FXLAB_ID_KEY_PETARD_BURN_VELOCITY
}

//----------------------------------------------------

KeyID	$FXLAB_ID_KEY_PETARD_BOX_TAIL_POSITION
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

KeyID	$FXLAB_ID_KEY_PETARD_BOX_TAIL_VELOCITY
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

KeyID	$FXLAB_ID_KEY_PETARD_BOX_TAIL
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
	KeyData	$FXLAB_ID_SPRITE_CLOWN_RED

	KeyName	ImageNoise
	KeyData	7.0


	KeyName	Rate
	KeyData 1.0

	KeyName	Age
	KeyData 15.0

	KeyName	StartPower
	KeyData 0.0

	KeyName	EndPower
	KeyData 1.0

	KeyName	PositionKeyID
	KeyData $FXLAB_ID_KEY_PETARD_BOX_TAIL_POSITION

	KeyName VelocityKeyID
	KeyData $FXLAB_ID_KEY_PETARD_BOX_TAIL_VELOCITY


	KeyName	PointPower
	KeyData 100.0

	KeyName	PointEpsilon
	KeyData 2.0

	KeyName	PointRadius
	KeyData 100.0
}

//-------------------------------------------------

KeyID	$FXLAB_ID_KEY_PETARD_BONUS_POSITION
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
	KeyData	30.0

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

KeyID	$FXLAB_ID_KEY_PETARD_BONUS_VELOCITY
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
	KeyData	3.0

	KeyName	DataB1
	KeyData	0

	KeyName	DataC1
	KeyData	0

	KeyName	DataA2
	KeyData	0

	KeyName	DataB2
	KeyData	0

	KeyName	DataC2
	KeyData	0.0
}


KeyID	$FXLAB_ID_KEY_PETARD_BONUS
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

	KeyTime	3.0

	KeyName	RegenerateTime
	KeyData	5.0


	KeyName	ImageType
	KeyData	$FXLAB_SCRIPT_SECTION_SPRITE

	KeyName	ImageID
	KeyData	$FXLAB_ID_SPRITE_CLOWN_RED

	KeyName	ImageNoise
	KeyData	7.0


	KeyName	Rate
	KeyData 10.0

	KeyName	Age
	KeyData 15.0

	KeyName	StartPower
	KeyData 0.0

	KeyName	EndPower
	KeyData 1.0

	KeyName	PositionKeyID
	KeyData $FXLAB_ID_KEY_PETARD_BONUS_POSITION

	KeyName VelocityKeyID
	KeyData $FXLAB_ID_KEY_PETARD_BONUS_VELOCITY
}


KeyID	$FXLAB_ID_KEY_MINOR_BONUS_CLIENT_LINK
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
	KeyData $FXLAB_ID_KEY_PETARD_BOX_TAIL

	KeyName	ExplodeType
//	KeyData	$FXLAB_CLIENT_PROCESS_OBJECT_SET
	KeyData -1.0

	KeyName	ExplodeID
//	KeyData	$FXLAB_ID_KEY_BOMB_EXPLOSION_CLIENT_SET
	KeyData -1.0

	KeyName SpeedBorder
	KeyData 0.1
}

KeyID	$FXLAB_ID_KEY_MAJOR_BONUS_CLIENT_LINK
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
	KeyData $FXLAB_ID_KEY_PETARD_BOX_TAIL

	KeyName	ExplodeType
//	KeyData	$FXLAB_CLIENT_PROCESS_SPARK_GENERATOR
	KeyData -1.0

	KeyName	ExplodeID
//	KeyData	$FXLAB_ID_KEY_PETARD_BONUS
	KeyData	-1.0

	KeyName SpeedBorder
	KeyData 0.1
}

