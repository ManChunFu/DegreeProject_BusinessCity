#include "DataManager.h"
#include "Shop.h"
#include "json/Document.h"

USING_NS_CC;

std::vector<Shop*> DataManager::getShops()
{
	std::string dataBytes = FileUtils::getInstance()->getStringFromFile("Json/Shop.json");
	std::vector<Shop*> shops;
	
	rapidjson::Document jsonDoc;
	jsonDoc.Parse<0>(dataBytes.c_str());
	
	
	if (jsonDoc.IsArray())
	{
		for (int index = 0; index < jsonDoc.Size(); index++)
		{
			shops.push_back(new Shop(jsonDoc[index]));
		}
	}

	return shops;
}



