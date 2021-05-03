#include "PeopleLocationData.h"

USING_NS_CC;

constexpr unsigned int stringToInt(const char* str, int h = 0)
{
	return !str[h] ? 5381 : (stringToInt(str, h + 1) * 33) ^ str[h];
}

PeopleLocationData::PeopleLocationData(rapidjson::Value& json)
{
	for (rapidjson::Value::ConstMemberIterator itr = json.MemberBegin(); itr != json.MemberEnd(); ++itr)
	{
		std::string propertyName = itr->name.GetString();

		switch (stringToInt(propertyName.c_str()))
		{
		case stringToInt("ShopId"):
			m_ShopId = itr->value.GetInt();
			break;
		case stringToInt("People"):
			if (itr->value.IsArray())
			{
				auto people = itr->value.GetArray();
				for (int index = 0; index < itr->value.Size(); index++)
				{
					m_PeopleSpritePaths.push_back(people[index].GetString());
				}
			}
			break;
		case stringToInt("PeopleLocation"):
			if (itr->value.IsArray())
			{
				auto location = itr->value.GetArray();
				m_PeopleLocation.x = location[0].GetFloat();
				m_PeopleLocation.y = location[1].GetFloat();
			}
			break;
		case stringToInt("ProductLocation"):
			if (itr->value.IsArray())
			{
				auto location = itr->value.GetArray();
				m_ProductLocation.x = location[0].GetFloat();
				m_ProductLocation.y = location[1].GetFloat();
			}
			break;
		}
	}
}

PeopleLocationData::~PeopleLocationData()
{
	m_PeopleSpritePaths.clear();
}
