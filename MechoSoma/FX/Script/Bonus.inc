#define FXLAB_BONUS_PARTICLE_SIZE 1.0
#define FXLAB_BONUS_PARTICLE_RATE 3.0

KeyID	$FXLAB_ID_KEY_TAKE_BONUS_POSITION
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

KeyID	$FXLAB_ID_KEY_TAKE_BONUS_VELOCITY
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

//-------------------------------------------------

KeyID $FXLAB_ID_KEY_BONUS_MODEL_JUMP
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
	KeyData	-1.0

	KeyName	Red
	KeyData	1.0

	KeyName	Green
	KeyData	1.0

	KeyName	Blue
	KeyData	1.0

	KeyName	Alpha
	KeyData	1.0


	KeyName	Radius
	KeyData	20.0

	KeyName	OffsetAngleX
	KeyData	"3.14*0.05"

	KeyName	OffsetAngleY
	KeyData	"3.14*0.05"

	KeyName	OffsetAngleZ
	KeyData	"3.14*0.05"
}


KeyID	$FXLAB_ID_KEY_TAKE_BONUS_JUMP
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
	KeyData	$FXLAB_ID_SPRITE_TAKE_BONUS_JUMP

	KeyName	ImageNoise
	KeyData	0


	KeyName	Rate
	KeyData $FXLAB_BONUS_PARTICLE_RATE

	KeyName	Age
	KeyData 15.0

	KeyName	StartPower
	KeyData 0.0

	KeyName	EndPower
	KeyData 1.0

	KeyName	PositionKeyID
	KeyData $FXLAB_ID_KEY_TAKE_BONUS_POSITION

	KeyName VelocityKeyID
	KeyData $FXLAB_ID_KEY_TAKE_BONUS_VELOCITY
}

KeyID	$FXLAB_ID_KEY_CORE_TAKE_BONUS_JUMP
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
	KeyData	0.0

	KeyName Blue
	KeyData	1.0

	KeyName Alpha
	KeyData 0.8

	KeyName	Size
	KeyData	$FXLAB_BONUS_PARTICLE_SIZE

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
	KeyData 0.1

	KeyName	Size
	KeyData	$FXLAB_BONUS_PARTICLE_SIZE

	KeyName	Angle
	KeyData	"3.14*2.0"

	KeyName	Frame
	KeyData	1.0
}

//--------------------------------------------------


KeyID $FXLAB_ID_KEY_BONUS_MODEL_NITRO
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
	KeyData	-1.0

	KeyName	Red
	KeyData	1.0

	KeyName	Green
	KeyData	1.0

	KeyName	Blue
	KeyData	1.0

	KeyName	Alpha
	KeyData	1.0


	KeyName	Radius
	KeyData	20.0

	KeyName	OffsetAngleX
	KeyData	"3.14*0.05"

	KeyName	OffsetAngleY
	KeyData	"3.14*0.05"

	KeyName	OffsetAngleZ
	KeyData	"3.14*0.05"
}

KeyID	$FXLAB_ID_KEY_TAKE_BONUS_NITRO
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
	KeyData	$FXLAB_ID_SPRITE_TAKE_BONUS_NITRO

	KeyName	ImageNoise
	KeyData	0


	KeyName	Rate
	KeyData $FXLAB_BONUS_PARTICLE_RATE

	KeyName	Age
	KeyData 15.0

	KeyName	StartPower
	KeyData 0.0

	KeyName	EndPower
	KeyData 1.0

	KeyName	PositionKeyID
	KeyData $FXLAB_ID_KEY_TAKE_BONUS_POSITION

	KeyName VelocityKeyID
	KeyData $FXLAB_ID_KEY_TAKE_BONUS_VELOCITY
}

KeyID	$FXLAB_ID_KEY_CORE_TAKE_BONUS_NITRO
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
	KeyData	0.0

	KeyName Blue
	KeyData	0.0

	KeyName Alpha
	KeyData 0.8

	KeyName	Size
	KeyData	$FXLAB_BONUS_PARTICLE_SIZE

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
	KeyData 0.1

	KeyName	Size
	KeyData	$FXLAB_BONUS_PARTICLE_SIZE

	KeyName	Angle
	KeyData	"3.14*2.0"

	KeyName	Frame
	KeyData	1.0
}

