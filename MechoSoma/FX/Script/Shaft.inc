KeyID	$FXLAB_ID_KEY_SHAFT_MACHINE_DAMAGE
{
	KeyAttribute	"$FXLAB_KEY_ATTRIBUTE_REPEAT | $FXLAB_KEY_ATTRIBUTE_STORAGE"

	KeyDefine	Power
	KeyDefine	Radius
	KeyDefine	Type
	KeyDefine	Length

	KeyTime	0.0

	KeyName	Power
	KeyData	2.0

	KeyName	Radius
	KeyData	5.0

	KeyName	Type
	KeyData	$FXLAB_DAMAGE_SPHERE_TYPE_LIGHTNING

	KeyName	Length
	KeyData	250.0
}

KeyID	$FXLAB_ID_KEY_SHAFT_MACHINE_LIGHTNING
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
	KeyData 0.05

	KeyName xP
	KeyData 0.8

	KeyName yP
	KeyData 0.8

	KeyName xLevel
	KeyData 3.0

	KeyName yLevel
	KeyData 3.0


	KeyName Radius
	KeyData 200.0


	KeyName Red
	KeyData 0.4

	KeyName	Green
	KeyData 0.4

	KeyName Blue
	KeyData 1.0


	KeyName Num
	KeyData 2.0

	KeyName OmniID
	KeyData $FXLAB_ID_KEY_SHAFT_MACHINE_OMNI
}


KeyID	$FXLAB_ID_KEY_SHAFT_MACHINE_OMNI
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
	KeyData	20.0


	KeyTime	25.0

	KeyName	Red
	KeyData	0.5

	KeyName Green
	KeyData	0.5

	KeyName Blue
	KeyData 0.6

	KeyName Radius
	KeyData	20.0


	KeyTime	50.0

	KeyName	Red
	KeyData	0.3

	KeyName Green
	KeyData	0.3

	KeyName Blue
	KeyData 0.6

	KeyName Radius
	KeyData	20.0
}


KeyID $FXLAB_ID_KEY_SHAFT_MACHINE_LINK
{
	KeyAttribute	"$FXLAB_KEY_ATTRIBUTE_REPEAT | $FXLAB_KEY_ATTRIBUTE_STORAGE"
	
	KeyDefine	TailType
	KeyDefine	TailID
	KeyDefine	ExplodeType
	KeyDefine	ExplodeID
	KeyDefine   SpeedBorder

	KeyTime	0.0

	KeyName	TailType
	KeyData	$FXLAB_CLIENT_PROCESS_LIGHTNING_LINE

	KeyName	TailID
	KeyData $FXLAB_ID_KEY_SHAFT_MACHINE_LIGHTNING

	KeyName	ExplodeType
	KeyData	-1.0

	KeyName	ExplodeID
	KeyData	-1.0

	KeyName SpeedBorder
	KeyData -1.0
}
