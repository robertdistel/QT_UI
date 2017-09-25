#include "domain.h"
#include "global_defs.h"
#include "sharedmemory.h"

#include <stdlib.h>
#include <QString>

const uint8_t Domain1_Mask = 0;
const uint8_t Domain2_Mask = 1;
const uint8_t Domain3_Mask = 2;

Domain::Domain(SharedMemory<TMM_CntlBuffer>& ctrl_buffer_): ctrl_buffer(ctrl_buffer_)
{
    //attach this layout
    setLayout(&Layout);

    Layout.addWidget(&Domain1,0,0,1,1);
    Layout.addWidget(&Domain2,1,0,1,1);
    Layout.addWidget(&Domain3,2,0,1,1);
    Layout.setColumnMinimumWidth(0,::width);
    Layout.setColumnStretch(0,1);
    for(size_t k=0; k<3; k++)
    {
        Layout.setRowMinimumHeight(k,::height);
        Layout.setRowStretch(k,1);
    }
    //set sizing policy
    QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);


    Domain1.setSizePolicy(sizePolicy);
    Domain2.setSizePolicy(sizePolicy);
    Domain3.setSizePolicy(sizePolicy);

    Domain1.setCheckable(true);
    Domain2.setCheckable(true);
    Domain3.setCheckable(true);
    Domain1.setChecked( !(ctrl_buffer->domain_mask[ Domain1_Mask]));
    Domain2.setChecked( !(ctrl_buffer->domain_mask [Domain2_Mask]));
    Domain3.setChecked( !(ctrl_buffer->domain_mask  [Domain3_Mask]));

    Domain1.setText("plaintext");
    Domain2.setText("restricted");
    Domain3.setText("classified");

    setFrameShadow(QFrame::Raised);
    setFrameShape(QFrame::Box);

    connect(&Domain1,SIGNAL(toggled(bool)),this,SLOT(Domain_changed()));
    connect(&Domain2,SIGNAL(toggled(bool)),this,SLOT(Domain_changed()));
    connect(&Domain3,SIGNAL(toggled(bool)),this,SLOT(Domain_changed()));
    Domain_changed();
}

Domain::~Domain()
{

}



void Domain::Domain_changed()
{
    QPalette  palette;

    uint32_t state=0;

    if (!Domain1.isChecked())
    {
        ctrl_buffer->domain_mask[Domain1_Mask]=true;
        palette.setColor(QPalette::ButtonText,Qt::black);    //i dont care about it
    }
    else
    {
        ctrl_buffer->domain_mask[Domain1_Mask]=false;
        palette.setColor(QPalette::ButtonText,Qt::darkGray);    //i dont care about it
    }

    Domain1.setPalette(palette);


    if (!Domain2.isChecked())
    {
        ctrl_buffer->domain_mask[Domain2_Mask]=true;
        palette.setColor(QPalette::ButtonText,Qt::black);    //i dont care about it
    }
    else
    {
        ctrl_buffer->domain_mask[Domain2_Mask]=false;
        palette.setColor(QPalette::ButtonText,Qt::darkGray);    //i dont care about it
    }

    Domain2.setPalette(palette);

    if (!Domain3.isChecked())
    {
        ctrl_buffer->domain_mask[Domain3_Mask]=true;
        state = state | Domain3_Mask;
        palette.setColor(QPalette::ButtonText,Qt::black);    //i dont care about it
    }
    else
    {
        ctrl_buffer->domain_mask[Domain3_Mask]=false;
        palette.setColor(QPalette::ButtonText,Qt::darkGray);    //i dont care about it
    }

    Domain3.setPalette(palette);
}
