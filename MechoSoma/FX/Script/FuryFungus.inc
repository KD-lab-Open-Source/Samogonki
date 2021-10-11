KeyID	$FXLAB_ID_KEY_CORE_FURY_FUNGUS_FOG
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
	KeyData	0.7

	KeyName	Green
	KeyData	0.9

	KeyName Blue
	KeyData	0.7

	KeyName Alpha
	KeyData 0.0

	KeyName	Size
	KeyData	8.0

	KeyName	Angle
	KeyData	0.0

	KeyName	Frame
	KeyData	0.0


	KeyTime	16.0

	KeyName Alpha
	KeyData 0.5


	KeyTime	32.0

	KeyName	Red
	KeyData	0.7

	KeyName	Green
	KeyData	0.9

	KeyName Blue
	KeyData	0.7

	KeyName Alpha
	KeyData 0.0

	KeyName	Size
	KeyData	32.0

	KeyName	Angle
	KeyData	"3.14*4.0"

	KeyName	Frame
	KeyData	1.0
}

KeyID	$FXLAB_ID_KEY_FURY_FUNGUS_FOG_POSITION
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
	KeyData	4.0

	KeyName	DataA1
	KeyData	30.0

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

KeyID	$FXLAB_ID_KEY_FURY_FUNGUS_FOG_VELOCITY
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
	KeyData	0.0

	KeyName	DataA1
	KeyData	0.0

	KeyName	DataB1
	KeyData	0.0

	KeyName	DataC1
	KeyData	1.0

	KeyName	DataA2
	KeyData	0

	KeyName	DataB2
	KeyData	0

	KeyName	DataC2
	KeyData	0
}

KeyID	$FXLAB_ID_KEY_FURY_FUNGUS_FOG
{
	KeyAttribute	$FXLAB_KEY_ATTRIBUTE_FREEZE
	
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
	KeyData	$FXLAB_ID_SPRITE_FURY_FUNGUS_FOG

	KeyName	ImageNoise
	KeyData	0

	
	KeyName		Rate
	KeyData		0.7
	
	KeyName		Age
	KeyData		40.0

	KeyName		StartPower
	KeyData		0.0

	KeyName		EndPower
	KeyData		1.0

	KeyName		PositionKeyID
	KeyData		$FXLAB_ID_KEY_FURY_FUNGUS_FOG_POSITION

	KeyName		VelocityKeyID
	KeyData		$FXLAB_ID_KEY_FURY_FUNGUS_FOG_VELOCITY


	KeyTime	10.0

	KeyName	RegenerateTime
	KeyData	15.0

	KeyName	ImageType
	KeyData	$FXLAB_SCRIPT_SECTION_SPRITE

	KeyName	ImageID
	KeyData	$FXLAB_ID_SPRITE_FURY_FUNGUS_FOG

	KeyName	ImageNoise
	KeyData	0

	
	KeyName		Rate
	KeyData		0.5
	
	KeyName		Age
	KeyData		40.0

	KeyName		StartPower
	KeyData		0.0

	KeyName		EndPower
	KeyData		1.0

	KeyName		PositionKeyID
	KeyData		$FXLAB_ID_KEY_FURY_FUNGUS_FOG_POSITION

	KeyName		VelocityKeyID
	KeyData		$FXLAB_ID_KEY_FURY_FUNGUS_FOG_VELOCITY
}

KeyID	$FXLAB_ID_KEY_FURY_FUNGUS_CIRCLE
{
	KeyAttribute	"$FXLAB_KEY_ATTRIBUTE_REPEAT | $FXLAB_KEY_ATTRIBUTE_STORAGE"

	KeyDefine	Force
	KeyDefine	Radius
	KeyDefine	MinZ
	KeyDefine	MaxZ

	KeyTime	0.0

	KeyName	Force
	KeyData	0.0

	KeyName	Radius
	KeyData	15.0

	KeyName MinZ
	KeyData -16.0

	KeyName MaxZ
	KeyData 16.0
}

KeyID	$FXLAB_ID_KEY_FURY_FUNGUS_LASH
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

	KeyDefine	DeltaAnimation

	KeyTime	0.0

	KeyName	Model
	KeyData	53.0

	KeyName Red
	KeyData	1.0

	KeyName	Green
	KeyData	0.0

	KeyName Blue
	KeyData	0.0

	KeyName Alpha
	KeyData 0.7


	KeyName Radius
	KeyData 7.0

	KeyName AngleX
	KeyData 0.0

	KeyName AngleY
	KeyData	0.0

	KeyName AngleZ
	KeyData 0.0


	KeyName DeltaAnimation
	KeyData 0.0


	KeyTime	10.0

	KeyName	Model
	KeyData	53.0

	KeyName Red
	KeyData	0.8

	KeyName	Green
	KeyData	1.0

	KeyName Blue
	KeyData	0.8

	KeyName Alpha
	KeyData 0.7


	KeyName Radius
	KeyData 10.0

	KeyName AngleX
	KeyData 0.0

	KeyName AngleY
	KeyData	0.0

	KeyName AngleZ
	KeyData 0.0


	KeyName DeltaAnimation
	KeyData 0.005
}

