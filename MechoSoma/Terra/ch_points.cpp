#include <StdAfx.h>
#include "TERRA.H"
#include "Mesh3ds.h"

#ifdef _SURMAP_
#include "sqint.h"
#include "sqexp.h"
#endif

#include "port.h"

//int editCh_point=-1;
sCh_points Ch_points;

void sCh_points::ins (int _ID){
	if (edit != -1) {
		del(edit); edit = -1;
	}
	if (max_number < CH_POINTS_MAX) {
		if (_ID >= max_number ) {
//			El[max_number] = new(sCh_point);
			edit = max_number;
		}
		else {
			for (int i=max_number-1; i >= _ID; i--) {
				El[i+1] = El[i];
				for (int j=0; j<=max_number; j++){
					Link[i+1][j] = Link[i][j];
					Link[i][j]=0;
					Link[j][i+1] = Link[j][i];
					Link[j][i] = 0;
				}
			}
//			El[_ID] = new(sCh_point);
			edit = _ID;
		}
		max_number++;
	}
}
void sCh_points::del(int _ID) {
	if(_ID < max_number && _ID>=0) {
//		delete(El[_ID]);
		int i;
		for(i=_ID; i<max_number-1; i++) {
			El[i] = El[i+1];
			for (int j=0; j<max_number;j++){
				Link[i][j] = Link[i+1][j];
				Link[i+1][j] = 0;
				Link[j][i] = Link[j][i+1];
				Link[j][i+1] = 0;
			}
		}
		El[i+1].x1 = -1;
		El[i+1].y1 = -1;
		El[i+1].x2 = -1;
		El[i+1].y2 = -1;
		max_number--;
	}
}
void sCh_points::addLink(int _ID,int _Link){
/*	int i=0;
	if(_Link==-1|| _ID==-1) return;
	while( i < CH_POINT_LINK_MAX && El[_ID]->Link[i]!=-1) i++;
	El[_ID]->Link[i] = _Link;*/
	Link[_ID][_Link]=1;
	Link[_Link][_ID]=1;
}
void sCh_points::delallLink(int _ID) {
/*	int i=0;
	while( i < CH_POINT_LINK_MAX ) { //&& El[_ID]->Link[i]!=0) {
		El[_ID]->Link[i] = -1;
		i++;
	}*/
	for (int i=0; i<CH_POINTS_MAX; i++){
		Link[_ID][i]=0;
		Link[i][_ID]=0;
	}
}

void sCh_points::set1(int MapX,int MapY,int ScX,int ScY)
{
	El[edit].x1 = MapX;
	El[edit].y1 = MapY;
#ifdef _SURMAP_
	if(GCM.setFV(FLAG_ANIMATION_CH_POINTS))	fl_animation = 1;
#endif
}
void sCh_points::set2(int MapX,int MapY,int ScX,int ScY)
{
	El[edit].x2 = MapX;
	El[edit].y2 = MapY;
#ifdef _SURMAP_
	if(GCM.clearFV(FLAG_ANIMATION_CH_POINTS))	fl_animation = 0;
#endif
}

int vmapOpenResource(char* fname,XStream& fh);

// For workins the FILES
XStream Cmap(0);
void sCh_points::load(char* fname,int number_track)
{
	if(number_track==-1) number_track=currient_track;
	//if(number_track!=currient_track) save(fname,currient_track);
	reset();
	XBuffer xb;
	char tmpstr[10];
	int i;
	xb < fname < port_itoa(number_track,tmpstr,10);
	currient_track=number_track;
#ifdef _WIN32
	if(!Cmap.open(xb,XS_IN)) return;
#else
	if(!vmapOpenResource(xb, Cmap)) return;
#endif
	long s = Cmap.size();
	char temp[7];
	Cmap.read(temp,sizeof(temp));
	Cmap.read(&El,sizeof(El));
	Cmap.read(&max_number,sizeof(max_number));
	Cmap.read(&St,sizeof(St));
	Cmap.read(&max_start,sizeof(max_start));
	Cmap.close();
	for (i=0;i<max_number;i++) 
		for (int j=0;j<max_number;j++) 
			if(Link[i][j]!=Link[j][i]/* && max_number != 0*/) 
				ErrH.Abort("Invalid Ch_points file"); 
	//if (max_number == 0) {
//		max_number = 3;
//		max_start = 0;
//	}
#ifdef _MECHOSOMA_
	for (i=0;i<max_number;i++) {
		El[i].x2=calcDX(El[i].x1, El[i].x2);
		El[i].y2=calcDX(El[i].y1, El[i].y2);
	}
#endif	
}

void sCh_points::save(char* fname,int number_track)
{
	if (edit != -1) {
		del(edit); edit = -1;
	}
	if (editstart != -1) {
		delstart(editstart); editstart = -1;
	}
	if(number_track==-1) number_track=currient_track;
	XBuffer xb;
	char tmpstr[10];
	xb < fname < port_itoa(number_track,tmpstr,10);
	if(!Cmap.open(xb,XS_OUT)) return;
	Cmap.write(&El,sizeof(El));
	Cmap.write(&Link,sizeof(Link));
	Cmap.write(&max_number,sizeof(max_number));
	Cmap.write(&St,sizeof(St));
	Cmap.write(&max_start,sizeof(max_start));
	Cmap.close();

}
void sCh_points::insstart(int _num,int x,int y,int z)
{
/*	St[max_start].x=x;
	St[max_start].y=y;
	St[max_start].z=z;
	St[max_start].ax=ax;
	St[max_start].ay=ay;
	St[max_start].az=az;
	max_start++;*/
	if (editstart != -1) {
		delstart(editstart); editstart = -1;
	}
	if (max_start < ST_POINTS_MAX) {
		if ( _num >= max_start ) {
			editstart = max_start;
		}
		else {
			for (int i=max_start-1; i >= _num; i--) {
				St[i+1] = St[i];
			}
			editstart = _num;
		}
		St[editstart].x=(float)x;
		St[editstart].y=(float)y;
		St[editstart].z=(float)z;
		if(editstart > 0){
			St[editstart].ax=St[editstart-1].ax;
			St[editstart].ay=St[editstart-1].ay;
			St[editstart].az=St[editstart-1].az;
		}
		else {
			St[editstart].ax=0;
			St[editstart].ay=0;
			St[editstart].az=0;
		}
		max_start++;
	}
}
void sCh_points::delstart(int _num){
	if(_num < max_start && _num>=0) {
		for(int i=_num; i<max_start-1; i++) {
			St[i] = St[i+1];
		}
		max_start--;
	}
}

#ifdef _SURMAP_
void sCh_points::drawstart(){
//		char tmpstr[16];
//		int subx=XCYCL(CX-xc);
//		int suby=YCYCL(CY-yc);
		for(int i=0; i<Ch_points.max_start;i++){
			DrawMeshPoint(i,Ch_points.St[i].x, Ch_points.St[i].y, Ch_points.St[i].z, Ch_points.St[i].ax, Ch_points.St[i].ay, Ch_points.St[i].az );
//			int xs = Ch_points.St[i].x - subx; if (xs<0) xs+=H_SIZE;
//			int ys = Ch_points.St[i].y - suby; if (ys<0) ys+=V_SIZE;
//			sysfont.draw(xs,ys,(char*)itoa(i,tmpstr,10),COL1,-1);
		}

}
#endif
