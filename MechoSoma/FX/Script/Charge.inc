KeyID	$FXLAB_ID_KEY_CORE_CHARGE_ENERGY
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
	KeyData 0.0

	KeyName	Size
	KeyData	6.0

	KeyName	Angle
	KeyData	0.0

	KeyName	Frame
	KeyData	0.0


	KeyTime	4.0

	KeyName	Alpha
	KeyData	1.0


	KeyTime	16.0

	KeyName	Red
	KeyData	1.0

	KeyName	Green
	KeyData	0.0

	KeyName Blue
	KeyData	0.0

	KeyName Alpha
	KeyData 0.0

	KeyName	Size
	KeyData	6.0

	KeyName	Angle
	KeyData	"3.14*8.0"

	KeyName	Frame
	KeyData	1.0
}


KeyID	$FXLAB_ID_KEY_CHARGE_ENERGY_POSITION
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
	KeyData	30.0

	KeyName	DataB1
	KeyData	20.0

	KeyName	DataC1
	KeyData	0

	KeyName	DataA2
	KeyData	0

	KeyName	DataB2
	KeyData	0

	KeyName	DataC2
	KeyData	0
}

KeyID	$FXLAB_ID_KEY_CHARGE_ENERGY_VELOCITY
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

KeyID	$FXLAB_ID_KEY_CHARGE_ENERGY
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

	KeyDefine	Power1
	KeyDefine	Epsilon1
	KeyDefine	Power2
	KeyDefine	Epsilon2

	KeyTime	0.0

	KeyName	RegenerateTime
	KeyData	15.0

	KeyName	ImageType
	KeyData	$FXLAB_SCRIPT_SECTION_SPRITE

	KeyName	ImageID
	KeyData	$FXLAB_ID_SPRITE_CHARGE_ENERGY

	KeyName	ImageNoise
	KeyData	0


	KeyName	Rate
	KeyData 2.5

	KeyName	Age
	KeyData 16.0

	KeyName	StartPower
	KeyData 0.5

	KeyName	EndPower
	KeyData 1.0

	KeyName	PositionKeyID
	KeyData	$FXLAB_ID_KEY_CHARGE_ENERGY_POSITION

	KeyName	VelocityKeyID
	KeyData	$FXLAB_ID_KEY_CHARGE_ENERGY_VELOCITY


	KeyName	Power1
	KeyData	50.0

	KeyName	Epsilon1
	KeyData	100.0

	KeyName	Power2
	KeyData	-60.0

	KeyName	Epsilon2
	KeyData	500.0
}


KeyID	$FXLAB_ID_KEY_CHARGE_ENERGY_IMPULSE_LINK
{
	KeyAttribute	"$FXLAB_KEY_ATTRIBUTE_NONE | $FXLAB_KEY_ATTRIBUTE_STORAGE"
	
	KeyDefine	TailType
	KeyDefine	TailID
	KeyDefine	ExplodeType
	KeyDefine	ExplodeID
	KeyDefine   SpeedBorder	

	KeyTime	20.0

	KeyName	TailType
	KeyData	$FXLAB_CLIENT_PROCESS_EXPLODE

	KeyName	TailID
	KeyData $FXLAB_ID_KEY_CHARGE_ENERGY

	KeyName	ExplodeType
	KeyData	-1.0

	KeyName	ExplodeID
	KeyData	-1.0

	KeyName SpeedBorder
	KeyData -1.0
}


KeyID	$FXLAB_ID_KEY_CHARGE_ENERGY_CYCLE_LINK
{
	KeyAttribute	"$FXLAB_KEY_ATTRIBUTE_REPEAT | $FXLAB_KEY_ATTRIBUTE_STORAGE"
	
	KeyDefine	TailType
	KeyDefine	TailID
	KeyDefine	ExplodeType
	KeyDefine	ExplodeID
	KeyDefine   SpeedBorder	

	KeyTime	0.0

	KeyName	TailType
	KeyData	$FXLAB_CLIENT_PROCESS_EXPLODE

	KeyName	TailID
	KeyData $FXLAB_ID_KEY_CHARGE_ENERGY

	KeyName	ExplodeType
	KeyData	-1.0

	KeyName	ExplodeID
	KeyData	-1.0

	KeyName SpeedBorder
	KeyData -1.0
}

//-----------------------------------------------------

KeyID	$FXLAB_ID_KEY_CORE_CHARGE_MANA
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
	KeyData 0.0

	KeyName	Size
	KeyData	6.0

	KeyName	Angle
	KeyData	0.0

	KeyName	Frame
	KeyData	0.0


	KeyTime	4.0

	KeyName	Alpha
	KeyData	1.0


	KeyTime	16.0

	KeyName	Red
	KeyData	0.0

	KeyName	Green
	KeyData	0.0

	KeyName Blue
	KeyData	1.0

	KeyName Alpha
	KeyData 0.0

	KeyName	Size
	KeyData	6.0

	KeyName	Angle
	KeyData	"3.14*8.0"

	KeyName	Frame
	KeyData	1.0
}

