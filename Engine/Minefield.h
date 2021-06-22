#pragma once
#include "Graphics.h"
#include "RectI.h"
#include "Sound.h"

class Minefield
{
public:
	enum class State
	{
		kLose,
		kWin,
		kPlaying
	};
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
		void Draw(Graphics& gfx, const Vei2& screenPosition, Minefield::State fieldState) const;
		void Reveal();
		bool IsRevealed() const;
		void ToggleFlag();
		bool IsFlagged() const;
		bool HasNoNeighborMines() const;
		void SetNeighborMineCount(int mineCount);
	private:
		State state_{ State::kHidden };
		bool hasMine_{ false };
		int nNeighborMines_{ -1 };
	};

public:
	Minefield(int nMines, const Vei2& origin, int width, int height);
	Minefield(int nMines, int width, int height);
	void Draw(Graphics& gfx) const;
	RectI GetRect() const;
	void OnRevealClick(const Vei2& screenPosition);
	void OnFlagClick(const Vei2& screenPosition);
	State GetState() const;
private:
	void RevealTile_(const Vei2& gridPosition);
	Tile& TileAt_(const Vei2& gridPosition);
	const Tile& TileAt_(const Vei2& gridPosition) const;
	Vei2 ScreenToGrid_(const Vei2& screenPosition);
	int CountNeighborMines_(const Vei2& gridPosition);
	bool GameIsWon_() const;
private:
	int width_;
	int height_;
	static constexpr int borderThickness_{10};
	static constexpr Color borderColor_{ Colors::Blue };
	Sound sndLose{ Sound(L"spayed.wav") };
	Tile* field_;
	Vei2 origin_;
	State state_{ State::kPlaying };
};

