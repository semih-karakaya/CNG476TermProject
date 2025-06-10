package buslora3.simulations;


simple StopSimple
{
    parameters:
        @class(StopSimple);  // C++ tarafındaki sınıf ismi
        @display("i=misc/building"); 
    gates:
        input in;   // bus → stopX (query veya service)
        output out; // stopX → bus (response)
}