KeyID	$FXLAB_ID_KEY_CHARGE_MANA_POSITION
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
	KeyData	30.0

	KeyName	DataB1
	KeyData	20.0

	KeyName	DataC1
	KeyData	0

	KeyName	DataA2
	KeyData	0

	KeyName	DataB2
	KeyData	0

	KeyName	DataC2
	KeyData	0
}

KeyID	$FXLAB_ID_KEY_CHARGE_MANA_VELOCITY
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

KeyID	$FXLAB_ID_KEY_CHARGE_MANA
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

	KeyDefine	Power1
	KeyDefine	Epsilon1
	KeyDefine	Power2
	KeyDefine	Epsilon2

	KeyTime	0.0

	KeyName	RegenerateTime
	KeyData	15.0

	KeyName	ImageType
	KeyData	$FXLAB_SCRIPT_SECTION_SPRITE

	KeyName	ImageID
	KeyData	$FXLAB_ID_SPRITE_CHARGE_MANA

	KeyName	ImageNoise
	KeyData	0


	KeyName	Rate
	KeyData 2.5

	KeyName	Age
	KeyData 16.0

	KeyName	StartPower
	KeyData 0.5

	KeyName	EndPower
	KeyData 1.0

	KeyName	PositionKeyID
	KeyData	$FXLAB_ID_KEY_CHARGE_MANA_POSITION

	KeyName	VelocityKeyID
	KeyData	$FXLAB_ID_KEY_CHARGE_MANA_VELOCITY


	KeyName	Power1
	KeyData	50.0

	KeyName	Epsilon1
	KeyData	100.0

	KeyName	Power2
	KeyData	-60.0

	KeyName	Epsilon2
	KeyData	500.0
}

KeyID	$FXLAB_ID_KEY_CHARGE_MANA_IMPULSE_LINK
{
	KeyAttribute	"$FXLAB_KEY_ATTRIBUTE_NONE | $FXLAB_KEY_ATTRIBUTE_STORAGE"
	
	KeyDefine	TailType
	KeyDefine	TailID
	KeyDefine	ExplodeType
	KeyDefine	ExplodeID
	KeyDefine   SpeedBorder	

	KeyTime	20.0

	KeyName	TailType
	KeyData	$FXLAB_CLIENT_PROCESS_EXPLODE

	KeyName	TailID
	KeyData $FXLAB_ID_KEY_CHARGE_MANA

	KeyName	ExplodeType
	KeyData	-1.0

	KeyName	ExplodeID
	KeyData	-1.0

	KeyName SpeedBorder
	KeyData -1.0
}

KeyID	$FXLAB_ID_KEY_CHARGE_MANA_CYCLE_LINK
{
	KeyAttribute	"$FXLAB_KEY_ATTRIBUTE_REPEAT | $FXLAB_KEY_ATTRIBUTE_STORAGE"
	
	KeyDefine	TailType
	KeyDefine	TailID
	KeyDefine	ExplodeType
	KeyDefine	ExplodeID
	KeyDefine   SpeedBorder	

	KeyTime	20.0

	KeyName	TailType
	KeyData	$FXLAB_CLIENT_PROCESS_EXPLODE

	KeyName	TailID
	KeyData $FXLAB_ID_KEY_CHARGE_MANA

	KeyName	ExplodeType
	KeyData	-1.0

	KeyName	ExplodeID
	KeyData	-1.0

	KeyName SpeedBorder
	KeyData -1.0
}

//-------------------------------------

KeyID	$FXLAB_ID_KEY_CORE_PLACE_ENERGY_STAR
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
	KeyData	0.2

	KeyName Blue
	KeyData	0.0

	KeyName Alpha
	KeyData 0.0

	KeyName	Size
	KeyData	16.0

	KeyName	Angle
	KeyData	0.0

	KeyName	Frame
	KeyData	0.0


	KeyTime	8.0

	KeyName	Alpha
	KeyData	1.0


	KeyTime	16.0

	KeyName	Red
	KeyData	1.0

	KeyName	Green
	KeyData	0.2

	KeyName Blue
	KeyData	0.0

	KeyName Alpha
	KeyData 0.0

	KeyName	Size
	KeyData	8.0

	KeyName	Angle
	KeyData	"3.14*8.0"

	KeyName	Frame
	KeyData	1.0
}

KeyID	$FXLAB_ID_KEY_CORE_PLACE_ENERGY_FOG
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
	KeyData	0.3

	KeyName Alpha
	KeyData 0.0

	KeyName	Size
	KeyData	16.0

	KeyName	Angle
	KeyData	0.0

	KeyName	Frame
	KeyData	0.0


	KeyTime	8.0

	KeyName	Alpha
	KeyData	1.0


	KeyTime	16.0

	KeyName	Red
	KeyData	1.0

	KeyName	Green
	KeyData	0.3

	KeyName Blue
	KeyData	0.3

	KeyName Alpha
	KeyData 0.0

	KeyName	Size
	KeyData	64.0

	KeyName	Angle
	KeyData	"3.14*8.0"

	KeyName	Frame
	KeyData	1.0
}


