
#include "StdAfx.h"
#include <stdio.h>
#include "Body.h"
#include "CollisionHandler.hpp"
#include "Vitamina.hpp"

using namespace Vitamina;

void CContact::Initialize(real ts, int relaxation_type,real relaxation_time)
{
 int b;
 if(friction!=0.0)
 {
  MakeOrthonormalBasis(tangent[0],tangent[1],normal);
  Zero(v);
  for(b=0;b<2;b++)
  if(body[b])
  {
   body[b]->GetInverseTOIWorld(TOI_i[b]);
   body[b]->GetPosition(relp[b]);
   Subtract(relp[b],point,relp[b]);
   real vel[3];
   CrossProduct(vel,body[b]->GetAngularVelocity(),relp[b]);
   AddTo(vel,body[b]->GetVelocity());
   
   real gr_vel[3];//to prevent sliding on slopes
   if(ts>0.0)
    Scale(gr_vel,body[b]->gravity,ts*0.5);
   else
    Zero(gr_vel);
   //AddScaledTo(vel,body[b]->gravity,ts);//use timestep taken from the contact
    
   if(b==0)
   {
    v[0]+=DotProduct(vel,normal);
    v[1]+=DotProduct(vel,tangent[0])+DotProduct(gr_vel,tangent[0]);
    v[2]+=DotProduct(vel,tangent[1])+DotProduct(gr_vel,tangent[1]);
   }
   else
   {
    v[0]-=DotProduct(vel,normal);
    v[1]-=DotProduct(vel,tangent[0])+DotProduct(gr_vel,tangent[0]);
    v[2]-=DotProduct(vel,tangent[1])+DotProduct(gr_vel,tangent[1]);
   }
  }
 }
 else
 {//frictionless
  v[0]=0.0;
  for(b=0;b<2;b++)
  if(body[b])
  {
   body[b]->GetInverseTOIWorld(TOI_i[b]);
   body[b]->GetPosition(relp[b]);
   Subtract(relp[b],point,relp[b]);
   real vel[3];
   CrossProduct(vel,body[b]->GetAngularVelocity(),relp[b]);
   AddTo(vel,body[b]->GetVelocity());

   if(ts>0.0)
    AddScaledTo(vel,body[b]->gravity,ts);
   //else if(ts<0.0)
   // AddScaledTo(vel,body[b]->gravity,time_step);//use timestep taken from the contact

   if(b==0)
    v[0]+=DotProduct(vel,normal);
   else
    v[0]-=DotProduct(vel,normal);
  }
 }
 if(relaxation_type==2)
  v[0]-=penetration/relaxation_time;
}

