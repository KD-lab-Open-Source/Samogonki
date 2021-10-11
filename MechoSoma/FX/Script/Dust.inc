#define FXLAB_DUST_LIFE_TIME	6.0

KeyID	$FXLAB_ID_KEY_MECHOS_DUST_POSITION
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

KeyID	$FXLAB_ID_KEY_MECHOS_DUST_VELOCITY
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
	KeyData	1.0

	KeyName	DataA1
	KeyData	1.0

	KeyName	DataB1
	KeyData	0.1

	KeyName	DataC1
	KeyData	2.0

	KeyName	DataA2
	KeyData	0

	KeyName	DataB2
	KeyData	0

	KeyName	DataC2
	KeyData	0
}

//-----------------------------------------------------------
//Generic

KeyID	$FXLAB_ID_KEY_CORE_MECHOS_DUST000
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
	KeyData	0.3

	KeyName	Green
	KeyData	0.3

	KeyName Blue
	KeyData	0.3

	KeyName Alpha
	KeyData 0.8

	KeyName	Size
	KeyData	4.0

	KeyName	Angle
	KeyData	0.0

	KeyName	Frame
	KeyData	0.0


	KeyTime	16.0

	KeyName	Red
	KeyData	0.3

	KeyName	Green
	KeyData	0.3

	KeyName Blue
	KeyData	0.3

	KeyName Alpha
	KeyData 0.0

	KeyName	Size
	KeyData	24.0

	KeyName	Angle
	KeyData	"3.14*8.0"

	KeyName	Frame
	KeyData	1.0
}

KeyID	$FXLAB_ID_KEY_MECHOS_DUST000
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

	KeyTime	5.0

	KeyName	RegenerateTime
	KeyData	15.0

	KeyName	ImageType
	KeyData	$FXLAB_SCRIPT_SECTION_SPRITE

	KeyName	ImageID
	KeyData	$FXLAB_ID_SPRITE_MECHOS_DUST000

	KeyName	ImageNoise
	KeyData	0


	KeyName	Rate
	KeyData 0.5

	KeyName	Age
	KeyData $FXLAB_DUST_LIFE_TIME

	KeyName	StartPower
	KeyData 0.0

	KeyName	EndPower
	KeyData 1.0

	KeyName	PositionKeyID
	KeyData	$FXLAB_ID_KEY_MECHOS_DUST_POSITION

	KeyName	VelocityKeyID
	KeyData	$FXLAB_ID_KEY_MECHOS_DUST_VELOCITY
}

//-----------------------------------------------------------
//Town

KeyID	$FXLAB_ID_KEY_CORE_MECHOS_DUST302
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
	KeyData	0.65

	KeyName Blue
	KeyData	0.3

	KeyName Alpha
	KeyData 0.8

	KeyName	Size
	KeyData	4.0

	KeyName	Angle
	KeyData	0.0

	KeyName	Frame
	KeyData	0.0


	KeyTime	16.0

	KeyName	Red
	KeyData	0.7

	KeyName	Green
	KeyData	0.65

	KeyName Blue
	KeyData	0.3

	KeyName Alpha
	KeyData 0.0

	KeyName	Size
	KeyData	24.0

	KeyName	Angle
	KeyData	"3.14*8.0"

	KeyName	Frame
	KeyData	1.0
}

KeyID	$FXLAB_ID_KEY_MECHOS_DUST302
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

	KeyTime	5.0

	KeyName	RegenerateTime
	KeyData	15.0

	KeyName	ImageType
	KeyData	$FXLAB_SCRIPT_SECTION_SPRITE

	KeyName	ImageID
	KeyData	$FXLAB_ID_SPRITE_MECHOS_DUST302

	KeyName	ImageNoise
	KeyData	0


	KeyName	Rate
	KeyData 0.5

	KeyName	Age
	KeyData $FXLAB_DUST_LIFE_TIME

	KeyName	StartPower
	KeyData 0.0

	KeyName	EndPower
	KeyData 1.0

	KeyName	PositionKeyID
	KeyData	$FXLAB_ID_KEY_MECHOS_DUST_POSITION

	KeyName	VelocityKeyID
	KeyData	$FXLAB_ID_KEY_MECHOS_DUST_VELOCITY
}

//-------------

KeyID	$FXLAB_ID_KEY_CORE_MECHOS_DUST303
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
	KeyData	0.2

	KeyName	Green
	KeyData	0.5

	KeyName Blue
	KeyData	0.2

	KeyName Alpha
	KeyData 0.8

	KeyName	Size
	KeyData	4.0

	KeyName	Angle
	KeyData	0.0

	KeyName	Frame
	KeyData	0.0


	KeyTime	16.0

	KeyName	Red
	KeyData	0.2

	KeyName	Green
	KeyData	0.5

	KeyName Blue
	KeyData	0.2

	KeyName Alpha
	KeyData 0.0

	KeyName	Size
	KeyData	24.0

	KeyName	Angle
	KeyData	"3.14*8.0"

	KeyName	Frame
	KeyData	1.0
}

