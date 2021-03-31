#include "GlobalTime.h"
#include "GameTime.h"


GlobalTime::GlobalTime()
{
	m_Gametime = new GameTime();
	m_ElapsedTime = 0;
}

GlobalTime::~GlobalTime()
{
	delete m_Gametime;
	m_Gametime = nullptr;

	for (unsigned index = 0; index < Listeners.size(); index++)
	{
		Listeners.erase(Listeners.begin() + index);
	}
	Listeners.clear();
}

void GlobalTime::update(float delta)
{
	m_ElapsedTime += delta;

	// update minute
	if (m_ElapsedTime < 3)
		return;

	m_Gametime->minute++;
	m_ElapsedTime = 0;
	
	// update hour
	if (m_Gametime->minute > 59)
	{
		m_Gametime->minute = 0;
		m_Gametime->hour++;

		// update weekday
		if (m_Gametime->hour > 23)
		{
			m_Gametime->hour = 0;
			m_Gametime->day++;

			// update week
			if (m_Gametime->day > 6)
			{
				m_Gametime->day = 0;
				m_Gametime->week++;

				if (onEveryWeekChanges)
					onEveryWeekChanges(this, m_Gametime->week);
			}

			if (onEveryDayChanges)
				onEveryDayChanges(this, m_Gametime->day);
		}

		if (onEveryHourChanges)
			onEveryHourChanges(this, m_Gametime->hour);
	}

	if (Listeners.size() > 0)
	{
		for (auto listener : Listeners)
			listener(this, m_Gametime->minute);
	}
}

void GlobalTime::addMinuteEventListener(const onEveryMinuteChanges& changes)
{
	Listeners.push_back(changes);
}


