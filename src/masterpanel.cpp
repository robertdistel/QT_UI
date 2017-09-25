#include "masterpanel.h"
#include "global_defs.h"

void MasterPanel::MicGainChanged(int v)
{
        ctrl_buffer->setTMM_MicGain(v);

}

void MasterPanel::MasterGainChanged(int v)
{
        ctrl_buffer->setTMM_MasterHeadsetGain(v);
}

MasterPanel::MasterPanel(SharedMemory<TMM_CntlBuffer>& ctrl_buffer_)
    :MicLevel("Mic", std::function<int8_t(void)>([&](){return ctrl_buffer->getTMM_MicPower();}))
    ,ctrl_buffer(ctrl_buffer_)
{
    //attach this layout
    setLayout(&Layout);

    Layout.addWidget(&MasterVolumeLabel,0,0,1,1);
    Layout.addWidget(&MasterVolume,1,0,2,1);
    Layout.addWidget(&MicLevel,0,1,3,1);
    Layout.addWidget(&MasterMicLabel,0,2,1,1);
    Layout.addWidget(&MasterMic,1,2,2,1);
    for(size_t k=0; k<3; k++)
    {
        Layout.setColumnMinimumWidth(k,::width);
        Layout.setColumnStretch(k,1);
        Layout.setRowMinimumHeight(k,::height);
        Layout.setRowStretch(k,3);
    }
    Layout.setRowStretch(0,1);
    //set sizing policy
    QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);


    //set the size of the radio function panel
    setFrameShadow(QFrame::Raised);
    setFrameShape(QFrame::Box);

    MasterVolume.setOrientation( Qt::Vertical);
    MasterVolume.setRange(-30,30);
    MasterVolume.setTickInterval(6);
    MasterVolume.setTickPosition(QSlider::TicksBothSides);
    auto v=ctrl_buffer->getTMM_MasterHeadsetGain();
    if (v<-30) v=-30;
    if (v>30)  v=30;
    MasterVolume.setValue(v);
    MasterVolume.setSizePolicy(sizePolicy);

    connect(&MasterVolume,SIGNAL(valueChanged(int)),this,SLOT(MasterGainChanged(int)));


    MasterVolumeLabel.setText("Master\nVolume");
    MasterVolumeLabel.setAlignment(Qt::AlignCenter);
    MasterVolumeLabel.setSizePolicy(sizePolicy);

    MasterMic.setOrientation( Qt::Vertical);
    MasterMic.setRange(-30,30);
    MasterMic.setTickInterval(6);
    MasterMic.setTickPosition(QSlider::TicksBothSides);
    v=ctrl_buffer->getTMM_MicGain();
    if (v<-30) v=-30;
    if (v>30)  v=30;
    MasterMic.setValue(v);
    MasterMic.setSizePolicy(sizePolicy);

    connect(&MasterMic,SIGNAL(valueChanged(int)),this,SLOT(MicGainChanged(int)));




    MasterMicLabel.setText("MIC\nGain");
    MasterMicLabel.setAlignment(Qt::AlignCenter);
    MasterMicLabel.setSizePolicy(sizePolicy);

    MicLevel.setSizePolicy(sizePolicy);



}
