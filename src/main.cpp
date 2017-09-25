#include <QPushButton>
#include <QApplication>
#include <QFrame>

#include "global_defs.h"
#include "sharedmemory.h"
#include "media_source.h"
#include "multicast_group.h"
#include "TMM_StreamCtrl.h"
#include "radio.h"
#include "shoutdown.h"
#include "domain.h"
#include "vu_meter.h"

#include <boost/program_options.hpp>
#include <iostream>
#include <functional>
#include <cstdlib>
#include "masterpanel.h"




namespace po = boost::program_options;

int main(int argc, char **argv)
{




    QApplication app (argc, argv);

    std::string supported_names;
    supported_names = "Supported identities:\n";

    for (uint8_t k=0; k< MediaSource::size(); k++)
    {
        supported_names = supported_names +  std::to_string(uint32_t(k)) + ") " + MediaSource(k).getName() + "\n";
    }




    std::string ctrl_file_name(RootSwapName);
    uint16_t identity(0);
    bool initialise_swap(false);

    po::options_description description ("MyTool Usage");
    description.add_options ()
            ("ctrl_file", po::value<std::string > (&ctrl_file_name), (std::string("file used as persistent swap \n default=")+ctrl_file_name).c_str())
            ("re-init", po::value<bool> (&initialise_swap), "reset persistent swap back to default")
            ("identity,i", po::value<uint16_t > (&identity), supported_names.c_str())
            ("help,h", "Display this help message")
            ("version,v", "Display the version number");
    po::variables_map vm;
    po::store (po::command_line_parser (argc, argv).options (description).run (), vm);
    po::notify (vm);



    if (vm.count ("help"))
    {
        std::cout << description;
        return -1;
    }

    if (vm.count ("version"))
    {
        std::cout << "MyTool Version 1.0" << std::endl;
        return -1;
    }

    if (identity>=MediaSource::size()) return -1;

    MediaSource me(identity);
    SharedMemory<TMM_CntlBuffer> ctrl_buffer(ctrl_file_name.c_str(),initialise_swap); //publish the buffer out to the world


    QFrame frame;
    QGridLayout                     Layout;
    //attach this layout
    frame.setLayout(&Layout);


    //set sizing policy
    QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);


    //set the size of the radio function panel
    frame.setFrameShadow(QFrame::Raised);
    frame.setFrameShape(QFrame::Box);

    Radio radio1(me,MediaSource("Radio 1"),MulticastGroup("Radio 1"),ctrl_buffer);
    Radio radio2(me,MediaSource("Radio 2"),MulticastGroup("Radio 2"),ctrl_buffer);
    Domain domain(ctrl_buffer);
    Shoutdown shoutdown(me,MulticastGroup("Shoutdown"),ctrl_buffer);

    //std::function<int8_t(void)> f([&](){return ctrl_buffer->getTMM_Power();}); //take a reference to ctrl_buffer
    MasterPanel masterpanel(ctrl_buffer);

    Layout.addWidget(&radio1,0,2,3,1);
    Layout.addWidget(&radio2,0,3,3,1);
    Layout.addWidget(&masterpanel,0,1,4,1);
    Layout.addWidget(&shoutdown,3,2,1,2);
    Layout.addWidget(&domain,0,0,4,1);
    for(size_t k=0; k<4; k++)
    {
        Layout.setColumnMinimumWidth(k,3*::width);
        Layout.setColumnStretch(k,1);
        Layout.setRowMinimumHeight(k,3*::height);
        Layout.setRowStretch(k,1);
    }







    frame.show();

    return app.exec();
}
