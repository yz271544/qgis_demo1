//
// Created by Lyndon on 2024/12/27.
//

#ifndef IMAGEUTIL_H
#define IMAGEUTIL_H

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#if defined(_WIN32)
#include <windows.h>
#endif

#include <iostream>
#include <string>
#include <utility>
#include <QByteArray>
#include <QString>
#include <QStringList>
#include <stdexcept>
#include <QDebug>


class ImageUtil {
public:
	static std::pair<QString, QByteArray> parse_base64_image(const QString& base64_str);
};



#endif //IMAGEUTIL_H
