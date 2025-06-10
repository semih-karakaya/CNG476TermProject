package buslora3.simulations;



// Sadece omnetpp.h kullanılıyor, INET yok.
network BusNetwork
{
    @display("bg=lightgreen;bgb=600,400");
    submodules:
        stop0: StopSimple;
        stop1: StopSimple {
            @display("p=461,183");
        }
        stop2: StopSimple {
            @display("p=100,132");
        }
        bus: BusSimple {
            @display("p=270,183");
        }
    connections allowunconnected:
        // stop0 <-> bus
        bus.out[0] --> stop0.in;
        stop0.out --> bus.in[0];

        // stop1 <-> bus
        bus.out[1] --> stop1.in;
        stop1.out --> bus.in[1];

        // stop2 <-> bus
        bus.out[2] --> stop2.in;
        stop2.out --> bus.in[2];
}
