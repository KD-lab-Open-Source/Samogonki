#include "StdAfx.h"

#include "aci_parser.h"

#include "fxlabID.h"
#include "fxlabResource.h"
#include "fxlabKey.h"

void fxlabKeyType::Open(scrDataBlock* data)
{
	int i,j,k,l;
	float time;
	int mode;
	scrDataBlock* p;
	fxlabKeyPointType* point;
	fxlabKeyDataType* key;

	KeyNum = 0;
	PointNum = 0;
	p = data;
	while(p){
		switch(p->ID){
			case FXLAB_SCRIPT_KEY_ATTRIBUTE:
				Attribute = *(p->i_dataPtr);
				break;
			case FXLAB_SCRIPT_KEY_DEFINITION:
				KeyNum++;
				break;
			case FXLAB_SCRIPT_KEY_TIME:
				PointNum++;
				break;
		};
		p = p->next;
	};

	if((Attribute & FXLAB_KEY_ATTRIBUTE_STORAGE) && PointNum > 1)
			ErrH.Abort("Key Storage Bad Time in fxlabResource");

	PointData = new fxlabKeyPointType[PointNum];
	KeyStorage = new fxlabKeyDataType[KeyNum*PointNum];

	for(i = 0;i < PointNum;i++)
		PointData[i].Data = KeyStorage + i * KeyNum;

	Name = new char*[KeyNum];
	i = 0;
	p = data;
	while(p){
		if(p->ID == FXLAB_SCRIPT_KEY_DEFINITION){
			Name[i] = new char[strlen(p->c_dataPtr) + 1];
			strcpy(Name[i],p->c_dataPtr);
			i++;
		};
		p = p->next;
	};

	i = 0;
	p = data;
	while(p){
		if(p->ID == FXLAB_SCRIPT_KEY_TIME){
			PointData[i].Time = (float)(*(p->d_dataPtr));
			if(PointData[i].Time < 0)
				ErrH.Abort("Incorrect Definition Time in fxlabResource");
			i++;
		};
		p = p->next;
	};

	for(i = 0;i < PointNum - 1;i++){
		if(PointData[i].Time >= PointData[i + 1].Time)
			ErrH.Abort("Incorrect Definition Time in fxlabResource");
	};

	time = 0;
	mode = FXLAB_KEY_MODE_LINEAR;
	key = NULL;
	point = NULL;

	p = data;
	while(p){
		switch(p->ID){
			case FXLAB_SCRIPT_KEY_MODE:
				mode = *(p->i_dataPtr);
				break;
			case FXLAB_SCRIPT_KEY_TIME:
				time = (float)(*(p->d_dataPtr));

				point = NULL;
				for(i = 0;i < PointNum;i++){
					if(PointData[i].Time == time){
						point = PointData + i;
						break;
					};
				};

				if(!point)
					ErrH.Abort("Key Time not found in fxlabResource");
				break;
			case FXLAB_SCRIPT_KEY_NAME:
				if(!point)
					ErrH.Abort("Key Name defined without Time in fxlabResource");

				key = NULL;
				for(i = 0;i < KeyNum;i++){
					if(!strcmp(p->c_dataPtr,Name[i])){
						key = point->Data + i;
						break;
					};
				};

				if(!key)
					ErrH.Abort("Key Name not Found in fxlabResource");
				break;
			case FXLAB_SCRIPT_KEY_DATA:
				if(!key)
					ErrH.Abort("Key Name not Define in fxlabResource");
				if(!point)
					ErrH.Abort("Key Time not Define in fxlabResource");

				key->Mode = mode;
				key->Value = (float)(*(p->d_dataPtr));
				key->Delta = 0;
				mode = FXLAB_KEY_MODE_LINEAR;
				break;
		};
		p = p->next;
	};

	for(i = 0;i < KeyNum;i++){
		if(PointData[0].Data[i].Value == FXLAB_RESOURCE_KEY_DEFAULT_VALUE)
			ErrH.Abort("UnDefined FirstTime Data Key Field in fxScript");
	};

	if(!(Attribute & FXLAB_KEY_ATTRIBUTE_STORAGE)){
		if(PointNum < 2)
			ErrH.Abort("UnDefined Second Phase in fxScript");

		for(i = 0;i < KeyNum;i++){
			if(PointData[PointNum - 1].Data[i].Value == FXLAB_RESOURCE_KEY_DEFAULT_VALUE)
				ErrH.Abort("UnDefined LastTime Data Key Field in fxScript");
		};

		for(i = 1;i < PointNum - 1;i++){
			for(j = 0;j < KeyNum;j++){
				if(PointData[i].Data[j].Value == FXLAB_RESOURCE_KEY_DEFAULT_VALUE){

					for(k = i - 1;k >= 0;k--){
						if(PointData[k].Data[j].Value != FXLAB_RESOURCE_KEY_DEFAULT_VALUE)
							break;
					};

					for(l = i + 1;l < PointNum;l++){
						if(PointData[l].Data[j].Value != FXLAB_RESOURCE_KEY_DEFAULT_VALUE)
							break;
					};
				
					if(PointData[l].Data[j].Mode == FXLAB_KEY_MODE_IMPULSE){
						PointData[i].Data[j].Value = PointData[l].Data[j].Value;
						PointData[i].Data[j].Mode = FXLAB_KEY_MODE_IMPULSE;
					}else{
						PointData[i].Data[j].Value = PointData[k].Data[j].Value + (PointData[i].Time - PointData[k].Time) * (PointData[l].Data[j].Value - PointData[k].Data[j].Value) / (PointData[l].Time - PointData[k].Time);
						PointData[i].Data[j].Mode = FXLAB_KEY_MODE_LINEAR;
					};
				};
			};
		};

		for(i = 0;i < PointNum - 1;i++){
			for(j = 0;j < KeyNum;j++){
				PointData[i].Data[j].Delta = (PointData[i + 1].Data[j].Value - PointData[i].Data[j].Value) / (PointData[i + 1].Time - PointData[i].Time);
			};
		};

		for(i = 0;i < PointNum;i++){
			for(j = 0;j < KeyNum;j++){
				if(PointData[i].Data[j].Value == FXLAB_RESOURCE_KEY_DEFAULT_VALUE)
					ErrH.Abort("Bad Interpolation Key in fxlabResource");

				PointData[i].Mode = FXLAB_KEY_MODE_LINEAR;
				if(PointData[i].Mode == FXLAB_KEY_MODE_LINEAR && PointData[i].Data[j].Mode == FXLAB_KEY_MODE_IMPULSE)
					PointData[i].Mode = FXLAB_KEY_MODE_IMPULSE;
			};
		};
	};

	Scale = PointData[PointNum - 1].Time;
	InvScale = Scale ? 1.0f / Scale : 0; // fuck YOU!!!
};