KeyID	$FXLAB_ID_KEY_PLACE_ENERGY_POSITION
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

KeyID	$FXLAB_ID_KEY_PLACE_ENERGY_VELOCITY
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
	KeyData	0

	KeyName	DataC1
	KeyData	7.0

	KeyName	DataA2
	KeyData	0

	KeyName	DataB2
	KeyData	0

	KeyName	DataC2
	KeyData	0
}

KeyID	$FXLAB_ID_KEY_PLACE_ENERGY_FOG
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

	KeyTime	0.0

	KeyName	RegenerateTime
	KeyData	15.0

	KeyName	ImageType
	KeyData	$FXLAB_SCRIPT_SECTION_SPRITE

	KeyName	ImageID
	KeyData	$FXLAB_ID_SPRITE_PLACE_ENERGY_FOG

	KeyName	ImageNoise
	KeyData	1.0


	KeyName	Rate
	KeyData 0.5

	KeyName	Age
	KeyData 20.0

	KeyName	StartPower
	KeyData 0.0

	KeyName	EndPower
	KeyData 1.0

	KeyName	PositionKeyID
	KeyData	$FXLAB_ID_KEY_PLACE_ENERGY_POSITION

	KeyName	VelocityKeyID
	KeyData	$FXLAB_ID_KEY_PLACE_ENERGY_VELOCITY
}

KeyID	$FXLAB_ID_KEY_PLACE_ENERGY_STAR
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

	KeyTime	0.0

	KeyName	RegenerateTime
	KeyData	15.0

	KeyName	ImageType
	KeyData	$FXLAB_SCRIPT_SECTION_SPRITE

	KeyName	ImageID
	KeyData	$FXLAB_ID_SPRITE_PLACE_ENERGY_STAR

	KeyName	ImageNoise
	KeyData	1.0


	KeyName	Rate
	KeyData 1.0

	KeyName	Age
	KeyData 20.0

	KeyName	StartPower
	KeyData 0.0

	KeyName	EndPower
	KeyData 1.0

	KeyName	PositionKeyID
	KeyData	$FXLAB_ID_KEY_PLACE_ENERGY_POSITION

	KeyName	VelocityKeyID
	KeyData	$FXLAB_ID_KEY_PLACE_ENERGY_VELOCITY
}


//--------------------------------------------

KeyID $FXLAB_ID_KEY_PLACE_ENERGY_SENSOR
{
	KeyAttribute	"$FXLAB_KEY_ATTRIBUTE_STORAGE | $FXLAB_KEY_ATTRIBUTE_REPEAT"
	
	KeyDefine	Power
	KeyDefine	Radius
	KeyDefine	DamageType

	KeyTime	0.0

	KeyName	Power
	KeyData	1.0

	KeyName	Radius
	KeyData	50.0

	KeyName	DamageType
	KeyData	$FXLAB_DAMAGE_SPHERE_TYPE_CHARGE_ENERGY
}

KeyID $FXLAB_ID_KEY_PLACE_MANA_SENSOR
{
	KeyAttribute	"$FXLAB_KEY_ATTRIBUTE_STORAGE | $FXLAB_KEY_ATTRIBUTE_REPEAT"
	
	KeyDefine	Power
	KeyDefine	Radius
	KeyDefine	DamageType

	KeyTime	0.0

	KeyName	Power
	KeyData	0.1

	KeyName	Radius
	KeyData	150.0

	KeyName	DamageType
	KeyData	$FXLAB_DAMAGE_SPHERE_TYPE_CHARGE_MANA
}

//--------------------------------

KeyID	$FXLAB_ID_KEY_CORE_PLACE_MANA_STAR
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
	KeyData 0.0

	KeyName	Size
	KeyData	16.0

	KeyName	Angle
	KeyData	0.0

	KeyName	Frame
	KeyData	0.0


	KeyTime	8.0

	KeyName	Alpha
	KeyData	1.0


	KeyTime	16.0

	KeyName	Red
	KeyData	0.0

	KeyName	Green
	KeyData	0.0

	KeyName Blue
	KeyData	1.0

	KeyName Alpha
	KeyData 0.0

	KeyName	Size
	KeyData	8.0

	KeyName	Angle
	KeyData	"3.14*8.0"

	KeyName	Frame
	KeyData	1.0
}


KeyID	$FXLAB_ID_KEY_PLACE_MANA_POSITION
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
	KeyData	$FXLAB_SWARM_DOMAIN_CYLINDER

	KeyName	DataX
	KeyData	0.0

	KeyName	DataY
	KeyData	0.0

	KeyName	DataZ
	KeyData	50.0

	KeyName	DataA1
	KeyData	0.0

	KeyName	DataB1
	KeyData	0.0

	KeyName	DataC1
	KeyData	100.0

	KeyName	DataA2
	KeyData	130.0

	KeyName	DataB2
	KeyData	150.0

	KeyName	DataC2
	KeyData	0
}

