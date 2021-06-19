#include "Minefield.h"
#include <assert.h>
#include <random>
#include "Vei2.h"
#include "SpriteCodex.h"
#include <algorithm>


void Minefield::Tile::SpawnMine()
{
	assert(!HasMine());
	hasMine_ = true;
}

bool Minefield::Tile::HasMine() const
{
	return hasMine_;
}

void Minefield::Tile::Draw(Graphics& gfx, const Vei2& screenPosition, Minefield::State fieldState) const
{
	if (fieldState != Minefield::State::kLose)
	{
		switch (state_)
		{
		case Minefield::Tile::State::kHidden:
				SpriteCodex::DrawTileButton(gfx, screenPosition);

			break;
		case Minefield::Tile::State::kFlagged:
				SpriteCodex::DrawTileButton(gfx, screenPosition);
				SpriteCodex::DrawTileFlag(gfx, screenPosition);
			break;
		case Minefield::Tile::State::kRevealed:
			if (!HasMine())
			{
				SpriteCodex::DrawTileNumber(gfx, nNeighborMines_, screenPosition);
			}
			else
			{
				SpriteCodex::DrawTileBomb(gfx, screenPosition);
			}
			break;
		}
	}
	else
	{
		switch (state_)
		{
		case Minefield::Tile::State::kHidden:
			if (!HasMine())
			{
				SpriteCodex::DrawTileBomb(gfx, screenPosition);
			}
			else
			{
				SpriteCodex::DrawTileButton(gfx, screenPosition);
			}
			break;
		case Minefield::Tile::State::kFlagged:
			if (!HasMine())
			{
				SpriteCodex::DrawTileBomb(gfx, screenPosition);
				SpriteCodex::DrawTileFlag(gfx, screenPosition);
			}
			else
			{
				SpriteCodex::DrawTileBomb(gfx, screenPosition);
				SpriteCodex::DrawTileCross(gfx, screenPosition);
			}
			break;
		case Minefield::Tile::State::kRevealed:
			if (!HasMine())
			{
				SpriteCodex::DrawTileNumber(gfx, nNeighborMines_, screenPosition);
			}
			else
			{
				SpriteCodex::DrawTileBombRed(gfx, screenPosition);
			}
			break;
		}
	}
}

void Minefield::Tile::Reveal()
{
	assert(state_ == State::kHidden);
	state_ = State::kRevealed;
}

bool Minefield::Tile::IsRevealed() const
{
	return state_ == State::kRevealed;
}

void Minefield::Tile::ToggleFlag()
{
	assert(!IsRevealed());
	if (state_ == State::kHidden)
	{
			state_ = State::kFlagged;
		
	}
	else
	{
		state_ = State::kHidden;
	}
}

bool Minefield::Tile::IsFlagged() const
{
	return state_ == State::kFlagged;
}

bool Minefield::Tile::HasNoNeighborMines() const
{
	return nNeighborMines_ == 0;
}

void Minefield::Tile::SetNeighborMineCount(int mineCount)
{
	assert(nNeighborMines_ = -1);
	nNeighborMines_ = mineCount;
}

Minefield::Minefield(int nMines, const Vei2& origin)
	:
	origin_{ origin }
{
	assert(origin_.x > 0);
	assert(origin_.y > 0);
	assert(origin_.x + width_ * SpriteCodex::tileSize < Graphics::ScreenWidth);
	assert(origin_.y + height_ * SpriteCodex::tileSize < Graphics::ScreenHeight);
	assert(nMines > 0 && nMines < width_ * height_);
	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> xDist(0, width_ - 1);
	std::uniform_int_distribution<int> yDist(0, height_ - 1);
	
	for (int nSpawned = 0; nSpawned < nMines ; nSpawned++)
	{
		Vei2 spawnPosition;
		do 
		{
			spawnPosition = { xDist(rng), yDist(rng) };
		} while (TileAt_(spawnPosition).HasMine());
		TileAt_(spawnPosition).SpawnMine();
	}
	for (Vei2 gridPosition = { 0,0 }; gridPosition.y < height_; gridPosition.y++)
	{
		for (gridPosition.x = 0; gridPosition.x < width_; gridPosition.x++)
		{
			TileAt_(gridPosition).SetNeighborMineCount(CountNeighborMines_(gridPosition));
		}
	}
}

Minefield::Minefield(int nMines)
	:
	Minefield(
		nMines, 
		Vei2{
			(Graphics::ScreenWidth / 2) - (width_ * SpriteCodex::tileSize / 2),
			(Graphics::ScreenHeight / 2) - (height_ * SpriteCodex::tileSize / 2)
		}
	)
{
}

