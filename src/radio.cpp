
#include "radio.h"
#include "global_defs.h"
#include "sharedmemory.h"


#include <stdlib.h>
#include <QString>

Radio::Radio(QWidget* parent, MediaSource theOperator_, MediaSource theRadio_,MulticastGroup theRadioGroup_, SharedMemory<TMM_CntlBuffer>& ctrl_buffer_)
    :QFrame(parent),theOperator(theOperator_),theRadio(theRadio_),theRadioGroup(theRadioGroup_), ctrl_buffer(ctrl_buffer_)
{
    RX_Button.setCheckable(true);
    RX_Button.setChecked(!ctrl_buffer->isEnabled(theRadio,theRadioGroup)); //set receiver state based on inital shmem state
    TX_Button.setCheckable(false);
    RX_Button.setText(QString(theRadioGroup.getName()) + QString("\nRX"));
    TX_Button.setText("TX");
    RX_Button.setParent(this);
    TX_Button.setParent(this);
    RX_Button.setGeometry(::width/2,(::height*1)/2, ::width,::height);
    TX_Button.setGeometry(::width/2,(::height*4)/2, ::width,::height);
    setFixedSize(2* ::width, (6.5 * ::height)/2);
    setFrameShadow(QFrame::Raised);
    setFrameShape(QFrame::Box);
    StatusTimer.setInterval(500); // check every 500 ms
    StatusTimer.setSingleShot(false);
    connect(&StatusTimer, SIGNAL(timeout()), this, SLOT(timer_update()));
    StatusTimer.start();
    connect(&RX_Button, SIGNAL(toggled(bool)),this, SLOT(RX_toggled(bool)));
    connect(&TX_Button, SIGNAL(pressed()),this, SLOT(TX_pressed()));
    connect(&TX_Button, SIGNAL(released()),this, SLOT(TX_released()));
    timer_update(); //kick off a refresh to update the state
}

Radio::~Radio()
{
    StatusTimer.stop();
}

void Radio::timer_update()
{
    QPalette  palette;

    if(ctrl_buffer->checkActiveAndClear(theOperator.allOtherOperators(),theRadioGroup)) //i am seeing the other operator keying this radio
    {
        palette.setColor(QPalette::ButtonText,Qt::green);
    }
    else
    {
        palette.setColor(QPalette::ButtonText,Qt::black);
    }

    TX_Button.setPalette(palette);

    if (ctrl_buffer->isEnabled(theRadio,theRadioGroup)) //reciever is enabled
    {
        if(ctrl_buffer->checkActiveAndClear(theRadio,theRadioGroup)) //and activity detected
        {
            palette.setColor(QPalette::ButtonText,Qt::red);     //indicate activity and
        }
        else
        {
            palette.setColor(QPalette::ButtonText,Qt::black);
        }
    }
    else
    {
        palette.setColor(QPalette::ButtonText,Qt::darkGray);    //i dont care about it
    }


    RX_Button.setPalette(palette);
    update();
}

void Radio::RX_toggled(bool state)
{
    if (!state)
        ctrl_buffer->setEnabled(theRadio,theRadioGroup);
    else
        ctrl_buffer->clearEnabled(theRadio,theRadioGroup);

    timer_update();
}

void Radio::TX_pressed(void)
{
    if (ctrl_buffer->isEnabled(theRadio,theRadioGroup))        //only enable if receiver is enabled
        ctrl_buffer->setEnabled(theOperator,theRadioGroup);  //enable transmitter
    timer_update();
}

void Radio::TX_released(void)
{
    ctrl_buffer->clearEnabled(theOperator,theRadioGroup); //disable transmitter
}
