//
// Created by Lyndon on 2025/1/7.
//

#ifndef PAPERSPECIFICATION_H
#define PAPERSPECIFICATION_H

#include <QDebug>
#include <QVector>
#include <QString>
#include <QMap>
#include <QException>
#include <utility>

// 假设这是从配置文件中获取的纸张列表
inline QVector<QString> LAYOUT_PAPER_LIST = {"A3", "A4", "A5", "B4", "B5", "A900", "Letter", "Legal", "Executive", "Folio", "Custom"};

// 自定义异常类，继承自 QException
class PaperSpecificationException : public QException {
public:
    explicit PaperSpecificationException(QString  message) : message(std::move(message)) {}

    // 重写 clone 方法
    [[nodiscard]] PaperSpecificationException* clone() const override {
        return new PaperSpecificationException(*this);
    }

    // 重写 raise 方法
    void raise() const override {
        throw *this;
    }

    // 获取异常信息
    [[nodiscard]] QString getMessage() const {
        return message;
    }

private:
    QString message;
};

// 纸张规格枚举类
class PaperSpecification : public QException {
public:
    // 枚举类，表示纸张类型
    enum class Type {
        A3,
        A4,
        A5,
        B4,
        B5,
        A900,
        Letter,
        Legal,
        Executive,
        Folio,
        Custom
    };

    // 构造函数
    explicit PaperSpecification(Type type) : type(type) {}

    explicit PaperSpecification(QString type);

    // 获取纸张名称
    [[nodiscard]] QString getPaperName() const;

    // 获取布局纸张列表
    static QVector<PaperSpecification> getLayoutPaperList();

    // 获取纸张尺寸
    [[nodiscard]] QPair<int, int> getPaperSize() const;

private:
    Type type; // 纸张类型
};

#endif //PAPERSPECIFICATION_H
