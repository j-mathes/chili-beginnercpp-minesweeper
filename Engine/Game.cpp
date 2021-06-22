/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.cpp																			  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#include "MainWindow.h"
#include "Game.h"
#include "SpriteCodex.h"
#include <assert.h>

Game::Game(MainWindow& wnd)
	:
	wnd( wnd ),
	gfx( wnd ),
	menu( { gfx.GetRect().GetCenter().x,200 } )
{
}

Game::~Game()
{
	DestroyField();
}

void Game::Go()
{
	gfx.BeginFrame();	
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
	while( !wnd.mouse.IsEmpty() )
	{
		const auto e = wnd.mouse.Read();
		if( state == State::Minesweeper )
		{
			if( pField->GetState() == Minefield::State::kPlaying)
			{
				if( e.GetType() == Mouse::Event::Type::LPress )
				{
					const Vei2 mousePos = e.GetPos();
					if( pField->GetRect().Contains( mousePos ) )
					{
						pField->OnRevealClick( mousePos );
					}
				}
				else if( e.GetType() == Mouse::Event::Type::RPress )
				{
					const Vei2 mousePos = e.GetPos();
					if( pField->GetRect().Contains( mousePos ) )
					{
						pField->OnFlagClick( mousePos );
					}
				}
			}
			else if (pField->GetState() != Minefield::State::kPlaying)
			{
				if (e.GetType() == Mouse::Event::Type::LPress || e.GetType() == Mouse::Event::Type::RPress)
				{
					DestroyField();
					state = State::SelectionMenu;
				}
			}
		}
		else
		{
			const SelectionMenu::Size s = menu.ProcessMouse( e );
			switch( s )
			{
			case SelectionMenu::Size::Small:
				CreateField(15, 10, 10);
				state = State::Minesweeper;
				break;
			case SelectionMenu::Size::Medium:
				CreateField(30, 15, 15);
				state = State::Minesweeper;
				break;
			case SelectionMenu::Size::Large:
				CreateField(45, 20, 20);
				state = State::Minesweeper;
				break;
			}
		}
	}
}

void Game::CreateField(int nMines, int width, int height)
{
	assert(pField == nullptr);
	pField = new Minefield(nMines, width, height);
}

void Game::DestroyField()
{
	delete pField;
	pField = nullptr;
}

void Game::ComposeFrame()
{
	if( state == State::Minesweeper )
	{
		pField->Draw( gfx );
		if( pField->GetState() == Minefield::State::kWin )
		{
			SpriteCodex::DrawWin(gfx, gfx.GetRect().GetCenter());
		}
	}
	else
	{
		menu.Draw( gfx );
	}
}
