//
// Created by etl on 12/27/24.
//
#include <QtTest>

class TestJson : public QObject
{
    Q_OBJECT
private slots:
    void testCase1()
    {
        QVERIFY(2 + 2 == 4);
    }

    void testCase2() {
        QString jsonStr = R"({
        "name": "Alice",
        "age": 25,
        "hobbies": ["reading", "swimming"],
        "address": {
            "city": "New York",
            "street": "Broadway"
        }
    })";
    }
};

QTEST_MAIN(TestJson)
#include "test_json.moc"