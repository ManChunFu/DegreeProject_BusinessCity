#include "ShopUpgrade.h"
#include "ui/UIWidget.h"
#include "Shop.h"
#include "Player.h"
#include "ShopProduct.h"
#include "MouseOverMenuItem.h"
#include "GameData.h"
#include "GameFunctions.h"

USING_NS_CC;

ShopUpgrade::~ShopUpgrade()
{
	m_Shop = nullptr;
	m_ShopUpgradePrices.clear();
	m_ShopLockWidgets.clear();
	m_MessageText = nullptr;
}

void ShopUpgrade::createShopUpgrade(Shop* shop, ui::Widget* parentWidget, Vec2 panelMidPoint)
{
	m_Player = GameData::getInstance().m_Player;
	m_Shop = shop;
	parentWidget->setVisible(false);
	auto darkCyanColor = GameData::getInstance().m_ColorType.DarkCyan;

	auto upgradeListSize = m_Shop->m_UpgradeableTo.size();
	auto allShops = GameData::getInstance().m_Shops;
	auto spritePos = Vec2(panelMidPoint.x - 170.f, panelMidPoint.y + 20.f);
	for (unsigned index = 0; index < upgradeListSize; index++)
	{
		auto upgradeShop = allShops[m_Shop->m_UpgradeableTo[index]];

		auto upgradeShopSprite = Sprite::createWithSpriteFrameName(upgradeShop->m_ShopLook_Normal);
		if (upgradeShopSprite)
			GameFunctions::displaySprite(upgradeShopSprite, spritePos, parentWidget, 1, 0.5f, 0.5f);

		auto widget = ui::Widget::create();
		parentWidget->addChild(widget, 2);
		m_ShopLockWidgets.pushBack(widget);

		if (index == 0)
		{
			auto upgradableShopSprite = MouseOverMenuItem::creatMouseOverMenuButton("Lock_100.png", "Lock_100_Lit.png", "Lock_100_Disable.png",
				CC_CALLBACK_1(ShopUpgrade::upgradeCallback, this, index));
			if (upgradableShopSprite)
			{
				auto lockMenuSprite = displayMenuButton(upgradableShopSprite, CC_CALLBACK_2(ShopUpgrade::onMouseOver, this),
					spritePos, itemTypes::BUTTON, 0.5f, true);

				auto lockMenu = Menu::create(lockMenuSprite, NULL);
				lockMenu->setPosition(Vec2::ZERO);
				widget->addChild(lockMenu, 1);
			}
		}
		else
		{
			auto lockSprite = Sprite::createWithSpriteFrameName("Lock_100_Disable.png");
			if (lockSprite)
				GameFunctions::displaySprite(lockSprite, upgradeShopSprite->getContentSize() * 0.5f, upgradeShopSprite, 1);
		}

		spritePos.y -= 120.f;
	}

	// requirement text
	auto requirementText = Label::createWithTTF("Upgrade Requirement", "fonts/NirmalaB.ttf", 16);
	if (requirementText)
	{
		requirementText->enableGlow(Color4B::BLACK);
		GameFunctions::displayLabel(requirementText, Color4B::WHITE, Vec2(panelMidPoint.x, panelMidPoint.y + 70.f), parentWidget, 1,
			true, TextHAlignment::LEFT);
	}

	std::array<std::string, 2> contentTexts = { "Price  $", "Employees" };
	auto contentPos = Vec2(panelMidPoint.x - 50.f, panelMidPoint.y + 30.f);
	for (unsigned index = 0; index < upgradeListSize; index++)
	{
		for (unsigned contentIndex = 0; contentIndex < 2; contentIndex++)
		{
			auto contentTitle = Label::createWithTTF(contentTexts[contentIndex], "fonts/NirmalaB.ttf", 16);
			if (contentTitle)
			{
				contentTitle->enableGlow(Color4B::BLACK);
				GameFunctions::displayLabel(contentTitle, Color4B::WHITE, contentPos, parentWidget, 1, true, TextHAlignment::LEFT);
			}
			contentPos.y -= 40.f;
		}

		contentPos.y += 80.f;

		auto priceText = Label::createWithTTF("", "fonts/Nirmala.ttf", 16);
		if (priceText)
		{
			priceText->setString(index % upgradeListSize == 0 ? GetShopUpgradePrice(m_Shop->m_UpgradeableTo[0]) :
				GetShopUpgradePrice(m_Shop->m_UpgradeableTo[1]));

			priceText->enableShadow(darkCyanColor);
			GameFunctions::displayLabel(priceText, Color4B::WHITE, Vec2(contentPos.x + 60.f, contentPos.y), parentWidget, 1, true,
				TextHAlignment::LEFT);
		}
		
		auto staffText = Label::createWithTTF(std::to_string(m_RequirementStaff), "fonts/Nirmala.ttf", 16);
		if (staffText)
		{
			staffText->enableShadow(darkCyanColor);
			GameFunctions::displayLabel(staffText, Color4B::WHITE, Vec2(contentPos.x + 90.f, contentPos.y -40.f), parentWidget, 1, true,
				TextHAlignment::LEFT);
		}
		
		contentPos.y = panelMidPoint.y - 90.f;
	}

	/*for (unsigned index = 0; index < upgradeListSize; index++)
	{
		if (m_ShopUpgradePrices[index] > m_Player->getCurrentCash())
			
	}*/
	
	m_ThisPanel = Sprite::create("Sprites/CheckBoxes/Panel_Blue_80.png");
	m_ThisPanel->setPosition(panelMidPoint.x +170.f, panelMidPoint.y - 40.f);
	parentWidget->addChild(m_ThisPanel, 2);
	//m_ThisPanel->setVisible(false);
	auto messagePos = Vec2(30.f, m_ThisPanel->getContentSize().height * 0.5f + 20.f);
	
	// player can only upgrade to one level (not jump to 2 levels)
	auto shopType = allShops[m_Shop->m_UpgradeableTo[0]]->m_ShopType; 
	std::array<std::string, 2> messageTexts = { "Do you want to upgrade to a", shopType + " ?" };

	for (unsigned index = 0; index < 2; ++index)
	{
		auto text = Label::createWithTTF(messageTexts[index], "fonts/NirmalaB.TTF", 16);
		if (text)
		{
			text->setMaxLineWidth(m_ThisPanel->getContentSize().width - 20.f);
			text->enableWrap(true);
			(index == 0)? text->enableShadow(darkCyanColor) : text->enableGlow(Color4B::BLACK);
			GameFunctions::displayLabel(text, Color4B::WHITE, messagePos, m_ThisPanel, 1, true, TextHAlignment::LEFT);
		}

		messagePos.y -= 30.f;
	}
	
}

void ShopUpgrade::upgradeCallback(cocos2d::Ref* pSender, unsigned shopId)
{
	if (m_Player->getCurrentCash() > m_ShopUpgradePrices[shopId])
	{
		m_ShopLockWidgets.at(shopId)->setVisible(false);
		showMessage(true);
	}
	else
		showMessage(false);
}

void ShopUpgrade::onMouseOver(MouseOverMenuItem* menuItem, cocos2d::Event* event)
{
}

std::string ShopUpgrade::GetShopUpgradePrice(unsigned shopId)
{
	auto price = GameData::getInstance().m_Shops[shopId]->m_ShopPrice;
	price -= m_Shop->m_ShopPrice * 2.5f;
	m_ShopUpgradePrices.push_back(price);

	return std::to_string(price);
}

void ShopUpgrade::showMessage(bool success)
{
	//if (success)
	m_ThisPanel->setVisible(true);
}
