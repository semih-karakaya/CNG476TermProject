#include "StopNodeSimple.h"

Define_Module(StopSimple);

void StopSimple::initialize()
{
    // Mod adından durak indeksini al (stop0, stop1, stop2)
    std::string modname = getName();
    if (modname == "stop0")
        arrivalRate = 5.0;
    else if (modname == "stop1")
        arrivalRate = 3.0;
    else if (modname == "stop2")
        arrivalRate = 4.0;
    else {
        EV_WARN << getFullPath() << ": beklenmeyen mod adı “"
                << modname << "”, default arrivalRate=1.0\n";
        arrivalRate = 1.0;
    }

    // İlk arrivalEvent’i schedule et (exponential(1/arrivalRate) kadar zaman sonra)
    arrivalEvent = new cMessage("arrivalEvent");
    simtime_t first = exponential(1.0 / arrivalRate);
    scheduleAt(simTime() + first, arrivalEvent);

    EV_INFO << getFullPath() << ": arrivalRate=" << arrivalRate
            << ", first arrival at " << first << "\n";
}

void StopSimple::handleMessage(cMessage *msg)
{
    if (msg == arrivalEvent) {
        // Poisson yolcu geliş event’i: queue++ ve bir sonraki arrivalEvent’i schedule et
        queue++;
        EV_INFO << getFullPath() << ": new passenger arrived, queue="
                << queue << "\n";

        // Bir sonraki arrivalEvent’ı schedule et
        simtime_t next = simTime() + exponential(1.0 / arrivalRate);
        scheduleAt(next, arrivalEvent);
        // NOT: Burada delete msg yapmıyoruz, çünkü arrivalEvent’i bir sonraki tur için yeniden kullandık.
    }
    else {
        const char *name = msg->getName();

        // “query<i>” geldiyse:
        if (strncmp(name, "query", 5) == 0) {
            int idx = atoi(name + 5);  // “query2” → idx = 2
            std::string responseName = std::string("response") + std::to_string(idx);
            cMessage *reply = new cMessage(responseName.c_str());
            reply->addPar("queueSize").setLongValue(queue);
            EV_INFO << getFullPath() << ": received " << name
                    << ", replying queueSize=" << queue << "\n";
            send(reply, "out");
        }
        // “service<i>” geldiyse:
        else if (strncmp(name, "service", 7) == 0) {
            EV_INFO << getFullPath() << ": received " << name
                    << ", resetting queue from " << queue << " to 0\n";
            queue = 0;
            // Burada bir ackService göndermiyoruz; sadece queue=0 yapıyor
        }
        else {
            EV_WARN << getFullPath() << ": received unknown msg “"
                    << name << "” – deleting\n";
        }
        delete msg;  // query veya service mesajını güvenle siliyoruz.
    }
}

void StopSimple::finish()
{
    // arrivalEvent hâlâ scheduled ise iptal et ve sil:
    if (arrivalEvent->isScheduled())
        cancelAndDelete(arrivalEvent);
    else
        delete arrivalEvent;
    recordScalar("finalQueueSize", queue);
}
