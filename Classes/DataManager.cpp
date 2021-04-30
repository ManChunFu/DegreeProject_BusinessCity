#include "DataManager.h"
#include "Shop.h"
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
		for (int index = 0; index < jsonDoc.Size(); index++)
		{
			Shop* newShop = new Shop(jsonDoc[index]);
			shops.insert(std::pair<unsigned, Shop*>(newShop->m_ShopId, newShop));
		}
	}

	return shops;
}