KeyID	$FXLAB_ID_KEY_MECHOS_DUST303
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

	KeyTime	5.0

	KeyName	RegenerateTime
	KeyData	15.0

	KeyName	ImageType
	KeyData	$FXLAB_SCRIPT_SECTION_SPRITE

	KeyName	ImageID
	KeyData	$FXLAB_ID_SPRITE_MECHOS_DUST303

	KeyName	ImageNoise
	KeyData	0


	KeyName	Rate
	KeyData 0.5

	KeyName	Age
	KeyData $FXLAB_DUST_LIFE_TIME

	KeyName	StartPower
	KeyData 0.0

	KeyName	EndPower
	KeyData 1.0

	KeyName	PositionKeyID
	KeyData	$FXLAB_ID_KEY_MECHOS_DUST_POSITION

	KeyName	VelocityKeyID
	KeyData	$FXLAB_ID_KEY_MECHOS_DUST_VELOCITY
}

//----------------

KeyID	$FXLAB_ID_KEY_CORE_MECHOS_DUST305
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
	KeyData	0.1

	KeyName Alpha
	KeyData 0.8

	KeyName	Size
	KeyData	4.0

	KeyName	Angle
	KeyData	0.0

	KeyName	Frame
	KeyData	0.0


	KeyTime	16.0

	KeyName	Red
	KeyData	0.2

	KeyName	Green
	KeyData	0.2

	KeyName Blue
	KeyData	0.1

	KeyName Alpha
	KeyData 0.0

	KeyName	Size
	KeyData	24.0

	KeyName	Angle
	KeyData	"3.14*8.0"

	KeyName	Frame
	KeyData	1.0
}

KeyID	$FXLAB_ID_KEY_MECHOS_DUST305
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

	KeyTime	5.0

	KeyName	RegenerateTime
	KeyData	15.0

	KeyName	ImageType
	KeyData	$FXLAB_SCRIPT_SECTION_SPRITE

	KeyName	ImageID
	KeyData	$FXLAB_ID_SPRITE_MECHOS_DUST305

	KeyName	ImageNoise
	KeyData	0


	KeyName	Rate
	KeyData 0.5

	KeyName	Age
	KeyData $FXLAB_DUST_LIFE_TIME

	KeyName	StartPower
	KeyData 0.0

	KeyName	EndPower
	KeyData 1.0

	KeyName	PositionKeyID
	KeyData	$FXLAB_ID_KEY_MECHOS_DUST_POSITION

	KeyName	VelocityKeyID
	KeyData	$FXLAB_ID_KEY_MECHOS_DUST_VELOCITY
}



//-----------------------------------------------------------
//Lava

KeyID	$FXLAB_ID_KEY_CORE_MECHOS_DUST400
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
	KeyData	0.35

	KeyName Blue
	KeyData	0.2

	KeyName Alpha
	KeyData 0.8

	KeyName	Size
	KeyData	4.0

	KeyName	Angle
	KeyData	0.0

	KeyName	Frame
	KeyData	0.0


	KeyTime	16.0

	KeyName	Red
	KeyData	0.5

	KeyName	Green
	KeyData	0.35

	KeyName Blue
	KeyData	0.2

	KeyName Alpha
	KeyData 0.0

	KeyName	Size
	KeyData	24.0

	KeyName	Angle
	KeyData	"3.14*8.0"

	KeyName	Frame
	KeyData	1.0
}

KeyID	$FXLAB_ID_KEY_MECHOS_DUST400
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

	KeyTime	5.0

	KeyName	RegenerateTime
	KeyData	15.0

	KeyName	ImageType
	KeyData	$FXLAB_SCRIPT_SECTION_SPRITE

	KeyName	ImageID
	KeyData	$FXLAB_ID_SPRITE_MECHOS_DUST400

	KeyName	ImageNoise
	KeyData	0


	KeyName	Rate
	KeyData 0.5

	KeyName	Age
	KeyData $FXLAB_DUST_LIFE_TIME

	KeyName	StartPower
	KeyData 0.0

	KeyName	EndPower
	KeyData 1.0

	KeyName	PositionKeyID
	KeyData	$FXLAB_ID_KEY_MECHOS_DUST_POSITION

	KeyName	VelocityKeyID
	KeyData	$FXLAB_ID_KEY_MECHOS_DUST_VELOCITY
}


//-------------

KeyID	$FXLAB_ID_KEY_CORE_MECHOS_DUST402
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
	KeyData	0.35

	KeyName Blue
	KeyData	0.2

	KeyName Alpha
	KeyData 0.8

	KeyName	Size
	KeyData	4.0

	KeyName	Angle
	KeyData	0.0

	KeyName	Frame
	KeyData	0.0


	KeyTime	16.0

	KeyName	Red
	KeyData	0.7

	KeyName	Green
	KeyData	0.35

	KeyName Blue
	KeyData	0.2

	KeyName Alpha
	KeyData 0.0

	KeyName	Size
	KeyData	24.0

	KeyName	Angle
	KeyData	"3.14*8.0"

	KeyName	Frame
	KeyData	1.0
}

