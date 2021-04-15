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
	m_ShopLockWidget = nullptr;
	m_MessageText = nullptr;
	m_MessageWidgets.clear();
	m_TempButton = nullptr;
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

		m_ShopLockWidget = ui::Widget::create();
		parentWidget->addChild(m_ShopLockWidget, 2);

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
				m_ShopLockWidget->addChild(lockMenu, 1);
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

	// requirements
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

		auto upgradeShop = allShops[m_Shop->m_UpgradeableTo[index]];
		auto staffText = Label::createWithTTF(std::to_string(upgradeShop->m_RequiredEmployees), "fonts/Nirmala.ttf", 16);
		if (staffText)
		{
			staffText->enableShadow(darkCyanColor);
			GameFunctions::displayLabel(staffText, Color4B::WHITE, Vec2(contentPos.x + 90.f, contentPos.y - 40.f), parentWidget, 1, true,
				TextHAlignment::LEFT);
		}

		contentPos.y = panelMidPoint.y - 90.f;
	}

#pragma region create message panel
	// message
	m_ThisPanel = Sprite::create("Sprites/CheckBoxes/Panel_Blue_80.png");
	m_ThisPanel->setPosition(panelMidPoint.x + 170.f, panelMidPoint.y - 40.f);
	parentWidget->addChild(m_ThisPanel, 2);
	m_ThisPanel->setVisible(false);
	auto messagePos = Vec2(30.f, m_ThisPanel->getContentSize().height * 0.5f + 20.f);

	// player can only upgrade to one level (not jump to 2 levels)
	auto shopType = allShops[m_Shop->m_UpgradeableTo[0]]->m_ShopType;
	std::array<std::string, 2> messageTexts = { "Do you want to upgrade to a", shopType + " ?" };
	std::array<std::string, 2> buttonTexts = { "Upgrade", "Cancel" };
	auto taroColor = GameData::getInstance().m_ColorType.Taro;
	
	for (unsigned index = 0; index < 2; ++index)
	{
		auto messageWidget = ui::Widget::create();
		messageWidget->setPosition(Vec2::ZERO);
		messageWidget->setVisible(false);
		m_ThisPanel->addChild(messageWidget, 1);
		m_MessageWidgets.pushBack(messageWidget);

		if (index == 0)
		{
			for (unsigned contentIndex = 0; contentIndex < 2; ++contentIndex)
			{
				auto text = Label::createWithTTF(messageTexts[contentIndex], "fonts/NirmalaB.TTF", 16);
				if (text)
				{
					text->setMaxLineWidth(m_ThisPanel->getContentSize().width - 20.f);
					text->enableWrap(true);
					(contentIndex == 0) ? text->enableShadow(darkCyanColor) : text->enableGlow(Color4B::BLACK);
					GameFunctions::displayLabel(text, Color4B::WHITE, messagePos, m_MessageWidgets.at(EMessages::E_Qualified), 1, true, 
						TextHAlignment::LEFT);
				}

				auto button = MouseOverMenuItem::creatMouseOverMenuButton((contentIndex == 0) ? CC_CALLBACK_1(ShopUpgrade::proceedUpgradeCallback, this) :
					CC_CALLBACK_1(ShopUpgrade::cancelUpgradeCallback, this));
				if (button)
				{
					m_MenuItems.pushBack(displayMenuButton(button, CC_CALLBACK_2(ShopUpgrade::onMouseOver, this), (contentIndex == 0) ?
						Vec2(messagePos.x + 30.f, messagePos.y - 70.f) : Vec2(messagePos.x + 120.f, messagePos.y - 40.f),
						itemTypes::BUTTON, 0.7f, true, Vec2(710.f, 220.f)));
					auto buttonText = Label::createWithTTF(buttonTexts[contentIndex], "fonts/NirmalaB.ttf", 20);
					if (buttonText)
					{
						buttonText->enableGlow(Color4B::WHITE);
						GameFunctions::displayLabel(buttonText, taroColor, button->getContentSize() * 0.5f, button, 1);
					}
				}
				messagePos.y -= 30.f;
			}
		}
		else
		{
			auto text = Label::createWithTTF("You don't have enough money or employee", "fonts/NirmalaB.TTf", 16);
			if (text)
			{
				text->setMaxLineWidth(m_ThisPanel->getContentSize().width - 80.f);
				text->enableWrap(true);
				text->enableShadow(darkCyanColor);
				GameFunctions::displayLabel(text, Color4B::WHITE, Vec2(messagePos.x + 75.f, messagePos.y + 60.f), 
					m_MessageWidgets.at(EMessages::E_DisQualified), 1);
			}

			auto okButton = MouseOverMenuItem::creatMouseOverMenuButton(CC_CALLBACK_1(ShopUpgrade::cancelUpgradeCallback, this));
			if (okButton)
			{
				displayMenuButton(okButton, CC_CALLBACK_2(ShopUpgrade::onMouseOver, this), Vec2(messagePos.x + 70.f, messagePos.y -10.f),
					itemTypes::DEFAULT, 0.7f, true, Vec2(710.f, 220.f));

				auto okText = Label::createWithTTF("OK", "fonts/NirmalaB.ttf", 16);
				if (okText)
				{
					okText->enableGlow(Color4B::WHITE);
					GameFunctions::displayLabel(okText, taroColor, okButton->getContentSize() * 0.5f, okButton, 1);
				}

				auto okButtonMenu = Menu::create(okButton, NULL);
				okButtonMenu->setPosition(Vec2::ZERO);
				m_MessageWidgets.at(EMessages::E_DisQualified)->addChild(okButtonMenu, 1);
			}
		}
	}

	auto menuButtons = Menu::createWithArray(m_MenuItems);
	menuButtons->setPosition(Vec2::ZERO);
	m_MessageWidgets.at(EMessages::E_Qualified)->addChild(menuButtons, 1);

#pragma endregion

}

void ShopUpgrade::upgradeCallback(cocos2d::Ref* pSender, unsigned shopId)
{
	if (GameData::getInstance().isPopupOpen())
		return;

	if (m_Player->getCurrentCash() > m_ShopUpgradePrices[shopId])
	{
		m_ShopLockWidget->setVisible(false);
		showMessage(true);
	}
	else
		showMessage(false);
}

void ShopUpgrade::proceedUpgradeCallback(cocos2d::Ref* pSender)
{
}

void ShopUpgrade::cancelUpgradeCallback(cocos2d::Ref* pSender)
{
	m_ThisPanel->setVisible(false);
	m_MessageWidgets.at(EMessages::E_DisQualified)->setVisible(false);
	GameData::getInstance().notifyPopupOpen(false);
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
	m_ThisPanel->setVisible(true);
	GameData::getInstance().notifyPopupOpen(true);
	if (success)
	{
		m_MessageWidgets.at(EMessages::E_Qualified)->setVisible(true);
		return;
	}
	m_MessageWidgets.at(EMessages::E_DisQualified)->setVisible(true);
}
