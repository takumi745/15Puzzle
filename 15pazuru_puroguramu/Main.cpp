#include "DxLib.h"

#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 480
#define SCREEN_COLOR   32

#define BACK_CHIP "Res\\BackGround.jpg"
#define FONT_FILE "Font\\GAU_private_R.TTF"
#define FONT_CHIP "GAU_font_PRIVAtE"
#define SOUND_CHIP "Sound\\BGM.mp3"

#define PANEL_CHIP "Res\\15puzzle.jpg"
#define PANEL_SIZE_X  185
#define PANEL_SIZE_Y  185
#define SPLIT_MAX	   16
#define SPLIT_X			4
#define SPLIT_Y			4
#define RANDOM_NUM	  500 

enum SCENE_STATUS
{
	TITLE,
	GAME,
	CLEAR,
	GAMEOVER,
};
SCENE_STATUS Scene = TITLE;

int GHBack;
int Font;
int BGM;

int StartTime;
int GameTime;
int Move;
int LimitCounter;
int Score;
int FadeColor;

bool ClearFlag;
bool GameOverFlag;
bool FadeFlag;
bool FlashFlag;
bool KeyFlag;

int GHPicsAll;
int GHPics[SPLIT_MAX];
int Panel[SPLIT_MAX];
int PanelWidth = PANEL_SIZE_X / SPLIT_X;
int PanelHeight = PANEL_SIZE_Y / SPLIT_Y;
int PanelSpace = SPLIT_MAX - 1;;

void DrawFadeOut() 
{
	if (FadeColor >= 0 && FadeColor < 256)
		SetDrawBright(255 - FadeColor, 255 - FadeColor, 255 - FadeColor);

	FadeColor += 2;

	if (FadeColor == 256) {
		switch (Scene) {
		case TITLE:
			Scene = GAME;
			break;
		}
	}
}

void ChangePanel(int x, int y) 
{
	//4�~4��Click�����̈�
	int P1 = y * SPLIT_X + x;
	int P2 = -1;

	if ((x > 0) && (Panel[P1 - 1] == PanelSpace)) {
		P2 = P1 - 1;
	}
	if ((x < SPLIT_X - 1) && (Panel[P1 + 1] == PanelSpace)) {
		P2 = P1 + 1;
	}
	if ((y > 0) && (Panel[P1 - SPLIT_X] == PanelSpace)) {
		P2 = P1 - SPLIT_X;
	}
	if ((y < SPLIT_X - 1) && (Panel[P1 + SPLIT_X] == PanelSpace)) {
		P2 = P1 + SPLIT_X;
	}
	if (P2 != -1) {
		Panel[P2] = Panel[P1];
		Panel[P1] = PanelSpace;

		if (Scene == TITLE) {
			LimitCounter++;
		}
		else if (Scene == GAME) {
			Move++;
		}
	}
	
}

void DrawGameOver() 
{
	DrawRotaGraph(320, 240, 2.5, 0, GHBack, TRUE);

	SetFontSize(32);
	DrawString(100, 100, "GAMEOVER", GetColor(0, 0, 0));

	if (!KeyFlag) {
		if (GetMouseInput()&MOUSE_INPUT_LEFT) {
			KeyFlag = true;
			FadeColor = 0;
			FadeFlag = false;
			Scene = TITLE;
		}
	}
	if (!GetMouseInput()&MOUSE_INPUT_LEFT) {
		KeyFlag = false;
	}
}

void DrawGame() 
{
	SetDrawBright(255, 255, 255);

	GameTime = (GetNowCount() - StartTime) / 1000;

	DrawRotaGraph(320, 240, 2.5, 0, GHBack, TRUE);
	SetFontSize(25);
	DrawFormatString(10, 10, GetColor(0, 0, 0), "TIME");
	DrawFormatString(10, 60, GetColor(0, 0, 0), "%2d/60", GameTime);
	DrawFormatString(10, 110, GetColor(0, 0, 0), "LIMIT");
	DrawFormatString(10, 160, GetColor(0, 0, 0), "  %2d", LimitCounter);
	DrawFormatString(10, 210, GetColor(0, 0, 0), "MOVE");
	DrawFormatString(10, 260, GetColor(0, 0, 0), "  %2d", Move);
	
	if (!KeyFlag) {
		if (GetMouseInput()&MOUSE_INPUT_LEFT) {
			KeyFlag = true;
			int x, y;
			GetMousePoint(&x, &y);
			if ((x > (SCREEN_WIDTH - PANEL_SIZE_X) / 2 && x < (SCREEN_WIDTH + PANEL_SIZE_X) / 2) ||
				(y > (SCREEN_HEIGHT - PANEL_SIZE_Y) / 2 && y < (SCREEN_HEIGHT + PANEL_SIZE_Y) / 2)) {
				ChangePanel((x - (SCREEN_WIDTH - PANEL_SIZE_X) / 2) / PanelWidth, (y - (SCREEN_HEIGHT - PANEL_SIZE_Y) / 2) / PanelHeight);
			}
		}
	}
	if (!GetMouseInput()&MOUSE_INPUT_LEFT) {
		KeyFlag = false;
	}
	
	if (GameTime == 60 || Move > LimitCounter) {
		GameOverFlag = true;
		Scene = GAMEOVER;
	}

	ClearFlag = true;
	for (int i = 0; i < SPLIT_MAX; i++) {
		if (Panel[i] != i)
			ClearFlag = false;
	}
	if (ClearFlag) {
		Score = (60 - GameTime) * 100 + (LimitCounter - Move) * 50;
		Scene = CLEAR;
	}

	for (int i = 0; i < SPLIT_MAX; i++) {
		if (Panel[i] < PanelSpace) {
			DrawGraph((i%SPLIT_X)*PanelWidth + (SCREEN_WIDTH - PANEL_SIZE_X) / 2, (i / SPLIT_Y)*PanelHeight + (SCREEN_HEIGHT - PANEL_SIZE_Y) / 2,
				GHPics[Panel[i]], FALSE);
		}
	}
}