KeyID	$FXLAB_ID_KEY_PLACE_MANA_VELOCITY
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
	KeyData	0

	KeyName	DataC1
	KeyData	-5.0

	KeyName	DataA2
	KeyData	0

	KeyName	DataB2
	KeyData	0

	KeyName	DataC2
	KeyData	0
}

KeyID	$FXLAB_ID_KEY_PLACE_MANA
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

	KeyTime	0.0

	KeyName	RegenerateTime
	KeyData	15.0

	KeyName	ImageType
	KeyData	$FXLAB_SCRIPT_SECTION_SPRITE

	KeyName	ImageID
	KeyData	$FXLAB_ID_SPRITE_PLACE_MANA_STAR

	KeyName	ImageNoise
	KeyData	1.0


	KeyName	Rate
	KeyData 1.0

	KeyName	Age
	KeyData 20.0

	KeyName	StartPower
	KeyData 0.0

	KeyName	EndPower
	KeyData 1.0

	KeyName	PositionKeyID
	KeyData	$FXLAB_ID_KEY_PLACE_MANA_POSITION

	KeyName	VelocityKeyID
	KeyData	$FXLAB_ID_KEY_PLACE_MANA_VELOCITY
}


//----------------------------------------


KeyID	$FXLAB_ID_KEY_CORE_DAMAGE_WARNING
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
	KeyData 0.2

	KeyName	Size
	KeyData	2.0

	KeyName	Angle
	KeyData	0.0

	KeyName	Frame
	KeyData	0.0


	KeyTime	4.0

	KeyName Alpha
	KeyData 1.0


	KeyTime	16.0

	KeyName	Red
	KeyData	1.0

	KeyName	Green
	KeyData	0.0

	KeyName Blue
	KeyData	0.0

	KeyName Alpha
	KeyData 0.0

	KeyName	Size
	KeyData	2.0

	KeyName	Angle
	KeyData	"3.14*8.0"

	KeyName	Frame
	KeyData	1.0
}


KeyID	$FXLAB_ID_KEY_DAMAGE_WARNING_POSITION
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
	KeyData	20.0

	KeyName	DataB1
	KeyData	20.0

	KeyName	DataC1
	KeyData	0.0

	KeyName	DataA2
	KeyData	0.0

	KeyName	DataB2
	KeyData	0.0

	KeyName	DataC2
	KeyData	0
}

KeyID	$FXLAB_ID_KEY_DAMAGE_WARNING_VELOCITY
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

KeyID	$FXLAB_ID_KEY_DAMAGE_WARNING
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

	KeyDefine	Power1
	KeyDefine	Epsilon1
	KeyDefine	Power2
	KeyDefine	Epsilon2

	KeyTime	0.0

	KeyName	RegenerateTime
	KeyData	15.0

	KeyName	ImageType
	KeyData	$FXLAB_SCRIPT_SECTION_SPRITE

	KeyName	ImageID
	KeyData	$FXLAB_ID_SPRITE_DAMAGE_WARNING

	KeyName	ImageNoise
	KeyData	1.0


	KeyName	Rate
	KeyData 2.5

	KeyName	Age
	KeyData 10.0

	KeyName	StartPower
	KeyData 0.0

	KeyName	EndPower
	KeyData 1.0

	KeyName	PositionKeyID
	KeyData	$FXLAB_ID_KEY_DAMAGE_WARNING_POSITION

	KeyName	VelocityKeyID
	KeyData	$FXLAB_ID_KEY_DAMAGE_WARNING_VELOCITY


	KeyName	Power1
	KeyData	-35.0

	KeyName	Epsilon1
	KeyData	100.0

	KeyName	Power2
	KeyData	50.0

	KeyName	Epsilon2
	KeyData	700.0
}

KeyID	$FXLAB_ID_KEY_DAMAGE_WARNING_LINK
{
	KeyAttribute	"$FXLAB_KEY_ATTRIBUTE_REPEAT | $FXLAB_KEY_ATTRIBUTE_STORAGE"
	
	KeyDefine	TailType
	KeyDefine	TailID
	KeyDefine	ExplodeType
	KeyDefine	ExplodeID
	KeyDefine   SpeedBorder	

	KeyTime	0.0

	KeyName	TailType
	KeyData	$FXLAB_CLIENT_PROCESS_EXPLODE

	KeyName	TailID
	KeyData $FXLAB_ID_KEY_DAMAGE_WARNING

	KeyName	ExplodeType
	KeyData	-1.0

	KeyName	ExplodeID
	KeyData	-1.0

	KeyName SpeedBorder
	KeyData -1.0
}

//-----------------------------------------