//---------------------------------------------------

KeyID $FXLAB_ID_KEY_BONUS_MODEL_CHARGE
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
	KeyData	-1.0

	KeyName	Red
	KeyData	1.0

	KeyName	Green
	KeyData	1.0

	KeyName	Blue
	KeyData	1.0

	KeyName	Alpha
	KeyData	1.0


	KeyName	Radius
	KeyData	20.0

	KeyName	OffsetAngleX
	KeyData	"3.14*0.05"

	KeyName	OffsetAngleY
	KeyData	"3.14*0.05"

	KeyName	OffsetAngleZ
	KeyData	"3.14*0.05"
}

KeyID	$FXLAB_ID_KEY_TAKE_BONUS_CHARGE
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
	KeyData	$FXLAB_ID_SPRITE_TAKE_BONUS_CHARGE

	KeyName	ImageNoise
	KeyData	0


	KeyName	Rate
	KeyData $FXLAB_BONUS_PARTICLE_RATE

	KeyName	Age
	KeyData 15.0

	KeyName	StartPower
	KeyData 0.0

	KeyName	EndPower
	KeyData 1.0

	KeyName	PositionKeyID
	KeyData $FXLAB_ID_KEY_TAKE_BONUS_POSITION

	KeyName VelocityKeyID
	KeyData $FXLAB_ID_KEY_TAKE_BONUS_VELOCITY
}

KeyID	$FXLAB_ID_KEY_CORE_TAKE_BONUS_CHARGE
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
	KeyData	0.0

	KeyName Blue
	KeyData	0.0

	KeyName Alpha
	KeyData 0.8

	KeyName	Size
	KeyData	$FXLAB_BONUS_PARTICLE_SIZE

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
	KeyData 0.1

	KeyName	Size
	KeyData	$FXLAB_BONUS_PARTICLE_SIZE

	KeyName	Angle
	KeyData	"3.14*2.0"

	KeyName	Frame
	KeyData	1.0
}

//---------------------------------------------------

KeyID $FXLAB_ID_KEY_BONUS_MODEL_MANA
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
	KeyData	-1.0

	KeyName	Red
	KeyData	1.0

	KeyName	Green
	KeyData	1.0

	KeyName	Blue
	KeyData	1.0

	KeyName	Alpha
	KeyData	1.0


	KeyName	Radius
	KeyData	20.0

	KeyName	OffsetAngleX
	KeyData	"3.14*0.05"

	KeyName	OffsetAngleY
	KeyData	"3.14*0.05"

	KeyName	OffsetAngleZ
	KeyData	"3.14*0.05"
}

KeyID	$FXLAB_ID_KEY_TAKE_BONUS_MANA
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
	KeyData	$FXLAB_ID_SPRITE_TAKE_BONUS_MANA

	KeyName	ImageNoise
	KeyData	0


	KeyName	Rate
	KeyData $FXLAB_BONUS_PARTICLE_RATE

	KeyName	Age
	KeyData 15.0

	KeyName	StartPower
	KeyData 0.0

	KeyName	EndPower
	KeyData 1.0

	KeyName	PositionKeyID
	KeyData $FXLAB_ID_KEY_TAKE_BONUS_POSITION

	KeyName VelocityKeyID
	KeyData $FXLAB_ID_KEY_TAKE_BONUS_VELOCITY
}

KeyID	$FXLAB_ID_KEY_CORE_TAKE_BONUS_MANA
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
	KeyData	0.0

	KeyName Blue
	KeyData	1.0

	KeyName Alpha
	KeyData 0.8

	KeyName	Size
	KeyData	$FXLAB_BONUS_PARTICLE_SIZE

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
	KeyData 0.1

	KeyName	Size
	KeyData	$FXLAB_BONUS_PARTICLE_SIZE

	KeyName	Angle
	KeyData	"3.14*2.0"

	KeyName	Frame
	KeyData	1.0
}

