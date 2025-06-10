#include "BusSimple.h"
#include <fstream>
#include <sys/stat.h>  // stat() için

using namespace omnetpp;

Define_Module(BusSimple);

// Her finish() çağrısında runCounter artacak; 0,1,2,... şekilde run numarasını tutar
static int runCounter = 0;

void BusSimple::initialize()
{
    // visitCount dizisini sıfırla
    for (int i = 0; i < 3; i++)
        visitCount[i] = 0;

    // İlk query turunu başlat
    responsesReceived = 0;
    arrivalEvent = nullptr;

    for (int i = 0; i < 3; i++) {
        std::string queryName = "query" + std::to_string(i);
        cMessage *q = new cMessage(queryName.c_str());
        EV_INFO << getFullPath() << ": sending " << queryName
                << " to stop" << i << "\n";
        send(q, "out", i);
    }
}

void BusSimple::handleMessage(cMessage *msg)
{
    // 1) Eğer self‐message ise (“arrival<i>” event’i)
    if (msg->isSelfMessage()) {
        const char *name = msg->getName();  // örn. "arrival2"
        int idx = atoi(name + 7);           // “arrival” sonrası indeks (0..2)

        // Duraktaki queueSizes[idx] kadar yolcu al
        load = queueSizes[idx];
        visitCount[idx]++;
        EV_INFO << getFullPath() << ": arrived at stop" << idx
                << ", load=" << load
                << ", visitCount[" << idx << "]=" << visitCount[idx] << "\n";

        // “service<i>” mesajını gönder (duraktaki queue’yu sıfırlasın)
        std::string serviceName = "service" + std::to_string(idx);
        cMessage *svc = new cMessage(serviceName.c_str());
        send(svc, "out", idx);

        // Yeni query turunu başlat
        responsesReceived = 0;
        delete msg;  // arrivalEvent olduğu için doğrudan sil
        arrivalEvent = nullptr;

        for (int i = 0; i < 3; i++) {
            std::string queryName = "query" + std::to_string(i);
            cMessage *q = new cMessage(queryName.c_str());
            EV_INFO << getFullPath() << ": sending " << queryName
                    << " to stop" << i << "\n";
            send(q, "out", i);
        }
    }
    // 2) Değilse “response<i>” mesajı
    else {
        const char *name = msg->getName();  // örn. "response1"
        if (strncmp(name, "response", 8) == 0) {
            int idx = atoi(name + 8);
            int qsize = (int)msg->par("queueSize").longValue();
            queueSizes[idx] = qsize;
            responsesReceived++;
            EV_INFO << getFullPath() << ": received " << name
                    << " (queueSize=" << qsize
                    << ") from stop" << idx
                    << ", responsesReceived=" << responsesReceived << "\n";

            // Tüm 3 response geldi mi?
            if (responsesReceived == 3) {
                int bestIdx = 0;
                int bestQ = queueSizes[0];
                for (int i = 1; i < 3; i++) {
                    if (queueSizes[i] > bestQ) {
                        bestQ = queueSizes[i];
                        bestIdx = i;
                    }
                }
                EV_INFO << getFullPath() << ": all responses in; bestIdx="
                        << bestIdx << " (queue=" << bestQ << ")\n";

                // “arrival<bestIdx>” self‐message’ini schedule et
                std::string arrivalName = "arrival" + std::to_string(bestIdx);
                arrivalEvent = new cMessage(arrivalName.c_str());
                scheduleAt(simTime() + travelTime, arrivalEvent);
            }
        }
        else {
            EV_WARN << getFullPath() << ": received unknown msg “"
                    << name << "” – deleting\n";
        }
        delete msg;  // response mesajını sil
    }
}

void BusSimple::finish()
{
    // Eğer hala bir arrivalEvent planlıysa, önce iptal edip sil:
    if (arrivalEvent) {
        if (arrivalEvent->isScheduled())
            cancelEvent(arrivalEvent);
        delete arrivalEvent;
        arrivalEvent = nullptr;
    }

    // Bu run'ın numarasını al
    int runnum = runCounter;
    runCounter++;

    // “summary.txt” dosyasının var olup olmadığını kontrol et
    struct stat buffer;
    bool summaryExists = (stat("summary.txt", &buffer) == 0);

    // Dosya açma modu: yoksa yeni oluşturup başlık yaz; varsa append modunda aç
    std::ofstream ofs;
    if (!summaryExists) {
        ofs.open("summary.txt", std::ios::out);
        ofs << "==== SUMMARY ====\n";
    }
    else {
        ofs.open("summary.txt", std::ios::app);
    }

    // Bu run'ın detaylarını blok formatında yaz
    ofs << "--> Run " << runnum << ":\n";
    ofs << "    Final load on bus: " << load << "\n";
    for (int i = 0; i < 3; i++) {
        ofs << "      visitCount[" << i << "] = " << visitCount[i] << "\n";
    }

    // İstersen son run için kapanış satırı ekleyebilirsin, ama toplam run sayısı bilinmediğinden
    // bu adımı atlıyoruz. Yalnızca blokları art arda ekliyoruz.

    ofs.close();

    // (Opsiyonel) Scalar kaydı
    recordScalar("finalLoad", load);
    for (int i = 0; i < 3; i++) {
        recordScalar(("visitCount" + std::to_string(i)).c_str(), visitCount[i]);
    }
}