KeyID	$FXLAB_ID_KEY_MECHOS_DUST402
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

	KeyTime	5.0

	KeyName	RegenerateTime
	KeyData	15.0

	KeyName	ImageType
	KeyData	$FXLAB_SCRIPT_SECTION_SPRITE

	KeyName	ImageID
	KeyData	$FXLAB_ID_SPRITE_MECHOS_DUST402

	KeyName	ImageNoise
	KeyData	0


	KeyName	Rate
	KeyData 0.5

	KeyName	Age
	KeyData $FXLAB_DUST_LIFE_TIME

	KeyName	StartPower
	KeyData 0.0

	KeyName	EndPower
	KeyData 1.0

	KeyName	PositionKeyID
	KeyData	$FXLAB_ID_KEY_MECHOS_DUST_POSITION

	KeyName	VelocityKeyID
	KeyData	$FXLAB_ID_KEY_MECHOS_DUST_VELOCITY
}

//--------------------

KeyID	$FXLAB_ID_KEY_CORE_MECHOS_DUST412
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
	KeyData	0.15

	KeyName	Green
	KeyData	0.3

	KeyName Blue
	KeyData	0.15

	KeyName Alpha
	KeyData 0.8

	KeyName	Size
	KeyData	4.0

	KeyName	Angle
	KeyData	0.0

	KeyName	Frame
	KeyData	0.0


	KeyTime	16.0

	KeyName	Red
	KeyData	0.15

	KeyName	Green
	KeyData	0.3

	KeyName Blue
	KeyData	0.15

	KeyName Alpha
	KeyData 0.0

	KeyName	Size
	KeyData	24.0

	KeyName	Angle
	KeyData	"3.14*8.0"

	KeyName	Frame
	KeyData	1.0
}

KeyID	$FXLAB_ID_KEY_MECHOS_DUST412
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

	KeyTime	5.0

	KeyName	RegenerateTime
	KeyData	15.0

	KeyName	ImageType
	KeyData	$FXLAB_SCRIPT_SECTION_SPRITE

	KeyName	ImageID
	KeyData	$FXLAB_ID_SPRITE_MECHOS_DUST412

	KeyName	ImageNoise
	KeyData	0


	KeyName	Rate
	KeyData 0.5

	KeyName	Age
	KeyData $FXLAB_DUST_LIFE_TIME

	KeyName	StartPower
	KeyData 0.0

	KeyName	EndPower
	KeyData 1.0

	KeyName	PositionKeyID
	KeyData	$FXLAB_ID_KEY_MECHOS_DUST_POSITION

	KeyName	VelocityKeyID
	KeyData	$FXLAB_ID_KEY_MECHOS_DUST_VELOCITY
}


//-----------------------------------------------------------
//Water

KeyID	$FXLAB_ID_KEY_CORE_MECHOS_DUST100
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
	KeyData	0.2

	KeyName	Green
	KeyData	0.2

	KeyName Blue
	KeyData	0.2

	KeyName Alpha
	KeyData 0.8

	KeyName	Size
	KeyData	4.0

	KeyName	Angle
	KeyData	0.0

	KeyName	Frame
	KeyData	0.0


	KeyTime	16.0

	KeyName	Red
	KeyData	0.2

	KeyName	Green
	KeyData	0.2

	KeyName Blue
	KeyData	0.2

	KeyName Alpha
	KeyData 0.0

	KeyName	Size
	KeyData	24.0

	KeyName	Angle
	KeyData	"3.14*8.0"

	KeyName	Frame
	KeyData	1.0
}

KeyID	$FXLAB_ID_KEY_MECHOS_DUST100
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

	KeyTime	5.0

	KeyName	RegenerateTime
	KeyData	15.0

	KeyName	ImageType
	KeyData	$FXLAB_SCRIPT_SECTION_SPRITE

	KeyName	ImageID
	KeyData	$FXLAB_ID_SPRITE_MECHOS_DUST100

	KeyName	ImageNoise
	KeyData	0


	KeyName	Rate
	KeyData 0.5

	KeyName	Age
	KeyData $FXLAB_DUST_LIFE_TIME

	KeyName	StartPower
	KeyData 0.0

	KeyName	EndPower
	KeyData 1.0

	KeyName	PositionKeyID
	KeyData	$FXLAB_ID_KEY_MECHOS_DUST_POSITION

	KeyName	VelocityKeyID
	KeyData	$FXLAB_ID_KEY_MECHOS_DUST_VELOCITY
}

//----------------

KeyID	$FXLAB_ID_KEY_CORE_MECHOS_DUST102
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
	KeyData	0.3

	KeyName	Green
	KeyData	0.3

	KeyName Blue
	KeyData	0.3

	KeyName Alpha
	KeyData 0.8

	KeyName	Size
	KeyData	4.0

	KeyName	Angle
	KeyData	0.0

	KeyName	Frame
	KeyData	0.0


	KeyTime	16.0

	KeyName	Red
	KeyData	0.3

	KeyName	Green
	KeyData	0.3

	KeyName Blue
	KeyData	0.3

	KeyName Alpha
	KeyData 0.0

	KeyName	Size
	KeyData	24.0

	KeyName	Angle
	KeyData	"3.14*8.0"

	KeyName	Frame
	KeyData	1.0
}