void CContact::ComputeA()
{
 real mi;

 if(friction!=0.0)
 {
  if(body[0]!=0&&body[1]==0||body[1]!=0&&body[0]==0)
  {//one body is world
   int b=0;
   if(body[1]) b=1;

   CrossProduct(rxNt[b][0],relp[b],normal);
   CrossProduct(rxNt[b][1],relp[b],tangent[0]);
   CrossProduct(rxNt[b][2],relp[b],tangent[1]);
   //Multiply TOI_i[b] * rxN
   int j,k;
   for(j=0;j<3;j++)
   for(k=0;k<3;k++)
    TOI_i_rxNt[b][k][j]=DotProduct(TOI_i[b][j],rxNt[b][k]);
   //Multiply rxN^T * TOI_i_rxNt
   for(j=0;j<3;j++)
   for(k=j+1;k<3;k++)
    A[k][j]=A[j][k]=DotProduct(rxNt[b][j],TOI_i_rxNt[b][k]);
   A[0][0]=DotProduct(rxNt[b][0],TOI_i_rxNt[b][0]);
   A[1][1]=DotProduct(rxNt[b][1],TOI_i_rxNt[b][1]);
   A[2][2]=DotProduct(rxNt[b][2],TOI_i_rxNt[b][2]);
   massi_angular[b]=A[0][0];
   mi=1.0/body[b]->GetMass();
   A[0][0]+=mi;
   A[1][1]+=mi;
   A[2][2]+=mi;
   ASSERT(A[0][0]>0.0);
   ASSERT(A[1][1]>0.0);
   ASSERT(A[2][2]>0.0);
  }
  else
  {//both bodies are non-zero
   Zero(A[0]);
   Zero(A[1]);
   Zero(A[2]);
   //compute local mass inverted
   real local_massi[3][3];
   real rt=relp[0][0]*TOI_i[0][1][2];
   real diff_rt0=relp[0][1]*TOI_i[0][0][1]-relp[0][0]*TOI_i[0][1][1];
   real diff_rt1=relp[0][0]*TOI_i[0][2][2]-relp[0][2]*TOI_i[0][0][2];
   real diff_rt2=relp[0][1]*TOI_i[0][0][2]-rt;
   real diff_rt3=relp[0][1]*TOI_i[0][0][0]-relp[0][0]*TOI_i[0][0][1];
   local_massi[0][0]=relp[0][1]*relp[0][1]*TOI_i[0][2][2]+(relp[0][2]*TOI_i[0][1][1]-2.0*relp[0][1]*TOI_i[0][1][2])*relp[0][2];
   local_massi[1][1]=relp[0][2]*(relp[0][2]*TOI_i[0][0][0]-relp[0][0]*TOI_i[0][0][2])+diff_rt1*relp[0][0];
   local_massi[2][2]=diff_rt3*relp[0][1]-diff_rt0*relp[0][0];
   local_massi[0][1]=local_massi[1][0]=relp[0][2]*(rt-relp[0][2]*TOI_i[0][0][1])-diff_rt1*relp[0][1];
   local_massi[0][2]=local_massi[2][0]=diff_rt0*relp[0][2]-diff_rt2*relp[0][1];
   local_massi[1][2]=local_massi[2][1]=diff_rt2*relp[0][0]-diff_rt3*relp[0][2];
   
   real acc[3];
   Multiply(acc,local_massi,normal);
   massi_angular[0]=DotProduct(acc,normal);

   rt=relp[1][0]*TOI_i[1][1][2];
   diff_rt0=relp[1][1]*TOI_i[1][0][1]-relp[1][0]*TOI_i[1][1][1];
   diff_rt1=relp[1][0]*TOI_i[1][2][2]-relp[1][2]*TOI_i[1][0][2];
   diff_rt2=relp[1][1]*TOI_i[1][0][2]-rt;
   diff_rt3=relp[1][1]*TOI_i[1][0][0]-relp[1][0]*TOI_i[1][0][1];
   local_massi[0][0]+=relp[1][1]*relp[1][1]*TOI_i[1][2][2]+(relp[1][2]*TOI_i[1][1][1]-2.0*relp[1][1]*TOI_i[1][1][2])*relp[1][2];
   local_massi[1][1]+=relp[1][2]*(relp[1][2]*TOI_i[1][0][0]-relp[1][0]*TOI_i[1][0][2])+diff_rt1*relp[1][0];
   local_massi[2][2]+=relp[1][1]*diff_rt3-diff_rt0*relp[1][0];
   real tmp1[3];
   tmp1[0]=relp[1][2]*(rt-relp[1][2]*TOI_i[1][0][1])-diff_rt1*relp[1][1];
   tmp1[1]=diff_rt0*relp[1][2]-diff_rt2*relp[1][1];
   tmp1[2]=diff_rt2*relp[1][0]-diff_rt3*relp[1][2];
   local_massi[0][1]+=tmp1[0];
   local_massi[0][2]+=tmp1[1];
   local_massi[1][2]+=tmp1[2];
   local_massi[1][0]+=tmp1[0];
   local_massi[2][0]+=tmp1[1];
   local_massi[2][1]+=tmp1[2];
   //Congruence transform with N^T matrix
   real D[3][3];
   int i;
   for(i=0;i<3;i++)
   {
    D[0][i]=DotProduct(local_massi[i],normal);
    D[1][i]=DotProduct(local_massi[i],tangent[0]);
    D[2][i]=DotProduct(local_massi[i],tangent[1]);
   }
   A[0][0]=DotProduct(D[0],normal);
   A[1][1]=DotProduct(D[1],tangent[0]);
   A[2][2]=DotProduct(D[2],tangent[1]);
   A[1][0]=A[0][1]=DotProduct(D[0],tangent[0]);
   A[2][0]=A[0][2]=DotProduct(D[0],tangent[1]);
   A[2][1]=A[1][2]=DotProduct(D[1],tangent[1]);

   massi_angular[1]=A[0][0]-massi_angular[0];

   mi=1.0/body[0]->GetMass()+1.0/body[1]->GetMass();
   CrossProduct(rxNt[0][0],relp[0],normal);
   CrossProduct(rxNt[0][0],relp[1],normal);

   A[0][0]+=mi;
   A[1][1]+=mi;
   A[2][2]+=mi;
   ASSERT(A[0][0]>0.0);
   ASSERT(A[1][1]>0.0);
   ASSERT(A[2][2]>0.0);
  }
 }
 else
 {//frictionless
  int b;
  A[0][0]=0.0;

  for(b=0;b<2;b++)
  if(body[b])
  {
   CrossProduct(rxNt[b][0],relp[b],normal);
   //Multiply TOI_i[b] * rxN
   int j;
   real tmp[3];
   for(j=0;j<3;j++)
    tmp[j]=DotProduct(TOI_i[b][j],rxNt[b][0]);
   //Multiply rxN^T * tmp
   A[0][0]+=DotProduct(rxNt[b][0],tmp);
   massi_angular[b]=A[0][0];
   mi=1.0/body[b]->GetMass();
   A[0][0]+=mi;
  }
 }
}

