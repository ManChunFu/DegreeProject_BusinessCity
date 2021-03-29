#include "GlobalTime.h"


//void GlobalTime::update(float delta)
//{
//	m_ElapsedTime += delta;
//
//	// update minute
//	if (m_ElapsedTime < 4)
//		return;
//
//	m_CurrentMinute++;
//	m_ElapsedTime = 0;
//
//	// update hour
//	if (m_CurrentMinute > 59)
//	{
//		m_CurrentMinute = 0;
//		m_CurrentHour++;
//	}
//
//	// update weekday
//	if (m_CurrentHour > 23)
//	{
//		m_CurrentHour = 0;
//		m_Today++;
//	}
//
//	// update week
//	if (m_Today > 6)
//	{
//		m_Today = 0;
//		m_Weeks++;
//	}
//}