KeyID $FXLAB_ID_KEY_MECHOS_WAVE_MODEL
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
	KeyData	73.0

	KeyName	Red
	KeyData	0.7

	KeyName	Green
	KeyData	0.7

	KeyName	Blue
	KeyData	0.7

	KeyName	Alpha
	KeyData	0.7


	KeyName	Radius
	KeyData	10.0

	KeyName	OffsetAngleX
	KeyData	0.0

	KeyName	OffsetAngleY
	KeyData	0.0

	KeyName	OffsetAngleZ
	KeyData	0.0


	KeyTime 20.0

	KeyName	Alpha
	KeyData	0.5



	KeyTime 25.0

	KeyName	Model
	KeyData	73.0

	KeyName	Red
	KeyData	0.7

	KeyName	Green
	KeyData	0.7

	KeyName	Blue
	KeyData	0.7

	KeyName	Alpha
	KeyData	0.0


	KeyName	Radius
	KeyData	100.0

	KeyName	OffsetAngleX
	KeyData	0.0

	KeyName	OffsetAngleY
	KeyData	0.0

	KeyName	OffsetAngleZ
	KeyData	0.0
}


KeyID	$FXLAB_ID_KEY_DAMAGE_WARNING_IMPULSE
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

	KeyDefine	Power1
	KeyDefine	Epsilon1
	KeyDefine	Power2
	KeyDefine	Epsilon2

	KeyTime	1.0

	KeyName	RegenerateTime
	KeyData	15.0

	KeyName	ImageType
	KeyData	$FXLAB_SCRIPT_SECTION_SPRITE

	KeyName	ImageID
	KeyData	$FXLAB_ID_SPRITE_DAMAGE_WARNING

	KeyName	ImageNoise
	KeyData	1.0


	KeyName	Rate
	KeyData 10.0

	KeyName	Age
	KeyData 10.0

	KeyName	StartPower
	KeyData 0.0

	KeyName	EndPower
	KeyData 1.0

	KeyName	PositionKeyID
	KeyData	$FXLAB_ID_KEY_DAMAGE_WARNING_POSITION

	KeyName	VelocityKeyID
	KeyData	$FXLAB_ID_KEY_DAMAGE_WARNING_VELOCITY


	KeyName	Power1
	KeyData	-35.0

	KeyName	Epsilon1
	KeyData	100.0

	KeyName	Power2
	KeyData	50.0

	KeyName	Epsilon2
	KeyData	700.0
}

//-----------------------------------

KeyID	$FXLAB_ID_KEY_CORE_SNOW_LAYER
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
	KeyData 0.0

	KeyName	Size
	KeyData	4.0

	KeyName	Angle
	KeyData	0.0

	KeyName	Frame
	KeyData	0.0


	KeyTime	8.0

	KeyName Alpha
	KeyData 1.0


	KeyTime	16.0

	KeyName	Red
	KeyData	1.0

	KeyName	Green
	KeyData	1.0

	KeyName Blue
	KeyData	1.0

	KeyName Alpha
	KeyData 0.0

	KeyName	Size
	KeyData	8.0

	KeyName	Angle
	KeyData	"3.14*8.0"

	KeyName	Frame
	KeyData	1.0
}


KeyID	$FXLAB_ID_KEY_SNOW_LAYER_POSITION
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
	KeyData	$FXLAB_SWARM_DOMAIN_BOX

	KeyName	DataX
	KeyData	-100.0

	KeyName	DataY
	KeyData	-100.0

	KeyName	DataZ
	KeyData	0

	KeyName	DataA1
	KeyData	100.0

	KeyName	DataB1
	KeyData	100.0

	KeyName	DataC1
	KeyData	-200.0

	KeyName	DataA2
	KeyData	0

	KeyName	DataB2
	KeyData	0

	KeyName	DataC2
	KeyData	0
}

KeyID	$FXLAB_ID_KEY_SNOW_LAYER_VELOCITY
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
	KeyData	-0.5

	KeyName	DataA1
	KeyData	0.0

	KeyName	DataB1
	KeyData	0.0

	KeyName	DataC1
	KeyData	-20.0

	KeyName	DataA2
	KeyData	0

	KeyName	DataB2
	KeyData	0

	KeyName	DataC2
	KeyData	0
}


KeyID	$FXLAB_ID_KEY_SNOW_LAYER
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

	KeyTime	0.0

	KeyName	RegenerateTime
	KeyData	15.0

	KeyName	ImageType
	KeyData	$FXLAB_SCRIPT_SECTION_SPRITE

	KeyName	ImageID
	KeyData	$FXLAB_ID_SPRITE_SNOW_LAYER

	KeyName	ImageNoise
	KeyData	1.0


	KeyName	Rate
	KeyData 0.05

	KeyName	Age
	KeyData 25.0

	KeyName	StartPower
	KeyData 0.0

	KeyName	EndPower
	KeyData 1.0

	KeyName	PositionKeyID
	KeyData	$FXLAB_ID_KEY_SNOW_LAYER_POSITION

	KeyName	VelocityKeyID
	KeyData	$FXLAB_ID_KEY_SNOW_LAYER_VELOCITY
}


