#ifndef VU_METER_H
#define VU_METER_H

#include <QFrame>
#include <QTimer>
#include <functional>

class VU_Meter : public QFrame
{
    Q_OBJECT

public:
    explicit VU_Meter(QString title, const std::function<int8_t(void)>& f_);
    virtual ~VU_Meter();

public slots:
    void paintEvent(QPaintEvent *event) override;
    void timerExpired(void);


private:

    QString title;
    QTimer redrawTimer;
    std::function<int8_t(void)> f;
    int8_t max_val;

};

#endif // VU_METER_H