KeyID	$FXLAB_ID_KEY_MECHOS_DUST102
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

	KeyTime	5.0

	KeyName	RegenerateTime
	KeyData	15.0

	KeyName	ImageType
	KeyData	$FXLAB_SCRIPT_SECTION_SPRITE

	KeyName	ImageID
	KeyData	$FXLAB_ID_SPRITE_MECHOS_DUST102

	KeyName	ImageNoise
	KeyData	0


	KeyName	Rate
	KeyData 0.5

	KeyName	Age
	KeyData $FXLAB_DUST_LIFE_TIME

	KeyName	StartPower
	KeyData 0.0

	KeyName	EndPower
	KeyData 1.0

	KeyName	PositionKeyID
	KeyData	$FXLAB_ID_KEY_MECHOS_DUST_POSITION

	KeyName	VelocityKeyID
	KeyData	$FXLAB_ID_KEY_MECHOS_DUST_VELOCITY
}

//----------

KeyID	$FXLAB_ID_KEY_CORE_MECHOS_DUST142
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
	KeyData	0.7

	KeyName Blue
	KeyData	0.7

	KeyName Alpha
	KeyData 0.8

	KeyName	Size
	KeyData	4.0

	KeyName	Angle
	KeyData	0.0

	KeyName	Frame
	KeyData	0.0


	KeyTime	16.0

	KeyName	Red
	KeyData	0.7

	KeyName	Green
	KeyData	0.7

	KeyName Blue
	KeyData	0.7

	KeyName Alpha
	KeyData 0.0

	KeyName	Size
	KeyData	24.0

	KeyName	Angle
	KeyData	"3.14*8.0"

	KeyName	Frame
	KeyData	1.0
}

KeyID	$FXLAB_ID_KEY_MECHOS_DUST142
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

	KeyTime	5.0

	KeyName	RegenerateTime
	KeyData	15.0

	KeyName	ImageType
	KeyData	$FXLAB_SCRIPT_SECTION_SPRITE

	KeyName	ImageID
	KeyData	$FXLAB_ID_SPRITE_MECHOS_DUST142

	KeyName	ImageNoise
	KeyData	0


	KeyName	Rate
	KeyData 0.5

	KeyName	Age
	KeyData $FXLAB_DUST_LIFE_TIME

	KeyName	StartPower
	KeyData 0.0

	KeyName	EndPower
	KeyData 1.0

	KeyName	PositionKeyID
	KeyData	$FXLAB_ID_KEY_MECHOS_DUST_POSITION

	KeyName	VelocityKeyID
	KeyData	$FXLAB_ID_KEY_MECHOS_DUST_VELOCITY
}


//-----------------------------------------------------------
//Forest
//0

KeyID	$FXLAB_ID_KEY_CORE_MECHOS_DUST200
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
	KeyData	0.3

	KeyName	Green
	KeyData	0.4

	KeyName Blue
	KeyData	0.15

	KeyName Alpha
	KeyData 0.8

	KeyName	Size
	KeyData	4.0

	KeyName	Angle
	KeyData	0.0

	KeyName	Frame
	KeyData	0.0


	KeyTime	16.0

	KeyName	Red
	KeyData	0.3

	KeyName	Green
	KeyData	0.4

	KeyName Blue
	KeyData	0.15

	KeyName Alpha
	KeyData 0.0

	KeyName	Size
	KeyData	24.0

	KeyName	Angle
	KeyData	"3.14*8.0"

	KeyName	Frame
	KeyData	1.0
}

KeyID	$FXLAB_ID_KEY_MECHOS_DUST200
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

	KeyTime	5.0

	KeyName	RegenerateTime
	KeyData	15.0

	KeyName	ImageType
	KeyData	$FXLAB_SCRIPT_SECTION_SPRITE

	KeyName	ImageID
	KeyData	$FXLAB_ID_SPRITE_MECHOS_DUST200

	KeyName	ImageNoise
	KeyData	0


	KeyName	Rate
	KeyData 0.5

	KeyName	Age
	KeyData $FXLAB_DUST_LIFE_TIME

	KeyName	StartPower
	KeyData 0.0

	KeyName	EndPower
	KeyData 1.0

	KeyName	PositionKeyID
	KeyData	$FXLAB_ID_KEY_MECHOS_DUST_POSITION

	KeyName	VelocityKeyID
	KeyData	$FXLAB_ID_KEY_MECHOS_DUST_VELOCITY
}

//-----------------

