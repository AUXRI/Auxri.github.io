#include "CCell.h"
#include <iostream>

using namespace std;
CCell::CCell()
{
	m_pShip = 0;
	m_bBeated = false;
}
string CCell::PrintOwn()
{
	
	if (m_pShip)
		return "K ";
	else
		return "\3 ";
	/*if (!m_bBeated)
	{
		if (m_pShip)
			return "K ";
		else
			return "\3 ";
	}
	else
	{
		if (m_pShip)
			return "X ";
		else
			return "# ";
	}*/
	

}
string CCell::PrintForeign()
{
	if (m_bBeated)
	{
		if (m_pShip)
			return "X "; 
		else
			return "# ";
	}
	else
		if (m_pShip)
			return "K ";
		else
			return  "\3 ";
	/*if (m_bBeated)
	{
		if (m_pShip)
			return "X ";
		else
			return "# ";
	}
	else
		return "\3 ";*/
}

CCell::~CCell()
{

}