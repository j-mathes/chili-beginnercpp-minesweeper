#pragma once
#include "Graphics.h"
#include "RectI.h"

class Minefield
{
private:
	class Tile
	{
	public:
		enum class State
		{
			kHidden,
			kFlagged,
			kRevealed,
		};

	public:
		void SpawnMine();
		bool HasMine() const;
		void Draw(Graphics& gfx, const Vei2 screenPosition ) const;
	private:
		State state_{ State::kHidden };
		bool hasMine_{ false };
	};

public:
	Minefield(int nMines);
	void Draw(Graphics& gfx) const;
	RectI GetRect() const;
private:
	Tile& TileAt_(const Vei2& gridPosition);
	const Tile& TileAt_(const Vei2& gridPosition) const;
private:
	static constexpr int width_{ 20 };
	static constexpr int height_{ 16 };
	Tile field_[ width_ * height_ ];
};