KeyID	$FXLAB_ID_KEY_CORE_MECHOS_DUST202
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
	KeyData	0.35

	KeyName	Green
	KeyData	0.35

	KeyName Blue
	KeyData	0.15

	KeyName Alpha
	KeyData 0.8

	KeyName	Size
	KeyData	4.0

	KeyName	Angle
	KeyData	0.0

	KeyName	Frame
	KeyData	0.0


	KeyTime	16.0

	KeyName	Red
	KeyData	0.35

	KeyName	Green
	KeyData	0.35

	KeyName Blue
	KeyData	0.15

	KeyName Alpha
	KeyData 0.0

	KeyName	Size
	KeyData	24.0

	KeyName	Angle
	KeyData	"3.14*8.0"

	KeyName	Frame
	KeyData	1.0
}

KeyID	$FXLAB_ID_KEY_MECHOS_DUST202
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

	KeyTime	5.0

	KeyName	RegenerateTime
	KeyData	15.0

	KeyName	ImageType
	KeyData	$FXLAB_SCRIPT_SECTION_SPRITE

	KeyName	ImageID
	KeyData	$FXLAB_ID_SPRITE_MECHOS_DUST202

	KeyName	ImageNoise
	KeyData	0


	KeyName	Rate
	KeyData 0.5

	KeyName	Age
	KeyData $FXLAB_DUST_LIFE_TIME

	KeyName	StartPower
	KeyData 0.0

	KeyName	EndPower
	KeyData 1.0

	KeyName	PositionKeyID
	KeyData	$FXLAB_ID_KEY_MECHOS_DUST_POSITION

	KeyName	VelocityKeyID
	KeyData	$FXLAB_ID_KEY_MECHOS_DUST_VELOCITY
}


//-----------------

KeyID	$FXLAB_ID_KEY_CORE_MECHOS_DUST207
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
	KeyData	0.4

	KeyName	Green
	KeyData	0.3

	KeyName Blue
	KeyData	0.1

	KeyName Alpha
	KeyData 0.8

	KeyName	Size
	KeyData	4.0

	KeyName	Angle
	KeyData	0.0

	KeyName	Frame
	KeyData	0.0


	KeyTime	16.0

	KeyName	Red
	KeyData	0.4

	KeyName	Green
	KeyData	0.3

	KeyName Blue
	KeyData	0.1

	KeyName Alpha
	KeyData 0.0

	KeyName	Size
	KeyData	24.0

	KeyName	Angle
	KeyData	"3.14*8.0"

	KeyName	Frame
	KeyData	1.0
}

KeyID	$FXLAB_ID_KEY_MECHOS_DUST207
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

	KeyTime	5.0

	KeyName	RegenerateTime
	KeyData	15.0

	KeyName	ImageType
	KeyData	$FXLAB_SCRIPT_SECTION_SPRITE

	KeyName	ImageID
	KeyData	$FXLAB_ID_SPRITE_MECHOS_DUST207

	KeyName	ImageNoise
	KeyData	0


	KeyName	Rate
	KeyData 0.5

	KeyName	Age
	KeyData $FXLAB_DUST_LIFE_TIME

	KeyName	StartPower
	KeyData 0.0

	KeyName	EndPower
	KeyData 1.0

	KeyName	PositionKeyID
	KeyData	$FXLAB_ID_KEY_MECHOS_DUST_POSITION

	KeyName	VelocityKeyID
	KeyData	$FXLAB_ID_KEY_MECHOS_DUST_VELOCITY
}

//-----------------------------------------------------------
//Forest
//1

KeyID	$FXLAB_ID_KEY_CORE_MECHOS_DUST210
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
	KeyData	0.15

	KeyName	Green
	KeyData	0.15

	KeyName Blue
	KeyData	0.15

	KeyName Alpha
	KeyData 0.8

	KeyName	Size
	KeyData	4.0

	KeyName	Angle
	KeyData	0.0

	KeyName	Frame
	KeyData	0.0


	KeyTime	16.0

	KeyName	Red
	KeyData	0.15

	KeyName	Green
	KeyData	0.15

	KeyName Blue
	KeyData	0.15

	KeyName Alpha
	KeyData 0.0

	KeyName	Size
	KeyData	24.0

	KeyName	Angle
	KeyData	"3.14*8.0"

	KeyName	Frame
	KeyData	1.0
}

KeyID	$FXLAB_ID_KEY_MECHOS_DUST210
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

	KeyTime	5.0

	KeyName	RegenerateTime
	KeyData	15.0

	KeyName	ImageType
	KeyData	$FXLAB_SCRIPT_SECTION_SPRITE

	KeyName	ImageID
	KeyData	$FXLAB_ID_SPRITE_MECHOS_DUST210

	KeyName	ImageNoise
	KeyData	0


	KeyName	Rate
	KeyData 0.5

	KeyName	Age
	KeyData $FXLAB_DUST_LIFE_TIME

	KeyName	StartPower
	KeyData 0.0

	KeyName	EndPower
	KeyData 1.0

	KeyName	PositionKeyID
	KeyData	$FXLAB_ID_KEY_MECHOS_DUST_POSITION

	KeyName	VelocityKeyID
	KeyData	$FXLAB_ID_KEY_MECHOS_DUST_VELOCITY
}

//-----------------

