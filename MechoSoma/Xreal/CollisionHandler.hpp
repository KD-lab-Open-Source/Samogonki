
#define CRigidBody DynamicBody
class DynamicBody;
class CContact
{
public:
 CRigidBody* body[2];
 real normal[3];
 real point[3];
 real penetration;
 real friction;
 real restitution;
private:
 real relp[2][3];
 real tangent[2][3];
 real TOI_i[2][3][3];
 real A[3][3];
 real v[3];
 real rxNt[2][3][3];
 real TOI_i_rxNt[2][3][3];
 real massi_angular[2];//for projection

 int A_computed;
 int normal_mass_computed;

 void Initialize(real ts, int relaxation_type,real relaxation_time);
 void ComputeA();
 void ComputeNormalMassi();
 void ResolveImpact(real delta_velocity[2][3],real delta_omega[2][3],real normal_dv);
 void ResolvePenetration(real delta_velocity[2][3],real delta_omega[2][3],real h[2],real penetration,int relaxation_type);

 friend void CollisionHandler(CContact *c, int number_of_contacts, int relaxation_type, real relaxation_time, real time_step, real velocity_tolerance, real penetration_tolerance);
};

extern int contacts_vector_size;
extern CContact contacts_vector[];
