#include "ShopProduct.h"

ShopProduct::ShopProduct(rapidjson::Value json)
{
	for (rapidjson::Value::ConstMemberIterator itr = json.MemberBegin(); itr != json.MemberEnd(); ++itr)
	{
		auto comparedName = itr->name.GetString();
		if (comparedName == "Name")
		{
			m_Name = itr->name.GetString();
			continue;
		}

		if (comparedName == "Price")
			m_SalePrice = itr->name.GetInt();
		
	}

}