void CContact::ComputeNormalMassi()
{
 int b;

 for(b=0;b<2;b++)
 if(body[b])
 {
  real rxN[3];
  CrossProduct(rxN,relp[b],normal);
  real tmp[3];
  Multiply(tmp,TOI_i[b],rxN);
  massi_angular[b]=DotProduct(rxN,tmp);
 }
}

void CContact::ResolveImpact(real delta_velocity[2][3],real delta_omega[2][3],real normal_dv)
{//Resolves impact
 if(!A_computed)
 {
  ComputeA();
  A_computed=1;
  normal_mass_computed=1;
 }
 if(friction==0.0)
 {
  real f=normal_dv/A[0][0];
  //apply normal f
  if(body[0])
  {
   Scale(delta_velocity[0],normal,f/body[0]->GetMass());
   real torque_body[3];
   //CrossProduct(torque_body,relp[0],normal);
   Scale(torque_body,rxNt[0][0],f);
   Multiply(delta_omega[0],TOI_i[0],torque_body);
   real velocity[3];
   body[0]->GetVelocity(velocity);
   AddTo(velocity,delta_velocity[0]);
   body[0]->SetVelocity(velocity);
   body[0]->GetAngularVelocity(velocity);
   AddTo(velocity,delta_omega[0]);
   body[0]->SetAngularVelocity(velocity);
  }
  if(body[1])
  {
   Scale(delta_velocity[1],normal,-f/body[1]->GetMass());
   real torque_body[3];
   //CrossProduct(torque_body,relp[1],normal);
   Scale(torque_body,rxNt[1][0],-f);
   //SelfScale(torque_body,-f);
   Multiply(delta_omega[1],TOI_i[1],torque_body);
   real velocity[3];
   body[1]->GetVelocity(velocity);
   AddTo(velocity,delta_velocity[1]);
   body[1]->SetVelocity(velocity);
   body[1]->GetAngularVelocity(velocity);
   AddTo(velocity,delta_omega[1]);
   body[1]->SetAngularVelocity(velocity);
  }
 }
 else
 {
  real f[3];
  //find clamped friction impulse
  real rhs[3]={normal_dv,-v[1],-v[2]};
  solve3x3_cholesky(f,A,rhs);
  real td=sqrt(f[1]*f[1]+f[2]*f[2]);
  //We don't check opposing condition since it can be shown that if
  //clamped friction solution is inside the cone we can drive to the
  //same configuration without friction to introduce energy.
  if(td>friction*f[0])
  {
   f[0]=rhs[0]/(A[0][0]+A[0][1]*friction*f[1]/td+A[0][2]*friction*f[2]/td);
   if(f[0]<0.0)
   {
    ASSERT("Doing frictionless"&&0);
    f[0]=-v[0]*(1+restitution)/A[0][0];
    f[1]=0.0;
    f[2]=0.0;
   }
   else
   {
    //ASSERT(f[1]*v[1]+f[2]*v[2]<=0.0&&"Not serious. Opposing violation")
    //If the violation exists it occures only with small tangential velocity
    f[1]*=friction*f[0]/td;
    f[2]*=friction*f[0]/td;
   }
  }

  //apply calculated contact force
  real force[3];
  TripleScaledSum(force,normal,f[0],tangent[0],f[1],tangent[1],f[2]);
  
  if(body[0]==0||body[1]==0)
  {
   int b=0;
   if(body[1]) b=1;
   MultiplyTranspose(delta_omega[b],TOI_i_rxNt[b],f);
   Scale(delta_velocity[b],force,1.0/body[b]->GetMass());
   real velocity[3];
   body[b]->GetVelocity(velocity);
   AddTo(velocity,delta_velocity[b]);
   body[b]->SetVelocity(velocity);
   body[b]->GetAngularVelocity(velocity);
   AddTo(velocity,delta_omega[b]);
   body[b]->SetAngularVelocity(velocity);
  }
  else
  {
   Scale(delta_velocity[0],force,1.0/body[0]->GetMass());
   real torque_body[3];
   CrossProduct(torque_body,relp[0],force);
   Multiply(delta_omega[0],TOI_i[0],torque_body);
   real velocity[3];
   body[0]->GetVelocity(velocity);
   AddTo(velocity,delta_velocity[0]);
   body[0]->SetVelocity(velocity);
   body[0]->GetAngularVelocity(velocity);
   AddTo(velocity,delta_omega[0]);
   body[0]->SetAngularVelocity(velocity);

   Scale(delta_velocity[1],force,-1.0/body[1]->GetMass());
   CrossProduct(torque_body,force,relp[1]);//negative sign
   Multiply(delta_omega[1],TOI_i[1],torque_body);
   body[1]->GetVelocity(velocity);
   AddTo(velocity,delta_velocity[1]);
   body[1]->SetVelocity(velocity);
   body[1]->GetAngularVelocity(velocity);
   AddTo(velocity,delta_omega[1]);
   body[1]->SetAngularVelocity(velocity);
  }
 }
}

