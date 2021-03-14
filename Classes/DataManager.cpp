#include "DataManager.h"
#include "json/Document.h"


USING_NS_CC;

std::vector<Shop*> DataManager::getShops()
{
	std::string dataBytes = FileUtils::getInstance()->getStringFromFile("Json/Shop.json");
	
	rapidjson::Document doc;
	doc.Parse<0>(dataBytes.c_str());
	

	return std::vector<Shop*>();
}
