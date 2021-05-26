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
		void Draw(Graphics& gfx, const Vei2 screenPosition, bool hasClickedOnMine) const;
		void Reveal();
		bool IsRevealed() const;
		void ToggleFlag();
		bool IsFlagged() const;
		void SetNeighborMineCount(int mineCount);
	private:
		State state_{ State::kHidden };
		bool hasMine_{ false };
		int nNeighborMines{ -1 };
	};

public:
	Minefield(int nMines, Vei2& origin);
	void Draw(Graphics& gfx) const;
	RectI GetRect() const;
	void OnRevealClick(const Vei2& screenPosition);
	void OnFlagClick(const Vei2& screenPosition);
private:
	Tile& TileAt_(const Vei2& gridPosition);
	const Tile& TileAt_(const Vei2& gridPosition) const;
	Vei2 ScreenToGrid_(const Vei2& screenPosition);
	int CountNeighborMines_(const Vei2& gridPosition);
private:
	static constexpr int width_{ 20 };
	static constexpr int height_{ 16 };
	bool hasClickedOnMine_{ false };
	Tile field_[ width_ * height_ ];
	Vei2 origin_;
};

