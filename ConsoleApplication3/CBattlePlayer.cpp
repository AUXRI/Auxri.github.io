#define _CRT_SECURE_NO_WARNINGS
#include "CBattlePlayer.h"
#include <iostream>
#include <fstream>
#define TEST
CBattlePlayer::CBattlePlayer()
{
	m_sock = 0;
	m_pAnotherPlayer = 0;

}
CBattlePlayer::~CBattlePlayer()
{
	for (size_t i = 0; i < m_Ships.size(); i++)
	{
		delete m_Ships[i];
	}

	m_Ships.clear();

}
void CBattlePlayer::Message(string str)
{
	str = "\n" + str + "\n";
	send(m_sock, str.c_str(), str.size() + 1, MSG_OOB);
}

bool CBattlePlayer::PrepareShips()
{
	Message("Способ расстановки: \nhand-1\nready-2\nrandom-3\n");
	string rec = recieve();
	if (rec == "1\n")
	{
		Message("Расставляйте корабли\n");
		while (!ShipsAreReady())
		{
			if (Try2PlaceShip(recieve()))
			{
				Message("OK");
			}
			else
			{
				Message("Ошибка в расположении корабля!");
			}
			Message(m_Aqua.PrintForeign());
		}
		Message("Ваши корабли готовы!");

		return true;
	}
	else if (rec == "2\n")
	{
		file();
		Message("Ваши корабли готовы!");
		return true;
	}
	else if (rec == "3\n")
	{
		//random();
		Message("Ваши корабли готовы!");
		Message(m_Aqua.PrintForeign());
		return true;
	}

	else
	{
		Message("ERROR");
		PrepareShips();
	}
	return true;

}

bool CBattlePlayer::DoMove()
{
	Message("Ваш ход! \n->");
	string move = recieve();
	CShip* ship = NULL;
	int i, j;
	while (!m_Aqua.ParseCell(move, i, j)) {
		Message("дубина");
		Message("Ваш ход!\n->");
		move = recieve();
	}
	if (m_pAnotherPlayer->m_Aqua.ParseCell(move, i, j))
	{
		//Message(m_Aqua.PrintForeign());
		if (m_pAnotherPlayer->m_Aqua.TestShip(move, &ship))
		{
			Message("Есть пробитие!");
			//Message(m_pAnotherPlayer->m_Aqua.PrintForeign());
			m_pAnotherPlayer->Message("Пробили\n");
			m_pAnotherPlayer->Message(m_pAnotherPlayer->m_Aqua.PrintForeign());

			if (!ship->Alive())
			{
				Message("Корабль потоплен!");
				Message(m_pAnotherPlayer->m_Aqua.PrintForeign());
				m_pAnotherPlayer->Message(m_pAnotherPlayer->m_Aqua.PrintForeign());
				if (!m_pAnotherPlayer->IsAlive())
				{
					Message("Вы выиграли)");
					m_pAnotherPlayer->Message("Вы проиграли(");
					return true;
				}
			}
			DoMove();
		}
		else
		{
			Message(m_Aqua.PrintForeign());
			m_pAnotherPlayer->Message(m_pAnotherPlayer->m_Aqua.PrintForeign());
			Message("Мазила");
		}
	}
	return true;
}


bool CBattlePlayer::ShipsAreReady()
{
#ifndef TEST
	if (m_Ships.size() != 10)
		return false;

#endif
	int n4Deck, n3Deck, n2Deck, n1Deck;

	n4Deck = n3Deck = n2Deck = n1Deck = 0;

	for (size_t i = 0; i < m_Ships.size(); i++)
	{
		if (!m_Ships[i]->IsPlaced())
			return false;
		else {
			switch (m_Ships[i]->m_Deck)
			{
			case 1:
				n1Deck++;
				break;
			case 2:
				n2Deck++;
				break;
			case 3:
				n3Deck++;
				break;
			case 4:
				n4Deck++;
				break;

			}
		}
	}

#ifdef TEST
	if (n4Deck == 1)// временно
#else
	if (n1Deck == 4 and n2Deck == 3 and n3Deck == 2 and n4Deck == 1)
#endif	
		return true;

	return false;
}


