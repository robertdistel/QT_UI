#include "shoutdown.h"
#include "global_defs.h"
#include "sharedmemory.h"

#include <stdlib.h>
#include <QString>


Shoutdown::Shoutdown(QWidget* parent,MediaSource theOperator_, MulticastGroup   theShoutdownGroup_, SharedMemory<TMM_CntlBuffer>& ctrl_buffer_):QFrame(parent),theOperator(theOperator_),theShoutdownGroup(theShoutdownGroup_),ctrl_buffer(ctrl_buffer_)
{
    Button.setCheckable(false);
    Button.setText("Talk");
    Button.setParent(this);
    Button.setGeometry(::width/2,(::height*.5)/2, 3* ::width,::height);
    setFixedSize(4* ::width, (3.5 * ::height)/2);
    setFrameShadow(QFrame::Raised);
    setFrameShape(QFrame::Box);

    StatusTimer.setInterval(500); // check every 500 ms
    StatusTimer.setSingleShot(false);
    connect(&StatusTimer, SIGNAL(timeout()), this, SLOT(timer_update()));
    connect(&Button, SIGNAL(pressed()),this, SLOT(pressed()));
    connect(&Button, SIGNAL(released()),this, SLOT(released()));
    ctrl_buffer->setEnabled(theOperator.allOtherOperators() ,theShoutdownGroup);  //enable reception from all other operators

    StatusTimer.start();
}

Shoutdown::~Shoutdown()
{
    StatusTimer.stop();
}

void Shoutdown::timer_update()
{
    QPalette  palette;

    if(ctrl_buffer->checkActiveAndClear(theOperator.allOtherOperators(),theShoutdownGroup)) //i am seeing something other than me
    {
        palette.setColor(QPalette::ButtonText,Qt::green);
    }
    else
    {
        palette.setColor(QPalette::ButtonText,Qt::black);
    }

    Button.setPalette(palette);

    update();
}

void Shoutdown::pressed(void)
{

    ctrl_buffer->setEnabled(theOperator,theShoutdownGroup);  //enable transmitter
    timer_update();
}

void Shoutdown::released(void)
{
    ctrl_buffer->clearEnabled(theOperator,theShoutdownGroup);; //disable transmitter
}


