#ifndef __BUSLORA3_SIMULATIONS_STOPSIMPLE_H_
#define __BUSLORA3_SIMULATIONS_STOPSIMPLE_H_

#include <omnetpp.h>
using namespace omnetpp;

/**
 * StopSimple:
 *  - cSimpleModule’dan türetilir (sadece omnetpp.h).
 *  - Modül adı “stop0”, “stop1” veya “stop2” olarak gelir.
 *  - arrivalRate parametresi bu mod adı üzerinden C++ içinde sabit atanır:
 *        stop0 → λ = 5.0
 *        stop1 → λ = 3.0
 *        stop2 → λ = 4.0
 *  - queue: o duraktaki bekleyen yolcu sayısı (int).
 *  - arrivalEvent (cMessage*): bir sonraki Poisson yolcu gelişini schedule eden self‐message.
 *  - handleMessage():
 *      • Eğer gelen msg == arrivalEvent: queue++ ve yeniden arrivalEvent tetiklenir.
 *      • Eğer msg->getName()=="query":
 *            – “response” adı “response<i>” (i=0,1,2) olarak yaratılır,
 *            – msg’a parametre olarak queueSize ekler,
 *            – bus’a (send) gönderir.
 *            – (queue sıfırlanmaz; sadece bilgi raporu döner)
 *      • Eğer msg->getName()=="service":
 *            – queue = 0; (duraktaki tüm yolcular otobüse bindi)
 *            – bus’a istenen “ackService<i>” (i=0,1,2) mesajı dönüşü yapılabilir,
 *            – burada biz sadece queue sıfırlıyoruz ve msg’i siliyoruz.
 */
class StopSimple : public cSimpleModule
{
  private:
    double arrivalRate = 0.0;   // λ (yolcu/saniye), C++ içinde sabit atanır
    int queue = 0;              // Duraktaki bekleyen yolcu sayısı
    cMessage *arrivalEvent = nullptr;

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;
};

#endif // __BUSLORA3_SIMULATIONS_STOPSIMPLE_H_
