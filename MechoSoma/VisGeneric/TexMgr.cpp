#include "BaseDefine.h"
#include "TgaServ.h"
#include "TexMgr.h"
#include "base.h"
#include "PolyMgr.h"

#ifdef _MECHOSOMA_
int m3dOpenResource(char* fname,XStream& fh);
#endif

void SetExtension(char *fnameOld,char *extension,char *fnameNew)
{
	strcpy(fnameNew,fnameOld);
	int l;
	for(l=strlen(fnameOld)-1;l>=0&&fnameNew[l]!='\\';l--)
		if(fnameNew[l]=='.')
			break;
	if(l>=0) strcpy(&fnameNew[l+1],extension);
}

void SetExtension(char *name,char *extension,char *fname,char *path)
{
	char fn[255]; int i=0,k=0;
	do { fn[i]=name[i]; } while((name[i]!=0)&&(name[i++]!='.'));
	do { fn[i+k]=extension[k]; } while(extension[k++]!=0);
	if(path) { strcpy(fname,path); if(fname[strlen(fname) - 1] != '\\') strcat(fname,"\\"); } else strcpy(fname,TEXTURE_DIRECTORY); 
	strcat(fname,fn);
}

void cTextureBuffer::Release(unsigned int Type)
{
	for(int i=length()-1;i>=0;i--)
	{
		sTexture *Texture=Base[i];
		if((Type==0)&&(Texture->GetRef()!=1))
		{
//			while(Texture->GetRef()!=1) Texture->DecRef();
			ErrAbort("cTextureBuffer::Release()\r\nTexture used");
		}
		if(Texture->GetRef()==1) 
		{
			if(Texture->nTexture)
				P3D->DeleteTexture(Texture);
			Detach(Texture);
			Texture->Release();
		}
	}
}
sTexture* cTextureBuffer::BeginList(int x,int y)
{
	MultiTexture=new sTexture;
	if(x!=y) { XBuffer buf; buf<"Error: cTextureBuffer::BeginList()\r\nTexture xsize!=ysize"; ErrAbort(buf.address()); }
	MultiTexture->texture=new unsigned char[4*x*y];
	memset(MultiTexture->texture,0,4*x*y);
	MultiTexture->_x=ReturnBit(x), MultiTexture->_y=ReturnBit(y);
	xCurrent=yCurrent=yMax=0;
	MultiTexture->SetName("");
	Attach(MultiTexture);
	return MultiTexture;
}
sTextureChild* cTextureBuffer::AttachChild(char *fname,int id)
{
	unsigned char *buf=0;
	int x,y,fmt;
	if(LoadTGA(fname,(void**)&buf,x,y,fmt)==0) { XBuffer buf; buf<"cTextureBuffer::AttachChild()\r\nTexture not found: "<fname; ErrAbort(buf.address()); return 0; }
	if((xCurrent+x)>(1<<MultiTexture->_x))
	{
		xCurrent=0;
		yCurrent=yMax;
		yMax+=y;
		if(yMax>(1<<MultiTexture->_y)) { if(buf) delete buf; return 0; }
	}
	unsigned char *texture=MultiTexture->texture+4*(xCurrent+(yCurrent<<MultiTexture->_x)),*tmp=buf;
	assert(texture);
	for(int j=0;j<y;j++,texture+=4<<MultiTexture->_x)
		for(int i=0;i<4*x;i++,tmp++)
			texture[i]=*tmp;
	if(buf) delete buf;
	assert(0<=id&&id<255);
	sTextureChild &TextureChild=TextureChildArray(id);
	TextureChild.Texture=MultiTexture; MultiTexture->IncRef();
	TextureChild._x=ReturnBit(x); TextureChild._y=ReturnBit(y);
	TextureChild.xofs=xCurrent; TextureChild.yofs=yCurrent;
	float dTex=1.f/(1<<TextureChild.Texture->_x),sTex=(1<<TextureChild._x)*dTex;
	TextureChild.uofs=(float)TextureChild.xofs/(1<<TextureChild.Texture->_x);
	TextureChild.vofs=(float)TextureChild.yofs/(1<<TextureChild.Texture->_y);
	TextureChild.usize=(float)(1<<TextureChild._x)/(1<<TextureChild.Texture->_x);
	TextureChild.vsize=(float)(1<<TextureChild._y)/(1<<TextureChild.Texture->_y);
	xCurrent+=x;
	if(yMax<(yCurrent+y)) yMax=yCurrent+y;
	return &TextureChild;
}
sTexture* cTextureBuffer::EndList()
{
	xCurrent=yCurrent=yMax=0;
	sTexture *tmp=MultiTexture;
	MultiTexture=0;
	return tmp;
}
sTexture* cTextureBuffer::Get(char *name,char *path,char *warning)
{
	if(name==0||name[0]==0) return 0;
	char FullName[512];
	strcpy(FullName,path);
	int l=strlen(FullName);
	if(FullName[l-1]!='\\') strcat(FullName,"\\");
	strcat(FullName,name);
	for(int i=0;i<length();i++)
		if(strcmp(FullName,Base[i]->name)==0) 
		{ 
			Base[i]->IncRef(); 
			return Base[i]; 
		}
	sTexture *Texture;
	Texture=Load(FullName,warning);
	if(Texture==0) Texture=LoadTGA(FullName,warning);
//	if(tmp==0) tmp=LoadJPG(FullName,warning);
	if(Texture) { Attach(Texture); Texture->IncRef(); }
	if(Texture==0) { XBuffer buf; buf<"Error: cTextureBuffer::Get()\r\nTexture "<FullName<" not found in file "<warning; ErrAbort(buf.address()); }
	return Texture;
}
sTexture* cTextureBuffer::Load(char *fname,char *warning)
{
	int x=0,y=0;
	char PaletteName[255],TextureName[255];
	SetExtension(fname,TEXTURE_EXTENSION,TextureName);

	XStream fTexture(0);
#ifdef _MECHOSOMA_
	if(!m3dOpenResource(TextureName,fTexture)) return 0;
#else
	if(!fTexture.open(TextureName,XS_IN)) return 0;
#endif

	fTexture.read(&x,2); fTexture.read(&y,2);
	if(x!=y) { XBuffer buf; buf<"Error: cTextureBuffer::Load()\r\nTexture "<fname<" xsize!=ysize"; ErrAbort(buf.address()); }

	// for hardware acseleration
//	if((x!=64)&&(x!=128))
//		{ XBuffer buf; buf<"Error: cTextureBuffer::Load()\r\nTexture "<fname<" bad xsize="<=x<" (64 or 128)"; ErrAbort(buf.address()); }

	sTexture *Texture=new sTexture;
	Texture->_x=Texture->_y=0; 
	Texture->SetName(fname);
	if(fTexture.size()==(x*y+4))
	{
		Texture->texture=new unsigned char[x*y+768];
		fTexture.read(Texture->texture,x*y);
		fTexture.close();
	}
	while((x>>=1)!=0) Texture->_x++;
	while((y>>=1)!=0) Texture->_y++;

	SetExtension(fname,PALETTE_EXTENSION,PaletteName);

	XStream fPalette(0);
#ifdef _MECHOSOMA_
	m3dOpenResource(PaletteName,fPalette);
#else
	if(!fPalette.open(PaletteName,XS_IN)) { XBuffer buf; buf<"Palette not found: "<fname<" "<warning; ErrAbort(buf.address()); return 0; }
#endif

	if(fPalette.size()>768) { XBuffer buf; buf<"Error palette: "<fname; ErrAbort(buf.address()); return 0; }
	fPalette.read(Texture->GetPal(),fPalette.size());
	fPalette.close();

	Texture->SetAttribute(ATTRTEX_PAL8);
	return Texture;
}
sTexture* cTextureBuffer::LoadTGA(char *fname,char *warning)
{
	int x=0,y=0,fmt=0; 
	unsigned char *buf=0;
	char fName[255];
	SetExtension(fname,TEXTURE_EXTENSION,fName);
	if(!LoadTGA(fName,(void**)&buf,x,y,fmt)) return 0;
//	unsigned char *tmp=new unsigned char[x*y*2];
//	ConvertTextureFormat(buf,x,y,fmt,tmp,x,y,ATTRTEX_RGB565);
//	delete buf;
//	SaveTGA("tmp.tga",tmp,x,y,ATTRTEX_RGB565);
	sTexture *Texture=new sTexture;
	Texture->SetName(fname);
	Texture->texture=buf;
	Texture->_x=ReturnBit(x);
	Texture->_y=ReturnBit(y);
	Texture->SetAttribute(fmt);
	return Texture;
}
int cTextureBuffer::LoadTGA(char *fname,void **buf,int &x,int &y,int &fmt)
{
	char NameTGA[255];
	if((fname)&&(strcmp(fname,"")==0)) { *buf=0; return 0; }
	SetExtension(fname,"tga",NameTGA);
	TGA_Image	tga;
	if(!tga.Load(NameTGA)) return 0;
	x=tga.header->SizeX; y=tga.header->SizeY;
	unsigned char *p2=new unsigned char[x*y*4]; *buf=p2;
	switch(tga.header->colors)	
	{
/*		case 16:
			if(x*y*3!=tga.imageSize) ErrH.Abort("Incorrect TGA Format");
			fmt=ATTRTEX_RGB565;
			if(!(tga.header->flag&0x20))
			{
				unsigned char *p1=tga.imageData+x*(y-1)*2;
				for(int k=0;k<y;k++,p1-=x*2*2)
					for(int l=0;l<x;l++,p1+=2,p2+=2)
					{
						*(p2+0)=(unsigned char)(*(p1+0));
						*(p2+1)=(unsigned char)(*(p1+1));
					}
			}
			else
			{
				unsigned char *p1=tga.imageData;
				for(int k=0;k<y;k++)
					for(int l=0;l<x;l++,p1+=2,p2+=2)
					{
						*(p2+0)=(unsigned char)(*(p1+0));
						*(p2+1)=(unsigned char)(*(p1+1));
					}
			}
			break;
*/		case 24:
			if(x*y*3!=tga.imageSize) ErrH.Abort("Incorrect TGA Format");
			fmt=ATTRTEX_RGB888;
			if(!(tga.header->flag&0x20))
			{
				unsigned char *p1=tga.imageData+x*(y-1)*3;
				for(int k=0;k<y;k++,p1-=x*3*2)
					for(int l=0;l<x;l++,p1+=3,p2+=3)
					{
						*(p2) = (unsigned char)(*(p1 + 2));
						*(p2 + 1) = (unsigned char)(*(p1 + 1));
						*(p2 + 2) = (unsigned char)(*p1);
					}
			}
			else
			{
				unsigned char *p1=tga.imageData;
				for(int k=0;k<y;k++)
					for(int l=0;l<x;l++,p1+=3,p2+=3)
					{
						*(p2) = (unsigned char)(*(p1 + 2));
						*(p2 + 1) = (unsigned char)(*(p1 + 1));
						*(p2 + 2) = (unsigned char)(*p1);
					}
			}
			break;
		case 32:
			if(x*y*4!=tga.imageSize) ErrH.Abort("Incorrect TGA Format");
			fmt=ATTRTEX_RGBA8888;
			if(!(tga.header->flag&0x20))
			{
				unsigned char *p1=tga.imageData+x*(y-1)*4;
				for(int k=0;k<y;k++,p1-=x*4*2)
					for(int l=0;l<x;l++,p1+=4,p2+=4)
					{
						*(p2) = (unsigned char)(*(p1 + 2));
						*(p2 + 1) = (unsigned char)(*(p1 + 1));
						*(p2 + 2) = (unsigned char)(*p1);
						*(p2 + 3) = (unsigned char)(*(p1 + 3));
					}
			}
			else
			{
				unsigned char *p1=tga.imageData;
				for(int k=0;k<y;k++)
					for(int l=0;l<x;l++,p1+=4,p2+=4)
					{
						*(p2) = (unsigned char)(*(p1 + 2));
						*(p2 + 1) = (unsigned char)(*(p1 + 1));
						*(p2 + 2) = (unsigned char)(*p1);
						*(p2 + 3) = (unsigned char)(*(p1 + 3));
					}
			}
			break;
		default:
			ErrH.Abort("This TGA Format Are Not Suppoted");
	}
	return 1;
}
sTexture* cTextureBuffer::LoadJPG(char *fname,char *warning)
{
/*	int x=0,y=0,fmt=0; 
	unsigned char *buf=0;
	char fName[255];
	SetExtension(fname,"jpg",fName,path);

	int i,j=0;
	char tmpstr[10];
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerrpub;	
	FILE * infile;		
	JSAMPARRAY buffer;		
	int row_stride;		

	if(!(infile = fopen(fName,"rb")))return 0;
	sTexture *Texture=new sTexture;
	Texture->SetName(fname);
	Texture->MemTexture=buf;
	Texture->texture=Texture->MemTexture;
	Texture->_x=ReturnBit(x);
	Texture->_y=ReturnBit(y);
	Texture->SetAttribute(ATTRTEX_RGB565);

	cinfo.err=jpeg_std_error(&jerrpub);
	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo, infile);
	jpeg_read_header(&cinfo, TRUE);
	jpeg_start_decompress(&cinfo);

	row_stride = cinfo.output_width * cinfo.output_components;
	buffer = (*cinfo.mem->alloc_sarray)
		((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);
*
	for(;cinfo.output_scanline<cinfo.output_height;j++) 
	{
		jpeg_read_scanlines(&cinfo, buffer, 1);
		unsigned char* p=buffer[0];
		for(i=0;i<cinfo.output_width;i++)
		{
			short col;
			col=(*p++ <<8)&0x0F800;
			col+=(*p++ <<3)&0x7E0;
			col+=(*p++ >>3)&0x1F;
			ClBuf[0][j][i]=col;
		}
	}
	*
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	fclose(infile);
	return Texture;
*/
	return 0;
}
struct sDithering
{
	short r,g,b;
};
void cTextureBuffer::ConvertTextureFormat(void *pOld,int xOld,int yOld,int fmtOld,void *pNew,int xNew,int yNew,int fmtNew)
{
	if((xOld==xNew)&&(yOld=yNew))
	{ //only dithering
		if(fmtNew==fmtOld) return;
		if((fmtOld==ATTRTEX_RGB888)&&(fmtNew==ATTRTEX_RGB565))
		{
			int rshift=11,gshift=5,bshift=0,rcount=5,gcount=6,bcount=5;
			sDithering *dither=new sDithering[xNew*yNew];
			unsigned short *rgbNew=(unsigned short*)pNew;
			unsigned char *rgbOld=(unsigned char*)pOld;
			int index=0;
			for(int i=0;i<xNew*yNew;i++) { dither[i].r=0; dither[i].g=0; dither[i].b=0; }
			for(int j=0;j<yNew;j++)
				for(int i=0;i<xNew;i++,index++)
				{
					int x=i%xNew,y=j%yNew;
					int r=rgbOld[3*index+0]+(dither[x+y*xNew].r>>rcount),
						g=rgbOld[3*index+1]+(dither[x+y*xNew].g>>gcount),
						b=rgbOld[3*index+2]+(dither[x+y*xNew].b>>bcount);
					int ir=r>>(8-rcount),ig=g>>(8-gcount),ib=b>>(8-bcount),
						fr=r&((1<<(8-rcount))-1),fg=g&((1<<(8-gcount))-1),fb=b&((1<<(8-bcount))-1);
					float fR=fr/8.f,fG=fg/4.f,fB=fb/8.f;
					const int DITHERING_R=40,DITHERING_G=31,DITHERING_B=29;
					int l=round((fR*DITHERING_R+fG*DITHERING_G+fB*DITHERING_B)*0.4f);
					int dr=l/DITHERING_R; l-=dr*DITHERING_R;
					int dg=l/DITHERING_G; l-=dg*DITHERING_G;
					int db=l/DITHERING_B; l-=db*DITHERING_B;
					ir+=dr; ig+=dg; ib+=db;
					rgbNew[index]=(ir<<rshift)+(ig<<gshift)+(ib<<bshift);
					int x1=(i+1)%xNew,y1=(j+1)%yNew;
					dither[x1+y*xNew].r+=(fr-(dr<<3))<<(rcount-2);
					dither[x1+y*xNew].g+=(fg-(dg<<2))<<(gcount-2);
					dither[x1+y*xNew].b+=(fb-(db<<3))<<(bcount-2);
					dither[x+y1*xNew].r+=(fr-(dr<<3))<<(rcount-2);
					dither[x+y1*xNew].g+=(fg-(dg<<2))<<(gcount-2);
					dither[x+y1*xNew].b+=(fb-(db<<3))<<(bcount-2);
				}
/*			for(int j=0;j<yNew;j++)
				for(int i=0;i<xNew;i++,index++)
				{
					int x=i%xNew,y=j%yNew;
					int r=rgbOld[3*index+0]+dither[x+y*xNew].r,
						g=rgbOld[3*index+1]+dither[x+y*xNew].g,
						b=rgbOld[3*index+2]+dither[x+y*xNew].b;
					int ir=r>>(8-rcount),ig=g>>(8-gcount),ib=b>>(8-bcount),
						fr=r&((1<<(8-rcount))-1),fg=g&((1<<(8-gcount))-1),fb=b&((1<<(8-bcount))-1);
					rgbNew[index]=(ir<<rshift)+(ig<<gshift)+(ib<<bshift);
					int x1=(i+1)%xNew,y1=(j+1)%yNew;
					dither[x1+y*xNew].r+=fr/4;
					dither[x1+y*xNew].g+=fg/4;
					dither[x1+y*xNew].b+=fb/4;
					dither[x+y1*xNew].r+=fr/4;
					dither[x+y1*xNew].g+=fg/4;
					dither[x+y1*xNew].b+=fb/4;
				}
*/
		}
	}
}
int cTextureBuffer::SaveTGA(char *fname,void *buf,int x,int y,int fmt)
{
	if((fname)&&(strcmp(fname,"")==0)) return 0;
	int bpp=0;
	void *p=0;
	switch(fmt)
	{
		case ATTRTEX_RGB888:
			{
				unsigned char *tmp=new unsigned char[x*y*3];
				unsigned char *rgb=(unsigned char*)buf;
				for(int i=0;i<x*y;i++)
				{
					tmp[3*i+0]=rgb[3*i+0];
					tmp[3*i+1]=rgb[3*i+1];
					tmp[3*i+2]=rgb[3*i+2];
				}
				bpp=24;
				p=tmp;
				break;
			}
		case ATTRTEX_RGB565:
			{
				unsigned short *tmp=new unsigned short[x*y];
				unsigned short *rgb=(unsigned short*)buf;
				for(int i=0;i<x*y;i++)
					tmp[i]=(((rgb[i]>>11)&31)<<10)+(((rgb[i]>>6)&31)<<5)+(((rgb[i]>>0)&31)<<0);
				bpp=16;
				p=tmp;
				break;
			}
		default:
			assert(0);
	}
	char NameTGA[255];
	SetExtension(fname,"tga",NameTGA);
	TGA_Image tga;
	if(p) tga.Save(NameTGA,x,y,bpp,p);
	if(p) delete p;
	return 1;
}
cTextureBuffer::~cTextureBuffer()
{ 
	for(int i=0;i<TextureChildArray.length();i++)
		if(TextureChildArray[i].Texture)
			TextureChildArray[i].Texture->DecRef();
	Release(0); 
}

