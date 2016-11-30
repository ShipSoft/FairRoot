/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
/**
 * FairMQEx9aSamplerBin.h
 *
 * @since 2016-03-08
 * @author R. Karabowicz
 */

#ifndef FAIRMQEX9aSAMPLERBIN_H_
#define FAIRMQEX9aSAMPLERBIN_H_

#include <string>
#include <thread>
#include <fstream>

#include "FairMQDevice.h"

class FairMQEx9aSamplerBin : public FairMQDevice
{
  public:
    enum
    {
        InputFileName = FairMQDevice::Last,
        Last
    };

    FairMQEx9aSamplerBin();
    virtual ~FairMQEx9aSamplerBin();

    void AddInputFileName  (std::string tempString) { fFileNames  .push_back(tempString); }
    
    void SetMaxIndex(int64_t tempInt) {fMaxIndex=tempInt;}

    void ListenForAcks();

    void SetOutputChannelName(std::string tstr) {fOutputChannelName = tstr;}
    void SetAckChannelName(std::string tstr) {fAckChannelName = tstr;}

 protected:
    virtual bool ConditionalRun();
    virtual void PreRun();
    virtual void PostRun();
    virtual void InitTask();
    
 private: 
    std::string     fOutputChannelName;
    std::string     fAckChannelName;

    std::vector<std::string>     fFileNames;
    std::ifstream   fInputFile;
    int             fCurrentFile;

    int64_t         fMaxIndex;

    int             fEventCounter;

    FairMQEx9aSamplerBin(const FairMQEx9aSamplerBin&);
    FairMQEx9aSamplerBin& operator=(const FairMQEx9aSamplerBin&);

    std::thread* fAckListener;
};

#endif /* FAIRMQEX9aSAMPLER_H_ */