//---------------------------------

KeyID $FXLAB_ID_KEY_BONUS_MODEL_STARS
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
	KeyData	-1.0

	KeyName	Red
	KeyData	0.7

	KeyName	Green
	KeyData	0.3

	KeyName	Blue
	KeyData	0.0

	KeyName	Alpha
	KeyData	1.0


	KeyName	Radius
	KeyData	40.0

	KeyName	OffsetAngleX
	KeyData	"3.14*0.05"

	KeyName	OffsetAngleY
	KeyData	"3.14*0.05"

	KeyName	OffsetAngleZ
	KeyData	"3.14*0.05"
}

KeyID	$FXLAB_ID_KEY_TAKE_BONUS_STARS
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
	KeyData	$FXLAB_ID_SPRITE_TAKE_BONUS_STARS

	KeyName	ImageNoise
	KeyData	0


	KeyName	Rate
	KeyData $FXLAB_BONUS_PARTICLE_RATE

	KeyName	Age
	KeyData 15.0

	KeyName	StartPower
	KeyData 0.0

	KeyName	EndPower
	KeyData 1.0

	KeyName	PositionKeyID
	KeyData $FXLAB_ID_KEY_TAKE_BONUS_POSITION

	KeyName VelocityKeyID
	KeyData $FXLAB_ID_KEY_TAKE_BONUS_VELOCITY
}

KeyID	$FXLAB_ID_KEY_CORE_TAKE_BONUS_STARS
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
	KeyData	0.3

	KeyName Blue
	KeyData	0.0

	KeyName Alpha
	KeyData 0.8

	KeyName	Size
	KeyData	$FXLAB_BONUS_PARTICLE_SIZE

	KeyName	Angle
	KeyData	0.0

	KeyName	Frame
	KeyData	0.0


	KeyTime	32.0

	KeyName	Red
	KeyData	0.7

	KeyName	Green
	KeyData	0.3

	KeyName Blue
	KeyData	0.0

	KeyName Alpha
	KeyData 0.1

	KeyName	Size
	KeyData	$FXLAB_BONUS_PARTICLE_SIZE

	KeyName	Angle
	KeyData	"3.14*2.0"

	KeyName	Frame
	KeyData	1.0
}

//----------------

KeyID $FXLAB_ID_KEY_BONUS_MODEL_PART
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
	KeyData	-1.0

	KeyName	Red
	KeyData	1.0

	KeyName	Green
	KeyData	1.0

	KeyName	Blue
	KeyData	1.0

	KeyName	Alpha
	KeyData	1.0


	KeyName	Radius
	KeyData	20.0

	KeyName	OffsetAngleX
	KeyData	"3.14*0.05"

	KeyName	OffsetAngleY
	KeyData	"3.14*0.05"

	KeyName	OffsetAngleZ
	KeyData	"3.14*0.05"
}

//----------------------------------
//Protection


KeyID	$FXLAB_ID_KEY_TAKE_BONUS_PROTECTION_FIRE
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
	KeyData	$FXLAB_ID_SPRITE_TAKE_BONUS_PROTECTION_FIRE

	KeyName	ImageNoise
	KeyData	0


	KeyName	Rate
	KeyData $FXLAB_BONUS_PARTICLE_RATE

	KeyName	Age
	KeyData 15.0

	KeyName	StartPower
	KeyData 0.0

	KeyName	EndPower
	KeyData 1.0

	KeyName	PositionKeyID
	KeyData $FXLAB_ID_KEY_TAKE_BONUS_POSITION

	KeyName VelocityKeyID
	KeyData $FXLAB_ID_KEY_TAKE_BONUS_VELOCITY
}