void fxlabKeyType::Close()
{
	int i;
	for(i = 0;i < KeyNum;i++)
		delete Name[i];
	delete Name;
	delete KeyStorage;
	delete PointData;
};
  
int fxlabKeyType::GetKey(float* point,float time)
{
	if(!(Attribute & FXLAB_KEY_ATTRIBUTE_STORAGE)){
		if(time > Scale){
			if(Attribute & FXLAB_KEY_ATTRIBUTE_REPEAT)
				time -= (float)(Scale*floor(time * InvScale));
			else{
				if(Attribute & FXLAB_KEY_ATTRIBUTE_FREEZE)
					time = Scale;
				else{
					FillLinear(point,Scale);
					return 0;
				};
			};
		};
		FillLinear(point,time);
	}else{
		if(time > Scale && !(Attribute & (FXLAB_KEY_ATTRIBUTE_REPEAT | FXLAB_KEY_ATTRIBUTE_FREEZE))){
			FillStorage(point,Scale);
			return 0;
		}else
			FillStorage(point,time);
	};
	return 1;
};

int fxlabKeyType::GetKeyStatus(float time)
{
	if(time > Scale && !(Attribute & (FXLAB_KEY_ATTRIBUTE_REPEAT | FXLAB_KEY_ATTRIBUTE_FREEZE)))
		return 0;
	return 1;
};

void fxlabKeyType::FillStorage(float* point,float time)
{
	int i;
	float* d;
	fxlabKeyDataType* p;

	d = point;
	p = PointData[0].Data;
	for(i = 0;i < KeyNum;i++){
		*d = p->Value;
		p++;
		d++;
	};
};

void fxlabKeyType::FillLinear(float* point,float time)
{
	int ind,i;
	float *d;
	fxlabKeyDataType *n;
	fxlabKeyDataType *p;
	float t;

	for(ind = 0;ind < PointNum;ind++){
		if(PointData[ind].Time > time)
			break;
	};

	d = point;
	if(ind < PointNum){
		if(!ind || time == 0.0f){
			p = PointData[0].Data;
			for(i = 0;i < KeyNum;i++){
				*d = p->Value;
				p++;
				d++;
			};
		}else{
			t = time - PointData[ind - 1].Time;
			p = PointData[ind - 1].Data;
			n = PointData[ind].Data;
			if(n->Mode == FXLAB_KEY_MODE_IMPULSE){
				for(i = 0;i < KeyNum;i++){
					if(n->Mode == FXLAB_KEY_MODE_IMPULSE)
						*d = p->Value;
					else
						*d = p->Value + p->Delta * t;
					p++;
					d++;
				};
			}else{
				for(i = 0;i < KeyNum;i++){
					*d = p->Value + p->Delta * t;
					p++;
					d++;
				};
			};
		};
	}else{
		p = PointData[PointNum - 1].Data;
		for(i = 0;i < KeyNum;i++){
			*d = p->Value;
			p++;
			d++;
		};
	};
};