KeyID	$FXLAB_ID_KEY_CORE_MECHOS_DUST212
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
	KeyData	0.3

	KeyName	Green
	KeyData	0.3

	KeyName Blue
	KeyData	0.7

	KeyName Alpha
	KeyData 0.8

	KeyName	Size
	KeyData	4.0

	KeyName	Angle
	KeyData	0.0

	KeyName	Frame
	KeyData	0.0


	KeyTime	16.0

	KeyName	Red
	KeyData	0.3

	KeyName	Green
	KeyData	0.3

	KeyName Blue
	KeyData	0.7

	KeyName Alpha
	KeyData 0.0

	KeyName	Size
	KeyData	24.0

	KeyName	Angle
	KeyData	"3.14*8.0"

	KeyName	Frame
	KeyData	1.0
}

KeyID	$FXLAB_ID_KEY_MECHOS_DUST212
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

	KeyTime	5.0

	KeyName	RegenerateTime
	KeyData	15.0

	KeyName	ImageType
	KeyData	$FXLAB_SCRIPT_SECTION_SPRITE

	KeyName	ImageID
	KeyData	$FXLAB_ID_SPRITE_MECHOS_DUST212

	KeyName	ImageNoise
	KeyData	0


	KeyName	Rate
	KeyData 0.5

	KeyName	Age
	KeyData $FXLAB_DUST_LIFE_TIME

	KeyName	StartPower
	KeyData 0.0

	KeyName	EndPower
	KeyData 1.0

	KeyName	PositionKeyID
	KeyData	$FXLAB_ID_KEY_MECHOS_DUST_POSITION

	KeyName	VelocityKeyID
	KeyData	$FXLAB_ID_KEY_MECHOS_DUST_VELOCITY
}



//-----------------------------------------------------------
//Forest
//2

KeyID	$FXLAB_ID_KEY_CORE_MECHOS_DUST220
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
	KeyData	0.3

	KeyName Blue
	KeyData	0.1

	KeyName Alpha
	KeyData 0.8

	KeyName	Size
	KeyData	4.0

	KeyName	Angle
	KeyData	0.0

	KeyName	Frame
	KeyData	0.0


	KeyTime	16.0

	KeyName	Red
	KeyData	0.1

	KeyName	Green
	KeyData	0.3

	KeyName Blue
	KeyData	0.1

	KeyName Alpha
	KeyData 0.0

	KeyName	Size
	KeyData	24.0

	KeyName	Angle
	KeyData	"3.14*8.0"

	KeyName	Frame
	KeyData	1.0
}

KeyID	$FXLAB_ID_KEY_MECHOS_DUST220
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

	KeyTime	5.0

	KeyName	RegenerateTime
	KeyData	15.0

	KeyName	ImageType
	KeyData	$FXLAB_SCRIPT_SECTION_SPRITE

	KeyName	ImageID
	KeyData	$FXLAB_ID_SPRITE_MECHOS_DUST220

	KeyName	ImageNoise
	KeyData	0


	KeyName	Rate
	KeyData 0.5

	KeyName	Age
	KeyData $FXLAB_DUST_LIFE_TIME

	KeyName	StartPower
	KeyData 0.0

	KeyName	EndPower
	KeyData 1.0

	KeyName	PositionKeyID
	KeyData	$FXLAB_ID_KEY_MECHOS_DUST_POSITION

	KeyName	VelocityKeyID
	KeyData	$FXLAB_ID_KEY_MECHOS_DUST_VELOCITY
}

//-----------------

KeyID	$FXLAB_ID_KEY_CORE_MECHOS_DUST222
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
	KeyData	0.32

	KeyName	Green
	KeyData	0.23

	KeyName Blue
	KeyData	0.0

	KeyName Alpha
	KeyData 0.8

	KeyName	Size
	KeyData	4.0

	KeyName	Angle
	KeyData	0.0

	KeyName	Frame
	KeyData	0.0


	KeyTime	16.0

	KeyName	Red
	KeyData	0.32

	KeyName	Green
	KeyData	0.23

	KeyName Blue
	KeyData	0.0

	KeyName Alpha
	KeyData 0.0

	KeyName	Size
	KeyData	24.0

	KeyName	Angle
	KeyData	"3.14*8.0"

	KeyName	Frame
	KeyData	1.0
}

KeyID	$FXLAB_ID_KEY_MECHOS_DUST222
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

	KeyTime	5.0

	KeyName	RegenerateTime
	KeyData	15.0

	KeyName	ImageType
	KeyData	$FXLAB_SCRIPT_SECTION_SPRITE

	KeyName	ImageID
	KeyData	$FXLAB_ID_SPRITE_MECHOS_DUST222

	KeyName	ImageNoise
	KeyData	0


	KeyName	Rate
	KeyData 0.5

	KeyName	Age
	KeyData $FXLAB_DUST_LIFE_TIME

	KeyName	StartPower
	KeyData 0.0

	KeyName	EndPower
	KeyData 1.0

	KeyName	PositionKeyID
	KeyData	$FXLAB_ID_KEY_MECHOS_DUST_POSITION

	KeyName	VelocityKeyID
	KeyData	$FXLAB_ID_KEY_MECHOS_DUST_VELOCITY
}

