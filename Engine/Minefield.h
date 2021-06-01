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
			kFlaggedCorrect,
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
		bool IsFlaggedCorrectly() const;
		bool HasNoNeighborMines() const;
		void SetNeighborMineCount(int mineCount);
	private:
		State state_{ State::kHidden };
		bool hasMine_{ false };
		int nNeighborMines{ -1 };
	};

public:
	Minefield(int nMines, Vei2& origin);
	Minefield(int nMines);
	void Draw(Graphics& gfx) const;
	RectI GetRect() const;
	void OnRevealClick(const Vei2& screenPosition);
	void OnFlagClick(const Vei2& screenPosition);
	int CorrectMinesFlagged() const;
	int TilesRevealed() const;
	int TotalTiles() const;
private:
	void RevealTile_(const Vei2& gridPosition);
	Tile& TileAt_(const Vei2& gridPosition);
	const Tile& TileAt_(const Vei2& gridPosition) const;
	Vei2 ScreenToGrid_(const Vei2& screenPosition);
	int CountNeighborMines_(const Vei2& gridPosition);
private:
	static constexpr int width_{ 10 };
	static constexpr int height_{ 10 };
	bool hasClickedOnMine_{ false };
	Tile field_[ width_ * height_ ];
	Vei2 origin_;
};

