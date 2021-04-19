#pragma once
#include "cocos2d.h"

struct EmployeeData
{
	EmployeeData(unsigned id, std::string name, std::string profileLink) : m_Id(id), m_Name(name), m_ProfileLink(profileLink) {};
	virtual ~EmployeeData() {};

	unsigned m_Id = 0;
	std::string m_Name = "";
	std::string m_ProfileLink = "";
};