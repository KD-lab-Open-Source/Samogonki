//////////////////////////////////////////////////////////////////
//  Таймеры для отсчета длительностей
//
//  1. Время измеряется в милисекундах.
//
//  2. Таймеры: 
//	отмерение времени
//	задержка события
//	задержка true-условия с "усреднением"
//	выполнение в течении указанного времени
//
//  3. Сброс таймеров - stop().
//
//  4. Типы синхронизации (через SyncroTimer)
//	по clocki()
//	по frames - с указанием ориентировочного FPS
//
//////////////////////////////////////////////////////////////////
#ifndef	__DURATION_TIMER_H__
#define __DURATION_TIMER_H__

#include "SynchroTimer.h"

extern SyncroTimer global_time;
extern SyncroTimer non_stop_global_time;

///////////////////////////////////////////////////////////////////////////////////////////////
//		Ordinary Timers
///////////////////////////////////////////////////////////////////////////////////////////////

class BaseTimer {
protected:
	time_type start_time;
public:
	BaseTimer() { start_time = 0; }
	void stop() { start_time = 0; }
};

// Измерение времени
class MeasurementTimer : public BaseTimer {
public:
	void start();
	time_type operator () () const; // Время с момента старта
};

// Таймер - выполнение в течении указанного времени
class DurationTimer : public BaseTimer {
public:
	void start(time_type duration);
	time_type operator () () const;  // true:  был start и не прошло время duration, возвращает остаток времени
	int operator ! () const { return (*this)() ? 0 : 1; } 
};

// Таймер - задержка события
class DelayTimer : public BaseTimer {
public:
	void start(time_type delay);
	time_type operator () () const;  // true:  был start и прошло время delay, возвращает время от конца задержки
	int operator ! () const { return (*this)() ? 0 : 1; } 
};

// Таймер - задержка true-условия 
class DelayConditionTimer : public BaseTimer {
public:
	int operator () (int condition, time_type delay);  // true: condition == true выполнилось время delay назад. 
};

// Таймер - усреднение true-условия 
class AverageConditionTimer : public BaseTimer {
public:
	int operator () (int condition, time_type delay);  // true: condition == true выполнялось время delay. 
};

// Таймер - гистерезис true-условия 
class HysteresisConditionTimer : public BaseTimer {
	int turned_on;
public:
	HysteresisConditionTimer() { turned_on = 0; }
	// true: condition == true выполнялось время on_delay, скидывается, если condition == false время off_delay
	int operator () (int condition, time_type on_delay, time_type off_delay);  
	int operator () (int condition, time_type delay) { return (*this)(condition, delay, delay); }
	int operator () () { return turned_on; }
};


///////////////////////////////////////////////////////////////////////////////////////////////
//		Non-Stop Timers
///////////////////////////////////////////////////////////////////////////////////////////////

// Измерение времени
class MeasurementNonStopTimer : public BaseTimer {
public:
	void start();
	time_type operator () () const; // Время с момента старта
};

// Таймер - выполнение в течении указанного времени
class DurationNonStopTimer : public BaseTimer {
public:
	void start(time_type duration);
	time_type operator () () const;  // true:  был start и не прошло время duration, возвращает остаток времени
	int operator ! () const { return (*this)() ? 0 : 1; } 
};

// Таймер - задержка события
class DelayNonStopTimer : public BaseTimer {
public:
	void start(time_type delay);
	time_type operator () () const;  // true:  был start и прошло время delay, возвращает время от конца задержки
	int operator ! () const { return (*this)() ? 0 : 1; } 
};




///////////////////////////////////////////////////////////////////////////////////////////////
//		Inline definitions of Ordynary Timers
///////////////////////////////////////////////////////////////////////////////////////////////

// Отмерение времени
inline void MeasurementTimer::start()
{
	start_time = global_time();
}	
inline time_type MeasurementTimer::operator()() const
{
	return start_time ? global_time() - start_time : 0;
}

// Таймер - задержка события
inline void DelayTimer::start(time_type delay)
{
	start_time = global_time() + delay;
}	
inline time_type DelayTimer::operator () () const
{
	return start_time && global_time() > start_time ? global_time() - start_time : 0;
}
	
// Таймер - выполнение в течении указанного времени
inline void DurationTimer::start(time_type duration)
{
	start_time = global_time() + duration;
}	
inline time_type DurationTimer::operator () () const
{
	return start_time > global_time() ? start_time - global_time() : 0;
}


// Таймер - задержка true-условия 
inline int DelayConditionTimer::operator () (int condition, time_type delay)
{
	if(condition){
		if(start_time){
			if(global_time() - start_time >= delay)
				return 1;
			}
		else
			start_time = global_time();
		}
	return 0;
}

// Таймер - усреднение true-условия 
inline int AverageConditionTimer::operator () (int condition, time_type delay)
{
	if(condition){
		if(start_time){
			if(global_time() - start_time >= delay)
				return 1;
			}
		else
			start_time = global_time();
		}
	else 
		start_time = 0;
	return 0;
}

// Таймер - гистерезис true-условия 
inline int HysteresisConditionTimer::operator () (int condition, time_type on_delay, time_type off_delay)
{
	if(turned_on && condition || !turned_on && !condition){
		start_time = 0;
		return turned_on;
		}

	if(!turned_on && condition){
		if(start_time){
			if(global_time() - start_time >= on_delay){
				turned_on = 1;
				start_time = 0;
				}
			}
		else
			start_time = global_time();
		}

	if(turned_on && !condition){
		if(start_time){
			if(global_time() - start_time >= off_delay){
				turned_on = 0;
				start_time = 0;
				}
			}
		else
			start_time = global_time();
		}

	return turned_on;
}


///////////////////////////////////////////////////////////////////////////////////////////////
//		Inline definitions of Non Stopped Timers
///////////////////////////////////////////////////////////////////////////////////////////////

// Отмерение времени
inline void MeasurementNonStopTimer::start()
{
	start_time = non_stop_global_time();
}	
inline time_type MeasurementNonStopTimer::operator()() const
{
	return start_time ? non_stop_global_time() - start_time : 0;
}

// Таймер - задержка события
inline void DelayNonStopTimer::start(time_type delay)
{
	start_time = non_stop_global_time() + delay;
}	
inline time_type DelayNonStopTimer::operator () () const
{
	return start_time && non_stop_global_time() > start_time ? non_stop_global_time() - start_time : 0;
}
	
// Таймер - выполнение в течении указанного времени
inline void DurationNonStopTimer::start(time_type duration)
{
	start_time = non_stop_global_time() + duration;
}	
inline time_type DurationNonStopTimer::operator () () const
{
	return start_time > non_stop_global_time() ? start_time - non_stop_global_time() : 0;
}




#endif // __DURATION_TIMER_H__
