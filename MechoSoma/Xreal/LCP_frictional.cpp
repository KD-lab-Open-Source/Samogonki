////////////////////////////////////////////////////////////////////////////////////////
// Implementation of pivoting method by David Baraff.
// See his article "Fast Contact Force Computation for 
// Nonpenetrating Rigid Bodies", SIGGRAPH 94, 
// July 24-29 for details.
////////////////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "LCP_Solver.h"
#include "CHOLESKY.H"
#include "Statistics.h"

//#define _FrictionalListing_

void LCP_Solver::compute_force_frictional()
{
//	iterations = 0;
	prp_t = _CLEAN_F_;
	C_size_frictional = C_size;
	f_t = 0;
	a_t = q_t + Mtn*f;

	while(1){

		/////////////////////////////////////////////////////////////
		//  Установка касательных условий
		int d;
		drive_friction = _CLEAN_F_;
		for(d = 1; d <= size_frictional; d++){
			if(prp_t(d) != _CLEAN_F_) // условие уже установлено
				continue;

			switch(prp(fn_index(d))){
				case _C_:
					if(fabs(a_t(d)) > a_t_zero_threshould)
						drive_to_zero_frictional(d);
					else
						// нет касательного ускорения
						prp_t(d) = a_t(d) < 0 ? _NCP_ : _NCM_;
					break;

				case _NC_:  // нет нормальной силы на данном контакте
					if(fabs(f_t(d)) > LCP_delta_tolerance)  // убираем ранее установленную касательную силу
						drive_to_zero_frictional(d);
					else
						prp_t(d) = a_t(d) < 0 ? _NCP_ : _NCM_;
					break;

				case _CLEAN_:  // устанавливать касательное условие после установки нормального 
					break;
				}
			}
	
		/////////////////////////////////////////////////////////////
		//  Установка нормальных условий 	
		drive_friction = 0;
		for(d = 1; d <= size; d++){
			if(prp(d) != _CLEAN_)
				continue;
			if(a(d) < 0)
				drive_to_zero_frictional(d);
			else
				prp(d) = _NC_;
			}
		
		int done = 1;
		for(d = 1; d <= size; d++)
			if(prp(d) == _CLEAN_){
				done = 0;
				break;
				}
		for(d = 1; d <= size_frictional; d++)
			if(prp_t(d) == _CLEAN_F_){
				done = 0;
				break;
				}
		if(done)
			break;
		}

#ifdef _FINAL_VERSION_
	statistics_add(size, 5, size);
	statistics_add(size_frictional, 5, size_frictional);
	statistics_add(iterations, 5, iterations);
	check_solution();
#endif
}

void LCP_Solver::drive_to_zero_frictional(int d)
{
	while(1){
		mem_store();

		fdirection_frictional(d);

		da = M*df + Mnt*df_t;
		da_t = Mtn*df + Mtt*df_t;

		int index;
		ContactProperty pivot_mode;
		double step = maxstep_frictional(d, index, pivot_mode);
		
#ifdef _FrictionalListing_
		lout << "d: " << d << endl;
		lout << "drive_friction: " << drive_friction << endl;
		lout << "C_size_frictional: " << C_size_frictional << endl;
		lout << "step: " << step << endl;
		lout << "index: " << index << endl;
		lout << "prev_state: " << (pivot_mode >= _CLEAN_F_ ? prp_t(index) : prp(index)) << endl;
		lout << "pivot_mode: " << pivot_mode << endl;
		print_state();
#endif

		f = f + Vect(size, step)*df;
		a = a + Vect(size, step)*da;
		f_t = f_t + Vect(size_frictional, step)*df_t;
		a_t = a_t + Vect(size_frictional, step)*da_t;

		mem_restore();
		
		ContactProperty prev_state;
		if(pivot_mode >= _CLEAN_F_){
			prev_state = (ContactProperty)prp_t(index);
			prp_t(index) = pivot_mode;
			}
		else{
			prev_state = (ContactProperty)prp(index);
			prp(index) = pivot_mode;
			}

		switch(pivot_mode){
			//////////////////////////////////////
			//	Frictional
			case _CP_:
			case _CM_:
				if(!(prev_state & _CF_))
					C_size_frictional++;
				break;

			case _NCP_:
			case _NCM_:
				if(prev_state & _CF_)
					C_size_frictional--;
				break;

			case _CLEAN_F_:
				if(prev_state & _CF_)
					C_size_frictional--;
				break;

			/////////////////////////////////////
			//	Normal
			case _CLEAN_:
			case _NC_: {
				int index_t = ft_index(index);	// проверяем условие на трение, если есть 			
				if(index_t){
					if(prp_t(index_t) & _CF_)  // было clamped - убираем
						C_size_frictional--;
					prp_t(index_t) = _CLEAN_F_;  // очищаем
					}
				if(prev_state == _C_)
					C_size_frictional--;
				break; }

			case _C_:
//				assert(0); // если есть касательное условие, то его надо, возможно, очистить
				C_size_frictional++;
				break;
			}
	
		if(index == d && drive_friction == prev_state) {
			return;
        }

		if(iterations++ > LCP_max_iterations) {
            printf("ERR! LCP_Solver: too many iterations, terminating...");
            return;
        }
    }
}
	