void CBattlePlayer::file()
{
	FILE* file;
	file = fopen("Z:\\sb_chess\\ConsoleApplication3\\ships.txt", "r");
	if (file == 0)
	{
		cout << "ERROR";
		fclose(file);
	}
	char buf[2048];
	while (!feof(file))
	{
		fgets(buf, 100, file);
		Try2PlaceShip(buf);
	}
	Message(m_Aqua.PrintForeign());
	fclose(file);
}
//void CBattlePlayer::random()
//{
//	srand(time(0));
//	
//		int x = 0 + rand() % 10, y = 0 + rand() % 10, z = rand() % 2;
//		
//}
bool CBattlePlayer::Try2PlaceShip(string ship)
{


	int iDeck = 0;
	char buf[50];
	char cell[4][10];
	int detector = 0;
	sscanf_s(ship.c_str(), "%i(%[^)]", &iDeck, buf, 50);


	if (iDeck == 4)
	{
		//4(a1,b1,c3,c4)
		sscanf_s(buf, "%[^,],%[^,],%[^,],%[^\n],", cell[0], 10, cell[1], 10, cell[2], 10, cell[3], 10);

		if (((cell[3][1] - cell[2][1] < 2) && (cell[2][1] - cell[1][1] < 2) && (cell[1][1] - cell[0][1] < 2)) &&
			((cell[0][0] == cell[1][0] && cell[1][0] == cell[2][0] && cell[2][0] == cell[3][0])
				|| ((cell[0][0] != cell[1][0] && cell[1][0] - cell[0][0] == 1 && cell[0][1] == cell[1][1]) && (cell[2][0] != cell[1][0] && cell[2][0] - cell[1][0] == 1 && cell[1][1] == cell[2][1]) && (cell[3][0] != cell[2][0] && cell[3][0] - cell[2][0] == 1 && cell[2][1] == cell[3][1]))))

		{
			detector = 1;
		}
	}
	if (iDeck == 3)
	{
		sscanf_s(buf, "%[^,],%[^,],%[^\n],", cell[0], 10, cell[1], 10, cell[2], 10);
		if (((cell[2][1] - cell[1][1] < 2) && (cell[1][1] - cell[0][1] < 2)) &&
			((cell[0][0] == cell[1][0] && cell[1][0] == cell[2][0])
				|| ((cell[0][0] != cell[1][0] && cell[1][0] - cell[0][0] == 1 && cell[0][1] == cell[1][1]) && (cell[2][0] != cell[1][0] && cell[2][0] - cell[1][0] == 1 && cell[1][1] == cell[2][1]))))
		{
			detector = 1;
		}
	}
	if (iDeck == 2)
	{
		sscanf_s(buf, "%[^,],%[^\n],", cell[0], 10, cell[1], 10);
		if (((cell[1][1] - cell[0][1] < 2)) &&
			((cell[0][0] == cell[1][0])
				|| ((cell[0][0] != cell[1][0] && cell[1][0] - cell[0][0] == 1 && cell[0][1] == cell[1][1]))))
		{
			detector = 1;
		}
	}

	if (iDeck == 1)
	{
		sscanf_s(buf, "%[^\n],", cell[0], 10);
		detector = 1;
	}

	if (detector == 1)
	{


		CShip* s = new CShip(iDeck);

		for (int i = 0; i < iDeck; i++)
		{
			CCell* pCell = NULL;
			if (m_Aqua.Try2PlaceDeck(cell[i], &pCell, s))
			{
				s->m_pCells.push_back(pCell);
				pCell->m_pShip = s;
			}
			else
			{
				s->ClearCells();
				delete s;
				return false;
			}
		}
		m_Ships.push_back(s);

		return true;
	}
	return false;
}
string CBattlePlayer::recieve()
{
	char buff[1024];

	recv(m_sock, &buff[0], sizeof(buff), 0);

	return buff;
}
bool CBattlePlayer::IsAlive()
{
	for (size_t i = 0; i < m_Ships.size(); i++)
	{
		if (m_Ships[i]->Alive())
			return true;
	}
	return false;
}
//
//bool CBattlePlayer::Try2PlaceShip(string ship)
//{
//	int iDeck = 0;
//	char buf[50];
//	if (sscanf_s(ship.c_str(), "%i(%[^)]", &iDeck, buf, 50))
//	{
//
//
//
//		char cell[4][10];
//		int detector = 0;
//		if (iDeck == 4)
//		{
//			//4(a1,b1,c3,c4)
//			sscanf_s(buf, "%[^,],%[^,],%[^,],%[^\n],", cell[0], 10, cell[1], 10, cell[2], 10, cell[3], 10);
//		}
//		if (iDeck == 3)
//		{
//			sscanf_s(buf, "%[^,],%[^,],%[^\n],", cell[0], 10, cell[1], 10, cell[2], 10);
//		}
//		if (iDeck == 2)
//		{
//			sscanf_s(buf, "%[^,],%[^\n],", cell[0], 10, cell[1], 10);
//		}
//
//		if (iDeck == 1)
//		{
//			sscanf_s(buf, "%[^\n],", cell[0], 10);
//		}
//		CShip* s = new CShip(iDeck);
//
//		for (int i = 0; i < iDeck; i++)
//		{
//			CCell* pCell = NULL;
//			if (m_Aqua.Try2PlaceDeck(cell[i], &pCell, s))
//			{
//				s->m_pCells.push_back(pCell);
//				pCell->m_pShip = s;
//			}
//			else
//			{
//				s->ClearCells();
//				delete s;
//				return false;
//			}
//		}
//		char first_x = cell[iDeck - 1][0];
//		char first_y = cell[iDeck - 1][0];
//		char end_x = first_x;
//		char end_y = first_y;
//		char x, y;
//		iDeck--;
//		for (int i = 1; i < iDeck + 1; i++)
//		{
//			x = cell[iDeck - i][0];
//			y = cell[iDeck - i][1];
//			if (!((x == first_x) && (abs(end_y - y) == 1) || (y == first_y) && (abs(end_x - x) == 1)))
//			{
//				s->ClearCells();
//				delete s;
//				return false;
//			}
//			end_x = x;
//			end_y = y;
//		}
//		m_Ships.push_back(s);
//		return true;
//	}
//	return false;
//}
