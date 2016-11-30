#include "DxLib.h"
#include<array>
#include<cstring>

namespace {
	enum Status { TITLE, MAIN, CLEAR } status = TITLE;
}

struct DxWrap final{
	explicit DxWrap(const std::size_t x, const std::size_t y, const std::size_t bit,const std::size_t fps)
		:width(x), height(y), bit(bit), pics{}
	{
		SetGraphMode(x, y, bit, fps);
		ChangeWindowMode(TRUE);
		DxLib_Init();
		SetDrawScreen(DX_SCREEN_BACK);
	}
	DxWrap& load_graph(const char* image_src)
	{
		LoadDivGraph(image_src, 16, 4, 4, 80, 80, pics.data());
		pic = LoadGraph(image_src);
		DrawGraph(0, 0, pic, TRUE);
		return *this;
	}

	~DxWrap() { DxLib_End(); }
private:
	const std::size_t bit;
public:
	const std::size_t width, height;
	int pic;
	std::array<int, 16> pics;
};

struct GameMain final{
	explicit GameMain(const DxWrap& dxwrap,const int font)
		:dxwrap_(dxwrap),font_(font){}

	void Title(const char* mes) 
	{
		if (GetMouseInput() & MOUSE_INPUT_LEFT) {
			std::size_t i = 0;
			for (int& value : panel)value = i++;
			for (i = 0; i < 1000; i++)change(GetRand(3), GetRand(3));
			status = MAIN;
		}
		Message(mes);
	}

	void Game() 
	{
		if (GetMouseInput() & MOUSE_INPUT_LEFT) {
			int x, y;
			GetMousePoint(&x, &y);
			change(x / 80, y / 80);
			bool clear = true;
			std::size_t i = 0;
			for(const int& value:panel){
				if (value != i++) clear = false;
			}
			if (clear) status = CLEAR;
		}

		std::size_t i=0;
		for(int& value:panel){
			if (value < 15) {
				DrawGraph((i % 4) * 80, (i / 4) * 80, dxwrap_.pics[value], FALSE);
			}
			++i;
		}
	}

	void Message(const char* mes)
	{
		DrawGraph(0, 0, dxwrap_.pic, FALSE);
		DrawStringToHandle((dxwrap_.width - GetDrawStringWidthToHandle(mes, std::strlen(mes), font_)) / 2, dxwrap_.height / 2 - 10, mes, GetColor(0, 100, 255), font_);
	}
private:
	void change(const int x,const int y)
	{
		int p1 = y * 4 + x;
		int p2 = -1;
		if (x > 0 && panel[p1 - 1] == 15) p2 = p1 - 1;
		if (x < 3 && panel[p1 + 1] == 15) p2 = p1 + 1;
		if (y > 0 && panel[p1 - 4] == 15) p2 = p1 - 4;
		if (y < 3 && panel[p1 + 4] == 15) p2 = p1 + 4;
		if (p2 != -1) {
			panel[p2] = panel[p1];
			panel[p1] = 15;
		}
	}

	const DxWrap& dxwrap_;
	std::array<int, 16> panel;
	int font_;
};

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	DxWrap dx_handler(320, 320, 32, 60);
	dx_handler.load_graph("puzzle.png");

	GameMain gm(dx_handler, CreateFontToHandle("メイリオ", 30, 1, DX_FONTTYPE_ANTIALIASING_EDGE));
	
	while (!ScreenFlip() && !ProcessMessage() && !ClearDrawScreen()) {
		switch (status) {
			case TITLE:
				gm.Title("Click To Start"); break;
			case MAIN:
				gm.Game(); break;
			case CLEAR:
				gm.Message("GAME CLEAR"); break;
		}
	}
}