void LCP_Solver::fdirection_frictional(int d)
{
	df = 0;
	df_t = 0;
	double dir = 0;
	
	if(drive_friction)
		if(prp(fn_index(d)) == _C_)
			df_t(d) = dir = -SIGN(a_t(d));  // настраиваем касательную силу, чтобы минимизировать касательное ускорение
		else
			df_t(d) = dir = -SIGN(f_t(d)); // убираем касательную силу, т.к. нет нормальной
	else
		df(d) = 1; // настраиваем нормальную силу

	if(!C_size_frictional)
		return;

	//									   
	//	| Ac_c + mu*Ac_ncf       Ac_cf  |     |df_n|		     |-v1*dir|	
	//	|						  | *  |       |	=	|           | 
	//	|  Acf_c + mu*Acf_ncf	Acf_cf  |     |df_t |		      |-v3*dir|	
	//							                 
	

	Mat Acc(C_size_frictional, C_size_frictional);
	Vect v1(C_size_frictional);
	Vector<int> C_index(size); // first: n -> c, second: t -> c

	int ii, i;
	for(ii = 1, i = 1; i <= size; i++)
		if(prp(i) == _C_){
			//  Ac_c
			int jj, j;
			for(jj = 1, j = 1; j <= size; j++)
				if(prp(j) == _C_)
					Acc(ii, C_index(j) = jj++) = M(i, j);

			//  Ac_cf
			for(j = 1; j <= size_frictional; j++)
				if(prp_t(j) & _CF_)
					Acc(ii, jj++) = Mnt(i, j);
				else // _NCF_
					if(prp_t(j) & _NCF_ && prp(fn_index(j)) == _C_)
						//  Ac_c += mu*Ac_ncf
						Acc(ii, C_index(fn_index(j)) ) += prp_t(j) == _NCP_ ? mu_t(j)*Mnt(i, j) : -mu_t(j)*Mnt(i, j);
					
			v1(ii++) = -(drive_friction ? dir*Mnt(i, d) : M(i, d));
			}

	for(i = 1; i <= size_frictional; i++)
		if(prp_t(i) & _CF_){
			//  Acf_c
			int jj, j;
			for(jj = 1, j = 1; j <= size; j++)
				if(prp(j) == _C_)
					Acc(ii, C_index(j) = jj++) = Mtn(i, j);

			//  Acf_cf
			for(j = 1; j <= size_frictional; j++)
				if(prp_t(j) & _CF_)
					Acc(ii, jj++) = Mtt(i, j);
				else  //  _NCF_
					if(prp_t(j) & _NCF_ && prp(fn_index(j)) == _C_)
						//  Acf_c += mu*Acf_ncf
						Acc(ii, C_index(fn_index(j)) ) += prp_t(j) == _NCP_ ? mu_t(j)*Mtt(i, j) : -mu_t(j)*Mtt(i, j);

			v1(ii++) = -(drive_friction ? dir*Mtt(i, d) : Mtn(i, d));
			}


	solve(Acc, v1);

	for(ii = i = 1; i <= size; i++)
		if(prp(i) == _C_)
			df(i) = v1(ii++);

	for(i = 1; i <= size_frictional; i++)
		if(prp_t(i) & _CF_)
			df_t(i) = v1(ii++);
		else
			if(prp_t(i) & _NCF_ && prp(fn_index(i)) == _C_)
				df_t(i) = prp_t(i) == _NCP_ ? mu_t(i)*df(fn_index(i)) : -mu_t(i)*df(fn_index(i));
}

