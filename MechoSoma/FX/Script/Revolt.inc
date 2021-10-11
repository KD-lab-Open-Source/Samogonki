KeyID $FXLAB_ID_KEY_REVOLT_SPACE_MODEL
{
	KeyAttribute	$FXLAB_KEY_ATTRIBUTE_FREEZE

	KeyDefine	Model
	KeyDefine	Red
	KeyDefine	Green
	KeyDefine	Blue
	KeyDefine	Alpha

	KeyDefine	Radius
	KeyDefine   OffsetAngleX
	KeyDefine   OffsetAngleY
	KeyDefine   OffsetAngleZ

	KeyDefine	DeltaPhase
	KeyDefine	DeltaRadius

	KeyTime	0.0

	KeyName	Model
	KeyData	72.0

	KeyName	Red
	KeyData	1.0

	KeyName	Green
	KeyData	0.0

	KeyName	Blue
	KeyData	0.0

	KeyName	Alpha
	KeyData	0.0


	KeyName	Radius
	KeyData	2.5

	KeyName	OffsetAngleX
	KeyData	0.0

	KeyName	OffsetAngleY
	KeyData	0.0

	KeyName	OffsetAngleZ
	KeyData	"3.14 * 0.05"


	KeyName	DeltaPhase
	KeyData "3.14 * 0.05"

	KeyName DeltaRadius
	KeyData	0.1


	KeyTime	50.0

	KeyName	Model
	KeyData	72.0

	KeyName	Red
	KeyData	0.0

	KeyName	Green
	KeyData	0.0

	KeyName	Blue
	KeyData	1.0

	KeyName	Alpha
	KeyData	0.5


	KeyName	Radius
	KeyData	2.5

	KeyName	OffsetAngleX
	KeyData	0.0

	KeyName	OffsetAngleY
	KeyData	0.0

	KeyName	OffsetAngleZ
	KeyData	"3.14 * 0.001"


	KeyName	DeltaPhase
	KeyData "3.14 * 0.05"

	KeyName DeltaRadius
	KeyData	0.1
}

KeyID	$FXLAB_ID_KEY_REVOLT_SPACE_LINK
{
	KeyAttribute	"$FXLAB_KEY_ATTRIBUTE_REPEAT | $FXLAB_KEY_ATTRIBUTE_STORAGE"
	
	KeyDefine	TailType
	KeyDefine	TailID
	KeyDefine	ExplodeType
	KeyDefine	ExplodeID
	KeyDefine   SpeedBorder

	KeyTime	0.0

	KeyName	TailType
	KeyData	$FXLAB_CLIENT_PROCESS_REVOLT_MODEL

	KeyName	TailID
	KeyData $FXLAB_ID_KEY_REVOLT_SPACE_MODEL

	KeyName	ExplodeType
	KeyData	-1.0

	KeyName	ExplodeID
	KeyData	-1.0

	KeyName SpeedBorder
	KeyData -1.0
}
