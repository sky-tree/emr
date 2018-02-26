#include "UIConstants.h"
#include "BaseMath.h"


#if _MSC_VER >= 1600      
#pragma execution_character_set("utf-8")      
#endif

const float UIConstants::SCROLL_DIS_PERCENT_Y = 0.1f;

const float UIConstants::DEFAULT_ROW_DIS_H = 4.0f;

const float UIConstants::IMG_DRAG_ALPHA = 0.35f;

//const float UIConstants::WindowParams::W_H_RATIO = 0.70173f;

//const float UIConstants::PaperTypeAndWH::A4_W = 21;
//const float UIConstants::PaperTypeAndWH::A4_H = 29.7;
//const float UIConstants::PaperTypeAndWH::B5_W = 18.2;
//const float UIConstants::PaperTypeAndWH::B5_H = 25.7;

QColor* UIConstants::MyColor::WHITE_COLOR_OBJ = BaseMath::createColor(UIConstants::MyColor::WHITE);
QColor* UIConstants::MyColor::BLACK_COLOR_OBJ = BaseMath::createColor(UIConstants::MyColor::BLACK);
const QColor* UIConstants::MyColor::ONLY_READ_COLOR_OBJ = BaseMath::createColor(UIConstants::MyColor::ONLY_READ_COLOR);
const QColor* UIConstants::MyColor::PARAGRAPH_NUM_COLOR_OBJ = BaseMath::createColor(UIConstants::MyColor::PARAGRAPH_NUM_COLOR);
//const float BaseConstants::MouseDragScaleAbout::AREA_PERCENT = 0.05f;