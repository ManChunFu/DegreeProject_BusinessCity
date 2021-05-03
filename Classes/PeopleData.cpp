#include "PeopleData.h"
#include "PeopleLocationData.h"

USING_NS_CC;

constexpr unsigned int stringToInt(const char* str, int h = 0)
{
	return !str[h] ? 5381 : (stringToInt(str, h + 1) * 33) ^ str[h];
}

PeopleData::PeopleData(rapidjson::Value& json)
{
	for (rapidjson::Value::ConstMemberIterator itr = json.MemberBegin(); itr != json.MemberEnd(); ++itr)
	{
		std::string propertyName = itr->name.GetString();

		switch (stringToInt(propertyName.c_str()))
		{
		case stringToInt("SceneId"):
			m_SceneId = itr->value.GetInt();
			break;
		case stringToInt("Locations"):
			if (itr->value.IsArray())
			{
				auto locations = itr->value.GetArray();
				for (int index = 0; index < itr->value.Size(); ++index)
				{
					PeopleLocationData* peopleData = new PeopleLocationData((rapidjson::Value&)(locations[index]));
					m_PeopleLocations.insert(std::pair<unsigned, PeopleLocationData*>(peopleData->m_ShopId, peopleData));
				}
			}
			break;
		}
	}
}

PeopleData::~PeopleData()
{
	for (auto people : m_PeopleLocations)
	{
		delete people.second;
	}
	m_PeopleLocations.clear();
}