void DrawClear() 
{
	DrawRotaGraph(320, 240, 2.5, 0, GHBack, TRUE);

	SetFontSize(32);
	DrawString(100, 100, "CLEAR", GetColor(0, 0, 0));

	SetFontSize(20);
	DrawFormatString(120, 240, GetColor(0, 0, 0), "TIME");
	DrawFormatString(120, 260, GetColor(0, 0, 0), "  %2d", GameTime);
	DrawFormatString(120, 300, GetColor(0, 0, 0), "MOVE");
	DrawFormatString(120, 320, GetColor(0, 0, 0), "  %2d", Move);
	DrawFormatString(120, 360, GetColor(0, 0, 0), "SCORE");
	DrawFormatString(120, 380, GetColor(0, 0, 0), "%d", Score);

	if (!KeyFlag) {
		if (GetMouseInput()&MOUSE_INPUT_LEFT) {
			KeyFlag = true;
			FadeColor = 0;
			FadeFlag = false;
			Scene = TITLE;
		}
	}
	if (!GetMouseInput()&MOUSE_INPUT_LEFT) {
		KeyFlag = false;
	}
}

void DrawTitle() 
{
	StartTime = GetNowCount();
	LimitCounter = 0;
	Move = 0;
	
	DrawRotaGraph(315, 240, 2.5, 0, GHBack, TRUE);

	SetFontSize(64);
	DrawString(80, 100, "15PUZZLE", GetColor(0, 0, 0));

	SetFontSize(25);
	static int count = 0;
	count = (count + 1) % 30;
	if (count != 0) {
		if (!FlashFlag) {
			DrawString(180, 300, "CLICK TO START", GetColor(0, 0, 0));
		}
	}
	else {
		FlashFlag = !FlashFlag;
	}

	if (FadeFlag) {
		for (int i = 0; i < SPLIT_MAX; i++) {
			Panel[i] = i;
		}

		SRand(RANDOM_NUM);
		for (int i = 0; i < 200; i++) {
			ChangePanel(GetRand(3), GetRand(3));
		}
		DrawFadeOut();
		return;
	}
	
	if (!KeyFlag) {
		if (GetMouseInput()&MOUSE_INPUT_LEFT) {
			KeyFlag = true;
			FadeFlag = true;
		}
	}
	if (!GetMouseInput()&MOUSE_INPUT_LEFT) {
		KeyFlag = false;
	}
}

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) 
{
	SetGraphMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_COLOR);
	ChangeWindowMode(TRUE);
	DxLib_Init();
	SetDrawScreen(DX_SCREEN_BACK);

	GHBack = LoadGraph(BACK_CHIP);
	GHPicsAll = LoadGraph(PANEL_CHIP);
	LoadDivGraph(PANEL_CHIP, SPLIT_MAX, SPLIT_X, SPLIT_Y, PanelWidth, PanelHeight, GHPics);

	BGM = LoadSoundMem(SOUND_CHIP);
	PlaySoundMem(BGM, DX_PLAYTYPE_LOOP);

	LPCSTR FontPath = FONT_FILE;
	AddFontResourceEx(FontPath, FR_PRIVATE, NULL);
	ChangeFont(FONT_CHIP, DX_CHARSET_DEFAULT);

	GameOverFlag = false;
	FlashFlag = false;
	KeyFlag = false;
	FadeFlag = false;
	FadeColor = 0;

	while (!ProcessMessage()) 
	{
		ClearDrawScreen();
		switch (Scene) 
		{
		case TITLE:
			DrawTitle();
			break;
		case GAME:
			DrawGame();
			break;
		case CLEAR:
			DrawClear();
			break;
		case GAMEOVER:
			DrawGameOver();
			break;
		}
		ScreenFlip();
	}
	RemoveFontResource(FontPath);
	DxLib_End();
	return 0;
}