KeyID	$FXLAB_ID_KEY_CORE_TAKE_BONUS_PROTECTION_FIRE
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
	KeyData	0.3

	KeyName Blue
	KeyData	0.0

	KeyName Alpha
	KeyData 0.8

	KeyName	Size
	KeyData	$FXLAB_BONUS_PARTICLE_SIZE

	KeyName	Angle
	KeyData	0.0

	KeyName	Frame
	KeyData	0.0


	KeyTime	32.0

	KeyName	Red
	KeyData	1.0

	KeyName	Green
	KeyData	0.3

	KeyName Blue
	KeyData	0.0

	KeyName Alpha
	KeyData 0.1

	KeyName	Size
	KeyData	$FXLAB_BONUS_PARTICLE_SIZE

	KeyName	Angle
	KeyData	"3.14*2.0"

	KeyName	Frame
	KeyData	1.0
}

//-----------------------------------------------

KeyID	$FXLAB_ID_KEY_TAKE_BONUS_PROTECTION_ELECTRIC
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
	KeyData	$FXLAB_ID_SPRITE_TAKE_BONUS_PROTECTION_ELECTRIC

	KeyName	ImageNoise
	KeyData	0


	KeyName	Rate
	KeyData $FXLAB_BONUS_PARTICLE_RATE

	KeyName	Age
	KeyData 15.0

	KeyName	StartPower
	KeyData 0.0

	KeyName	EndPower
	KeyData 1.0

	KeyName	PositionKeyID
	KeyData $FXLAB_ID_KEY_TAKE_BONUS_POSITION

	KeyName VelocityKeyID
	KeyData $FXLAB_ID_KEY_TAKE_BONUS_VELOCITY
}

KeyID	$FXLAB_ID_KEY_CORE_TAKE_BONUS_PROTECTION_ELECTRIC
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
	KeyData	0.0

	KeyName Blue
	KeyData	1.0

	KeyName Alpha
	KeyData 0.8

	KeyName	Size
	KeyData	$FXLAB_BONUS_PARTICLE_SIZE

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
	KeyData 0.1

	KeyName	Size
	KeyData	$FXLAB_BONUS_PARTICLE_SIZE

	KeyName	Angle
	KeyData	"3.14*2.0"

	KeyName	Frame
	KeyData	1.0
}

//-----------------------------------

KeyID	$FXLAB_ID_KEY_TAKE_BONUS_PROTECTION_ARMOR
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
	KeyData	$FXLAB_ID_SPRITE_TAKE_BONUS_PROTECTION_ARMOR

	KeyName	ImageNoise
	KeyData	0


	KeyName	Rate
	KeyData $FXLAB_BONUS_PARTICLE_RATE

	KeyName	Age
	KeyData 15.0

	KeyName	StartPower
	KeyData 0.0

	KeyName	EndPower
	KeyData 1.0

	KeyName	PositionKeyID
	KeyData $FXLAB_ID_KEY_TAKE_BONUS_POSITION

	KeyName VelocityKeyID
	KeyData $FXLAB_ID_KEY_TAKE_BONUS_VELOCITY
}

KeyID	$FXLAB_ID_KEY_CORE_TAKE_BONUS_PROTECTION_ARMOR
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
	KeyData 0.8

	KeyName	Size
	KeyData	$FXLAB_BONUS_PARTICLE_SIZE

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
	KeyData	0.5

	KeyName Alpha
	KeyData 0.1

	KeyName	Size
	KeyData	$FXLAB_BONUS_PARTICLE_SIZE

	KeyName	Angle
	KeyData	"3.14*2.0"

	KeyName	Frame
	KeyData	1.0
}


//-----------------------------------

KeyID	$FXLAB_ID_KEY_TAKE_BONUS_PROTECTION_BIO
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
	KeyData	$FXLAB_ID_SPRITE_TAKE_BONUS_PROTECTION_BIO

	KeyName	ImageNoise
	KeyData	0


	KeyName	Rate
	KeyData $FXLAB_BONUS_PARTICLE_RATE

	KeyName	Age
	KeyData 15.0

	KeyName	StartPower
	KeyData 0.0

	KeyName	EndPower
	KeyData 1.0

	KeyName	PositionKeyID
	KeyData $FXLAB_ID_KEY_TAKE_BONUS_POSITION

	KeyName VelocityKeyID
	KeyData $FXLAB_ID_KEY_TAKE_BONUS_VELOCITY
}