//-------------------------- Interface Charges--------------

KeyID	$FXLAB_ID_KEY_CORE_CHARACTER_BUBBLE
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
	KeyData	1.0

	KeyName Blue
	KeyData	0.2

	KeyName Alpha
	KeyData 0.0

	KeyName	Size
	KeyData	6.0

	KeyName	Angle
	KeyData	0.0

	KeyName	Frame
	KeyData	0.0


	KeyTime	5.0

	KeyName Alpha
	KeyData 1.0


	KeyTime	12.0

	KeyName Alpha
	KeyData 1.0


	KeyTime	16.0

	KeyName	Red
	KeyData	0.6

	KeyName	Green
	KeyData	1.0

	KeyName Blue
	KeyData	0.2

	KeyName Alpha
	KeyData 0.0

	KeyName	Size
	KeyData	12.0

	KeyName	Angle
	KeyData	0.0

	KeyName	Frame
	KeyData	1.0
}


KeyID	$FXLAB_ID_KEY_CHARACTER_BUBBLE_POSITION
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
	KeyData	0

	KeyName	DataA1
	KeyData	0.0

	KeyName	DataB1
	KeyData	50.0

	KeyName	DataC1
	KeyData	0

	KeyName	DataA2
	KeyData	0

	KeyName	DataB2
	KeyData	0

	KeyName	DataC2
	KeyData	0
}

KeyID	$FXLAB_ID_KEY_CHARACTER_BUBBLE_VELOCITY
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

KeyID	$FXLAB_ID_KEY_CHARACTER_BUBBLE
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

	KeyDefine	Damp

	KeyTime	0.0

	KeyName	RegenerateTime
	KeyData	15.0

	KeyName	ImageType
	KeyData	$FXLAB_SCRIPT_SECTION_SPRITE

	KeyName	ImageID
	KeyData	$FXLAB_ID_SPRITE_CHARACTER_BUBBLE

	KeyName	ImageNoise
	KeyData	0


	KeyName	Rate
	KeyData 0.17

	KeyName	Age
	KeyData 15.0

	KeyName	StartPower
	KeyData 0.0

	KeyName	EndPower
	KeyData 1.0

	KeyName	PositionKeyID
	KeyData	$FXLAB_ID_KEY_CHARACTER_BUBBLE_POSITION

	KeyName	VelocityKeyID
	KeyData	$FXLAB_ID_KEY_CHARACTER_BUBBLE_VELOCITY


	KeyName Damp
	KeyData	0.93
}


//--------------------------------------------------------

KeyID	$FXLAB_ID_KEY_CORE_BOSS_STEAM
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
	KeyData	"0.35*1.2"

	KeyName	Green
	KeyData	"0.5*1.2"

	KeyName Blue
	KeyData	"0.2*1.2"

	KeyName Alpha
	KeyData 0.2

	KeyName	Size
	KeyData	32.0

	KeyName	Angle
	KeyData	0.0

	KeyName	Frame
	KeyData	0.0


	KeyTime 8.0

	KeyName Alpha
	KeyData 0.5


	KeyTime	32.0

	KeyName	Red
	KeyData	"0.35*1.2"

	KeyName	Green
	KeyData	"0.5*1.2"

	KeyName Blue
	KeyData	"0.25*1.2"

	KeyName Alpha
	KeyData 0.2

	KeyName	Size
	KeyData	32.0

	KeyName	Angle
	KeyData	"3.14*8.0"

	KeyName	Frame
	KeyData	1.0
}

KeyID	$FXLAB_ID_KEY_BOSS_STEAM_TAIL_POSITION
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

KeyID	$FXLAB_ID_KEY_BOSS_STEAM_TAIL_VELOCITY
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

KeyID	$FXLAB_ID_KEY_BOSS_STEAM_TAIL
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
	KeyData	$FXLAB_ID_SPRITE_BOSS_STEAM

	KeyName	ImageNoise
	KeyData	0


	KeyName	Rate
	KeyData 1.0

	KeyName	Age
	KeyData 20.0

	KeyName	StartPower
	KeyData 0.0

	KeyName	EndPower
	KeyData 1.0

	KeyName	PositionKeyID
	KeyData $FXLAB_ID_KEY_BOSS_STEAM_TAIL_POSITION

	KeyName VelocityKeyID
	KeyData $FXLAB_ID_KEY_BOSS_STEAM_TAIL_VELOCITY


	KeyName	PointPower
	KeyData 100.0

	KeyName	PointEpsilon
	KeyData 100.0

	KeyName	PointRadius
	KeyData 200.0
}

//----------------------------------

