//
// Created by Lyndon on 2025/1/7.
//

#include "PaperSpecification.h"


PaperSpecification::PaperSpecification(QString type)
{
    if (type == "A3") this->type = Type::A3;
    else if (type == "A4") this->type = Type::A4;
    else if (type == "A5") this->type = Type::A5;
    else if (type == "B4") this->type = Type::B4;
    else if (type == "B5") this->type = Type::B5;
    else if (type == "A900") this->type = Type::A900;
    else if (type == "Letter") this->type = Type::Letter;
    else if (type == "Legal") this->type = Type::Legal;
    else if (type == "Executive") this->type = Type::Executive;
    else if (type == "Folio") this->type = Type::Folio;
    else if (type == "Custom") this->type = Type::Custom;
    else throw PaperSpecificationException("Unknown paper specification");
}

QString PaperSpecification::getPaperName() const  {
    switch (type) {
    case Type::A3: return "A3";
    case Type::A4: return "A4";
    case Type::A5: return "A5";
    case Type::B4: return "B4";
    case Type::B5: return "B5";
    case Type::A900: return "A900";
    case Type::Letter: return "Letter";
    case Type::Legal: return "Legal";
    case Type::Executive: return "Executive";
    case Type::Folio: return "Folio";
    case Type::Custom: return "Custom";
    default: throw PaperSpecificationException("Unknown paper specification");
    }
}

QVector<PaperSpecification> PaperSpecification::getLayoutPaperList() {
    QVector<PaperSpecification> layoutPaperList;
    for (const auto& configAvailable : LAYOUT_PAPER_LIST) {
        try {
            // 将字符串转换为 Type 枚举
            Type type;
            if (configAvailable == "A3") type = Type::A3;
            else if (configAvailable == "A4") type = Type::A4;
            else if (configAvailable == "A5") type = Type::A5;
            else if (configAvailable == "B4") type = Type::B4;
            else if (configAvailable == "B5") type = Type::B5;
            else if (configAvailable == "A900") type = Type::A900;
            else if (configAvailable == "Letter") type = Type::Letter;
            else if (configAvailable == "Legal") type = Type::Legal;
            else if (configAvailable == "Executive") type = Type::Executive;
            else if (configAvailable == "Folio") type = Type::Folio;
            else if (configAvailable == "Custom") type = Type::Custom;
            else throw PaperSpecificationException("Unknown paper specification");

            layoutPaperList.append(PaperSpecification(type));
        } catch (const std::exception& e) {
            qWarning() << "not support paper: " << e.what();
        }
    }
    return layoutPaperList;
}

// 获取纸张尺寸
QPair<int, int> PaperSpecification::getPaperSize() const {
    static const QMap<Type, QPair<int, int>> paperSizes = {
        {Type::A3, {297, 420}},
        {Type::A4, {210, 297}},
        {Type::A5, {148, 210}},
        {Type::B4, {250, 353}},
        {Type::B5, {176, 250}},
        {Type::A900, {900, 1272}},
        {Type::Letter, {216, 279}},
        {Type::Legal, {216, 356}},
        {Type::Executive, {184, 267}},
        {Type::Folio, {210, 330}},
        {Type::Custom, {0, 0}}
    };
    return paperSizes.value(type);
}