//-----------------

KeyID	$FXLAB_ID_KEY_CORE_MECHOS_DUST227
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
	KeyData	0.3

	KeyName	Green
	KeyData	0.3

	KeyName Blue
	KeyData	0.0

	KeyName Alpha
	KeyData 0.8

	KeyName	Size
	KeyData	4.0

	KeyName	Angle
	KeyData	0.0

	KeyName	Frame
	KeyData	0.0


	KeyTime	16.0

	KeyName	Red
	KeyData	0.3

	KeyName	Green
	KeyData	0.3

	KeyName Blue
	KeyData	0.0

	KeyName Alpha
	KeyData 0.0

	KeyName	Size
	KeyData	24.0

	KeyName	Angle
	KeyData	"3.14*8.0"

	KeyName	Frame
	KeyData	1.0
}

KeyID	$FXLAB_ID_KEY_MECHOS_DUST227
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

	KeyTime	5.0

	KeyName	RegenerateTime
	KeyData	15.0

	KeyName	ImageType
	KeyData	$FXLAB_SCRIPT_SECTION_SPRITE

	KeyName	ImageID
	KeyData	$FXLAB_ID_SPRITE_MECHOS_DUST227

	KeyName	ImageNoise
	KeyData	0


	KeyName	Rate
	KeyData 0.5

	KeyName	Age
	KeyData $FXLAB_DUST_LIFE_TIME

	KeyName	StartPower
	KeyData 0.0

	KeyName	EndPower
	KeyData 1.0

	KeyName	PositionKeyID
	KeyData	$FXLAB_ID_KEY_MECHOS_DUST_POSITION

	KeyName	VelocityKeyID
	KeyData	$FXLAB_ID_KEY_MECHOS_DUST_VELOCITY
}

//-----------------------------------------------------------
//Forest
//3

KeyID	$FXLAB_ID_KEY_CORE_MECHOS_DUST230
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
	KeyData	0.15

	KeyName	Green
	KeyData	0.4

	KeyName Blue
	KeyData	0.1

	KeyName Alpha
	KeyData 0.8

	KeyName	Size
	KeyData	4.0

	KeyName	Angle
	KeyData	0.0

	KeyName	Frame
	KeyData	0.0


	KeyTime	16.0

	KeyName	Red
	KeyData	0.15

	KeyName	Green
	KeyData	0.4

	KeyName Blue
	KeyData	0.1

	KeyName Alpha
	KeyData 0.0

	KeyName	Size
	KeyData	24.0

	KeyName	Angle
	KeyData	"3.14*8.0"

	KeyName	Frame
	KeyData	1.0
}

KeyID	$FXLAB_ID_KEY_MECHOS_DUST230
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

	KeyTime	5.0

	KeyName	RegenerateTime
	KeyData	15.0

	KeyName	ImageType
	KeyData	$FXLAB_SCRIPT_SECTION_SPRITE

	KeyName	ImageID
	KeyData	$FXLAB_ID_SPRITE_MECHOS_DUST230

	KeyName	ImageNoise
	KeyData	0


	KeyName	Rate
	KeyData 0.5

	KeyName	Age
	KeyData $FXLAB_DUST_LIFE_TIME

	KeyName	StartPower
	KeyData 0.0

	KeyName	EndPower
	KeyData 1.0

	KeyName	PositionKeyID
	KeyData	$FXLAB_ID_KEY_MECHOS_DUST_POSITION

	KeyName	VelocityKeyID
	KeyData	$FXLAB_ID_KEY_MECHOS_DUST_VELOCITY
}

//-----------------

KeyID	$FXLAB_ID_KEY_CORE_MECHOS_DUST232
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
	KeyData	0.42

	KeyName	Green
	KeyData	0.34

	KeyName Blue
	KeyData	0.1

	KeyName Alpha
	KeyData 0.8

	KeyName	Size
	KeyData	4.0

	KeyName	Angle
	KeyData	0.0

	KeyName	Frame
	KeyData	0.0


	KeyTime	16.0

	KeyName	Red
	KeyData	0.42

	KeyName	Green
	KeyData	0.34

	KeyName Blue
	KeyData	0.1

	KeyName Alpha
	KeyData 0.0

	KeyName	Size
	KeyData	24.0

	KeyName	Angle
	KeyData	"3.14*8.0"

	KeyName	Frame
	KeyData	1.0
}

KeyID	$FXLAB_ID_KEY_MECHOS_DUST232
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

	KeyTime	5.0

	KeyName	RegenerateTime
	KeyData	15.0

	KeyName	ImageType
	KeyData	$FXLAB_SCRIPT_SECTION_SPRITE

	KeyName	ImageID
	KeyData	$FXLAB_ID_SPRITE_MECHOS_DUST232

	KeyName	ImageNoise
	KeyData	0


	KeyName	Rate
	KeyData 0.5

	KeyName	Age
	KeyData $FXLAB_DUST_LIFE_TIME

	KeyName	StartPower
	KeyData 0.0

	KeyName	EndPower
	KeyData 1.0

	KeyName	PositionKeyID
	KeyData	$FXLAB_ID_KEY_MECHOS_DUST_POSITION

	KeyName	VelocityKeyID
	KeyData	$FXLAB_ID_KEY_MECHOS_DUST_VELOCITY
}


