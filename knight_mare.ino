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

enum GAMESTATE
{                 //--画面モードを表す列挙定数
  GAME_TITLE = 1, //タイトル、メイン、ステージ開始、中断、ゲームオーバー、ゲームクリア
  GAME_MAIN,
  GAME_START,
  GAME_PAUSE,
  GAME_OVER,
  GAME_CLEAR,
  GAME_TEST
};

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
  float X = 0, Y = 0;
  g_lasttime = millis();

  while ((arduboy.pressed(A_BUTTON) == false) || (arduboy.pressed(B_BUTTON) == false))
  {
    if (!(arduboy.nextFrame()))
      continue;

    long curtime = millis(); //現在の時刻を記録
    g_frametime = (float)(curtime - g_lasttime) / 1000.0f;
    g_lasttime = curtime;

    float mv = 80.0f * g_frametime; //移動量計算
    if (arduboy.pressed(DOWN_BUTTON))
      Y += mv;
    if (arduboy.pressed(UP_BUTTON))
      Y -= mv;
    if (arduboy.pressed(RIGHT_BUTTON))
      X += mv;
    if (arduboy.pressed(LEFT_BUTTON))
      X -= mv;

    arduboy.clear();
    arduboy.setCursor(54,0);
    sprites.drawSelfMasked(X, Y, SPRITE8x8, 0);
    arduboy.print(g_lasttime);
    arduboy.print(" : ");
    arduboy.print(g_frametime);
    arduboy.print(" : ");
    arduboy.print(mv);

    arduboy.display(); //--画面消去
  }

  // put your main code here, to run repeatedly:
}