KeyID	$FXLAB_ID_KEY_CORE_TAKE_BONUS_PROTECTION_BIO
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
	KeyData 0.8

	KeyName	Size
	KeyData	$FXLAB_BONUS_PARTICLE_SIZE

	KeyName	Angle
	KeyData	0.0

	KeyName	Frame
	KeyData	0.0


	KeyTime	32.0

	KeyName	Red
	KeyData	0.0

	KeyName	Green
	KeyData	1.0

	KeyName Blue
	KeyData	0.0

	KeyName Alpha
	KeyData 0.1

	KeyName	Size
	KeyData	$FXLAB_BONUS_PARTICLE_SIZE

	KeyName	Angle
	KeyData	"3.14*2.0"

	KeyName	Frame
	KeyData	1.0
}

//--------------------------------------

KeyID	$FXLAB_ID_KEY_CORE_BONUS_FIRE
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
	KeyData	16.0

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
	KeyData	24.0

	KeyName	Angle
	KeyData	"3.14*8.0"

	KeyName	Frame
	KeyData	1.0
}


KeyID	$FXLAB_ID_KEY_BONUS_FIRE_POSITION
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
	KeyData	0.0

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

KeyID	$FXLAB_ID_KEY_BONUS_FIRE_VELOCITY
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
	KeyData	4.0

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

KeyID	$FXLAB_ID_KEY_BONUS_FIRE
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

	KeyDefine	Damp


	KeyTime	0.0

	KeyName	RegenerateTime
	KeyData	15.0


	KeyName	ImageType
	KeyData	$FXLAB_SCRIPT_SECTION_SPRITE

	KeyName	ImageID
	KeyData	$FXLAB_ID_SPRITE_BONUS_FIRE

	KeyName	ImageNoise
	KeyData	0


	KeyName		Rate
	KeyData		1.0

	KeyName		Age
	KeyData		15.0

	KeyName		StartPower
	KeyData		0.5

	KeyName		EndPower
	KeyData		1.0

	KeyName		PositionKeyID
	KeyData		$FXLAB_ID_KEY_BONUS_FIRE_POSITION

	KeyName		VelocityKeyID
	KeyData		$FXLAB_ID_KEY_BONUS_FIRE_VELOCITY


	KeyName		Damp
	KeyData		0.95


	KeyTime	20.0

	KeyName	RegenerateTime
	KeyData	15.0

	KeyName	ImageType
	KeyData	$FXLAB_SCRIPT_SECTION_SPRITE

	KeyName	ImageID
	KeyData	$FXLAB_ID_SPRITE_BONUS_FIRE
 
	KeyName	ImageNoise
	KeyData	0


	KeyName		Rate
	KeyData		1.0

	KeyName		Age
	KeyData		15.0

	KeyName		StartPower
	KeyData		0.0

	KeyName		EndPower
	KeyData		1.0

	KeyName		PositionKeyID
	KeyData		$FXLAB_ID_KEY_BONUS_FIRE_POSITION

	KeyName		VelocityKeyID
	KeyData		$FXLAB_ID_KEY_BONUS_FIRE_VELOCITY

	KeyName		Damp
	KeyData		0.95
}

KeyID $FXLAB_ID_KEY_BONUS_FIRE_OMNI
{
	KeyAttribute	$FXLAB_KEY_ATTRIBUTE_NONE

	KeyDefine	Red
	KeyDefine	Green
	KeyDefine	Blue
	KeyDefine	Radius

	KeyTime	0.0

	KeyName	Red
	KeyData	1.5

	KeyName Green
	KeyData	0.5

	KeyName Blue
	KeyData 0.3

	KeyName Radius
	KeyData	30.0


	KeyTime	6.0

	KeyName	Red
	KeyData	0.1

	KeyName Green
	KeyData	0.1

	KeyName Blue
	KeyData 0.1

	KeyName Radius
	KeyData	15.0
}


