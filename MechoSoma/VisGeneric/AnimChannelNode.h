#ifndef __ANIMCHANNELNODE_H__
#define __ANIMCHANNELNODE_H__

struct sKey3f
{
	Vect3f		v;
	float		time;
};
struct sKey4f
{
	QuatF		q;
	float		time;
};
struct sKeyVisible
{
	int			visible;
	float		time;
};

enum eAttributeAnimChannel
{
	ATTRCHANNEL_NULL						=	0,
	ATTRCHANNEL_VISIBLE						=	1<<1,	// цепочки могут содержать анимацию видимости
	ATTRCHANNEL_POSITION					=	1<<2,	// цепочки могут содержать анимацию позиции
	ATTRCHANNEL_ROTATION					=	1<<3,	// цепочки могут содержать анимацию вращения
	ATTRCHANNEL_SCALE						=	1<<4,	// цепочки могут содержать анимацию масштаба
};

class cAnimChainNode
{ // одна цепочка анимации
	friend class cAnimChannelNode;
	cBaseDynArray<sKeyVisible>	KeyVisible;			// анимация видимости объекта
	cBaseDynArray<sKey3f>		KeyPosition;		// анимация позиции объекта
	cBaseDynArray<sKey4f>		KeyRotation;		// анимация вращения объекта
	cBaseDynArray<sKey3f>		KeyScale;			// анимация масштаба объекта
	float						Time;				// продолжительность цепочки анимации
	cString						name;				// имя анимационной цепочки
public:
	cAnimChainNode();
	~cAnimChainNode();
	void Release();
	float& GetTime()								{ return Time; }
	sKey3f& GetPos(int number)						{ return KeyPosition[number]; }
	sKey4f& GetRot(int number)						{ return KeyRotation[number]; }
	sKey3f& GetScale(int number)					{ return KeyScale[number]; }
	sKeyVisible& GetVisible(int number)				{ return KeyVisible[number]; }
	int GetNumberPos()								{ return KeyPosition.length(); }
	int GetNumberRot()								{ return KeyRotation.length(); }
	int GetNumberScale()							{ return KeyScale.length(); }
	int GetNumberVisible()							{ return KeyVisible.length(); }
	sKey3f& GetNewPos()								{ return KeyPosition(KeyPosition.length()); }
	sKey4f& GetNewRot()								{ return KeyRotation(KeyRotation.length()); }
	sKey3f& GetNewScale()							{ return KeyScale(KeyScale.length()); }
	sKeyVisible& GetNewVisible()					{ return KeyVisible(KeyVisible.length()); }
	void SetName(char *NameChain)					{ name=NameChain; }
	char* GetName()									{ return name; }
	// анимационные действия
	void GetMatrix(float phase,int &visible,MatXf &Matrix);
};

class cAnimChannelNode : public sAttribute, public cBaseDynArray<cAnimChainNode>
{ // список каналов анимации
public:
	cAnimChannelNode(int number=0);
	~cAnimChannelNode();
	void NewChannel(int number)						{ cBaseDynArray<cAnimChainNode>::New(number); }
	cAnimChainNode* GetChannel(int number)			{ return &cBaseDynArray<cAnimChainNode>::Base[number]; }
	int GetNumberChannel()							{ return cBaseDynArray<cAnimChainNode>::length(); }
	inline void NewPos(int nChain,int number)		{ GetChannel(nChain)->KeyPosition.New(number); if(number>0) SetAttribute(ATTRCHANNEL_POSITION); }
	inline void NewRot(int nChain,int number)		{ GetChannel(nChain)->KeyRotation.New(number); if(number>0) SetAttribute(ATTRCHANNEL_ROTATION); }
	inline void NewScale(int nChain,int number)		{ GetChannel(nChain)->KeyScale.New(number); if(number>0) SetAttribute(ATTRCHANNEL_SCALE); }
	inline void NewVisible(int nChain,int number)	{ GetChannel(nChain)->KeyVisible.New(number); if(number>0) SetAttribute(ATTRCHANNEL_VISIBLE); }
};

#endif //__ANIMCHANNELNODE_H__