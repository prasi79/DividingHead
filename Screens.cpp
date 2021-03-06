//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "Screens.h"
#include "CONFIG.h"
#include "Settings.h"
#include "MotorController.h"
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void drawBackButton(HalDC* hal, bool active)
{
 int screenWidth = hal->getScreenWidth();
 int screenHeight = hal->getScreenHeight();
 int buttonWidth = screenWidth - BUTTON_X_OFFSET*2;

  int top = screenHeight - BUTTON_HEIGHT - BUTTON_Y_OFFSET;
  int left = BUTTON_X_OFFSET;

  
  hal->setColor(active ? BUTTON_ACTIVE_BACK_COLOR : BUTTON_INACTIVE_BACK_COLOR);
  hal->fillRoundRect(left,top,left + buttonWidth, top + BUTTON_HEIGHT);

  if(!active)
  {
    hal->setColor(VGA_SILVER);
    hal->drawRoundRect(left,top,left + buttonWidth, top + BUTTON_HEIGHT);
  }

  hal->setFont(SCREEN_BIG_FONT);
  hal->setColor(active ? BUTTON_ACTIVE_FONT_COLOR : BUTTON_INACTIVE_FONT_COLOR);
  hal->setBackColor(active ? BUTTON_ACTIVE_BACK_COLOR : BUTTON_INACTIVE_BACK_COLOR);
  
  int fontWidth = hal->getFontWidth(SCREEN_BIG_FONT);
  int fontHeight = hal->getFontHeight(SCREEN_BIG_FONT);

  String strToDraw;
  strToDraw = TXT_BACK_BUTTON;
  
  int len = hal->print(strToDraw.c_str(),0,0,0,true);
  hal->print(strToDraw.c_str(),(screenWidth - fontWidth*len)/2,top + (BUTTON_HEIGHT-fontHeight)/2);  
  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void drawStepperStatus(HalDC* hal,bool active)
{
  hal->setFont(Various_Symbols_32x32);
  int fontWidth = hal->getFontWidth(Various_Symbols_32x32);
  int screenWidth = hal->getScreenWidth();
  int offset = 5;
  
  if(active)
  {
    hal->setBackColor(SCREEN_BACK_COLOR);
    hal->setColor(STEPPER_STATUS_COLOR);
    hal->print("_",screenWidth-fontWidth-offset,offset);
  }
  else
  {
    hal->setColor(SCREEN_BACK_COLOR);
    int left = screenWidth-fontWidth-offset;
    hal->fillRect(left,offset,left+fontWidth,offset+fontWidth);
  }
    
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void setButtonActive(UTFT_Buttons_Rus* bb, int bID)
{
  bb->setButtonBackColor(bID, BUTTON_ACTIVE_BACK_COLOR);
  bb->setButtonFontColor(bID, BUTTON_ACTIVE_FONT_COLOR);  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void setButtonInactive(UTFT_Buttons_Rus* bb, int bID)
{
  bb->setButtonBackColor(bID, BUTTON_INACTIVE_BACK_COLOR);
  bb->setButtonFontColor(bID, BUTTON_INACTIVE_FONT_COLOR);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// MainScreen
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
MainScreen* StartScreen = NULL;        
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
MainScreen::MainScreen() : AbstractHALScreen()
{
  StartScreen = this;
  buttons = new UTFT_Buttons_Rus(Screen.getUTFT());
  buttons->setTextFont(SCREEN_BIG_FONT);
  buttons->setButtonColors(BUTTON_COLORS);

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
MainScreen::~MainScreen()
{
  delete buttons;  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MainScreen::onDeactivate()
{
  // станем неактивными
  Settings.setLastSelectedMenu(lastActiveButton);
  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MainScreen::onActivate()
{
  // мы активизируемся
  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MainScreen::onEvent(Event event, void* param)
{
  if(!isActive())
    return;  

    switch(event)
    {
      case StepsRequested: // запросили шагать на определённое кол-во шагов
      {
        
      }
      break; // StepsRequested

      #ifdef USE_KEYBOARD
      case KeyboardEvent:
      break;
      #endif
      
      case StepperWorkDone: // движок остановился
      {
        
      }
      break; // StepperWorkDone

      case RotationRequested: // запросили вращение
      {
        
      }
      break; // RotationRequested

      case ButtonStateChanged: // изменилось состояние кнопки
      {
        Screen.notifyAction(this); // говорим, что мы отработали чего-то, т.е. на экране происходит действия.
        ButtonEventParam* p = (ButtonEventParam*) param;
        
        switch(p->button)
        {
          case LEFT_BUTTON:
          {
           if(p->state & BUTTON_CLICKED)
            {
              int requested = lastActiveButton;
              requested--;
              if(requested < 0)
                requested = buttonList.size()-1;

                setButtonInactive(buttons,lastActiveButton);
      
                buttons->drawButton(lastActiveButton);
      
                lastActiveButton = requested;
                setButtonActive(buttons,lastActiveButton);
                  
                buttons->drawButton(lastActiveButton);
            }
          }
          break;

          case RIGHT_BUTTON:
          {
           if(p->state & BUTTON_CLICKED)
            {
              int requested = lastActiveButton;
              requested++;
              
              if(size_t(requested) >= buttonList.size())
                requested = 0;

                setButtonInactive(buttons,lastActiveButton);
      
                buttons->drawButton(lastActiveButton);
      
                lastActiveButton = requested;
                setButtonActive(buttons,lastActiveButton);
                  
                buttons->drawButton(lastActiveButton);
            }
          }
          break;
          
        } // switch
        
      }
      break; // ButtonStateChanged
    
      case EncoderPositionChanged: // смена позиции энкодера
      {
        int changes = *((int*) param);
        if(changes != 0)
        {
          int requested = lastActiveButton;
          requested += changes;
          if(requested < 0)
            requested = buttonList.size()-1;

          if(size_t(requested) >= buttonList.size())
            requested = 0;

          setButtonInactive(buttons,lastActiveButton);

          buttons->drawButton(lastActiveButton);

          lastActiveButton = requested;
          setButtonActive(buttons,lastActiveButton);
            
          buttons->drawButton(lastActiveButton);

          Screen.notifyAction(this); // говорим, что мы отработали чего-то, т.е. на экране происходит действия.
        }
      }
      break; // EncoderPositionChanged

      case EncoderButtonClicked: // кликнута кнопка энкодера
      {
        switch(lastActiveButton)
        {
          case DIVIDE_DEGREE_BUTTON:
          {
            DBGLN(F("Switch to divide by degrees screen!"));
            Screen.switchToScreen(DivideByDegrees);     
          }
          break;

          case DIVIDE_PARTS_BUTTON:
          {
            DBGLN(F("Switch to divide by parts screen!"));     
            Screen.switchToScreen(DivideByParts);
          }
          break;

          case STEP_BUTTON:
          {
            DBGLN(F("Switch to steps screen!"));
            Screen.switchToScreen(Steps);     
          }
          break;

          case ROTATION_BUTTON:
          {
            DBGLN(F("Switch to rotation screen!"));
            Screen.switchToScreen(Rotation);     
          }
          break;

          case ROTATION_CONTINUOUS_BUTTON:
          {
            DBGLN(F("Switch to rotate continuous screen!"));
            Screen.switchToScreen(RotationContinuous);                 
          }
          break;

          case SETTINGS_BUTTON:
          {
            DBGLN(F("Switch to settings screen!"));
            Screen.switchToScreen(Tune); 
          }
          break;

        } // switch

        Screen.notifyAction(this); // говорим, что мы отработали чего-то, т.е. на экране происходит действия.

      }
      break; // EncoderButtonClicked
      
    } // switch    
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MainScreen::doSetup(HalDC* hal)
{
  int screenWidth = hal->getScreenWidth();
  int screenHeight = hal->getScreenHeight();
  int buttonWidth = screenWidth - BUTTON_X_OFFSET*2;

  int top = BUTTON_Y_OFFSET;
  int btn = buttons->addButton(BUTTON_X_OFFSET,top,buttonWidth,BUTTON_HEIGHT,TXT_DEGREE_DIVIDE_BUTTON);

  buttonList.push_back(btn);

  top += BUTTON_HEIGHT + BUTTON_Y_OFFSET;
  btn = buttons->addButton(BUTTON_X_OFFSET,top,buttonWidth,BUTTON_HEIGHT,TXT_PART_DIVIDE_BUTTON);

  buttonList.push_back(btn);

  top += BUTTON_HEIGHT + BUTTON_Y_OFFSET;
  btn = buttons->addButton(BUTTON_X_OFFSET,top,buttonWidth,BUTTON_HEIGHT,TXT_STEP_BUTTON);

  buttonList.push_back(btn);

  top += BUTTON_HEIGHT + BUTTON_Y_OFFSET;
  btn = buttons->addButton(BUTTON_X_OFFSET,top,buttonWidth,BUTTON_HEIGHT,TXT_ROTATE_BUTTON);

  buttonList.push_back(btn);

  top += BUTTON_HEIGHT + BUTTON_Y_OFFSET;
  btn = buttons->addButton(BUTTON_X_OFFSET,top,buttonWidth,BUTTON_HEIGHT,TXT_ROTATE_CONTINUOUS_BUTTON);

  buttonList.push_back(btn);



  top = screenHeight - BUTTON_HEIGHT - BUTTON_Y_OFFSET;
  btn = buttons->addButton(BUTTON_X_OFFSET,top,buttonWidth,BUTTON_HEIGHT,TXT_SETTINGS_BUTTON);

  buttonList.push_back(btn);

  lastActiveButton = Settings.getLastSelectedMenu();
  setButtonActive(buttons,lastActiveButton);

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MainScreen::doUpdate(HalDC* hal)
{
  if(!isActive())
    return;

}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MainScreen::drawGUI(HalDC* hal)
{
  if(!isActive())
  return;
  
  buttons->drawButtons();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MainScreen::doDraw(HalDC* hal)
{
   hal->clearScreen();

   // тут отрисовка текущего состояния
   drawGUI(hal);

   hal->updateDisplay();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// TuneScreen
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
TuneScreen* Tune = NULL;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
TuneScreen::TuneScreen() : AbstractHALScreen()
{
  Tune = this;
  buttons = new UTFT_Buttons_Rus(Screen.getUTFT());
  buttons->setTextFont(SCREEN_BIG_FONT);
  buttons->setButtonColors(BUTTON_COLORS);
  lastActiveButton = 0;

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
TuneScreen::~TuneScreen()
{
  delete buttons;  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TuneScreen::onDeactivate()
{
  // станем неактивными
  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TuneScreen::onActivate()
{
  // мы активизируемся
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TuneScreen::onEvent(Event event, void* param)
{
  if(!isActive())
    return;  

    switch(event)
    {
      #ifdef USE_KEYBOARD
      case KeyboardEvent:
      break;
      #endif
            
      case StepsRequested: // запросили шагать на определённое кол-во шагов
      {
        
      }
      break; // StepsRequested
      
      case StepperWorkDone: // движок остановился
      {
        
      }
      break; // StepperWorkDone

      case RotationRequested: // запросили вращение
      {
        
      }
      break; // RotationRequested

      case ButtonStateChanged: // изменилось состояние кнопки
      {
        Screen.notifyAction(this); // говорим, что мы отработали чего-то, т.е. на экране происходит действия.
        ButtonEventParam* p = (ButtonEventParam*) param;
        
        switch(p->button)
        {
          case LEFT_BUTTON:
          {
           if(p->state & BUTTON_CLICKED)
            {
              int requested = lastActiveButton;
              requested--;
              if(requested < 0)
                requested = buttonList.size()-1;

                setButtonInactive(buttons,lastActiveButton);
      
                buttons->drawButton(lastActiveButton);
      
                lastActiveButton = requested;
                setButtonActive(buttons,lastActiveButton);
                  
                buttons->drawButton(lastActiveButton);
            }
          }
          break;

          case RIGHT_BUTTON:
          {
           if(p->state & BUTTON_CLICKED)
            {
              int requested = lastActiveButton;
              requested++;
              
              if(size_t(requested) >= buttonList.size())
                requested = 0;

                setButtonInactive(buttons,lastActiveButton);
      
                buttons->drawButton(lastActiveButton);
      
                lastActiveButton = requested;
                setButtonActive(buttons,lastActiveButton);
                  
                buttons->drawButton(lastActiveButton);
            }
          }
          break;
          
        } // switch
        
      }
      break; // ButtonStateChanged
    
      case EncoderPositionChanged: // смена позиции энкодера
      {
        int changes = *((int*) param);
        if(changes != 0)
        {
          int requested = lastActiveButton;
          requested += changes;
          if(requested < 0)
            requested = buttonList.size()-1;

          if(size_t(requested) >= buttonList.size())
            requested = 0;

          setButtonInactive(buttons,lastActiveButton);

          buttons->drawButton(lastActiveButton);

          lastActiveButton = requested;
          setButtonActive(buttons,lastActiveButton);
            
          buttons->drawButton(lastActiveButton);

          Screen.notifyAction(this); // говорим, что мы отработали чего-то, т.е. на экране происходит действия.
        }
      }
      break; // EncoderPositionChanged

      case EncoderButtonClicked: // кликнута кнопка энкодера
      {
        switch(lastActiveButton)
        {
          case MOTOR_SETUP_BUTTON: // настройки двигателя
          {
            DBGLN(F("Switch to motor setup screen!")); 
             Screen.switchToScreen(MotorSetup);    
          }
          break;

          case MICROSTEP_MODE_BUTTON: // настройка микрошагания
          {
            DBGLN(F("Switch to microstep screen!"));  
             Screen.switchToScreen(Microstep);        
          }
          break;

          case REDUCTION_BUTTON: // настройка редукции
          {
            DBGLN(F("Switch to reduction screen!"));
            Screen.switchToScreen(Reduction);     
          }
          break;

          case BACK_FROM_TUNE_BUTTON: // возврат на главный экран
          {
            DBGLN(F("Switch to main screen!"));
            Screen.switchToScreen(StartScreen);     
          }
          break;

        } // switch

        Screen.notifyAction(this); // говорим, что мы отработали чего-то, т.е. на экране происходит действия.

      }
      break; // EncoderButtonClicked
      
    } // switch    
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TuneScreen::doSetup(HalDC* hal)
{
  int screenWidth = hal->getScreenWidth();
  int screenHeight = hal->getScreenHeight();
  int buttonWidth = screenWidth - BUTTON_X_OFFSET*2;

  int top = BUTTON_Y_OFFSET;
  int btn = buttons->addButton(BUTTON_X_OFFSET,top,buttonWidth,BUTTON_HEIGHT,TXT_MOTOR_BUTTON); // шагов на оборот

  buttonList.push_back(btn);

  top += BUTTON_HEIGHT + BUTTON_Y_OFFSET;
  btn = buttons->addButton(BUTTON_X_OFFSET,top,buttonWidth,BUTTON_HEIGHT,TXT_MICROSTEP_DIVIDER_BUTTON); // делитель режима шага

  buttonList.push_back(btn);

  top += BUTTON_HEIGHT + BUTTON_Y_OFFSET;
  btn = buttons->addButton(BUTTON_X_OFFSET,top,buttonWidth,BUTTON_HEIGHT,TXT_REDUCTION_BUTTON); // настройки редукции

  buttonList.push_back(btn);




  top = screenHeight - BUTTON_HEIGHT - BUTTON_Y_OFFSET;
  btn = buttons->addButton(BUTTON_X_OFFSET,top,buttonWidth,BUTTON_HEIGHT,TXT_BACK_BUTTON);

  buttonList.push_back(btn);

  setButtonActive(buttons,0);

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TuneScreen::doUpdate(HalDC* hal)
{
  if(!isActive())
    return;

}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TuneScreen::drawGUI(HalDC* hal)
{
  if(!isActive())
  return;
  
  buttons->drawButtons();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TuneScreen::doDraw(HalDC* hal)
{
   hal->clearScreen();

   // тут отрисовка текущего состояния
   drawGUI(hal);

   hal->updateDisplay();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// SplashScreen
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
SplashScreen* Splash = NULL;        
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
SplashScreen::SplashScreen() : AbstractHALScreen()
{
  Splash = this;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SplashScreen::onDeactivate()
{
  // станем неактивными
  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SplashScreen::onActivate()
{
  // мы активизируемся
  showTime = 0;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SplashScreen::doSetup(HalDC* hal)
{

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SplashScreen::doUpdate(HalDC* hal)
{
  if(!isActive())
    return;

  if(showTime && (millis() - showTime > 3000))
  {
    DBGLN(F("Splash done, switch to main screen!"));
    Screen.switchToScreen(StartScreen);
  }
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SplashScreen::doDraw(HalDC* hal)
{
   hal->clearScreen();

   // тут отрисовка текущего состояния
   DBGLN(F("Show splash screen!"));

   extern unsigned short splash[];
   int splashWidth = 100;
   int splashHeight = 83;
   int screenWidth = hal->getScreenWidth();
   
   hal->drawBitmap((screenWidth - splashWidth)/2, 10, splashWidth, splashHeight, (unsigned int*)splash);

   hal->setFont(SCREEN_BIG_FONT);
   int fontWidth = hal->getFontWidth(SCREEN_BIG_FONT);
   int fontHeight = hal->getFontHeight(SCREEN_BIG_FONT);
   hal->setBackColor(SCREEN_BACK_COLOR);
   hal->setColor(SCREEN_TEXT_COLOR);

   int top = splashHeight + 20;
   
   String strToDraw = F("ДЕЛИТЕЛЬНАЯ ГОЛОВКА");
   int len = hal->print(strToDraw.c_str(),0,0,0,true);
   hal->print(strToDraw.c_str(),(screenWidth - fontWidth*len)/2,top);

    hal->setColor(SCREEN_TEXT_COLOR);

   top += fontHeight + 10;
   strToDraw = F("v. 0.1a");
   len = hal->print(strToDraw.c_str(),0,0,0,true);
   hal->print(strToDraw.c_str(),(screenWidth - fontWidth*len)/2,top);

   top += fontHeight*2 + 10;
   strToDraw = F("(c) Порохня Дмитрий");
   len = hal->print(strToDraw.c_str(),0,0,0,true);
   hal->print(strToDraw.c_str(),(screenWidth - fontWidth*len)/2,top);

   top += fontHeight + 10;
   strToDraw = F("2018-2019");
   len = hal->print(strToDraw.c_str(),0,0,0,true);
   hal->print(strToDraw.c_str(),(screenWidth - fontWidth*len)/2,top);

   hal->setColor(SCREEN_TEXT_COLOR);
   top += fontHeight*2 + 10;
   strToDraw = F("spywarrior@gmail.com");
   len = hal->print(strToDraw.c_str(),0,0,0,true);
   hal->print(strToDraw.c_str(),(screenWidth - fontWidth*len)/2,top);
   
   hal->updateDisplay();

   showTime = millis();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// RotationScreen
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
RotationScreen* Rotation = NULL;
RotationScreen* RotationContinuous = NULL;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
RotationScreen::RotationScreen(bool c) : AbstractHALScreen()
{
  if(!c)
    Rotation = this;
  else
    RotationContinuous = this;
    
  wantRedrawRotationSpeed = false;
  lastRotationSpeedLength = 0;
  isInWork = false;
  ccwButtonPressed = false;
  cwButtonPressed = false;
  wantDrawStepperStatus = false;
  continuous = c;

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
RotationScreen::~RotationScreen()
{
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void RotationScreen::validate()
{
  uint16_t before = rotationSpeed;
  if(rotationSpeed < 1)
    rotationSpeed = 1;
  if(rotationSpeed > 100)
    rotationSpeed = 100;

  if(before != rotationSpeed)
    wantRedrawRotationSpeed = true;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void RotationScreen::onDeactivate()
{
  // станем неактивными
  validate();
  Settings.setRotationSpeed(rotationSpeed);
  wantRedrawRotationSpeed = false;
  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void RotationScreen::onActivate()
{
  // мы активизируемся
  rotationSpeed = Settings.getRotationSpeed();
  bool w = !MotorController.isOnIdle();
  if(w != isInWork)
  {
    isInWork = w;
    wantDrawStepperStatus = true;
  }
  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void RotationScreen::startRotate(bool ccw)
{
  if(Settings.getRotationSpeed() != rotationSpeed)
  {
    validate();
    Settings.setRotationSpeed(rotationSpeed);
  }
    
  isInWork = true;

  RotationEventParam rr;
  rr.start = true;
  rr.ccw = ccw;
  rr.speed = rotationSpeed;
  
  #ifdef _DEBUG
  if(ccw)
  {
    DBGLN(F("START ROTATE CCW !"));
  }
  else
  {
    DBGLN(F("START ROTATE CW !"));
    
  }
  #endif

  Events.raise(this,RotationRequested, &rr);
  wantDrawStepperStatus = true;

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void RotationScreen::stopRotate(bool ccw)
{
  isInWork = false;    

  RotationEventParam rr;
  rr.start = false;
  rr.ccw = ccw;
  rr.speed = 0;  

  #ifdef _DEBUG
  if(ccw)
  {
    DBGLN(F("STOP ROTATE CCW !"));
  }
  else
  {
    DBGLN(F("STOP ROTATE CW !"));
    
  }
  #endif

  Events.raise(this,RotationRequested, &rr);
  wantDrawStepperStatus = true;

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void RotationScreen::onEvent(Event event, void* param)
{
  if(!isActive())
    return;  

    switch(event)
    {
      case StepsRequested: // запросили шагать на определённое кол-во шагов
      {
        
      }
      break; // StepsRequested
      
      case StepperWorkDone: // движок остановился
      {
        
      }
      break; // StepperWorkDone

      case RotationRequested: // запросили вращение
      {
        
      }
      break; // RotationRequested

      #ifdef USE_KEYBOARD
      case KeyboardEvent:
      {
          int8_t keyCode = *((int8_t*) param);
          if(keyCode == DELETE_KEY)
            rotationSpeed = 0;
          else
          if(keyCode == BACKSPACE_KEY)
          {
              rotationSpeed /= 10;
          }
          else
          {
            rotationSpeed *= 10;
            rotationSpeed += keyCode;
            validate();
          }
          wantRedrawRotationSpeed = true;
          Screen.notifyAction(this); // говорим, что мы отработали чего-то, т.е. на экране происходит действия.
      }
      break; // KeyboardEvent
      #endif
            
      case EncoderPositionChanged: // смена позиции энкодера
      {
        int changes = *((int*) param);
        if(changes != 0)
        {
            Screen.notifyAction(this); // говорим, что мы отработали чего-то, т.е. на экране происходит действия.
            rotationSpeed += changes;
            validate();                              
            wantRedrawRotationSpeed = true;
        }
      }
      break; // EncoderPositionChanged

      case EncoderButtonClicked: // кликнута кнопка энкодера
      {
        // переключаемся на стартовый экран
        DBGLN(F("Back to main screen!"));
        Screen.switchToScreen(StartScreen);              
      }
      break; // EncoderButtonClicked


      case ButtonStateChanged: // состояние какой-либо кнопки изменилось
      {
        Screen.notifyAction(this); // говорим, что мы отработали чего-то, т.е. на экране происходит действия.
        ButtonEventParam* p = (ButtonEventParam*) param;

        if(!continuous)
        {        
                switch(p->button)
                {
                  case LEFT_BUTTON:
                  {
                    if(!cwButtonPressed)
                    {
                        bool isPressedNow = p->state & BUTTON_PRESSED;
                        if(isPressedNow)
                        {
                         ccwButtonPressed = true;
                         startRotate(true);
                        }
                        else
                        {              
                          if(p->state & BUTTON_CLICKED)
                          {
                           ccwButtonPressed = false;
                           stopRotate(true);
                          }
                        }
                    }
                  }
                  break;
        
                  case RIGHT_BUTTON:
                  {
                    if(!ccwButtonPressed)
                    {
                        bool isPressedNow = p->state & BUTTON_PRESSED;
                        if(isPressedNow)
                        {
                         cwButtonPressed = true;
                         startRotate(false);
                        }
                        else
                        {              
                          if(p->state & BUTTON_CLICKED)
                          {
                           cwButtonPressed = false;
                           stopRotate(false);
                          }
                        }
                    }
                  }
                  break;
                  
                } // switch
        } // if
        else
        {
              if(p->state & BUTTON_CLICKED)
              {
                // кнопка кликнута
                if(isInWork)
                {
                  // работаем ещё, надо остановить мотор
                  DBGLN(F("Stop motor prematurely!!!"));
                  stopRotate(p->button == LEFT_BUTTON);
                }
                else              
                {
                  // режим простоя, запускаем шагание
                  startRotate(p->button == LEFT_BUTTON);
                }
                
              } // if(p->state & BUTTON_CLICKED)   
        
        } // else
      }
      break; // ButtonStateChanged
      
    } // switch    
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void RotationScreen::doSetup(HalDC* hal)
{
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void RotationScreen::doUpdate(HalDC* hal)
{
  if(!isActive())
    return;

    if(isInWork)
       Screen.notifyAction(this); // говорим, что мы отработали чего-то, т.е. на экране происходит действия.

    if(wantRedrawRotationSpeed)
    {
      wantRedrawRotationSpeed = false;
      drawRotationSpeed(hal,80);
    } // if

    if(wantDrawStepperStatus)
    {
      wantDrawStepperStatus = false;
      drawStepperStatus(hal,isInWork);
    }

}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void RotationScreen::doDraw(HalDC* hal)
{
   hal->clearScreen();

   // тут отрисовка текущего состояния
   drawGUI(hal);

   hal->updateDisplay();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void RotationScreen::drawGUI(HalDC* hal)
{
  int screenWidth = hal->getScreenWidth();
   
   hal->setFont(SCREEN_BIG_FONT);
   int fontWidth = hal->getFontWidth(SCREEN_BIG_FONT);
   int fontHeight = hal->getFontHeight(SCREEN_BIG_FONT);
   hal->setBackColor(SCREEN_BACK_COLOR);
   hal->setColor(SCREEN_TEXT_COLOR);  

  int top = 20;
  int vSpacing = 10;
   
   String strToDraw;
   if(!continuous)
    strToDraw = TXT_ROTATION_SCREEN_CAPTION;
   else
    strToDraw = TXT_ROTATION_CONTINUOUS_SCREEN_CAPTION;
   
   int len = hal->print(strToDraw.c_str(),0,0,0,true);
   int left = (screenWidth - fontWidth*len)/2;
   hal->print(strToDraw.c_str(),left,top);

   top =  drawRotationSpeed(hal,80);
   top += 20;

   hal->setFont(SCREEN_BIG_FONT);
   fontWidth = hal->getFontWidth(SCREEN_BIG_FONT);
   fontHeight = hal->getFontHeight(SCREEN_BIG_FONT);
   hal->setBackColor(SCREEN_BACK_COLOR);
   hal->setColor(SCREEN_TEXT_COLOR);  


  #ifdef DISPLAY_SCREEN_HINTS
  
     if(!continuous)
      strToDraw = TXT_ROTATION_SCREEN_HINT_1;
     else
      strToDraw = TXT_ROTATION_CONTINUOUS_SCREEN_HINT_1;
      
     len = hal->print(strToDraw.c_str(),0,0,0,true);
     left = (screenWidth - fontWidth*len)/2;
     hal->print(strToDraw.c_str(),left,top);
     top += fontHeight + vSpacing;
  
     if(!continuous)
      strToDraw = TXT_ROTATION_SCREEN_HINT_2;
     else
      strToDraw = TXT_ROTATION_CONTINUOUS_SCREEN_HINT_2;
      
     len = hal->print(strToDraw.c_str(),0,0,0,true);
     left = (screenWidth - fontWidth*len)/2;
     hal->print(strToDraw.c_str(),left,top);
     
   #endif // #ifdef DISPLAY_SCREEN_HINTS


   drawBackButton(hal,true);
   drawStepperStatus(hal,isInWork);
   
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int RotationScreen::drawRotationSpeed(HalDC* hal, int top)
{
   int screenWidth = hal->getScreenWidth();
   hal->setFont(SevenSeg_XXXL_Num);
   int fontWidth = hal->getFontWidth(SevenSeg_XXXL_Num);
   int fontHeight = hal->getFontHeight(SevenSeg_XXXL_Num);

    
   String strToDraw;
   strToDraw = rotationSpeed;
   int len = hal->print(strToDraw.c_str(),0,0,0,true);
   int left = (screenWidth - fontWidth*len)/2;

   if(lastRotationSpeedLength && lastRotationSpeedLength != len)
   {
    hal->setColor(SCREEN_BACK_COLOR);
    hal->fillRect(left - fontWidth, top, left + fontWidth*4,top+fontHeight);
   }

   lastRotationSpeedLength = len;
   
   hal->setColor(DIGITS_FONT_COLOR);
   hal->setBackColor(SCREEN_BACK_COLOR);
   hal->print(strToDraw.c_str(),left,top);   

   return (top + fontHeight + 10);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// MotorSetupScreen
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
MotorSetupScreen* MotorSetup = NULL;        
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
MotorSetupScreen::MotorSetupScreen() : AbstractHALScreen()
{
  MotorSetup = this;
  wantRedrawStepsPerRevolution = false;
  lastStepsPerRevolutionLength = 0;

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
MotorSetupScreen::~MotorSetupScreen()
{
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MotorSetupScreen::validate()
{
  uint16_t before = stepsPerRevolution;

  if(stepsPerRevolution < 1)
    stepsPerRevolution = 1;
    
  if(stepsPerRevolution > 1000)
    stepsPerRevolution = 1000;
                
  if(before != stepsPerRevolution)
    wantRedrawStepsPerRevolution = true;  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MotorSetupScreen::onDeactivate()
{
  // станем неактивными
  validate();
  Settings.setStepsPerRevolution(stepsPerRevolution);
  wantRedrawStepsPerRevolution = false;
  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MotorSetupScreen::onActivate()
{
  // мы активизируемся
  stepsPerRevolution = Settings.getStepsPerRevolution();  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MotorSetupScreen::onEvent(Event event, void* param)
{
  if(!isActive())
    return;  

    switch(event)
    {
      case StepsRequested: // запросили шагать на определённое кол-во шагов
      {
        
      }
      break; // StepsRequested
      
      case StepperWorkDone: // движок остановился
      {
        
      }
      break; // StepperWorkDone

      case RotationRequested: // запросили вращение
      {
        
      }
      break; // RotationRequested

      #ifdef USE_KEYBOARD
      case KeyboardEvent:
      {
          int8_t keyCode = *((int8_t*) param);
          if(keyCode == DELETE_KEY)
            stepsPerRevolution = 0;
          else
          if(keyCode == BACKSPACE_KEY)
          {
              stepsPerRevolution /= 10;
          }
          else
          {
            stepsPerRevolution *= 10;
            stepsPerRevolution += keyCode;
            validate();
          }
          wantRedrawStepsPerRevolution = true;
          Screen.notifyAction(this); // говорим, что мы отработали чего-то, т.е. на экране происходит действия.
      }
      break; // KeyboardEvent
      #endif     
            
      case EncoderPositionChanged: // смена позиции энкодера
      {
        int changes = *((int*) param);
        if(changes != 0)
        {
              Screen.notifyAction(this); // говорим, что мы отработали чего-то, т.е. на экране происходит действия.
              stepsPerRevolution += changes;
              
              validate();
                
              wantRedrawStepsPerRevolution = true;
        }
      }
      break; // EncoderPositionChanged

      case EncoderButtonClicked: // кликнута кнопка энкодера
      {
        Screen.notifyAction(this); // говорим, что мы отработали чего-то, т.е. на экране происходит действия.

        // переключаемся на экран настроек
        DBGLN(F("Back to settings screen!"));
        Screen.switchToScreen(Tune);              
        
      }
      break; // EncoderButtonClicked


      case ButtonStateChanged: // состояние какой-либо кнопки изменилось
      {        
      }
      break; // ButtonStateChanged
      
    } // switch    
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MotorSetupScreen::doSetup(HalDC* hal)
{
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MotorSetupScreen::doUpdate(HalDC* hal)
{
  if(!isActive())
    return;

    if(wantRedrawStepsPerRevolution)
    {
      wantRedrawStepsPerRevolution = false;
      drawStepsPerRevolution(hal,80);
    } // if

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MotorSetupScreen::doDraw(HalDC* hal)
{
   hal->clearScreen();

   // тут отрисовка текущего состояния
   drawGUI(hal);

   hal->updateDisplay();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MotorSetupScreen::drawGUI(HalDC* hal)
{
  int screenWidth = hal->getScreenWidth();
   
   hal->setFont(SCREEN_BIG_FONT);
   int fontWidth = hal->getFontWidth(SCREEN_BIG_FONT);
   hal->setBackColor(SCREEN_BACK_COLOR);
   hal->setColor(SCREEN_TEXT_COLOR);  

  int top = 20;
   
   String strToDraw = TXT_MOTOR_SETUP_SCREEN_CAPTION;
   int len = hal->print(strToDraw.c_str(),0,0,0,true);
   int left = (screenWidth - fontWidth*len)/2;
   hal->print(strToDraw.c_str(),left,top);

   top =  drawStepsPerRevolution(hal,80);

   drawBackButton(hal,true);
   
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int MotorSetupScreen::drawStepsPerRevolution(HalDC* hal, int top)
{
   int screenWidth = hal->getScreenWidth();
   hal->setFont(SevenSeg_XXXL_Num);
   int fontWidth = hal->getFontWidth(SevenSeg_XXXL_Num);
   int fontHeight = hal->getFontHeight(SevenSeg_XXXL_Num);

    
   String strToDraw;
   strToDraw = stepsPerRevolution;
   int len = hal->print(strToDraw.c_str(),0,0,0,true);
   int left = (screenWidth - fontWidth*len)/2;

   if(lastStepsPerRevolutionLength && lastStepsPerRevolutionLength != len)
   {
    if(len < lastStepsPerRevolutionLength)
    {
      // разрядность уменьшилась, надо пересчитать, какие прямоугольники заливать, поскольку мы рисуем по центру
      int diff = lastStepsPerRevolutionLength - len;
      int freeWidth = diff*fontWidth;
      
      hal->setColor(SCREEN_BACK_COLOR);
      hal->fillRect(left - freeWidth/2, top, left + freeWidth/2,top+fontHeight);
      hal->fillRect(left + len*fontWidth, top, left + len*fontWidth + freeWidth/2,top+fontHeight);
    }
   }

   lastStepsPerRevolutionLength = len;
   
   hal->setColor(DIGITS_FONT_COLOR);
   hal->setBackColor(SCREEN_BACK_COLOR);
   hal->print(strToDraw.c_str(),left,top);   

   return (top + fontHeight + 10);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// MicrostepScreen
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
MicrostepScreen* Microstep = NULL;        
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
MicrostepScreen::MicrostepScreen() : AbstractHALScreen()
{
  Microstep = this;
  wantRedrawSetting = false;
  lastSettingLength = 0;

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
MicrostepScreen::~MicrostepScreen()
{
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MicrostepScreen::validate()
{
  uint16_t before = setting;

  if(!(setting == 1 || setting == 2 || setting == 4 || setting == 8 || setting == 16 || setting == 32 || setting == 64))
    setting = 1;
    
  if(before != setting)
    wantRedrawSetting = true;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MicrostepScreen::onDeactivate()
{
  // станем неактивными
  validate();
  Settings.setDivider(setting);
  wantRedrawSetting = false;
  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MicrostepScreen::onActivate()
{
  // мы активизируемся
  setting = Settings.getDivider();  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MicrostepScreen::onEvent(Event event, void* param)
{
  if(!isActive())
    return;  

    switch(event)
    {
      case StepsRequested: // запросили шагать на определённое кол-во шагов
      {
        
      }
      break; // StepsRequested
      
      case StepperWorkDone: // движок остановился
      {
        
      }
      break; // StepperWorkDone

      case RotationRequested: // запросили вращение
      {
        
      }
      break; // RotationRequested
      
      #ifdef USE_KEYBOARD
      case KeyboardEvent:
      break; // KeyboardEvent      
      #endif
            
      case EncoderPositionChanged: // смена позиции энкодера
      {
        int changes = *((int*) param);
        if(changes != 0)
        {
              Screen.notifyAction(this); // говорим, что мы отработали чего-то, т.е. на экране происходит действия.

               if(changes > 0)
               {
                  if(setting == 1)
                    setting = 2;
                  else if(setting == 2)
                    setting = 4;
                  else if(setting == 4)
                    setting = 8;
                  else if(setting == 8)
                    setting = 16;
                  else if(setting == 16)
                    setting = 32;
                  else if(setting == 32)
                    setting = 64;
                  else
                    setting = 1;
               }
               else
               {
                  if(setting == 1)
                    setting = 64;
                  else if(setting == 2)
                    setting = 1;
                  else if(setting == 4)
                    setting = 2;
                  else if(setting == 8)
                    setting = 4;
                  else if(setting == 16)
                    setting = 8;
                  else if(setting == 32)
                    setting = 16;
                  else if(setting == 64)
                    setting = 32;
                 else
                    setting = 64;
               }
                
              wantRedrawSetting = true;
        }
      }
      break; // EncoderPositionChanged

      case EncoderButtonClicked: // кликнута кнопка энкодера
      {
        Screen.notifyAction(this); // говорим, что мы отработали чего-то, т.е. на экране происходит действия.

        // переключаемся на экран настроек
        DBGLN(F("Back to settings screen!"));
        Screen.switchToScreen(Tune);              
        
      }
      break; // EncoderButtonClicked


      case ButtonStateChanged: // состояние какой-либо кнопки изменилось
      {        
      }
      break; // ButtonStateChanged
      
    } // switch    
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MicrostepScreen::doSetup(HalDC* hal)
{
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MicrostepScreen::doUpdate(HalDC* hal)
{
  if(!isActive())
    return;

    if(wantRedrawSetting)
    {
      wantRedrawSetting = false;
      drawSetting(hal,80);
    } // if

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MicrostepScreen::doDraw(HalDC* hal)
{
   hal->clearScreen();

   // тут отрисовка текущего состояния
   drawGUI(hal);

   hal->updateDisplay();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MicrostepScreen::drawGUI(HalDC* hal)
{
  int screenWidth = hal->getScreenWidth();
   
   hal->setFont(SCREEN_BIG_FONT);
   int fontWidth = hal->getFontWidth(SCREEN_BIG_FONT);

   hal->setBackColor(SCREEN_BACK_COLOR);
   hal->setColor(SCREEN_TEXT_COLOR);  

  int top = 20;
   
   String strToDraw = TXT_MICROSTEP_SCREEN_CAPTION;
   int len = hal->print(strToDraw.c_str(),0,0,0,true);
   int left = (screenWidth - fontWidth*len)/2;
   hal->print(strToDraw.c_str(),left,top);

   top =  drawSetting(hal,80);

   drawBackButton(hal,true);
   
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int MicrostepScreen::drawSetting(HalDC* hal, int top)
{
   int screenWidth = hal->getScreenWidth();
   hal->setFont(SevenSeg_XXXL_Num);
   int fontWidth = hal->getFontWidth(SevenSeg_XXXL_Num);
   int fontHeight = hal->getFontHeight(SevenSeg_XXXL_Num);


  // у нас позиций - максимум 4, т.е. 1/64
  int widthOccupied = fontWidth*4;

  // вычисляем левую границу
  int left = (screenWidth - widthOccupied)/2;

  // рисуем первую единицу
  String strToDraw;
  strToDraw = '1';

  hal->setColor(DIGITS_FONT_COLOR);
  hal->setBackColor(SCREEN_BACK_COLOR);
  hal->print(strToDraw.c_str(),left,top);
  left += fontWidth;

  // рисуем двоеточие
  hal->setFont(AdditionalLettersFont);
  strToDraw = '0'; // рисуем слеш '/'
  hal->print(strToDraw.c_str(),left,top);
  left += fontWidth;
  hal->setFont(SevenSeg_XXXL_Num);

  // рисуем значение делителя
   strToDraw = setting;
   int len = hal->print(strToDraw.c_str(),0,0,0,true);
   if(lastSettingLength && lastSettingLength != len)
   {
    if(len < lastSettingLength)
    {
      // разрядность уменьшилась, надо пересчитать, какие прямоугольники заливать, поскольку мы рисуем по центру
      int diff = lastSettingLength - len;
      int freeWidth = diff*fontWidth;
      
      hal->setColor(SCREEN_BACK_COLOR);
      hal->fillRect(left + len*fontWidth, top, left + len*fontWidth + freeWidth,top+fontHeight);
    }
   }

   lastSettingLength = len;   
   hal->setColor(DIGITS_FONT_COLOR);
   hal->setBackColor(SCREEN_BACK_COLOR);
   hal->print(strToDraw.c_str(),left,top);   
  
   return (top + fontHeight + 10);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// StepsScreen
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
StepsScreen* Steps = NULL;        
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
StepsScreen::StepsScreen() : AbstractHALScreen()
{
  Steps = this;
  wantRedrawRotationSpeed = false;
  lastRotationSpeedLength = 0;
  lastStepsLength = 0;
  isInWork = false;
  wantDrawStepperStatus = false;
  wantRedrawSteps = false;
  steps = 200;

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
StepsScreen::~StepsScreen()
{
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void StepsScreen::validate()
{
  uint16_t speedBefore = rotationSpeed;

  if(rotationSpeed < 1)
    rotationSpeed = 1;
  if(rotationSpeed > 100)
    rotationSpeed = 100;
  
  if(speedBefore != rotationSpeed)
    wantRedrawRotationSpeed = true;


  uint16_t stepsBefore = steps;
  
  if(steps < 1)
      steps = 1;
      
  if(steps > 999)
    steps = 999;

  if(stepsBefore != steps)
    wantRedrawSteps = true;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void StepsScreen::onDeactivate()
{
  // станем неактивными
  validate();
  Settings.setRotationSpeed(rotationSpeed);
  Settings.setSteps(steps);
  wantRedrawRotationSpeed = false;
  wantRedrawBackButton = false;
  wantRedrawSteps = false;
  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void StepsScreen::onActivate()
{
  // мы активизируемся
  rotationSpeed = Settings.getRotationSpeed();
  steps = Settings.getSteps();
  selectedMenu = 0; // выбираем кол-во шагов
  
  bool w = !MotorController.isOnIdle();
  if(w != isInWork)
  {
    isInWork = w;
    wantDrawStepperStatus = true;
  }
  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void StepsScreen::onEvent(Event event, void* param)
{
  if(!isActive())
    return;  

    switch(event)
    {
      case StepsRequested: // запросили шагать на определённое кол-во шагов
      {
        
      }
      break; // StepsRequested
      
      case StepperWorkDone: // движок остановился
      {
        #ifdef _DEBUG
          int32_t thisSteps = steps;
          thisSteps -= MotorController.getRemainingSteps();
          DBG(thisSteps);
          DBGLN(F(" STEPS DONE!"));
        #endif
        
        if(isInWork)
        {
          isInWork = false;
          wantDrawStepperStatus = true;
        }
      }
      break; // StepperWorkDone

      case RotationRequested: // запросили вращение
      {
        
      }
      break; // RotationRequested

      #ifdef USE_KEYBOARD
      case KeyboardEvent:
      {
        if(selectedMenu == 0 || selectedMenu == 1)
        {
          int8_t keyCode = *((int8_t*) param);
          
          if(selectedMenu == 0) // шаги
          {
            if(keyCode == DELETE_KEY)
              steps = 0;
            else
            if(keyCode == BACKSPACE_KEY)
            {
                steps /= 10;
            }
            else
            {
              steps *= 10;
              steps += keyCode;
              validate();
            }
            wantRedrawSteps = true;
          }
          else // скорость
          {
            if(keyCode == DELETE_KEY)
              rotationSpeed = 0;
            else
            if(keyCode == BACKSPACE_KEY)
            {
                rotationSpeed /= 10;
            }
            else
            {
              rotationSpeed *= 10;
              rotationSpeed += keyCode;
              validate();
            }
            wantRedrawRotationSpeed = true;            
          }
          Screen.notifyAction(this); // говорим, что мы отработали чего-то, т.е. на экране происходит действия.
          
        } // selectedMenu == 0 || selectedMenu == 1
      }
      break; // KeyboardEvent      
      #endif
            
      case EncoderPositionChanged: // смена позиции энкодера
      {
        int changes = *((int*) param);
        if(changes != 0)
        {
            Screen.notifyAction(this); // говорим, что мы отработали чего-то, т.е. на экране происходит действия.
            switch(selectedMenu)
            {
              case 0: // выбрано кол-во шагов
              {
                steps += changes;
                validate();
                wantRedrawSteps = true;
              }
              break;

              case 1: // выбрана скорость
              {
                 rotationSpeed += changes;
                 validate();                  
                 wantRedrawRotationSpeed = true;
              }
              break;

              case 2: // выбрана кнопка "назад"
              {
                // переключаемся на стартовый экран
                DBGLN(F("Back to main screen!"));
                
                if(isInWork) // перед переключением на другой экран принудительно останавливаем мотор!!!
                  stopSteps(true);
                  
                Screen.switchToScreen(StartScreen);    
              }
              break;
              
            } // switch
            
        }
      }
      break; // EncoderPositionChanged

      case EncoderButtonClicked: // кликнута кнопка энкодера
      {

        int8_t lastSelMenu = selectedMenu;
        selectedMenu++;
        if(selectedMenu > 2)
          selectedMenu = 0;

          switch(lastSelMenu)
          {
            case 0: wantRedrawSteps = true; break;
            case 1: wantRedrawRotationSpeed = true; break;
            case 2: wantRedrawBackButton = true; break;
          }

          switch(selectedMenu)
          {
            case 0: wantRedrawSteps = true; break;
            case 1: wantRedrawRotationSpeed = true; break;
            case 2: wantRedrawBackButton = true; break;
          }

        validate();
        
        Screen.notifyAction(this); // говорим, что мы отработали чего-то, т.е. на экране происходит действия.
      }
      break; // EncoderButtonClicked


      case ButtonStateChanged: // состояние какой-либо кнопки изменилось
      {
        Screen.notifyAction(this); // говорим, что мы отработали чего-то, т.е. на экране происходит действия.
        ButtonEventParam* p = (ButtonEventParam*) param;

        if(p->state & BUTTON_CLICKED)
        {
          // кнопка кликнута
          if(isInWork)
          {
            // работаем ещё, надо остановить мотор
            DBGLN(F("Stop motor prematurely!!!"));
            stopSteps(p->button == LEFT_BUTTON);
          }
          else              
          {
            // режим простоя, запускаем шагание
            startSteps(p->button == LEFT_BUTTON);
          }
          
        } // if(p->state & BUTTON_CLICKED)   
        
        
      }
      break; // ButtonStateChanged
      
    } // switch    
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void StepsScreen::doSetup(HalDC* hal)
{
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void StepsScreen::doUpdate(HalDC* hal)
{
  if(!isActive())
    return;

    if(isInWork)
       Screen.notifyAction(this); // говорим, что мы отработали чего-то, т.е. на экране происходит действия.

    if(wantRedrawRotationSpeed)
    {
      wantRedrawRotationSpeed = false;
      drawRotationSpeed(hal,80);
    } // if

    if(wantRedrawSteps)
    {
      wantRedrawSteps = false;
      drawSteps(hal,80);
    }

    if(wantRedrawBackButton)
    {
      wantRedrawBackButton = false;
      drawBackButton(hal,selectedMenu == 2);
    }

    if(wantDrawStepperStatus)
    {
      wantDrawStepperStatus = false;
      drawStepperStatus(hal,isInWork);
    }

}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void StepsScreen::doDraw(HalDC* hal)
{
   hal->clearScreen();

   // тут отрисовка текущего состояния
   drawGUI(hal);

   hal->updateDisplay();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void StepsScreen::drawGUI(HalDC* hal)
{
  int screenWidth = hal->getScreenWidth();
   
   hal->setFont(SCREEN_BIG_FONT);
   int fontWidth = hal->getFontWidth(SCREEN_BIG_FONT);
   int fontHeight = hal->getFontHeight(SCREEN_BIG_FONT);
   hal->setBackColor(SCREEN_BACK_COLOR);
   hal->setColor(SCREEN_TEXT_COLOR);  

  int top = 20;
  int vSpacing = 10;
   
   String strToDraw = TXT_STEPS_SCREEN_CAPTION;
   int len = hal->print(strToDraw.c_str(),0,0,0,true);
   int left = (screenWidth - fontWidth*len)/2;
   hal->print(strToDraw.c_str(),left,top);


   hal->setColor(DIGITS_LABEL_FONT_COLOR);
   strToDraw = TXT_STEPS_SCREEN_STEPS_LABEL;
   hal->print(strToDraw.c_str(),40,80-fontHeight-6);
   strToDraw = TXT_STEPS_SCREEN_SPEED_LABEL;
   len = hal->print(strToDraw.c_str(),0,0,0,true);
   hal->print(strToDraw.c_str(),screenWidth - 40 - len*fontWidth,80-fontHeight-6);

   drawSteps(hal,80);
   top =  drawRotationSpeed(hal,80);
   top += 20;

   hal->setFont(SCREEN_BIG_FONT);
   fontWidth = hal->getFontWidth(SCREEN_BIG_FONT);
   fontHeight = hal->getFontHeight(SCREEN_BIG_FONT);
   hal->setBackColor(SCREEN_BACK_COLOR);
   hal->setColor(SCREEN_TEXT_COLOR);  

   #ifdef DISPLAY_SCREEN_HINTS
   
     strToDraw = TXT_STEPS_SCREEN_HINT_1;
     len = hal->print(strToDraw.c_str(),0,0,0,true);
     left = (screenWidth - fontWidth*len)/2;
     hal->print(strToDraw.c_str(),left,top);
     top += fontHeight + vSpacing;
  
     strToDraw = TXT_STEPS_SCREEN_HINT_2;
     len = hal->print(strToDraw.c_str(),0,0,0,true);
     left = (screenWidth - fontWidth*len)/2;
     hal->print(strToDraw.c_str(),left,top);
     
   #endif // #ifdef DISPLAY_SCREEN_HINTS


   drawBackButton(hal,selectedMenu == 2);
   drawStepperStatus(hal,isInWork);
   
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int StepsScreen::drawRotationSpeed(HalDC* hal, int top)
{
   int screenWidth = hal->getScreenWidth();
   hal->setFont(SevenSeg_XXXL_Num);
   int fontWidth = hal->getFontWidth(SevenSeg_XXXL_Num);
   int fontHeight = hal->getFontHeight(SevenSeg_XXXL_Num);

    
   String strToDraw;
   strToDraw = rotationSpeed;
   int len = hal->print(strToDraw.c_str(),0,0,0,true);
   int left = (screenWidth - fontWidth*len) - 40;

   if(lastRotationSpeedLength && lastRotationSpeedLength != len)
   {
      if(len < lastRotationSpeedLength) // уменьшили разрядность числа
      {
        hal->setColor(SCREEN_BACK_COLOR);
        int offset = left-fontWidth*(lastRotationSpeedLength-len);
        hal->fillRect(offset, top, offset + fontWidth*(lastRotationSpeedLength-len),top+fontHeight);
      }
   }

   lastRotationSpeedLength = len;
   
   hal->setColor(selectedMenu == 1 ? DIGITS_SELECTED_FONT_COLOR : DIGITS_FONT_COLOR);
   hal->setBackColor(selectedMenu == 1 ? DIGITS_SELECTED_BACK_COLOR : SCREEN_BACK_COLOR);
   hal->print(strToDraw.c_str(),left,top);   

   return (top + fontHeight + 10);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void StepsScreen::drawSteps(HalDC* hal, int top)
{
   hal->setFont(SevenSeg_XXXL_Num);
   int fontWidth = hal->getFontWidth(SevenSeg_XXXL_Num);
   int fontHeight = hal->getFontHeight(SevenSeg_XXXL_Num);

    
   String strToDraw;
   strToDraw = steps;
   int len = hal->print(strToDraw.c_str(),0,0,0,true);
   int left = 40;

   if(lastStepsLength && lastStepsLength != len)
   {    
      if(len < lastStepsLength) // уменьшили разрядность числа
      {
        hal->setColor(SCREEN_BACK_COLOR);
        int offset = left+fontWidth*len;
        hal->fillRect(offset, top, offset + fontWidth*(lastStepsLength-len),top+fontHeight);
      }
   }

   lastStepsLength = len;
   
   hal->setColor(selectedMenu == 0 ? DIGITS_SELECTED_FONT_COLOR : DIGITS_FONT_COLOR);
   hal->setBackColor(selectedMenu == 0 ? DIGITS_SELECTED_BACK_COLOR : SCREEN_BACK_COLOR);
   hal->print(strToDraw.c_str(),left,top);   

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void StepsScreen::startSteps(bool ccw)
{
  if(Settings.getRotationSpeed() != rotationSpeed)
  {
    validate();
    Settings.setRotationSpeed(rotationSpeed);
  }

  if(Settings.getSteps() != steps)
  {
    validate();
    Settings.setSteps(steps);
  }
    
  isInWork = true;

  StepsEventParam rr;
  rr.start = true;
  rr.ccw = ccw;
  rr.speed = rotationSpeed;
  rr.steps = steps;
  
  #ifdef _DEBUG
  if(ccw)
  {
    DBGLN(F("START STEPS CCW !"));
  }
  else
  {
    DBGLN(F("START STEPS CW !"));
    
  }
  #endif

  Events.raise(this,StepsRequested, &rr);
  wantDrawStepperStatus = true;

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void StepsScreen::stopSteps(bool ccw)
{
  if(!isInWork)
    return;
    
  isInWork = false;    

  StepsEventParam rr;
  rr.start = false;
  rr.ccw = ccw;
  rr.speed = 0;
  rr.steps = 0;  

  #ifdef _DEBUG
  if(ccw)
  {
    DBGLN(F("STOP STEPS CCW !"));
  }
  else
  {
    DBGLN(F("STOP STEPS CW !"));
    
  }
  #endif

  Events.raise(this,StepsRequested, &rr);
  wantDrawStepperStatus = true;

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ReductionScreen
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
ReductionScreen* Reduction = NULL;        
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
ReductionScreen::ReductionScreen() : AbstractHALScreen()
{
  Reduction = this;
  wantRedrawReductionMotor = false;
  wantRedrawReductionGear = false;

  lastReductionGearLength = 0;
  lastReductionMotorLength = 0;
  selectedMenu = 0; // выбираем первый делитель

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
ReductionScreen::~ReductionScreen()
{
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ReductionScreen::validate()
{
  uint16_t mBefore = reductionMotor;

 if(reductionMotor < 1)
    reductionMotor = 1;
    
  if(reductionMotor > 100)
    reductionMotor = 100;
                    
  if(mBefore != reductionMotor)
    wantRedrawReductionMotor = true;

  uint16_t gBefore = reductionGear;

  if(reductionGear < 1)
    reductionGear = 1;
  if(reductionGear > 100)
    reductionGear = 100;
  
  if(gBefore != reductionGear)
    wantRedrawReductionGear = true;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ReductionScreen::onDeactivate()
{
  // станем неактивными
  validate();
  Settings.setMotorReduction(reductionMotor);
  Settings.setGearReduction(reductionGear);
  
  wantRedrawReductionGear = false;
  wantRedrawReductionMotor = false;
  wantRedrawBackButton = false;
  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ReductionScreen::onActivate()
{
  // мы активизируемся
  reductionMotor = Settings.getMotorReduction();
  reductionGear = Settings.getGearReduction();
  selectedMenu = 0; // выбираем первый делитель
  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ReductionScreen::onEvent(Event event, void* param)
{
  if(!isActive())
    return;  

    switch(event)
    {
      case StepsRequested: // запросили шагать на определённое кол-во шагов
      {
      }
      break; // StepsRequested
      
      case StepperWorkDone: // движок остановился
      {
      }
      break; // StepperWorkDone

      case RotationRequested: // запросили вращение
      {
      }
      break; // RotationRequested
      
      #ifdef USE_KEYBOARD
      case KeyboardEvent:
      {
        if(selectedMenu == 0 || selectedMenu == 1)
        {
          int8_t keyCode = *((int8_t*) param);

          if(selectedMenu == 0) // reductionMotor
          {
            if(keyCode == DELETE_KEY)
              reductionMotor = 0;
            else
            if(keyCode == BACKSPACE_KEY)
            {
                reductionMotor /= 10;
            }
            else
            {
              reductionMotor *= 10;
              reductionMotor += keyCode;
              validate();
            }
            wantRedrawReductionMotor = true;
            
          } // selectedMenu == 0
          else // reductionGear
          {
            
            if(keyCode == DELETE_KEY)
              reductionGear = 0;
            else
            if(keyCode == BACKSPACE_KEY)
            {
                reductionGear /= 10;
            }
            else
            {
              reductionGear *= 10;
              reductionGear += keyCode;
              validate();
            }
            wantRedrawReductionGear = true;
            
          } // else
          
          Screen.notifyAction(this); // говорим, что мы отработали чего-то, т.е. на экране происходит действия.
          
        } // selectedMenu == 0 || selectedMenu == 1
      }
      break; // KeyboardEvent
      #endif
            
      case EncoderPositionChanged: // смена позиции энкодера
      {
        int changes = *((int*) param);
        if(changes != 0)
        {
            Screen.notifyAction(this); // говорим, что мы отработали чего-то, т.е. на экране происходит действия.
            switch(selectedMenu)
            {
              case 0: // выбран первый делитель
              {
                reductionMotor += changes;
                validate();
                wantRedrawReductionMotor = true;
              }
              break;

              case 1: // выбран второй делитель
              {
                 reductionGear += changes;
                 validate();                  
                wantRedrawReductionGear = true;
              }
              break;

              case 2: // выбрана кнопка "назад"
              {
                // переключаемся на экран настроек
                DBGLN(F("Back to settings screen!"));
                Screen.switchToScreen(Tune);    
              }
              break;
              
            } // switch
            
        }
      }
      break; // EncoderPositionChanged

      case EncoderButtonClicked: // кликнута кнопка энкодера
      {

        int8_t lastSelMenu = selectedMenu;
        selectedMenu++;
        if(selectedMenu > 2)
          selectedMenu = 0;

          switch(lastSelMenu)
          {
            case 0: wantRedrawReductionMotor = true; break;
            case 1: wantRedrawReductionGear = true; break;
            case 2: wantRedrawBackButton = true; break;
          }

          switch(selectedMenu)
          {
            case 0: wantRedrawReductionMotor = true; break;
            case 1: wantRedrawReductionGear = true; break;
            case 2: wantRedrawBackButton = true; break;
          }
          
        validate();
        Screen.notifyAction(this); // говорим, что мы отработали чего-то, т.е. на экране происходит действия.
      }
      break; // EncoderButtonClicked


      case ButtonStateChanged: // состояние какой-либо кнопки изменилось
      {      
      }
      break; // ButtonStateChanged
      
    } // switch    
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ReductionScreen::doSetup(HalDC* hal)
{
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ReductionScreen::doUpdate(HalDC* hal)
{
  if(!isActive())
    return;

    if(wantRedrawReductionMotor || wantRedrawReductionGear)
    {
      drawReductions(hal,80);
      wantRedrawReductionMotor = false;
      wantRedrawReductionGear = false;
    } // if

    if(wantRedrawBackButton)
    {
      wantRedrawBackButton = false;
      drawBackButton(hal,selectedMenu == 2);
    }

}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ReductionScreen::doDraw(HalDC* hal)
{
   hal->clearScreen();

   // тут отрисовка текущего состояния
   wantRedrawReductionMotor = true;
   wantRedrawReductionGear = true;
   drawGUI(hal);

   hal->updateDisplay();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ReductionScreen::drawGUI(HalDC* hal)
{
  int screenWidth = hal->getScreenWidth();
   
   hal->setFont(SCREEN_BIG_FONT);
   int fontWidth = hal->getFontWidth(SCREEN_BIG_FONT);
   hal->setBackColor(SCREEN_BACK_COLOR);
   hal->setColor(SCREEN_TEXT_COLOR);  

  int top = 20;
   
   String strToDraw = TXT_REDUCTION_SCREEN_CAPTION;
   int len = hal->print(strToDraw.c_str(),0,0,0,true);
   int left = (screenWidth - fontWidth*len)/2;
   hal->print(strToDraw.c_str(),left,top);

    top = 80;
  // рисуем слеш по центру экрана
  hal->setFont(AdditionalLettersFont);
  hal->setColor(DIGITS_FONT_COLOR);
  fontWidth = hal->getFontWidth(AdditionalLettersFont);
  left = (screenWidth - fontWidth)/2;
  strToDraw = '0'; // рисуем слеш '/'
  hal->print(strToDraw.c_str(),left,top);


  drawReductions(hal,top);


   drawBackButton(hal,selectedMenu == 2);
   
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ReductionScreen::drawReductions(HalDC* hal, int top)
{
   int screenWidth = hal->getScreenWidth();
   hal->setFont(SevenSeg_XXXL_Num);
   int fontWidth = hal->getFontWidth(SevenSeg_XXXL_Num);
   int fontHeight = hal->getFontHeight(SevenSeg_XXXL_Num);
     
  // у нас для редукции - максимально 3*2+1=7 позиций, например, 100/100
  // мы рисуем по центру экрана, редукция мотора выровнена вправо, редуктора - влево, между ними - слеш
  int screenCenter = screenWidth/2;

  if(wantRedrawReductionMotor) // надо перерисовать текущее значение числителя
  {
    wantRedrawReductionMotor = false;
    
    int rightBorder = screenCenter - fontWidth/2; // это наша правая граница, с которой начнутся все отсчёты
    String strToDraw;
    strToDraw = reductionMotor;
    int len = hal->print(strToDraw.c_str(),0,0,0,true);
    int left = rightBorder - len*fontWidth; // это - текущая левая граница, с которой надо рисовать

    if(lastReductionMotorLength && lastReductionMotorLength != len)
     {    
        if(len < lastReductionMotorLength) // уменьшили разрядность числа
        {
          hal->setColor(SCREEN_BACK_COLOR);
          int offset = left - fontWidth*(lastReductionMotorLength-len);
          hal->fillRect(offset, top, offset + fontWidth*(lastReductionMotorLength-len),top+fontHeight);
        }
     }  

   lastReductionMotorLength = len;
   
   hal->setColor(selectedMenu == 0 ? DIGITS_SELECTED_FONT_COLOR : DIGITS_FONT_COLOR);
   hal->setBackColor(selectedMenu == 0 ? DIGITS_SELECTED_BACK_COLOR : SCREEN_BACK_COLOR);
   hal->print(strToDraw.c_str(),left,top);          
    
  } // if(wantRedrawReductionMotor)

  if(wantRedrawReductionGear)  // надо перерисовать текущее значение знаменателя
  {
    wantRedrawReductionGear = false;
    
    int left = screenCenter + fontWidth/2; // наша левая граница
    String strToDraw;
    strToDraw = reductionGear;
    int len = hal->print(strToDraw.c_str(),0,0,0,true);

   if(lastReductionGearLength && lastReductionGearLength != len)
   {
      if(len < lastReductionGearLength) // уменьшили разрядность числа
      {
        hal->setColor(SCREEN_BACK_COLOR);
        int offset = left + fontWidth*len;
        hal->fillRect(offset, top, offset + fontWidth*(lastReductionGearLength-len),top+fontHeight);
      }
   }

   lastReductionGearLength = len;    

    hal->setColor(selectedMenu == 1 ? DIGITS_SELECTED_FONT_COLOR : DIGITS_FONT_COLOR);
    hal->setBackColor(selectedMenu == 1 ? DIGITS_SELECTED_BACK_COLOR : SCREEN_BACK_COLOR);
    hal->print(strToDraw.c_str(),left,top);          
    
  } // if(wantRedrawReductionGear)
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// DivideScreen
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DivideScreen* DivideByParts = NULL;
DivideScreen* DivideByDegrees = NULL;        
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DivideScreen::DivideScreen(DivideType t) : AbstractHALScreen()
{
  workMode = t;
  
  if(workMode == dtParts)
    DivideByParts = this;
  else
    DivideByDegrees = this;
  
  lastNumOfDivisionsLength = 0;
  lastCurrentPositionLength = 0;

  wantRedrawNumOfDivisions = false;
  wantRedrawCurrentPosition = false;

  isInWork = false;
  wantDrawStepperStatus = false;
  
  totalNumOfDivisions = 1;
  currentPosition = 0;
  
  isCWRotation = true;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DivideScreen::~DivideScreen()
{
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void DivideScreen::validate()
{
  uint8_t prevNumOfDivisions = totalNumOfDivisions;

  if(totalNumOfDivisions < 1)
    totalNumOfDivisions = 1;

  if(workMode == dtParts)
  {
  if(totalNumOfDivisions > 999)
    totalNumOfDivisions = 999;
  }
  else
  {
    if(totalNumOfDivisions > 359)
      totalNumOfDivisions = 359;
  }

  if(prevNumOfDivisions != totalNumOfDivisions)
    wantRedrawNumOfDivisions = true;
 
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void DivideScreen::onDeactivate()
{
  // станем неактивными
  validate();
  currentPosition = 0;
  wantRedrawNumOfDivisions = false;
  wantRedrawCurrentPosition = false;
  isCWRotation = true;
  
  if(workMode == dtParts)
    Settings.setNumOfDivisions(totalNumOfDivisions);
  else
    Settings.setDegrees(totalNumOfDivisions);
    

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void DivideScreen::onActivate()
{
  // мы активизируемся  
  if(workMode == dtParts)
    totalNumOfDivisions = Settings.getNumOfDivisions();
  else
    totalNumOfDivisions = Settings.getDegrees();
  
  bool w = !MotorController.isOnIdle();
  if(w != isInWork)
  {
    isInWork = w;
    wantDrawStepperStatus = true;
  }

  currentPosition = 0;
  isCWRotation = true;
  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void DivideScreen::onEvent(Event event, void* param)
{
  if(!isActive())
    return;  

    switch(event)
    {
      case StepsRequested: // запросили шагать на определённое кол-во шагов
      {
        
      }
      break; // StepsRequested
      
      case StepperWorkDone: // движок остановился
      {       
        if(isInWork)
        {
          isInWork = false;
          wantDrawStepperStatus = true;

            DBGLN(F("ONE DIVISION DONE!"));
            
            // остановились нормально, надо изменить значение текущего положения
            if(isCWRotation) // двигались по часовой
            {
              if(workMode == dtParts)
              {
                currentPosition++;
  
                if(currentPosition == totalNumOfDivisions)
                    currentPosition = 0;                              
              }
              else
              {
                currentPosition += totalNumOfDivisions;
                if(currentPosition > 360)
                  currentPosition -= 360;
              }
            }
            else // двигались против часовой
            {
              if(workMode == dtParts)
              {
                if(currentPosition == 0)
                  currentPosition = totalNumOfDivisions - 1;
                else
                  currentPosition--;
              }
              else
              {
                currentPosition -= totalNumOfDivisions;
                if(currentPosition < 0)
                  currentPosition = 360 + currentPosition;
              }
            }
          
            wantRedrawCurrentPosition = true;

        }
      }
      break; // StepperWorkDone

      case RotationRequested: // запросили вращение
      {
        
      }
      break; // RotationRequested

      #ifdef USE_KEYBOARD
      case KeyboardEvent:
      {
        if(!isInWork)
        {
          int8_t keyCode = *((int8_t*) param);
          
            if(keyCode == DELETE_KEY)
              totalNumOfDivisions = 0;
            else
            if(keyCode == BACKSPACE_KEY)
            {
                totalNumOfDivisions /= 10;
            }
            else
            {
              totalNumOfDivisions *= 10;
              totalNumOfDivisions += keyCode;
              validate();
            }
            
            wantRedrawNumOfDivisions = true;

            // сбрасываем стартовую позицию, поскольку значение кол-ва делений изменилось
            currentPosition = 0;
            wantRedrawCurrentPosition = true;
          
          Screen.notifyAction(this); // говорим, что мы отработали чего-то, т.е. на экране происходит действия.
        } // if(!isInWork)
      }
      break; // KeyboardEvent      
      #endif
            
      case EncoderPositionChanged: // смена позиции энкодера
      {
        if(!isInWork)
        {
            int changes = *((int*) param);
            if(changes != 0)
            {
                Screen.notifyAction(this); // говорим, что мы отработали чего-то, т.е. на экране происходит действия.
                
                totalNumOfDivisions += changes;
                validate();
                wantRedrawNumOfDivisions = true;
    
                // сбрасываем стартовую позицию, поскольку значение кол-ва делений изменилось
                currentPosition = 0;
                wantRedrawCurrentPosition = true;
                
            }
        } // if(!isInWork)
      }
      break; // EncoderPositionChanged

      case EncoderButtonClicked: // кликнута кнопка энкодера
      {             
        Screen.notifyAction(this); // говорим, что мы отработали чего-то, т.е. на экране происходит действия.
        validate();
        
        // переключаемся на стартовый экран
        DBGLN(F("Back to main screen!"));
        
        if(isInWork) // перед переключением на другой экран принудительно останавливаем мотор!!!
        {
          stopSteps(true);
        }
          
        Screen.switchToScreen(StartScreen);    
        
      }
      break; // EncoderButtonClicked


      case ButtonStateChanged: // состояние какой-либо кнопки изменилось
      {
        Screen.notifyAction(this); // говорим, что мы отработали чего-то, т.е. на экране происходит действия.
        ButtonEventParam* p = (ButtonEventParam*) param;

        if(p->state & BUTTON_CLICKED)
        {
          // кнопка кликнута
          if(isInWork)
          {
            // работаем ещё, надо остановить мотор
            DBGLN(F("Stop motor prematurely!!!"));

            stopSteps(p->button == LEFT_BUTTON);
            
            // принудительно сбрасываем текущую позицию, т.к. нас остановили досрочно
            currentPosition = 0;
            wantRedrawCurrentPosition = true;
            
          }
          else              
          {
            // режим простоя, запускаем шагание
            validate();
            if(workMode == dtParts)
              startSteps(p->button == LEFT_BUTTON);
            else
              startSteps(false); // в режиме деления по градусам движемся только в одну сторону
          }
          
        } // if(p->state & BUTTON_CLICKED)   
        
        
      }
      break; // ButtonStateChanged
      
    } // switch    
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void DivideScreen::doSetup(HalDC* hal)
{
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void DivideScreen::doUpdate(HalDC* hal)
{
  if(!isActive())
    return;

    if(isInWork)
       Screen.notifyAction(this); // говорим, что мы отработали чего-то, т.е. на экране происходит действия.

    if(wantRedrawNumOfDivisions)
    {
      wantRedrawNumOfDivisions = false;
      drawNumOfDivisions(hal,80);
    } // if

    if(wantRedrawCurrentPosition)
    {
      wantRedrawCurrentPosition = false;
      drawCurrentPosition(hal,80);
    }

    if(wantDrawStepperStatus)
    {
      wantDrawStepperStatus = false;
      drawStepperStatus(hal,isInWork);
    }

}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void DivideScreen::doDraw(HalDC* hal)
{
   hal->clearScreen();

   // тут отрисовка текущего состояния
   drawGUI(hal);

   hal->updateDisplay();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void DivideScreen::drawGUI(HalDC* hal)
{
  int screenWidth = hal->getScreenWidth();
   
   hal->setFont(SCREEN_BIG_FONT);
   int fontWidth = hal->getFontWidth(SCREEN_BIG_FONT);
   int fontHeight = hal->getFontHeight(SCREEN_BIG_FONT);
   hal->setBackColor(SCREEN_BACK_COLOR);
   hal->setColor(SCREEN_TEXT_COLOR);  

  int top = 20;
  int vSpacing = 10;
   
   String strToDraw;
   if(workMode == dtParts)
    strToDraw = TXT_PARTS_SCREEN_CAPTION;
   else
    strToDraw = TXT_DEGREES_SCREEN_CAPTION;
   
   int len = hal->print(strToDraw.c_str(),0,0,0,true);
   int left = (screenWidth - fontWidth*len)/2;
   hal->print(strToDraw.c_str(),left,top);


   hal->setColor(DIGITS_LABEL_FONT_COLOR);
   if(workMode == dtParts)
    strToDraw = TXT_PARTS_SCREEN_PARTS_LABEL;
   else
    strToDraw = TXT_DEGREES_SCREEN_PARTS_LABEL;
   hal->print(strToDraw.c_str(),40,80-fontHeight-6);
   
   if(workMode == dtParts)
    strToDraw = TXT_PARTS_SCREEN_CUR_PART_LABEL;
   else
    strToDraw = TXT_DEGREES_SCREEN_CUR_PART_LABEL;

   len = hal->print(strToDraw.c_str(),0,0,0,true);
   hal->print(strToDraw.c_str(),screenWidth - 40 - len*fontWidth,80-fontHeight-6);

   drawNumOfDivisions(hal,80);
   top =  drawCurrentPosition(hal,80);
   
   if(workMode == dtParts)
    top += 20;
   else
    top += 4;

   hal->setFont(SCREEN_BIG_FONT);
   fontWidth = hal->getFontWidth(SCREEN_BIG_FONT);
   fontHeight = hal->getFontHeight(SCREEN_BIG_FONT);
   hal->setBackColor(SCREEN_BACK_COLOR);
   hal->setColor(SCREEN_TEXT_COLOR);  

    #ifdef DISPLAY_SCREEN_HINTS
    
       if(workMode == dtParts)
        strToDraw = TXT_PARTS_SCREEN_HINT_1;
       else
        strToDraw = TXT_DEGREES_SCREEN_HINT_1;
       
       len = hal->print(strToDraw.c_str(),0,0,0,true);
       left = (screenWidth - fontWidth*len)/2;
       hal->print(strToDraw.c_str(),left,top);
       top += fontHeight + vSpacing;
    
       if(workMode == dtParts)
        strToDraw = TXT_PARTS_SCREEN_HINT_2;
       else
        strToDraw = TXT_DEGREES_SCREEN_HINT_2;
       
       len = hal->print(strToDraw.c_str(),0,0,0,true);
       left = (screenWidth - fontWidth*len)/2;
       hal->print(strToDraw.c_str(),left,top);
       top += fontHeight + vSpacing;
       
       if(workMode == dtParts)
       {
       }
       else
       {
          strToDraw = TXT_DEGREES_SCREEN_HINT_3;    
          len = hal->print(strToDraw.c_str(),0,0,0,true);
          left = (screenWidth - fontWidth*len)/2;
          hal->print(strToDraw.c_str(),left,top);
          top += fontHeight + vSpacing;    
       }

   #endif // #ifdef DISPLAY_SCREEN_HINTS

   drawBackButton(hal,true);
   drawStepperStatus(hal,isInWork);
   
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int DivideScreen::drawCurrentPosition(HalDC* hal, int top)
{
   int screenWidth = hal->getScreenWidth();
   hal->setFont(SevenSeg_XXXL_Num);
   int fontWidth = hal->getFontWidth(SevenSeg_XXXL_Num);
   int fontHeight = hal->getFontHeight(SevenSeg_XXXL_Num);

    
   String strToDraw;
   strToDraw = currentPosition;
   int len = hal->print(strToDraw.c_str(),0,0,0,true);
   int left = (screenWidth - fontWidth*len) - 40;

   if(lastCurrentPositionLength && lastCurrentPositionLength != len)
   {
      if(len < lastCurrentPositionLength) // уменьшили разрядность числа
      {
        hal->setColor(SCREEN_BACK_COLOR);
        int offset = left-fontWidth*(lastCurrentPositionLength-len);
        hal->fillRect(offset, top, offset + fontWidth*(lastCurrentPositionLength-len),top+fontHeight);
      }
   }

   lastCurrentPositionLength = len;
   
   hal->setColor(DIGITS_FONT_COLOR);
   hal->setBackColor(SCREEN_BACK_COLOR);
   hal->print(strToDraw.c_str(),left,top);   

   return (top + fontHeight + 10);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void DivideScreen::drawNumOfDivisions(HalDC* hal, int top)
{
   hal->setFont(SevenSeg_XXXL_Num);
   int fontWidth = hal->getFontWidth(SevenSeg_XXXL_Num);
   int fontHeight = hal->getFontHeight(SevenSeg_XXXL_Num);

    
   String strToDraw;
   strToDraw = totalNumOfDivisions;
   int len = hal->print(strToDraw.c_str(),0,0,0,true);
   int left = 40;

   if(lastNumOfDivisionsLength && lastNumOfDivisionsLength != len)
   {    
      if(len < lastNumOfDivisionsLength) // уменьшили разрядность числа
      {
        hal->setColor(SCREEN_BACK_COLOR);
        int offset = left+fontWidth*len;
        hal->fillRect(offset, top, offset + fontWidth*(lastNumOfDivisionsLength-len),top+fontHeight);
      }
   }

   lastNumOfDivisionsLength = len;
   
   hal->setColor(DIGITS_FONT_COLOR);
   hal->setBackColor(SCREEN_BACK_COLOR);
   hal->print(strToDraw.c_str(),left,top);   

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void DivideScreen::startSteps(bool ccw)
{
  if(workMode == dtParts)
  {
    if(Settings.getNumOfDivisions() != uint16_t(totalNumOfDivisions))
    {
      validate();
      Settings.setNumOfDivisions(totalNumOfDivisions);
    }
  }
  else
  {
    if(Settings.getDegrees() != uint16_t(totalNumOfDivisions))
    {
      validate();
      Settings.setDegrees(totalNumOfDivisions);
    }    
  }
  
  isInWork = true;
  isCWRotation = !ccw;

  int32_t stepsComputed = 0;
  float stepsPerRevolution = MotorController.getStepsPerRevolution();
  float stepsPerDivision;
  
  if(workMode == dtParts)
    stepsPerDivision = stepsPerRevolution / totalNumOfDivisions;
  else
  {
    stepsPerDivision = (stepsPerRevolution*totalNumOfDivisions)/360;
  }

  DBG(F("Steps per REV: "));
  DBGLN(stepsPerRevolution);

  DBG(F("Steps per DIV: "));
  DBGLN(stepsPerDivision);

  if(!ccw) // движемся по часовой
  {
      int thisPosition = workMode == dtParts ? currentPosition : currentPosition/totalNumOfDivisions;
    
      int currentStepsCount = thisPosition * stepsPerDivision;
      int nextStepsCount = (thisPosition + 1.00)  * stepsPerDivision;
      
      stepsComputed = (nextStepsCount - currentStepsCount);
    
  }
  else // движемся против часовой
  {
    int thisPosition = workMode == dtParts ? currentPosition : currentPosition/totalNumOfDivisions;
    int thisNumOfDivisions = workMode == dtParts ? totalNumOfDivisions : 360/totalNumOfDivisions;
    
      if(thisPosition == 0)
      {
        int currentStepsCount = stepsPerRevolution;
        int nextStepsCount = stepsPerDivision * (thisNumOfDivisions - 1.00);
        
        stepsComputed = (currentStepsCount - nextStepsCount);        
      }
      else
      {
        int currentStepsCount = thisPosition * stepsPerDivision;
        int nextStepsCount = (thisPosition - 1)  * stepsPerDivision;
        
        stepsComputed = (currentStepsCount - nextStepsCount);
        
      }
    
  } // else

  DBG(F("Steps computed for current DIV: "));
  DBGLN(stepsComputed);


  StepsEventParam rr;
  rr.start = true;
  rr.ccw = ccw;
  rr.speed = Settings.getRotationSpeed();
  rr.steps = stepsComputed;
  
  #ifdef _DEBUG
  if(ccw)
  {
    DBGLN(F("START STEPS CCW !"));
  }
  else
  {
    DBGLN(F("START STEPS CW !"));
    
  }
  #endif

  Events.raise(this,StepsRequested, &rr);
  wantDrawStepperStatus = true;

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void DivideScreen::stopSteps(bool ccw)
{
  if(!isInWork)
    return;
    
  isInWork = false;    

  StepsEventParam rr;
  rr.start = false;
  rr.ccw = ccw;
  rr.speed = 0;
  rr.steps = 0;  

  #ifdef _DEBUG
  if(ccw)
  {
    DBGLN(F("STOP STEPS CCW !"));
  }
  else
  {
    DBGLN(F("STOP STEPS CW !"));
    
  }
  #endif

  Events.raise(this,StepsRequested, &rr);
  wantDrawStepperStatus = true;

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------


