#ifndef __SOFTWARE16_H__
#define __SOFTWARE16_H__

#ifndef _MROUND_
#define _MROUND_
inline int mround(float x)
{
	int a;
	_asm {
		fld x
		fistp dword ptr a
	}
	return a;
}
#endif //_MROUND_

#define PREC_DRAW					10

#define SET_XYZW(v1,v2,v3)																		\
	{																							\
		xe1=mround(v1->xe); ye1=mround(v1->ye); z1=mround(v1->z*65535)<<PREC_DRAW;				\
		xe2=mround(v2->xe); ye2=mround(v2->ye); z2=mround(v2->z*65535)<<PREC_DRAW;				\
		xe3=mround(v3->xe); ye3=mround(v3->ye); z3=mround(v3->z*65535)<<PREC_DRAW;				\
	}
#define SET_MDRGBA(p1,p2,p3)																	\
	{																							\
		mr1=p1->dr()<<PREC_DRAW; mg1=p1->dg()<<PREC_DRAW; mb1=p1->db()<<PREC_DRAW; ma1=p1->da()<<PREC_DRAW;	\
		mr2=p2->dr()<<PREC_DRAW; mg2=p2->dg()<<PREC_DRAW; mb2=p2->db()<<PREC_DRAW; ma2=p2->da()<<PREC_DRAW;	\
		mr3=p3->dr()<<PREC_DRAW; mg3=p3->dg()<<PREC_DRAW; mb3=p3->db()<<PREC_DRAW; ma3=p3->da()<<PREC_DRAW;	\
	}
#define SET_ASRGBA(p1,p2,p3)																	\
	{																							\
		ar1=p1->sr()<<PREC_DRAW; ag1=p1->sg()<<PREC_DRAW; ab1=p1->sb()<<PREC_DRAW; aa1=p1->sa()<<PREC_DRAW;	\
		ar2=p2->sr()<<PREC_DRAW; ag2=p2->sg()<<PREC_DRAW; ab2=p2->sb()<<PREC_DRAW; aa2=p2->sa()<<PREC_DRAW;	\
		ar3=p3->sr()<<PREC_DRAW; ag3=p3->sg()<<PREC_DRAW; ab3=p3->sb()<<PREC_DRAW; aa3=p3->sa()<<PREC_DRAW;	\
	}
#define SET_UV1(p1,p2,p3)																		\
	{																							\
		u11=mround(p1->u1()*xst1_); v11=mround(p1->v1()*yst1_);									\
		u12=mround(p2->u1()*xst1_); v12=mround(p2->v1()*yst1_);									\
		u13=mround(p3->u1()*xst1_); v13=mround(p3->v1()*yst1_);									\
	}
#define SET_UV2(f_u11,f_v11,f_u21,f_v21, f_u12,f_v12,f_u22,f_v22, f_u13,f_v13,f_u23,f_v23)\
	{																							\
		u11=mround(f_u11*xst1_); v11=mround(f_v11*yst1_);										\
		u12=mround(f_u12*xst1_); v12=mround(f_v12*yst1_);										\
		u13=mround(f_u13*xst1_); v13=mround(f_v13*yst1_);										\
		u21=mround(f_u21*xst2_); v21=mround(f_v21*yst2_);										\
		u22=mround(f_u22*xst2_); v22=mround(f_v22*yst2_);										\
		u23=mround(f_u23*xst2_); v23=mround(f_v23*yst2_);										\
	}
#define SET_COORD_UV1(p1,p2,p3)				{ SET_XYZW(p1,p2,p3); SET_UV1(p1,p2,p3); }	
#define SET_COORD_MDRGBA(p1,p2,p3)			{ SET_XYZW(p1,p2,p3); SET_MDRGBA(p1,p2,p3); }	
#define SET_COORD_MDRGBA_SRGBA(p1,p2,p3)	{ SET_XYZW(p1,p2,p3); SET_MDRGBA(p1,p2,p3); SET_ASRGBA(p1,p2,p3); }	
#define SET_COORD_MDRGBA_UV1(p1,p2,p3)		{ SET_XYZW(p1,p2,p3); SET_MDRGBA(p1,p2,p3); SET_UV1(p1,p2,p3); }	
#define SET_COORD_MDRGBA_UV1_SRGBA(p1,p2,p3){ SET_XYZW(p1,p2,p3); SET_MDRGBA(p1,p2,p3); SET_ASRGBA(p1,p2,p3); SET_UV1(p1,p2,p3); }

#define SOFTWARE_DRAW_FIX(p1,p2,p3,FunctionPolygonRight,FunctionPolygonLeft)					\
	{																							\
		xe1=mround(p1.xe),ye1=mround(p1.ye), xe2=mround(p2.xe),ye2=mround(p2.ye), xe3=mround(p3.xe),ye3=mround(p3.ye);	\
		if(((xe3-xe2)*(ye3-ye1)-(xe3-xe1)*(ye3-ye2))>0)											\
			if (ye1<=ye2)																		\
			{																					\
				if(ye2<=ye3)		{ v1=&p1,v2=&p2,v3=&p3; FunctionPolygonLeft(scr,zBuffer); }	\
				else if(ye1<=ye3)	{ v1=&p1,v2=&p3,v3=&p2; FunctionPolygonRight(scr,zBuffer);}	\
				else 				{ v1=&p3,v2=&p1,v3=&p2; FunctionPolygonLeft(scr,zBuffer); }	\
			}																					\
			else																				\
			{																					\
				if(ye1<=ye3)		{ v1=&p2,v2=&p1,v3=&p3; FunctionPolygonRight(scr,zBuffer);}	\
				else if(ye2<=ye3)	{ v1=&p2,v2=&p3,v3=&p1; FunctionPolygonLeft(scr,zBuffer); }	\
				else				{ v1=&p3,v2=&p2,v3=&p1; FunctionPolygonRight(scr,zBuffer);}	\
			}																					\
	}

#endif // __SOFTWARE16_H__