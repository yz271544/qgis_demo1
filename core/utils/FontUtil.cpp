//
// Created by Lyndon on 2025/1/7.
//

#include "FontUtil.h"


int FontUtil::getTextFontWidth(const QString& data, const int fontSize, const float letterSpacing)
{
    const int dataLength = data.length();
    return qCeil(dataLength * SIM_PER_POUND_WIDTH * fontSize + (dataLength - 1) * SIM_PER_POUND_WIDTH * letterSpacing);
}

int FontUtil::getSingleTextSize(const float fontSize)
{
    return qCeil(SIM_PER_POUND_WIDTH * fontSize);
}