KeyID	$FXLAB_ID_KEY_CORE_BOSS_SMOKE
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
	KeyData 0.2

	KeyName	Size
	KeyData	16.0

	KeyName	Angle
	KeyData	0.0

	KeyName	Frame
	KeyData	0.0


	KeyTime 8.0

	KeyName Alpha
	KeyData 1.0


	KeyTime	32.0

	KeyName	Red
	KeyData	0.0

	KeyName	Green
	KeyData	0.0

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

KeyID	$FXLAB_ID_KEY_BOSS_SMOKE_TAIL_POSITION
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
	KeyData 8.0

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

KeyID	$FXLAB_ID_KEY_BOSS_SMOKE_TAIL_VELOCITY
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

KeyID	$FXLAB_ID_KEY_BOSS_SMOKE_TAIL
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

	KeyDefine	Damp


	KeyTime	0.0

	KeyName	RegenerateTime
	KeyData	15.0


	KeyName	ImageType
	KeyData	$FXLAB_SCRIPT_SECTION_SPRITE

	KeyName	ImageID
	KeyData	$FXLAB_ID_SPRITE_BOSS_SMOKE

	KeyName	ImageNoise
	KeyData	0


	KeyName	Rate
	KeyData 1.5

	KeyName	Age
	KeyData 35.0

	KeyName	StartPower
	KeyData 0.0

	KeyName	EndPower
	KeyData 1.0

	KeyName	PositionKeyID
	KeyData $FXLAB_ID_KEY_BOSS_SMOKE_TAIL_POSITION

	KeyName VelocityKeyID
	KeyData $FXLAB_ID_KEY_BOSS_SMOKE_TAIL_VELOCITY


	KeyName	Damp
	KeyData 0.95
}

//---------------------------------

KeyID $FXLAB_ID_KEY_CHARACTER_WAVE_MODEL
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
	KeyData	73.0

	KeyName	Red
	KeyData	0.6

	KeyName	Green
	KeyData 1.0

	KeyName	Blue
	KeyData	0.0

	KeyName	Alpha
	KeyData	0.7


	KeyName	Radius
	KeyData	8.0

	KeyName	OffsetAngleX
	KeyData	0.0

	KeyName	OffsetAngleY
	KeyData	0.0

	KeyName	OffsetAngleZ
	KeyData	0.0


	KeyTime 20.0

	KeyName	Alpha
	KeyData	0.5


	KeyTime 25.0

	KeyName	Model
	KeyData	73.0

	KeyName	Red
	KeyData	0.6

	KeyName	Green
	KeyData	1.0

	KeyName	Blue
	KeyData	0.3

	KeyName	Alpha
	KeyData	0.0


	KeyName	Radius
	KeyData	50.0

	KeyName	OffsetAngleX
	KeyData	0.0

	KeyName	OffsetAngleY
	KeyData	0.0

	KeyName	OffsetAngleZ
	KeyData	0.0
}

//-------------------------------------------------

KeyID	$FXLAB_ID_KEY_CORE_WATERFALL_FOG
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
	KeyData	"0.35*0.7"

	KeyName	Green
	KeyData	"0.5*0.7"

	KeyName Blue
	KeyData	"0.2*0.7"

	KeyName Alpha
	KeyData 0.0

	KeyName	Size
	KeyData	16.0

	KeyName	Angle
	KeyData	0.0

	KeyName	Frame
	KeyData	0.0


	KeyTime 4.0

	KeyName Alpha
	KeyData 1.0


	KeyTime	32.0

	KeyName	Red
	KeyData	"0.35*0.7"

	KeyName	Green
	KeyData	"0.5*0.7"

	KeyName Blue
	KeyData	"0.25*0.7"

	KeyName Alpha
	KeyData 0.0

	KeyName	Size
	KeyData	32.0

	KeyName	Angle
	KeyData	"3.14*8.0"

	KeyName	Frame
	KeyData	1.0
}

KeyID	$FXLAB_ID_KEY_WATERFALL_FOG_POSITION
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
	KeyData	0

	KeyName	DataA1
	KeyData 50.0

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

KeyID	$FXLAB_ID_KEY_WATERFALL_FOG_VELOCITY
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

KeyID	$FXLAB_ID_KEY_WATERFALL_FOG
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

	KeyTime	0.0

	KeyName	RegenerateTime
	KeyData	15.0


	KeyName	ImageType
	KeyData	$FXLAB_SCRIPT_SECTION_SPRITE

	KeyName	ImageID
	KeyData	$FXLAB_ID_SPRITE_WATERFALL_FOG

	KeyName	ImageNoise
	KeyData	0


	KeyName	Rate
	KeyData 1.0

	KeyName	Age
	KeyData 25.0

	KeyName	StartPower
	KeyData 0.0

	KeyName	EndPower
	KeyData 1.0

	KeyName	PositionKeyID
	KeyData $FXLAB_ID_KEY_WATERFALL_FOG_POSITION

	KeyName VelocityKeyID
	KeyData $FXLAB_ID_KEY_WATERFALL_FOG_VELOCITY
}

