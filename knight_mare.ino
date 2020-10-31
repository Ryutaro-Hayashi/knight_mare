#ifndef _ARDUBOY2_H
#define _ARDUBOY2_H
#include <Arduboy2.h>
Arduboy2 arduboy;
Sprites sprites;
#endif

#ifndef _ARDUBOYTONES_H
#define _ARDUBOYTONES_H
#include <ArduboyTones.h>
ArduboyTones sound(arduboy.audio.enabled);
#endif

#include "data.h"

#define SCRN_FPS 30
#define TLE_X 30 //--タイトル画面文字列のX
#define TLE_Y 12 //--タイトル画面文字列の1列目Y

enum GameState
{                 //--画面モードを表す列挙定数
  GAME_TITLE = 1, //タイトル、メイン、ステージ開始、中断、ゲームオーバー、ゲームクリア
  GAME_MAIN,
  GAME_START,
  GAME_PAUSE,
  GAME_OVER,
  GAME_CLEAR,
  GAME_TEST
};

enum SoundState
{ //--サウンド再生モードを示す列挙体
  SOUND_OFF = 0,
  SOUND_ON
};

SoundState g_soundstate = SOUND_ON;
GameState g_gamestate = GAME_TITLE;
float g_hx = 0, g_hy = 0; //--座標

//関数プロとタイイプ宣言
void DrawGameTitle();
void DrawGameMain();
void DrawGameClear();
void DrawGameOver();

long g_lasttime = 0;
float g_frametime = 0;

void setup()
{
  arduboy.setFrameRate(SCRN_FPS);
  arduboy.begin();
  arduboy.clear();
}

void loop()
{
  if (!(arduboy.nextFrame()))
    return;
  arduboy.clear();

  g_lasttime = millis();

  while ((arduboy.pressed(A_BUTTON) == false) || (arduboy.pressed(B_BUTTON) == false))
  {
    if (!(arduboy.nextFrame()))
      continue;

    long curtime = millis(); //現在の時刻を記録
    g_frametime = (float)(curtime - g_lasttime) / 1000.0f;
    g_lasttime = curtime;

    switch (g_gamestate)
    {
    case GAME_TITLE:
      DrawGameTitle();
      break;
    case GAME_MAIN:
      DrawGameMain();
    case GAME_CLEAR:
      DrawGameClear();
      break;
    case GAME_OVER:
      DrawGameOver();
      break;
    }

    arduboy.display(); //--画面消去
  }

  // put your main code here, to run repeatedly:
}

//タイトル画面描画
void DrawGameTitle()
{

  arduboy.drawBitmap(TLE_X, TLE_Y, op_title, 65, 23, 1);
  arduboy.setCursor(TLE_X, TLE_Y + 27);
  arduboy.println("A:GAME START");

  arduboy.setCursor(TLE_X, TLE_Y + 37);
  if (arduboy.pressed(B_BUTTON))
  {
    if (g_soundstate == SOUND_OFF)
    {
      g_soundstate = SOUND_ON;
    }
    else
    {
      g_soundstate = SOUND_ON;
    }
  }

  arduboy.setCursor(TLE_X, TLE_Y + 37);
  arduboy.print("B:SOUND ");

  if (g_soundstate == SOUND_ON)
  {
    arduboy.print("ON");
  }
  else
  {
    arduboy.print("OFF");
  }

  /*  if ((arduboy.pressed(A_BUTTON) == true))
    {
        setGameState(GAME_START);
    };
    if ((arduboy.pressed(B_BUTTON)))
    {
        setGameState(GAME_TEST);
    }
*/
}

//ゲーム本編描画
void DrawGameMain()
{
  float mv = 80.0f * g_frametime; //移動量計算
  if (arduboy.pressed(DOWN_BUTTON))
    g_hy += mv;
  if (arduboy.pressed(UP_BUTTON))
    g_hy -= mv;
  if (arduboy.pressed(RIGHT_BUTTON))
    g_hx += mv;
  if (arduboy.pressed(LEFT_BUTTON))
    g_hx -= mv;

  arduboy.clear();
  arduboy.setCursor(54, 0);
  sprites.drawSelfMasked(g_hx, g_hy, SPRITE8x8, 0);
  arduboy.print(g_lasttime);
  arduboy.print(" : ");
  arduboy.print(g_frametime);
  arduboy.print(" : ");
  arduboy.print(mv);
}

//ゲームクリア画面描画
void DrawGameClear()
{
}

//ゲームオーバー画面描画
void DrawGameOver()
{
}
