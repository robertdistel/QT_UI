#ifndef MASTERPANEL_H
#define MASTERPANEL_H

#include <QFrame>
#include <QSlider>
#include "media_source.h"
#include "multicast_group.h"
#include "sharedmemory.h"
#include "TMM_StreamCtrl.h"
#include "vu_meter.h"
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>

class MasterPanel : public QFrame
{
    Q_OBJECT

public:
    MasterPanel(SharedMemory<TMM_CntlBuffer>& ctrl_buffer_);
public slots:

    void MicGainChanged(int value);
    void MasterGainChanged(int value);

private:
    QGridLayout                     Layout;
    QSlider                         MasterVolume;
    QLabel                          MasterVolumeLabel;
    QSlider                         MasterMic;
    QLabel                          MasterMicLabel;
    VU_Meter                        MicLevel;

    QTimer                          StatusTimer;
    SharedMemory<TMM_CntlBuffer>&   ctrl_buffer;

};

#endif // MASTERPANEL_H