void CContact::ResolvePenetration(real delta_velocity[2][3],real delta_omega[2][3],real h[2],real penetration,int relaxation_type)
{
 ASSERT(penetration>0.0);
 real pa[2],pl[2];
 int b;

 if(relaxation_type==0)
 {
  if(!normal_mass_computed)
  {
   ComputeNormalMassi();
   normal_mass_computed=1;
  }
  real mit=0.0;
  if(body[0])
   mit+=massi_angular[0]+1.0/body[0]->GetMass();
  if(body[1])
  {
   mit+=massi_angular[1]+1.0/body[1]->GetMass();
   pa[1]=-penetration*massi_angular[1]/mit;
   pl[1]=-penetration/(mit*body[1]->GetMass());
  }
  if(body[0])
  {
   pa[0]=penetration*massi_angular[0]/mit;
   pl[0]=penetration/(mit*body[0]->GetMass());
  }
  for(b=0;b<2;b++)
  if(body[b])
  {
   //calculate h: dq will be represented as {domega,h}
   real t[3];
   CrossProduct(t,normal,relp[b]);
   Multiply(delta_omega[b],TOI_i[b],t);
   CrossProduct(t,delta_omega[b],relp[b]);
   h[b]=pa[b]/DotProduct(t,normal);
   ASSERT(h[b]!=0.0);

   Scale(delta_velocity[b],normal,pl[b]/h[b]);

   //change the body's position
   real pos[3];
   body[b]->GetPosition(pos);
   AddScaledTo(pos,normal,pl[b]);
   body[b]->SetPosition(pos);
   //change the body's quaternion
   real Q[4];
   body[b]->GetQuaternion(Q);
   apply_omega(Q,delta_omega[b],h[b]);
   body[b]->SetQuaternion(Q);
  }
 }
 else
 {
  real mit=0.0;
  if(body[0])
   mit+=1.0/body[0]->GetMass();
  if(body[1])
  {
   mit+=1.0/body[1]->GetMass();
   pa[1]=0.0;
   pl[1]=-penetration/(mit*body[1]->GetMass());
  }
  if(body[0])
  {
   pa[0]=0.0;
   pl[0]=penetration/(mit*body[0]->GetMass());
  }
  for(b=0;b<2;b++)
  if(body[b])
  {
   Zero(delta_omega[b]);
   h[b]=1.0;
   Scale(delta_velocity[b],normal,pl[b]);
   //change the body's position
   real pos[3];
   body[b]->GetPosition(pos);
   AddScaledTo(pos,normal,pl[b]);
   body[b]->SetPosition(pos);
  }
 }
#ifdef _DEBUG
 if(body[0]==0)
  pa[0]=pl[0]=0.0;
 if(body[1]==0)
  pa[1]=pl[1]=0.0;
 ASSERT(IS_EQ(pa[0]+pl[0]-pa[1]-pl[1],penetration,1e-6));
#endif//of #ifdef _DEBUG
}

