#include "DataManager.h"
#include "Shop.h"
#include "PeopleData.h"
#include "json/Document.h"

USING_NS_CC;

std::map<unsigned, Shop*> DataManager::getShops()
{
	std::string dataBytes = FileUtils::getInstance()->getStringFromFile("Json/Shop.json");

	std::map<unsigned, Shop*> shops;
	rapidjson::Document jsonDoc;
	jsonDoc.Parse<0>(dataBytes.c_str());
		
	if (jsonDoc.IsArray())
	{
		auto jsonDocSize = jsonDoc.Size();
		for (int index = 0; index < jsonDocSize; ++index)
		{
			Shop* newShop = new Shop(jsonDoc[index]);
			shops.insert(std::pair<unsigned, Shop*>(newShop->m_ShopId, newShop));
		}
	}

	return shops;
}

std::map<unsigned, PeopleData*> DataManager::getPeople()
{
	std::string dataBytes = FileUtils::getInstance()->getStringFromFile("Json/People.json");

	std::map<unsigned, PeopleData*> people;
	rapidjson::Document jsonDoc;
	jsonDoc.Parse<0>(dataBytes.c_str());

	if (jsonDoc.IsArray())
	{
		auto jsonDocSize = jsonDoc.Size();
		for (int index = 0; index < jsonDocSize; ++index)
		{
			PeopleData* newPeople = new PeopleData(jsonDoc[index]);
			people.insert(std::pair<unsigned, PeopleData*>(newPeople->m_SceneId, newPeople));
		}
	}

	return people;
}