void Minefield::Draw(Graphics& gfx) const
{
	gfx.DrawRect(GetRect().GetExpanded(borderThickness_),borderColor_);
	gfx.DrawRect(GetRect(), SpriteCodex::baseColor);
	for (Vei2 gridPosition = { 0, 0 }; gridPosition.y < height_; gridPosition.y++)
	{
		for (gridPosition.x = 0; gridPosition.x < width_; gridPosition.x++)
		{
			TileAt_(gridPosition).Draw(gfx, gridPosition * SpriteCodex::tileSize + origin_, state_);
		}
	}
}

RectI Minefield::GetRect() const
{
	return RectI(origin_.x, origin_.x + width_ * SpriteCodex::tileSize, origin_.y, origin_.y + height_ * SpriteCodex::tileSize);
}

void Minefield::OnRevealClick(const Vei2& screenPosition)
{
	if(state_ == State::kPlaying)
	{
		const Vei2 gridPosition = ScreenToGrid_(screenPosition);
		assert(gridPosition.x >= 0 && gridPosition.x < width_&& gridPosition.y >= 0 && gridPosition.y < height_);
		RevealTile_(gridPosition);
		if (GameIsWon_())
		{
			state_ = State::kWin;
		}
	}
}

void Minefield::OnFlagClick(const Vei2& screenPosition)
{
	if (state_ == State::kPlaying)
	{
		const Vei2 gridPosition = ScreenToGrid_(screenPosition);
		assert(gridPosition.x >= 0 && gridPosition.x < width_&& gridPosition.y >= 0 && gridPosition.y < height_);
		Tile& tile = TileAt_(gridPosition);
		if (!tile.IsRevealed())
		{
			tile.ToggleFlag();
			if (tile.IsFlagged() && GameIsWon_())
			{
				state_ = State::kWin;
			}
		}
	}
}

Minefield::State Minefield::GetState() const
{
	return state_;
}

void Minefield::RevealTile_(const Vei2& gridPosition)
{
	Tile& tile = TileAt_(gridPosition);
	if (!tile.IsRevealed() && !tile.IsFlagged())
	{
		tile.Reveal();
		if (tile.HasMine())
		{
			state_ = State::kLose;

			sndLose.Play();
		}
		else if (tile.HasNoNeighborMines())
		{
			const int xStart = std::max(0, gridPosition.x - 1);
			const int yStart = std::max(0, gridPosition.y - 1);
			const int xEnd = std::min(width_ - 1, gridPosition.x + 1);
			const int yEnd = std::min(height_ - 1, gridPosition.y + 1);
			for (Vei2 gridPosition = { xStart, yStart }; gridPosition.y <= yEnd; gridPosition.y++)
			{
				for (gridPosition.x = xStart; gridPosition.x <= xEnd; gridPosition.x++)
				{

						RevealTile_(gridPosition);

				}
			}
		}
	}
}

Minefield::Tile& Minefield::TileAt_(const Vei2& gridPosition)
{
	return field_[gridPosition.y * width_ + gridPosition.x];
}

const Minefield::Tile& Minefield::TileAt_(const Vei2& gridPosition) const
{
	return field_[gridPosition.y * width_ + gridPosition.x];
}

Vei2 Minefield::ScreenToGrid_(const Vei2& screenPosition)
{
	return (screenPosition - origin_) / SpriteCodex::tileSize;
}

int Minefield::CountNeighborMines_(const Vei2& gridPosition)
{
	const int xStart = std::max(0, gridPosition.x - 1);
	const int yStart = std::max(0, gridPosition.y - 1);
	const int xEnd = std::min(width_ - 1, gridPosition.x + 1);
	const int yEnd = std::min(height_ - 1, gridPosition.y + 1);

	int count = 0;
	for (Vei2 gridPosition = { xStart, yStart }; gridPosition.y <= yEnd; gridPosition.y++)
	{
		for (gridPosition.x = xStart; gridPosition.x <= xEnd; gridPosition.x++)
		{
			if (TileAt_(gridPosition).HasMine())
			{
				count++;
			}
		}
	}
	return count;
}

bool Minefield::GameIsWon_() const 
{
	for (const Tile& t : field_)
	{
		if ((t.HasMine() && !t.IsFlagged()) ||
			(!t.HasMine() && !t.IsRevealed()))
		{
			return false;
		}
	}
	return true;
}
