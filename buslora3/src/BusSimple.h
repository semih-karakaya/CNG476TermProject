#ifndef __BUSLORA3_SIMULATIONS_BUSSIMPLE_H_
#define __BUSLORA3_SIMULATIONS_BUSSIMPLE_H_

#include <omnetpp.h>
#include <vector>
using namespace omnetpp;

/**
 * BusSimple:
 *  - cSimpleModule’dan türetilir.
 *  - 3 durak var (stop0, stop1, stop2).
 *  - travelTime: durağa gitme süresi (sabit: 2s).
 *  - load: otobüsteki yolcu sayısı.
 *  - visitCount[3]: her durak kaç kez ziyaret edildi.
 *  - responsesReceived: o query turunda kaç tane response geldi (max 3).
 *  - queueSizes[3]: her duraktan gelen son queueSize değerleri.
 *  - arrivalEvent: hangi durağa varılacağını simüle eden self‐message; ad “arrival<i>”.
 *  - handleMessage():
 *      • msg->isSelfMessage() → “arrival<i>” event’i:
 *           – load = queueSizes[i]
 *           – visitCount[i]++
 *           – “service<i>” mesajı ile o duraktaki queue=0 yapılır
 *           – Yeni query turuna başla (responsesReceived=0, query0/1/2 gönder)
 *      • msg->getName()=="response<i>" → duraktan gelen queueSize:
 *           – queueSizes[i] = msg->par("queueSize")
 *           – responsesReceived++
 *           – Eğer responsesReceived==3 → en büyük queueSize’lı i’i bul,
 *               scheduleAt(simTime()+travelTime, new cMessage("arrival<i>"))
 */
class BusSimple : public cSimpleModule
{
  private:
    double travelTime = 2.0; // sabit: 2 saniye
    int load = 0;            // otobüsteki yolcu sayısı
    int visitCount[3];       // her durak kaç kez ziyaret edildi
    int responsesReceived;   // o query turunda kaç response alındı
    int queueSizes[3];       // her duraktan gelen queueSize
    cMessage *arrivalEvent;  // “arrival<i>” self‐message; hangi durağa gidileceğini simüle eder

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;
};

#endif // __BUSLORA3_SIMULATIONS_BUSSIMPLE_H_
