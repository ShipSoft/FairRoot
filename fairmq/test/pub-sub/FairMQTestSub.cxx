/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
/**
 * FairMQTestSub.cxx
 *
 * @since 2015-09-05
 * @author A. Rybalchenko
 */

#include <memory> // unique_ptr

#include "FairMQTestSub.h"
#include "FairMQLogger.h"

FairMQTestSub::FairMQTestSub()
{
}

void FairMQTestSub::Run()
{
    std::unique_ptr<FairMQMessage> ready(fTransportFactory->CreateMessage());
    int r1 = fChannels.at("control").at(0).Send(ready);
    if (r1 >= 0)
    {
        std::unique_ptr<FairMQMessage> msg(fTransportFactory->CreateMessage());
        int d1 = fChannels.at("data").at(0).Receive(msg);
        if (d1 >= 0)
        {
            std::unique_ptr<FairMQMessage> ack(fTransportFactory->CreateMessage());
            int a1 = fChannels.at("control").at(0).Send(ack);
            if (a1 < 0)
            {
                LOG(ERROR) << "Failed sending ack signal: a1 = " << a1;
            }
        }
        else
        {
            LOG(ERROR) << "Failed receiving data: d1 = " << d1;
        }
    }
    else
    {
        LOG(ERROR) << "Failed sending ready signal: r1 = " << r1;
    }
}

FairMQTestSub::~FairMQTestSub()
{
}
