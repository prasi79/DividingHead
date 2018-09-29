#pragma once
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "ScreenHAL.h"
#include "UTFT_Buttons_Rus.h"
#include "Events.h"
#include "CoreArray.h"
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// главный экран
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define DIVIDE_DEGREE_BUTTON 0
#define DIVIDE_PARTS_BUTTON 1
#define STEP_BUTTON 2
#define ROTATION_BUTTON 3
#define SETTINGS_BUTTON 4
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define BUTTON_HEIGHT 40
#define BUTTON_X_OFFSET 10
#define BUTTON_Y_OFFSET 10
#define BUTTON_COLORS VGA_BLACK, VGA_GRAY,VGA_SILVER,VGA_GRAY,VGA_WHITE
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class MainScreen : public AbstractHALScreen
{
  public:

  static AbstractHALScreen* create()
  {
    return new MainScreen();
  }
  
   virtual void onActivate();
   virtual void onDeactivate();

   virtual void onEvent(Event event, void* param);


protected:
  
    virtual void doSetup(HalDC* hal);
    virtual void doUpdate(HalDC* hal);
    virtual void doDraw(HalDC* hal);

private:
    MainScreen();
    ~MainScreen();


    UTFT_Buttons_Rus* buttons;
    void drawGUI(HalDC* hal);

    Vector<uint8_t> buttonList;

    int lastActiveButton;

  
};
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
extern MainScreen* StartScreen;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class SplashScreen : public AbstractHALScreen
{
  public:

  static AbstractHALScreen* create()
  {
    return new SplashScreen();
  }
  
   virtual void onActivate();
   virtual void onDeactivate();


protected:
  
    virtual void doSetup(HalDC* hal);
    virtual void doUpdate(HalDC* hal);
    virtual void doDraw(HalDC* hal);

private:
    SplashScreen();

    uint32_t showTime;

  
};
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
extern SplashScreen* Splash;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class RotationScreen : public AbstractHALScreen
{
  public:

  static AbstractHALScreen* create()
  {
    return new RotationScreen();
  }
  
   virtual void onActivate();
   virtual void onDeactivate();

   virtual void onEvent(Event event, void* param);


protected:
  
    virtual void doSetup(HalDC* hal);
    virtual void doUpdate(HalDC* hal);
    virtual void doDraw(HalDC* hal);

private:
    RotationScreen();
    ~RotationScreen();

     bool speedSelected;
     uint8_t rotationSpeed;
     bool wantRedrawRotationSpeed, wantRedrawBackButton;
     int lastRotationSpeedLength;
     void drawGUI(HalDC* hal);
     
     bool isInWork, ccwButtonPressed, cwButtonPressed;
     void startRotate(bool ccw);
     void stopRotate(bool ccw);

     bool wantDrawStepperStatus;
     
     int drawRotationSpeed(HalDC* hal, int top);
  
};
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
extern RotationScreen* Rotation;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class StepsScreen : public AbstractHALScreen
{
  public:

  static AbstractHALScreen* create()
  {
    return new StepsScreen();
  }
  
   virtual void onActivate();
   virtual void onDeactivate();

   virtual void onEvent(Event event, void* param);


protected:
  
    virtual void doSetup(HalDC* hal);
    virtual void doUpdate(HalDC* hal);
    virtual void doDraw(HalDC* hal);

private:
    StepsScreen();
    ~StepsScreen();

     int8_t selectedMenu;
     uint8_t rotationSpeed;
     uint16_t steps;
     bool wantRedrawRotationSpeed, wantRedrawBackButton, wantRedrawSteps;
     int lastRotationSpeedLength, lastStepsLength;
     void drawGUI(HalDC* hal);
     
     bool isInWork;

     bool wantDrawStepperStatus;
     
     int drawRotationSpeed(HalDC* hal, int top);
     void drawSteps(HalDC* hal, int top);

     void startSteps(bool ccw);
     void stopSteps(bool ccw);
  
};
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
extern StepsScreen* Steps;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------

