#pragma once

#include "Graphics.h"
#include "Vei2.h"

class SpriteCodex
{
public:
	// width and height of all tiles
	static constexpr int tileSize = 16;
	// base color for all tiles
	static constexpr Color baseColor = { 192,192,192 };
	// 16x16 tile sprites assume (192,192,192) background and top left origin
	static void DrawTile0(Graphics& gfx, const Vei2& pos);
	static void DrawTile1(Graphics& gfx, const Vei2& pos);
	static void DrawTile2(Graphics& gfx, const Vei2& pos);
	static void DrawTile3(Graphics& gfx, const Vei2& pos);
	static void DrawTile4(Graphics& gfx, const Vei2& pos);
	static void DrawTile5(Graphics& gfx, const Vei2& pos);
	static void DrawTile6(Graphics& gfx, const Vei2& pos);
	static void DrawTile7(Graphics& gfx, const Vei2& pos);
	static void DrawTile8(Graphics& gfx, const Vei2& pos);
	static void DrawTileButton(Graphics& gfx, const Vei2& pos);
	static void DrawTileCross(Graphics& gfx, const Vei2& pos);
	static void DrawTileFlag(Graphics& gfx, const Vei2& pos);
	static void DrawTileBomb(Graphics& gfx, const Vei2& pos);
	static void DrawTileBombRed(Graphics& gfx, const Vei2& pos);
	static void DrawTileNumber(Graphics& gfx, int n, const Vei2& pos);
};