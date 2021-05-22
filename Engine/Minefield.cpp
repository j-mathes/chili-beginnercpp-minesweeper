#include "Minefield.h"
#include <assert.h>
#include <random>
#include "Vei2.h"
#include "SpriteCodex.h"


void Minefield::Tile::SpawnMine()
{
	assert(!HasMine());
	hasMine_ = true;
}

bool Minefield::Tile::HasMine() const
{
	return hasMine_;
}

void Minefield::Tile::Draw(Graphics& gfx, const Vei2 screenPosition) const
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
			SpriteCodex::DrawTile0(gfx, screenPosition);
		}
		else
		{
			SpriteCodex::DrawTileBomb(gfx, screenPosition);
		}
		break;
	default:
		break;
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

Minefield::Minefield(int nMines)
{
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

	for (int i = 0; i < 140; i++)
	{
		const Vei2 gridPosition = { xDist(rng), yDist(rng) };
		if (!TileAt_(gridPosition).IsRevealed())
		{
			TileAt_(gridPosition).Reveal();
		}
	}
}

void Minefield::Draw(Graphics& gfx) const
{
	gfx.DrawRect(GetRect(), SpriteCodex::baseColor);
	for (Vei2 gridPosition = { 0,0 }; gridPosition.y < height_; gridPosition.y++)
	{
		for (gridPosition.x = 0; gridPosition.x < width_; gridPosition.x++)
		{
			TileAt_(gridPosition).Draw(gfx, gridPosition * SpriteCodex::tileSize);
		}
	}
}

RectI Minefield::GetRect() const
{
	return RectI(0, width_ * SpriteCodex::tileSize, 0, height_ * SpriteCodex::tileSize);
}

Minefield::Tile& Minefield::TileAt_(const Vei2& gridPosition)
{
	return field_[gridPosition.y * width_ + gridPosition.x];
}

const Minefield::Tile& Minefield::TileAt_(const Vei2& gridPosition) const
{
	return field_[gridPosition.y * width_ + gridPosition.x];
}