//-----------------

KeyID	$FXLAB_ID_KEY_CORE_MECHOS_DUST237
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
	KeyData	0.47

	KeyName Blue
	KeyData	0.1

	KeyName Alpha
	KeyData 0.8

	KeyName	Size
	KeyData	4.0

	KeyName	Angle
	KeyData	0.0

	KeyName	Frame
	KeyData	0.0


	KeyTime	16.0

	KeyName	Red
	KeyData	0.5

	KeyName	Green
	KeyData	0.47

	KeyName Blue
	KeyData	0.1

	KeyName Alpha
	KeyData 0.0

	KeyName	Size
	KeyData	24.0

	KeyName	Angle
	KeyData	"3.14*8.0"

	KeyName	Frame
	KeyData	1.0
}

KeyID	$FXLAB_ID_KEY_MECHOS_DUST237
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

	KeyTime	5.0

	KeyName	RegenerateTime
	KeyData	15.0

	KeyName	ImageType
	KeyData	$FXLAB_SCRIPT_SECTION_SPRITE

	KeyName	ImageID
	KeyData	$FXLAB_ID_SPRITE_MECHOS_DUST237

	KeyName	ImageNoise
	KeyData	0


	KeyName	Rate
	KeyData 0.5

	KeyName	Age
	KeyData $FXLAB_DUST_LIFE_TIME

	KeyName	StartPower
	KeyData 0.0

	KeyName	EndPower
	KeyData 1.0

	KeyName	PositionKeyID
	KeyData	$FXLAB_ID_KEY_MECHOS_DUST_POSITION

	KeyName	VelocityKeyID
	KeyData	$FXLAB_ID_KEY_MECHOS_DUST_VELOCITY
}

//-----------------------------------------------------------
//Forest
//4

KeyID	$FXLAB_ID_KEY_CORE_MECHOS_DUST240
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
	KeyData	0.9

	KeyName	Green
	KeyData	0.9

	KeyName Blue
	KeyData	0.9

	KeyName Alpha
	KeyData 0.8

	KeyName	Size
	KeyData	4.0

	KeyName	Angle
	KeyData	0.0

	KeyName	Frame
	KeyData	0.0


	KeyTime	16.0

	KeyName	Red
	KeyData	0.9

	KeyName	Green
	KeyData	0.9

	KeyName Blue
	KeyData	0.9

	KeyName Alpha
	KeyData 0.0

	KeyName	Size
	KeyData	24.0

	KeyName	Angle
	KeyData	"3.14*8.0"

	KeyName	Frame
	KeyData	1.0
}

KeyID	$FXLAB_ID_KEY_MECHOS_DUST240
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

	KeyTime	5.0

	KeyName	RegenerateTime
	KeyData	15.0

	KeyName	ImageType
	KeyData	$FXLAB_SCRIPT_SECTION_SPRITE

	KeyName	ImageID
	KeyData	$FXLAB_ID_SPRITE_MECHOS_DUST240

	KeyName	ImageNoise
	KeyData	0


	KeyName	Rate
	KeyData 0.5

	KeyName	Age
	KeyData $FXLAB_DUST_LIFE_TIME

	KeyName	StartPower
	KeyData 0.0

	KeyName	EndPower
	KeyData 1.0

	KeyName	PositionKeyID
	KeyData	$FXLAB_ID_KEY_MECHOS_DUST_POSITION

	KeyName	VelocityKeyID
	KeyData	$FXLAB_ID_KEY_MECHOS_DUST_VELOCITY
}

//-----------------

KeyID	$FXLAB_ID_KEY_CORE_MECHOS_DUST242
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
	KeyData 0.8

	KeyName	Size
	KeyData	4.0

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
	KeyData 0.0

	KeyName	Size
	KeyData	24.0

	KeyName	Angle
	KeyData	"3.14*8.0"

	KeyName	Frame
	KeyData	1.0
}

KeyID	$FXLAB_ID_KEY_MECHOS_DUST242
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

	KeyTime	5.0

	KeyName	RegenerateTime
	KeyData	15.0

	KeyName	ImageType
	KeyData	$FXLAB_SCRIPT_SECTION_SPRITE

	KeyName	ImageID
	KeyData	$FXLAB_ID_SPRITE_MECHOS_DUST242

	KeyName	ImageNoise
	KeyData	0


	KeyName	Rate
	KeyData 0.5

	KeyName	Age
	KeyData $FXLAB_DUST_LIFE_TIME

	KeyName	StartPower
	KeyData 0.0

	KeyName	EndPower
	KeyData 1.0

	KeyName	PositionKeyID
	KeyData	$FXLAB_ID_KEY_MECHOS_DUST_POSITION

	KeyName	VelocityKeyID
	KeyData	$FXLAB_ID_KEY_MECHOS_DUST_VELOCITY
}


