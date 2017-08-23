#ifndef GENEXPART1SINK_H
#define GENEXPART1SINK_H

// std
#include <iostream>
#include <memory>

// FairRoot
#include "FairMQDevice.h"
#include "SerializerExample.h"

// root
#include "TFile.h"
#include "TTree.h"

#include "MyHit.h"

class Ex1Sink : public FairMQDevice
{
  public:
    Ex1Sink() :
        FairMQDevice(),
        fInput(nullptr),
        fFileName(),
        fOutFile(nullptr),
        fTree(nullptr),
        fNumMsgs(0)
    {}

    Ex1Sink(const Ex1Sink&);
    Ex1Sink& operator=(const Ex1Sink&);

    virtual ~Ex1Sink()
    {
        if (fTree)
        {
            fTree->Write("", TObject::kOverwrite);
            delete fTree;
        }

        if (fOutFile)
        {
            if (fOutFile->IsOpen())
            {
                fOutFile->Close();
            }
            delete fOutFile;
        }
    }

  protected:
    virtual void Init()
    {
        fNumMsgs = fConfig->GetValue<int>("num-msgs");
        fFileName = fConfig->GetValue<std::string>("output-file");
        fOutFile = TFile::Open(fFileName.c_str(),"RECREATE");
        fInput = new TClonesArray("MyHit");
        fTree = new TTree("SerializationEx1", "Test output");
        fTree->Branch("MyHit","TClonesArray", &fInput);
    }

    virtual void Run()
    {
        int receivedMsgs = 0;
        while (CheckCurrentState(RUNNING))
        {
            std::unique_ptr<FairMQMessage> msg(NewMessage());
            if (Receive(msg, "data-in") > 0)
            {
                Deserialize<MyDeserializer>(*msg,fInput);
                receivedMsgs++;
                fTree->SetBranchAddress("MyHit", &fInput);
                fTree->Fill();

                if (fNumMsgs != 0)
                {
                    if (receivedMsgs == fNumMsgs)
                    {
                        break;
                    }
                }
            }
        }
        LOG(INFO) << "Received " << receivedMsgs << " messages!";
    }

  private:
    TClonesArray* fInput;
    std::string fFileName;
    TFile* fOutFile;
    TTree* fTree;
    int fNumMsgs;
};

#endif