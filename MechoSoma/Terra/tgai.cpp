#include "StdAfx.h"
#include "terra.h"
#include "tgai.h"

void TGAHEAD::save3layers(char* fname,int sizeX,int sizeY,unsigned char* Ra,unsigned char* Ga,unsigned char* Ba)
{//GetTargetName("outputnh.tga")
	init();
	XStream ff(fname, XS_OUT);
	Width=(short)sizeX;
	Height=(short)sizeY;
	ff.write(this,sizeof(TGAHEAD));
	unsigned char *line = new unsigned char[sizeX*3],*p;
	register unsigned int i,j;
	for(j = 0; j<sizeY; j++){
		p = line;
		for(i = 0; i<sizeX; i++){
			//*p++ = (unsigned char)((((*vidBuf)[j][i])&0x1F)<<3) ;
			//*p++ = (unsigned char)((((*vidBuf)[j][i])&0x7E0)>>3) ;
			//*p++ = (unsigned char)((((*vidBuf)[j][i])&0x0F800)>>8) ;
			*p++ = Ba[j*sizeX+i];
			*p++ = Ga[j*sizeX+i];
			*p++ = Ra[j*sizeX+i];
		}
		ff.write(line,sizeX*3);
	}
	ff.close();
	delete line;
}

void TGAHEAD::load3layers(char* fname,int sizeX,int sizeY,unsigned char* Ra,unsigned char* Ga,unsigned char* Ba)
{//GetTargetName("outputnh.tga")
	init();
	XStream ff(fname, XS_IN);
	ff.read(this,sizeof(TGAHEAD));
	if( (Width!=sizeX) || (Height!=sizeY) ) return;
	unsigned char *line = new unsigned char[sizeX*3],*p;
	register unsigned int i,j;
/*	for(j = 0; j<sizeY; j++){
		p = line;
		ff.read(line,sizeX*3);
		for(i = 0; i<sizeX; i++){
			//*p++ = (unsigned char)((((*vidBuf)[j][i])&0x1F)<<3) ;
			//*p++ = (unsigned char)((((*vidBuf)[j][i])&0x7E0)>>3) ;
			//*p++ = (unsigned char)((((*vidBuf)[j][i])&0x0F800)>>8) ;
			Ba[j*sizeX+i] = *p++;
			Ga[j*sizeX+i] = *p++;
			Ra[j*sizeX+i] = *p++;
		}
	}*/
	int ibeg,jbeg,iend,jend,ik,jk;
	if(ImageDescriptor&0x20) { jbeg=0; jend=V_SIZE; jk=1;}
	else { jbeg=V_SIZE-1; jend=-1; jk=-1;}
	if((ImageDescriptor&0x10)==0) { ibeg=0; iend=H_SIZE; ik=1;}
	else { ibeg=H_SIZE-1; iend=-1; ik=-1;}
	for(j=jbeg; j!=jend; j+=jk){
		p = line;
		ff.read(line,H_SIZE*3);
		for(i = ibeg; i!=iend; i+=ik){
			Ba[j*sizeX+i] = *p++;
			Ga[j*sizeX+i] = *p++;
			Ra[j*sizeX+i] = *p++;
		}
	}

	ff.close();
	delete line;
}