//------------------------------------------------------

KeyID	$FXLAB_ID_KEY_CHARACTER_WORLD_BURST_POSITION
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
	KeyData	0.0

	KeyName	DataB1
	KeyData	50.0

	KeyName	DataC1
	KeyData	0

	KeyName	DataA2
	KeyData	0

	KeyName	DataB2
	KeyData	0

	KeyName	DataC2
	KeyData	0
}

KeyID	$FXLAB_ID_KEY_CHARACTER_WORLD_BURST_VELOCITY
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
	KeyData	15.0

	KeyName	DataB1
	KeyData	15.0

	KeyName	DataC1
	KeyData	0.0

	KeyName	DataA2
	KeyData	0.0

	KeyName	DataB2
	KeyData	0.0

	KeyName	DataC2
	KeyData	0.0
}

KeyID	$FXLAB_ID_KEY_CHARACTER_WORLD_BURST
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

	KeyTime	1.0

	KeyName	RegenerateTime
	KeyData	15.0

	KeyName	ImageType
	KeyData	$FXLAB_SCRIPT_SECTION_SPRITE

	KeyName	ImageID
	KeyData	$FXLAB_ID_SPRITE_CHARACTER_WORLD_BURST

	KeyName	ImageNoise
	KeyData	0


	KeyName	Rate
	KeyData 50.0

	KeyName	Age
	KeyData 8.0

	KeyName	StartPower
	KeyData 0.0

	KeyName	EndPower
	KeyData 1.0

	KeyName	PositionKeyID
	KeyData $FXLAB_ID_KEY_CHARACTER_WORLD_BURST_POSITION

	KeyName VelocityKeyID
	KeyData $FXLAB_ID_KEY_CHARACTER_WORLD_BURST_VELOCITY
}

KeyID	$FXLAB_ID_KEY_CORE_CHARACTER_WORLD_BURST
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
	KeyData	0.3

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

KeyID	$FXLAB_ID_KEY_CHARACTER_WORLD_OMNI
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
	KeyData	300.0


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

//---------------------------------


KeyID	$FXLAB_ID_KEY_CHARACTER_WORLD_EXPLOSION_POSITION
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
	KeyData	20.0

	KeyName	DataB1
	KeyData	50.0

	KeyName	DataC1
	KeyData	0

	KeyName	DataA2
	KeyData	0

	KeyName	DataB2
	KeyData	0

	KeyName	DataC2
	KeyData	0
}

KeyID	$FXLAB_ID_KEY_CHARACTER_WORLD_EXPLOSION_VELOCITY
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
	KeyData	1.0

	KeyName	DataA1
	KeyData	15.0

	KeyName	DataB1
	KeyData	30.0

	KeyName	DataC1
	KeyData	0.0

	KeyName	DataA2
	KeyData	0.0

	KeyName	DataB2
	KeyData	0.0

	KeyName	DataC2
	KeyData	0.0
}

KeyID	$FXLAB_ID_KEY_CHARACTER_WORLD_EXPLOSION
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

	KeyDefine	Damp

	KeyTime	1.0

	KeyName	RegenerateTime
	KeyData	15.0

	KeyName	ImageType
	KeyData	$FXLAB_SCRIPT_SECTION_SPRITE

	KeyName	ImageID
	KeyData	$FXLAB_ID_SPRITE_CHARACTER_WORLD_EXPLOSION

	KeyName	ImageNoise
	KeyData	0


	KeyName	Rate
	KeyData 70.0

	KeyName	Age
	KeyData 20.0

	KeyName	StartPower
	KeyData 0.0

	KeyName	EndPower
	KeyData 1.0

	KeyName	PositionKeyID
	KeyData $FXLAB_ID_KEY_CHARACTER_WORLD_EXPLOSION_POSITION

	KeyName VelocityKeyID
	KeyData $FXLAB_ID_KEY_CHARACTER_WORLD_EXPLOSION_VELOCITY

	KeyName Damp
	KeyData	0.92
}

KeyID	$FXLAB_ID_KEY_CORE_CHARACTER_WORLD_EXPLOSION
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
	KeyData 1.0

	KeyName	Size
	KeyData	24.0

	KeyName	Angle
	KeyData	0.0

	KeyName	Frame
	KeyData	0.0


	KeyTime	8.0

	KeyName	Red
	KeyData	1.0

	KeyName	Green
	KeyData	1.0

	KeyName Blue
	KeyData	0.0

	KeyName Alpha
	KeyData 1.0


	KeyTime	16.0

	KeyName	Red
	KeyData	1.0

	KeyName	Green
	KeyData	1.0

	KeyName Blue
	KeyData	1.0

	KeyName Alpha
	KeyData 0.0

	KeyName	Size
	KeyData	16.0

	KeyName	Angle
	KeyData	0.0

	KeyName	Frame
	KeyData	1.0
}
