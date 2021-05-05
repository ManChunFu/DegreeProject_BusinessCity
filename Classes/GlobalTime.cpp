#include "GlobalTime.h"
#include "GameTime.h"
#include "UIPanel.h"

USING_NS_CC;

GlobalTime::GlobalTime()
{
	m_Gametime = new GameTime();
	m_ElapsedTime = 0;
}

GlobalTime::~GlobalTime()
{
	delete m_Gametime;
	m_Gametime = nullptr;

	m_MinuteListeners.clear();
	m_HourListeners.clear();
}

void GlobalTime::update(float delta)
{
	m_ElapsedTime += delta;

	// update minute
	if (m_ElapsedTime < m_DeltaCount)
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

		if (m_HourListeners.size() > 0)
		{
			for (auto listener : m_HourListeners)
			{
				listener.second(this, m_Gametime->hour);
			}
		}
	}

	if (m_MinuteListeners.size() > 0)
	{
		for (auto listener : m_MinuteListeners)
		{
			listener.second(this, m_Gametime->minute);
		}
	}

	m_DeltaCount = 3.f;
}

void GlobalTime::addMinuteEventListener(const onTimeChanges& changes, Node* node)
{
	m_MinuteListeners[node] = changes;
}

void GlobalTime::addHourEventListener(const onTimeChanges& changes, Node* node)
{
	m_HourListeners[node] = changes;
}

void GlobalTime::removdMinuteEventListener(Node* node)
{
	if (node == nullptr)
		return;

	auto found = m_MinuteListeners.find(node);
	if (found != m_MinuteListeners.end())
		m_MinuteListeners.erase(found);
}

void GlobalTime::removeHourEventListener(Node* node)
{
	if (node == nullptr)
		return;

	auto found = m_HourListeners.find(node);
	if (found != m_HourListeners.end())
		m_HourListeners.erase(found);

}

void GlobalTime::speedUp()
{
	m_DeltaCount = 0.f;

	m_Gametime->minute = 59;
	m_Gametime->hour = 23;
	
	if (m_MinuteListeners.size() > 0)
	{
		for (auto listener : m_MinuteListeners)
		{
			listener.second(this, m_Gametime->minute);
		}
	}

	if (m_HourListeners.size() > 0)
	{
		for (auto listener : m_HourListeners)
		{
			listener.second(this, m_Gametime->hour);
		}
	}

	if (onEveryDayChanges)
		onEveryDayChanges(this, m_Gametime->day);

}



