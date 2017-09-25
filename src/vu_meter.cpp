#include "vu_meter.h"
#include <functional>
#include <QPainter>
#include "global_defs.h"
#include "sharedmemory.h"
#include "TMM_StreamCtrl.h"
#include <iostream>
#include <QFrame>


void VU_Meter::timerExpired(void)
{
        update();
}

void VU_Meter::paintEvent(QPaintEvent *event)
{
    const auto MIN(-3);
    const auto MAX(40);

    if(!isEnabled())
        return; //dont do anything if we are not enabled

    //grab the current power of the stream
    int8_t pwr=-f();

    std::cout << (int)pwr << std::endl;
    if (pwr<MIN)
        pwr=MIN;
    if(pwr>MAX)
        pwr=MAX;

    max_val+=1;
    if (max_val>MAX)
        max_val=MAX;

    if (pwr<max_val)
        max_val = pwr;

    //power is now between 0 and 1

    //pwr is in dB , normalised such that 6dB is full scale square wave on 16 bit data - maximum power
#if 0
    float p = float(-pwr+3)/40;


    if (p<0)
        p=0;
    if (p>1)
        p=1;

    QPainter painter(this);
    QPixmap pixmap(":/images/VUM_Vert.png");

    painter.drawPixmap(rect(),pixmap);
    QRect bar = rect();

    bar.setLeft(35.0/90.0 * rect().width());
    bar.setRight(54.0/90.0 * rect().width());
    bar.setBottom((27.0+(354.0-27.0) *( p)) / 429.0 * rect().height());
    bar.seQString title, tTop(27.0 / 429.0 * rect().height());
    painter.fillRect(bar, QColor(20,35,42));
#endif
    QPainter painter(this);
    auto h=rect().height()/(MAX-MIN + 5);
    if (h<10)
        painter.setFont({"Helvetica", h});
    else
        painter.setFont({"Helvetica", 10});


    QColor colour;
    QRect bar;
    for(int k = MIN; k<MAX; k++)
    {
        if (k<3)
            colour=Qt::red;
        else if (k< 9)
            colour=Qt::yellow;
        else
            colour=Qt::green;
        bar.setLeft(rect().width()/4);
        bar.setWidth(rect().width()/2);
        bar.setTop(rect().height()*(k-MIN + 2)/(MAX-MIN + 5));
        bar.setHeight(rect().height()/(MAX-MIN + 5));
        if(pwr <= k)
            painter.fillRect(bar, colour);


        if(k % 3 == 0 && h>4) //enough space to write the numbers
            painter.drawText(bar,Qt::AlignCenter,QString::number(k));

        if(k==max_val && pwr!=max_val)
        {
            bar.setTop(rect().height()*(k-MIN + 2)/(MAX-MIN + 5));
            bar.setHeight(2);
            painter.fillRect(bar, colour);
        }

    }

    if (h>4)
    {
        bar.setTop(rect().height()*(MAX-MIN + 3)/(MAX-MIN + 5));
        bar.setHeight(rect().height()/(MAX-MIN + 5));
        painter.drawText(bar,Qt::AlignCenter,QString("dB"));
        bar.setTop(rect().height()*(1)/(MAX-MIN + 5));
        bar.setHeight(rect().height()/(MAX-MIN + 5));
        painter.drawText(bar,Qt::AlignCenter,title);

    }


    QFrame::paintEvent(event);


}

VU_Meter::VU_Meter(QString title_, const std::function<int8_t(void)>& f_)
    :title(title_)
    ,f(f_)
{
    setFrameShadow(QFrame::Raised);
    setFrameShape(QFrame::Box);

    redrawTimer.setInterval(200); // check every 500 ms
    redrawTimer.setSingleShot(false);
    connect(&redrawTimer, SIGNAL(timeout()), this, SLOT(timerExpired())); //every .5sec just repaint
    redrawTimer.start();

}


VU_Meter::~VU_Meter()
{

}
