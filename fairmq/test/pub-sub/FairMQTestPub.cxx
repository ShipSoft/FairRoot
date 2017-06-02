/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
/**
 * FairMQTestPub.cpp
 *
 * @since 2015-09-05
 * @author A. Rybalchenko
 */

#include <memory> // unique_ptr

#include "FairMQTestPub.h"
#include "FairMQLogger.h"

FairMQTestPub::FairMQTestPub()
{
}

void FairMQTestPub::Run()
{
    std::unique_ptr<FairMQMessage> ready1Msg(fTransportFactory->CreateMessage());
    int r1 = fChannels.at("control").at(0).Receive(ready1Msg);
    std::unique_ptr<FairMQMessage> ready2Msg(fTransportFactory->CreateMessage());
    int r2 = fChannels.at("control").at(0).Receive(ready2Msg);

    if (r1 >= 0 && r2 >= 0)
    {
        LOG(INFO) << "Received both ready signals, proceeding to publish data";

        std::unique_ptr<FairMQMessage> msg(fTransportFactory->CreateMessage());
        int d1 = fChannels.at("data").at(0).Send(msg);
        if (d1 < 0)
        {
            LOG(ERROR) << "Failed sending data: d1 = " << d1;
        }

        std::unique_ptr<FairMQMessage> ack1(fTransportFactory->CreateMessage());
        std::unique_ptr<FairMQMessage> ack2(fTransportFactory->CreateMessage());
        int a1 = fChannels.at("control").at(0).Receive(ack1);
        int a2 = fChannels.at("control").at(0).Receive(ack2);
        if (a1 >= 0 && a2 >= 0)
        {
            LOG(INFO) << "PUB-SUB test successfull";
        }
        else
        {
            LOG(ERROR) << "Failed receiving ack signal: a1 = " << a1 << ", a2 = " << a2;
        }
    }
}

FairMQTestPub::~FairMQTestPub()
{
}
