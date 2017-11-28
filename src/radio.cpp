
#include "radio.h"
#include "global_defs.h"
#include "sharedmemory.h"


#include <stdlib.h>
#include <QString>

Radio::Radio(MediaSource theOperator_, MediaSource theRadio_,MulticastGroup theRadioGroup_, SharedMemory<TMM_CntlBuffer>& ctrl_buffer_)
    :RX_LevelMeter("RX", std::function<int8_t(void)>( [&](){return ctrl_buffer->getPower(theRadio);}))
    ,theOperator(theOperator_)
    ,theRadio(theRadio_)
    ,theRadioGroup(theRadioGroup_)
    ,ctrl_buffer(ctrl_buffer_)
{
    //attach this layout
    setLayout(&Layout);

    Layout.addWidget(&RX_Volume,0,0,3,1);
    Layout.addWidget(&MuteButton,3,0,1,1);
    Layout.addWidget(&RX_Button,0,1,2,2);
    Layout.addWidget(&TX_Button,2,1,2,2);
    Layout.addWidget(&RX_LevelMeter,0,3,4,1);
    for(size_t k=0; k<4; k++)
    {
        Layout.setColumnMinimumWidth(k,::width);
        Layout.setColumnStretch(k,1);
        Layout.setRowMinimumHeight(k,::height);
        Layout.setRowStretch(k,1);
    }
    //set sizing policy
    QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);


    //set the size of the radio function panel
    setFrameShadow(QFrame::Raised);
    setFrameShape(QFrame::Box);


    RX_Button.setCheckable(true);
    RX_Button.setChecked(!ctrl_buffer->isEnabled(theRadio,theRadioGroup)); //set receiver state based on inital shmem state
    RX_Button.setText(QString(theRadioGroup.getName()) + QString("\nRX"));
    RX_Button.setSizePolicy(sizePolicy);

    TX_Button.setCheckable(false);
    TX_Button.setText("TX");
    TX_Button.setSizePolicy(sizePolicy);

    RX_Volume.setOrientation( Qt::Vertical);
    RX_Volume.setRange(-30,30);
    RX_Volume.setTickInterval(6);
    RX_Volume.setTickPosition(QSlider::TicksBothSides);
    auto v=ctrl_buffer->getGain(theRadio);
    if (v<-30) v=-30;
    if (v>30)  v=30;
    RX_Volume.setValue(v);
    RX_Volume.setSizePolicy(sizePolicy);

    MuteButton.setCheckable(true);
    MuteButton.setChecked(ctrl_buffer->getGain(theRadio)<-60);
    Mute_toggled();
    MuteButton.setText("mute");
    MuteButton.setSizePolicy(sizePolicy);

    RX_LevelMeter.setSizePolicy(sizePolicy);

    StatusTimer.setInterval(500); // check every 500 ms
    StatusTimer.setSingleShot(false);
    connect(&StatusTimer, SIGNAL(timeout()), this, SLOT(timer_update()));
    StatusTimer.start();
    connect(&RX_Button, SIGNAL(toggled(bool)),this, SLOT(RX_toggled(bool)));
    connect(&TX_Button, SIGNAL(pressed()),this, SLOT(TX_pressed()));
    connect(&TX_Button, SIGNAL(released()),this, SLOT(TX_released()));
    connect(&MuteButton, SIGNAL(toggled(bool)),this, SLOT(Mute_toggled()));
    connect(&RX_Volume,SIGNAL(valueChanged(int)),this,SLOT(GainChanged(int)));

    timer_update(); //kick off a refresh to update the state
}

Radio::~Radio()
{
    StatusTimer.stop();
}
void Radio::GainChanged(int v)
{
    if (!MuteButton.isChecked())
        ctrl_buffer->setGain(theRadio,v);

}

void Radio::Mute_toggled()
{
    bool state=MuteButton.isChecked();
    if (!state)
    {
        MuteButton.setText("mute");
        ctrl_buffer->setGain(theRadio,RX_Volume.value()); //should be the slider state
    }
    else
    {
        ctrl_buffer->setGain(theRadio,127);
        MuteButton.setText("unmute");
    }
    timer_update();

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
        RX_LevelMeter.setEnabled(true);
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
        RX_LevelMeter.setEnabled(false);
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
