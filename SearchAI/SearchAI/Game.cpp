#include "Game.h"
#include "InputManager.h"
#include <string>
#include <iostream>

Game::Game()
{
	
}

Game::~Game()
{
	for each (Button *b in m_Buttons)
	{
		delete b;
	}
	m_Buttons.clear();

	for each (Dropdown *d in m_Dropdowns)
	{
		delete d;
	}
	m_Dropdowns.clear();
	delete m_Pathfinder;

	ClearPoints();
}

void Game::Run()
{
	int windowWidth = 1600, windowHeight = 900;
	sf::Vector2i mouseScreenPosition;

	// Window creation
	string programName = "SearchAI";
	m_Window = new sf::RenderWindow(sf::VideoMode(windowWidth, windowHeight), programName, sf::Style::Default);
	m_Window->setFramerateLimit(60);
	m_Window->setVerticalSyncEnabled(true);

	m_Pathfinder = new Pathfinder(&m_TileMap);

	SetupButtons();
	SetupMap();

	m_StartShape.setRadius(10);
	m_StartShape.setFillColor(sf::Color::Green);
	m_EndShape.setRadius(10);
	m_EndShape.setFillColor(sf::Color::Yellow);

	sf::Clock deltaClock;

	float dt;

	bool running = true, focus = true;

	while (running)
	{
		dt = deltaClock.restart().asSeconds();

		sf::Event event;
		while (m_Window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				running = false;
			if (event.type == sf::Event::GainedFocus)
				focus = true;
			if (event.type == sf::Event::LostFocus)
				focus = false;
			if (event.type == sf::Event::Resized)
			{
				int newWidth = event.size.width, newHeight = event.size.height;
			}
			if (event.type == sf::Event::MouseWheelMoved)
				InputManager::GetInstance().SetMouseWheelValue(event.mouseWheelScroll.wheel);
		}

		if (focus)
		{
			InputManager::GetInstance().Update();

			mouseScreenPosition = sf::Mouse::getPosition(*m_Window);

			// --------------------Update--------------------

			Update(dt, mouseScreenPosition);

			// --------------------Render--------------------
			m_Window->clear();

			Render();

			// End frame
			m_Window->display();

			InputManager::GetInstance().Clear();
		}
		else
		{
			sf::Time time = sf::milliseconds(100);
			sf::sleep(time);
		}
	}
}

