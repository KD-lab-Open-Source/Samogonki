#ifndef __QUAT_MATH__
#define __QUAT_MATH__

inline QuatF derivation_by_Wglobal(const QuatF& q, const Vect3f& w)
{
	return QuatF(
	0.5f * (-q.x() * w.x - q.y() * w.y - q.z() * w.z),
	0.5f * ( q.s() * w.x + q.z() * w.y - q.y() * w.z),
	0.5f * ( -q.z() * w.x + q.s() * w.y + q.x() * w.z),
	0.5f * (q.y() * w.x - q.x() * w.y + q.s() * w.z));
}

inline QuatF derivation_by_Wlocal(const QuatF& q, const Vect3f& w)
{
	return QuatF(
	0.5 * (-q.x() * w.x - q.y() * w.y - q.z() * w.z),
	0.5 * ( q.s() * w.x - q.z() * w.y + q.y() * w.z),
	0.5 * ( q.z() * w.x + q.s() * w.y - q.x() * w.z),
	0.5 * (-q.y() * w.x + q.x() * w.y + q.s() * w.z));
}

inline Vect3f calc_Wglobal(const QuatF& q, const QuatF& dq)
{
	return Vect3f( 
		-q.x()*dq.s() + q.s()*dq.x() - q.z()*dq.y() + q.y()*dq.z(),
		-q.y()*dq.s() + q.z()*dq.x() + q.s()*dq.y() - q.x()*dq.z(),
		-q.z()*dq.s() - q.y()*dq.x() + q.x()*dq.y() + q.s()*dq.z() )*2;
}

inline Vect3f calc_Wlocal(const QuatF& q, const QuatF& dq)
{
	return Vect3f( 
		-q.x()*dq.s() + q.s()*dq.x() + q.z()*dq.y() - q.y()*dq.z(),
		-q.y()*dq.s() - q.z()*dq.x() + q.s()*dq.y() + q.x()*dq.z(),
		-q.z()*dq.s() + q.y()*dq.x() - q.x()*dq.y() + q.s()*dq.z() )*2;
}

#endif // __QUAT_MATH__
