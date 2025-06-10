package buslora3.simulations;


simple BusSimple
{
    parameters:
        @class(BusSimple);  // C++ tarafındaki sınıf ismi
        @display("i=misc/car"); 
    gates:
        input  in[3];   // stop0, stop1, stop2’den gelen
        output out[3];  // stop0, stop1, stop2’ye giden
}