double LCP_Solver::maxstep_frictional(int d, int& index, ContactProperty& pivot_mode)
{
	double step = DBL_INF;
	index = -1;
	
	///////////////////////////////////////
	// Check Driving contact
	if(drive_friction){	
		if(a_t(d)*da_t(d) < 0 && prp(fn_index(d)) == _C_ && fabs(da_t(d)) > LCP_delta_tolerance){
			double s = -a_t(d)/da_t(d);
			if(s < step){
				step = s;
				index = d;
				pivot_mode = f_t(d) + df_t(d) > 0 ? _CP_ : _CM_;
				}
			}

		if(df_t(d) == 1){
			double dft = df_t(d) - mu_t(d)*df(fn_index(d));
			if(dft > LCP_delta_tolerance){
				double s = (mu_t(d)*f(fn_index(d)) - f_t(d))/dft;
				if(s < step){
					step = s;
					index = d;
					pivot_mode = _NCP_;
					}
				}
			}
		else{
			double dft = df_t(d) + mu_t(d)*df(fn_index(d));
			if(dft < -LCP_delta_tolerance){
				double s = -(mu_t(d)*f(fn_index(d)) + f_t(d))/dft;
				if(s < step){
					step = s;
					index = d;
					pivot_mode = _NCM_;
					}
				}
			}
		}
	else
		if(da(d) > LCP_delta_tolerance){
			index = d;
			step = -a(d)/da(d);
			pivot_mode = _C_;
			}
		
	/////////////////////////////////////////
	// Check Normal contacts
		int i;
	for(i = 1;i <= size; i++)
		switch(prp(i)){
			case _C_:
				if(df(i) < -LCP_delta_tolerance){
					double s = -f(i)/df(i);
					if(s < step){
						step = s;
						index = i;
						pivot_mode = _NC_;
						}
					}
				break;

			case _NC_:
				if(da(i) < -LCP_delta_tolerance){
					double s = -a(i)/da(i);
					if(s < LCP_delta_tolerance){
						index = i;
						pivot_mode = _CLEAN_;
						return 0;
						}
					else
						if(s < step){
							step = s;
							index = i;
							pivot_mode = _C_;
							}
					}
				break;
			}


	////////////////////////////////////////////
	// Check Frictional contacts
	for(i = 1;i <= size_frictional; i++)
		switch(prp_t(i)){
			case _CP_:
				if(df_t(i) < -LCP_delta_tolerance){
					double s = -f_t(i)/df_t(i);
					if(s < step){
						step = s;
						index = i;
						pivot_mode = _CM_;
						}
					}
				else{
					double dft = df_t(i) - mu_t(i)*df(fn_index(i));					
					if(dft > LCP_delta_tolerance){
						double s = (mu_t(i)*f(fn_index(i)) - f_t(i))/dft;
						if(s < step){
							step = s;
							index = i;
							pivot_mode = _NCP_;
							}
						}
					}
				break;

			case _CM_:
				if(df_t(i) > LCP_delta_tolerance){
					double s = -f_t(i)/df_t(i);
					if(s < step){
						step = s;
						index = i;
						pivot_mode = _CP_;
						}
					}
				else{
					double dft = df_t(i) + mu_t(i)*df(fn_index(i));					
					if(dft < -LCP_delta_tolerance){
						double s = -(mu_t(i)*f(fn_index(i)) + f_t(i))/dft;
						if(s < step){
							step = s;
							index = i;
							pivot_mode = _NCM_;
							}
						}
					}
				break;

			case _NCP_: // a_t < 0
				if(da_t(i) > LCP_delta_tolerance){
					double s = -a_t(i)/da_t(i);
					if(s < LCP_delta_tolerance){
						index = i;
						pivot_mode = _CLEAN_F_;
						return 0;
						}
					else
						if(s < step){
							step = s;
							index = i;
							pivot_mode = _CP_;
							}
					}
				break;

			case _NCM_: // a_t > 0
				if(da_t(i) < -LCP_delta_tolerance){
					double s = -a_t(i)/da_t(i);
					if(s < LCP_delta_tolerance){
						index = i;
						pivot_mode = _CLEAN_F_;
						return 0;
						}
					else
						if(s < step){
							step = s;
							index = i;
							pivot_mode = _CM_;
							}
					}
				break;
			}


	if(index == -1)
		abort("LCP_Solver: maxstep_frictional fails");

//	assert(step > -1e-10);
	
	return step;
}