void Game::Update(float dt, sf::Vector2i mousePosition)
{
	Button *clickedButton = nullptr;
	bool mouseClick, rightClick;
	mouseClick = InputManager::GetInstance().GetInput(MOUSELEFTCLICK);
	rightClick = InputManager::GetInstance().GetInput(MOUSERIGHTCLICK);

	for each (Button *b in m_Buttons)
	{
		b->Update(dt, mousePosition);
		if (mouseClick && !m_DropdownIsActive)
		{
			if (b->GetMouseover())
			{
				clickedButton = b;
			}
		}
	}

	for each (vector<Tile*> tileRow in m_TileMap)
	{
		for each (Tile *t in tileRow)
		{
			if (t->GetActive())
			{
				t->Update(dt, mousePosition);

				if (t->GetID() != 1)
				{
					if (mouseClick)
					{
						if (t->GetMouseover())
						{
							m_StartTile = t;
							m_StartShape.setPosition(t->GetPosition() + sf::Vector2f(10, 10));
						}
					}
					else if (rightClick)
					{
						if (t->GetMouseover())
						{
							m_EndTile = t;
							m_EndShape.setPosition(t->GetPosition() + sf::Vector2f(10, 10));
						}
					}
				}
			}
		}
	}

	for each (Dropdown *d in m_Dropdowns)
	{
		d->Update(dt, mousePosition);
		if (mouseClick)
		{
			if (d->GetMouseover())
			{
				if (m_DropdownIsActive)
				{
					for each (Button *b in d->GetButtons())
					{
						if (b->GetMouseover())
						{
							clickedButton = b;
						}
					}
				}

				if (d->GetActive())
				{
					d->SetActive(false);
					m_DropdownIsActive = false;
				}
				else
				{
					d->SetActive(true);
					m_DropdownIsActive = true;
				}
			}
		}
	}
	m_DropdownIsActive = false;
	for each (Dropdown *d in m_Dropdowns)
	{
		if (d->GetActive() == true) m_DropdownIsActive = true;
	}

	if (clickedButton != nullptr)
	{
		short buttonID = clickedButton->GetID();

		if (buttonID == 0)
		{
			cout << "Search" << endl;

			if (m_StartTile != nullptr && m_EndTile != nullptr)
			{
				if (m_ActiveSearchMode == DEEP) SetPathLine(m_Pathfinder->DeepSearch(m_StartTile, m_EndTile));
				else if (m_ActiveSearchMode == BROAD) SetPathLine(m_Pathfinder->BroadSearch(m_StartTile, m_EndTile));
				else if (m_ActiveSearchMode == COST) SetPathLine(m_Pathfinder->CostSearch(m_StartTile, m_EndTile));
				else SetPathLine(m_Pathfinder->StarSearch(m_StartTile, m_EndTile));
			}
		}
		else if (buttonID == 1)
		{
			cout << "Switch to Map 1" << endl;
			SwitchMap(0);
		}
		else if (buttonID == 2)
		{
			cout << "Switch to Map 2" << endl;
			SwitchMap(1);
		}
		else if (buttonID == 3)
		{
			cout << "Switch to Map 3" << endl;
			SwitchMap(2);
		}
		else if (buttonID == 4)
		{
			cout << "Switch to Map 4" << endl;
			SwitchMap(3);
		}
		else if (buttonID == 5)
		{
			cout << "Deep Search" << endl;
			m_ActiveSearchMode = DEEP;
		}
		else if (buttonID == 6)
		{
			cout << "Broad Search" << endl;
			m_ActiveSearchMode = BROAD;
		}
		else if (buttonID == 7)
		{
			cout << "Cost Search" << endl;
			m_ActiveSearchMode = COST;
		}
		else if (buttonID == 8)
		{
			cout << "A* Search" << endl;
			m_ActiveSearchMode = Astar;
		}
		else if (buttonID == 9)
		{
			cout << "Clockwise Search" << endl;
			m_Pathfinder->SetSearchOrder(CLOCK);
		}
		else if (buttonID == 10)
		{
			cout << "Counter Clockwise Search" << endl;
			m_Pathfinder->SetSearchOrder(COUNTER);
		}
		else if (buttonID == 11)
		{
			cout << "Random order Search" << endl;
			m_Pathfinder->SetSearchOrder(RANDOM);
		}
	}
}

void Game::Render()
{
	for each (Button *b in m_Buttons)
	{
		b->Render(m_Window);
	}

	Dropdown *activeDropdown = nullptr;

	for each (Dropdown *d in m_Dropdowns)
	{
		if (d->GetActive()) activeDropdown = d;
		else d->Render(m_Window);
	}
	if (activeDropdown != nullptr) activeDropdown->Render(m_Window);

	for each (vector<Tile*> tileRow in m_TileMap)
	{
		for each (Tile *t in tileRow)
		{
			if (t->GetActive())
				t->Render(m_Window);
		}
	}

	for each (LineSegment *l in m_PathLine)
	{
		l->Render(m_Window);
	}

	if (m_StartTile != nullptr) m_Window->draw(m_StartShape);
	if (m_EndTile != nullptr) m_Window->draw(m_EndShape);
}

