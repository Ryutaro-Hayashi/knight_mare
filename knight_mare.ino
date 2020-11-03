#ifndef _ARDUBOY2_H
#define _ARDUBOY2_H
#include <Arduboy2.h>
Arduboy2 arduboy;
Sprites sprites;
#endif

#ifndef _ARDUBOYPLAYTUNE_H
#define _ARDUBOYPLAYTUNE_H
#include <ArduboyPlaytune.h>
ArduboyPlaytune tunes(arduboy.audio.enabled);
#endif

/*
#ifndef _ARDUBOYTONES_H
#define _ARDUBOYTONES_H
#include <ArduboyTones.h>
ArduboyTones sound(arduboy.audio.enabled);
#endif
*/

#ifndef _TINYFONT_H
#define _TINYFONT_H
#include "Tinyfont.h"
Tinyfont tinyfont = Tinyfont(arduboy.sBuffer, Arduboy2::width(), Arduboy2::height());
#endif

#include "data.h"

#define SCRN_FPS 60
#define TLE_X 30 //--タイトル画面文字列のX
#define TLE_Y 15 //--タイトル画面文字列の1列目Y

enum GameState
{                 //--画面モードを表す列挙定数
  GAME_TITLE = 1, //タイトル、メイン、ステージ開始、中断、ゲームオーバー、ゲームクリア
  GAME_STAGECALL,
  GAME_MAIN,
  GAME_START,
  GAME_PAUSE,
  GAME_OVER,
  GAME_CLEAR,
  GAME_TEST
};

int Stage_Num = 1;

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
void DrawGameStageCall();
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

  tunes.initChannel(PIN_SPEAKER_1);
  tunes.initChannel(PIN_SPEAKER_2);
  arduboy.audio.on();
}

void loop()
{
  if (!(arduboy.nextFrame()))
    return;

  g_lasttime = millis();

  while ((arduboy.pressed(A_BUTTON) == false) || (arduboy.pressed(B_BUTTON) == false))
  {
    if (!(arduboy.nextFrame()))
      continue;
    arduboy.clear();

    long curtime = millis(); //現在の時刻を記録
    g_frametime = (float)(curtime - g_lasttime) / 1000.0f;
    g_lasttime = curtime;

    switch (g_gamestate)
    {
    case GAME_TITLE:
      DrawGameTitle();
      break;
    case GAME_STAGECALL:
      DrawGameStageCall();
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

  arduboy.drawBitmap(TLE_X, TLE_Y, op_title, 65, 18, 1);

  tinyfont.setCursor(TLE_X, TLE_Y + 24);
  tinyfont.print("A:GAME START");

  if (arduboy.pressed(B_BUTTON))
  {
    switch (g_soundstate)
    {
    case SOUND_OFF:
      g_soundstate = SOUND_ON;
      arduboy.audio.on();
      break;
    case SOUND_ON:
      g_soundstate = SOUND_OFF;
      arduboy.audio.off();
      //arduboy.audio.off;
      break;
    }
    delay(200);
  }

  tinyfont.setCursor(TLE_X, TLE_Y + 30);
  tinyfont.print("B:SOUND ");

  if (g_soundstate == SOUND_ON)
  {
    tinyfont.print("ON");
  }
  else
  {
    tinyfont.print("OFF");
  }

  if (arduboy.pressed(A_BUTTON)) //Aボタンを押す
  {
    if (!tunes.playing())
      tunes.playScore(OP_BGM);
    g_gamestate = GAME_STAGECALL;

    for (int i = 0; i < 3; i++)
    {

      arduboy.clear();
      arduboy.drawBitmap(TLE_X, TLE_Y, op_title, 65, 18, 1);
      tinyfont.setCursor(TLE_X, TLE_Y + 24);
      tinyfont.print("               ");
      arduboy.display();
      delay(330);
      arduboy.clear();
      arduboy.drawBitmap(TLE_X, TLE_Y, op_title, 65, 18, 1);
      tinyfont.setCursor(TLE_X, TLE_Y + 24);
      tinyfont.print("A:GAME START");
      arduboy.display();
      delay(330);
    }
  }

  // play the tune if we aren't already
}

//ステージ開始前の[STAGE X]表示
void DrawGameStageCall(){
arduboy.clear();
tinyfont.setCursor(45,35);
tinyfont.print("STAGE ");
tinyfont.print(Stage_Num);
arduboy.display();
delay(4000);
g_gamestate=GAME_MAIN;
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
