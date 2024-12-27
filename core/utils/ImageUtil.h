//
// Created by Lyndon on 2024/12/27.
//

#ifndef IMAGEUTIL_H
#define IMAGEUTIL_H



#include <iostream>
#include <string>
#include <utility>
#include <QByteArray>
#include <QString>
#include <QStringList>
#include <stdexcept>


class ImageUtil {
public:
    static std::pair<QString, QByteArray> parse_base64_image(const QString& base64_str);
};



#endif //IMAGEUTIL_H