void Game::SetupButtons()
{
	if (!m_ButtonFont.loadFromFile("Resources/Fonts/calibri.ttf"))
		cout << "Could not find font calibri.ttf" << endl;

	Button *button = new Button(sf::String("Search"), &m_ButtonFont, 0);
	button->SetPosition(sf::Vector2f(830, 30));
	m_Buttons.push_back(button);

	Dropdown *dropdown = new Dropdown(sf::String("Map"), &m_ButtonFont, -1);
	dropdown->SetPosition(sf::Vector2f(830, 80));
	m_Dropdowns.push_back(dropdown);

	button = new Button(sf::String("Open Map"), &m_ButtonFont, 1);
	dropdown->AddButton(button);
	button = new Button(sf::String("Labyrinth"), &m_ButtonFont, 2);
	dropdown->AddButton(button);
	button = new Button(sf::String("Cost Map"), &m_ButtonFont, 3);
	dropdown->AddButton(button);
	button = new Button(sf::String("Small Map"), &m_ButtonFont, 4);
	dropdown->AddButton(button);

	dropdown = new Dropdown(sf::String("Search Type"), &m_ButtonFont, -1);
	dropdown->SetPosition(sf::Vector2f(830, 130));
	m_Dropdowns.push_back(dropdown);

	button = new Button(sf::String("Deep Search"), &m_ButtonFont, 5);
	dropdown->AddButton(button);
	button = new Button(sf::String("Broad Search"), &m_ButtonFont, 6);
	dropdown->AddButton(button);
	button = new Button(sf::String("Cost Search"), &m_ButtonFont, 7);
	dropdown->AddButton(button);
	button = new Button(sf::String("A* Search"), &m_ButtonFont, 8);
	dropdown->AddButton(button);

	dropdown = new Dropdown(sf::String("Search Order"), &m_ButtonFont, -1);
	dropdown->SetPosition(sf::Vector2f(830, 180));
	m_Dropdowns.push_back(dropdown);

	button = new Button(sf::String("Clockwise"), &m_ButtonFont, 9);
	dropdown->AddButton(button);
	button = new Button(sf::String("Counter Clockwise"), &m_ButtonFont, 10);
	dropdown->AddButton(button);
	button = new Button(sf::String("Random"), &m_ButtonFont, 11);
	dropdown->AddButton(button);
}

void Game::SetupMap()
{
	for (size_t y = 0; y < 20; y++)
	{
		vector<Tile*> tileRow;
		for (size_t x = 0; x < 20; x++)
		{
			Tile *tile;
			tile = new Tile(m_Map1[y][x]);

			tile->SetPosition(sf::Vector2f(x * 40 + 10, y * 40 + 10));

			tileRow.push_back(tile);
		}
		m_TileMap.push_back(tileRow);
	}

	for (size_t y = 0; y < 20; y++)
	{
		for (size_t x = 0; x < 20; x++)
		{
			Tile *neighbors[8];
			for (size_t i = 0; i < 8; i++)
			{
				neighbors[i] = nullptr;
			}

			if (y > 0)
			{
				if (x < 19) neighbors[1] = m_TileMap[y - 1][x + 1];

				neighbors[0] = m_TileMap[y - 1][x];

				if (x > 0) neighbors[7] = m_TileMap[y - 1][x - 1];
			}

			if (y < 19)
			{
				if (x < 19) neighbors[3] = m_TileMap[y + 1][x + 1];

				neighbors[4] = m_TileMap[y + 1][x];

				if (x > 0) neighbors[5] = m_TileMap[y + 1][x - 1];
			}

			if (x < 19) neighbors[2] = m_TileMap[y][x + 1];

			if (x > 0) neighbors[6] = m_TileMap[y][x - 1];

			m_TileMap[y][x]->SetNeighbors(neighbors);
		}
	}
}

void Game::ClearPoints()
{
	m_StartTile = nullptr;
	m_EndTile = nullptr;

	for each (LineSegment *l in m_PathLine)
	{
		delete l;
	}
	m_PathLine.clear();
}

void Game::SwitchMap(short ID)
{
	ClearPoints();

	for (size_t y = 0; y < 20; y++)
	{
		for (size_t x = 0; x < 20; x++)
		{
			Tile *tile = m_TileMap[y][x];
			tile->SetActive(true);

			tile->SetSearchState(CLEAR);

			if (ID == 0)
				tile->SetID(m_Map1[y][x]);
			else if (ID == 1)
			{
				if (x < 11)
					tile->SetID(m_Map2[y][x]);
				else
					tile->SetActive(false);
			}
			else if (ID == 2)
				tile->SetID(m_Map3[y][x]);
			else
			{
				if (y < 4 && x < 4)
					tile->SetID(m_Map4[y][x]);
				else
					tile->SetActive(false);
			}
		}
	}
}

void Game::SetPathLine(Node *node)
{
	for each (LineSegment *l in m_PathLine)
	{
		delete l;
	}
	m_PathLine.clear();

	Node *activeNode = node;

	while (activeNode->parent != nullptr)
	{
		LineSegment *segment = new LineSegment(activeNode->tile->GetPosition() + sf::Vector2f(20, 20), activeNode->parent->tile->GetPosition() + sf::Vector2f(20, 20), 10);
		m_PathLine.push_back(segment);

		activeNode = activeNode->parent;
	}
}