FC_DLL_EXPORT
void CollisionHandler(CContact *c, int NO_contacts, int relaxation_type, real relaxation_time, real time_step, real velocity_tolerance, real penetration_tolerance)
{
 //relaxation_type=0:projection
 //relaxation_type=1:linear projection
 //relaxation_type=2:relaxation with relaxation_time
 int i,iter,index;
 real delta_velocity[2][3],delta_omega[2][3],h[2];
 real max_penetration,max_vel;
 real cp[3];

 ASSERT(relaxation_type==0||relaxation_type==1||relaxation_type==2);
 ASSERT(time_step>=0.0);
 ASSERT(velocity_tolerance>=0.0);
 ASSERT(penetration_tolerance>=0.0);
 ASSERT(NO_contacts>=0);
 for(i=0;i<NO_contacts;i++)
 {
  //check stuff
  //ASSERT(c[i].penetration>=0.0);
  ASSERT(c[i].body[0]||c[i].body[1]);
  ASSERT(c[i].friction>=0.0);
  ASSERT("Normal must be normalized"&&IS_EQ(Magnitude(c[i].normal),1.0,1e-6));
  c[i].Initialize(time_step,relaxation_type,relaxation_time);
  c[i].A_computed=0;
  c[i].normal_mass_computed=0;
 } 
 iter=0;
 while(iter<NO_contacts*4)
 {
  //find contact with maximum magnitude of negative velocity
  max_vel=-velocity_tolerance;
  index=-1;
  for(i=0;i<NO_contacts;i++)
  {
   real dv=c[i].v[0]*(1.0+c[i].restitution);
   if(dv<max_vel)
   {
    max_vel=dv;
    index=i;
   }
  }
  if(index==-1)break;
  ASSERT(max_vel<0.0);
  c[index].ResolveImpact(delta_velocity,delta_omega,-max_vel+velocity_tolerance);
  //update contact velocities
  for(i=0;i<NO_contacts;i++)
  {
   if(c[i].body[0])
   {
    if(c[i].body[0]==c[index].body[0])
    {
     CrossProduct(cp,delta_omega[0],c[i].relp[0]);
     AddTo(cp,delta_velocity[0]);
     c[i].v[0]+=DotProduct(c[i].normal,cp);
     if(c[i].friction!=0.0)
     {
      c[i].v[1]+=DotProduct(c[i].tangent[0],cp);
      c[i].v[2]+=DotProduct(c[i].tangent[1],cp);
     }
    }
    else if(c[i].body[0]==c[index].body[1])
    {
     CrossProduct(cp,delta_omega[1],c[i].relp[0]);
     AddTo(cp,delta_velocity[1]);
     c[i].v[0]+=DotProduct(c[i].normal,cp);
     if(c[i].friction!=0.0)
     {
      c[i].v[1]+=DotProduct(c[i].tangent[0],cp);
      c[i].v[2]+=DotProduct(c[i].tangent[1],cp);
     }
    }
   }
   if(c[i].body[1])
   {
    if(c[i].body[1]==c[index].body[0])
    {
     CrossProduct(cp,delta_omega[0],c[i].relp[1]);
     AddTo(cp,delta_velocity[0]);
     c[i].v[0]-=DotProduct(c[i].normal,cp);
     if(c[i].friction!=0.0)
     {
      CrossProduct(cp,delta_omega[0],c[i].relp[1]);
      c[i].v[1]-=DotProduct(c[i].tangent[0],cp);
      c[i].v[2]-=DotProduct(c[i].tangent[1],cp);
     }
    }
    else if(c[i].body[1]==c[index].body[1])
    {
     CrossProduct(cp,delta_omega[1],c[i].relp[1]);
     AddTo(cp,delta_velocity[1]);
     c[i].v[0]-=DotProduct(c[i].normal,cp);
     if(c[i].friction!=0.0)
     {
      c[i].v[1]-=DotProduct(c[i].tangent[0],cp);
      c[i].v[2]-=DotProduct(c[i].tangent[1],cp);
     }
    }
   }
  }
#ifdef _DEBUG
  /* //causes state corrupt assert
  ASSERT(c[index].v[0]>-1e-6+velocity_tolerance);
  for(i=0;i<NO_contacts;i++)
  {
   real cc[3];
   Copy(cc,c[i].v);
   c[i].Initialize(time_step,relaxation_type,relaxation_time);
   ASSERT(IS_EQ(cc[0],c[i].v[0],1e-6));
   ASSERT(IS_EQ(cc[1],c[i].v[1],1e-6));
   ASSERT(IS_EQ(cc[2],c[i].v[2],1e-6));
  }
  */
#endif//of #ifdef _DEBUG
  iter++;
 }
 ASSERT("May happen occasionally. COLLISION HANDLER."&&iter<NO_contacts*4&&NO_contacts);

 if(relaxation_type!=2)
 {
  //Now compensate for penetration
  iter=0;
  while(iter<NO_contacts*4)
  {
   //find biggest penetration
   max_penetration=penetration_tolerance;
   index=-1;
   for(i=0;i<NO_contacts;i++)
   if(c[i].penetration>max_penetration)
   {
    max_penetration=c[i].penetration;
    index=i;
   }
   if(index==-1)break;
   ASSERT(max_penetration>0.0);
   c[index].ResolvePenetration(delta_velocity,delta_omega,h,max_penetration-penetration_tolerance*0.5,relaxation_type);
   //update penetrations
   for(i=0;i<NO_contacts;i++)
   {
    if(c[i].body[0])
    {
     if(c[i].body[0]==c[index].body[0])
     {
      CrossProduct(cp,delta_omega[0],c[i].relp[0]);
      AddTo(cp,delta_velocity[0]);
      c[i].penetration-=h[0]*DotProduct(cp,c[i].normal);//0
     }
     else if(c[i].body[0]==c[index].body[1])
     {
      CrossProduct(cp,delta_omega[1],c[i].relp[0]);
      AddTo(cp,delta_velocity[1]);
      c[i].penetration-=h[1]*DotProduct(cp,c[i].normal);//-
     }
    }
    if(c[i].body[1])
    {
     if(c[i].body[1]==c[index].body[0])
     {
      CrossProduct(cp,delta_omega[0],c[i].relp[1]);
      AddTo(cp,delta_velocity[0]);
      c[i].penetration+=h[0]*DotProduct(cp,c[i].normal);//+
     }
     else if(c[i].body[1]==c[index].body[1])
     {
      CrossProduct(cp,delta_omega[1],c[i].relp[1]);
      AddTo(cp,delta_velocity[1]);
      c[i].penetration+=h[1]*DotProduct(cp,c[i].normal);//+
     }
    }
   }
   iter++;
  }
  ASSERT("May happen occasionally. PROJECTOR."&&iter<NO_contacts*4&&NO_contacts);
 }
}
