#pragma once
#include "cocos2d.h"

struct EmployeeData
{
	EmployeeData(unsigned id, std::string name, std::string profileLink) : m_Id(id), m_Name(name), m_ProfileLink(profileLink) {};
	EmployeeData(unsigned id, cocos2d::Sprite* pic, float posX) : m_Id(id), m_ProfilePic(pic), m_PicPositionX(posX) {};

	virtual ~EmployeeData() { m_ProfilePic = nullptr; };

	unsigned m_Id = 0;
	std::string m_Name = "";
	std::string m_ProfileLink = "";
	cocos2d::Sprite* m_ProfilePic = nullptr;
	float m_PicPositionX = 0.f;
};