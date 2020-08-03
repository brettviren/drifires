// A subset of WCT units.  These are NOT the same as Garfield (in
// general).  ALL numbers accessible to users MUST be expressed in WCT
// system of units.  drifiers will convert between WCT and Garfield
// when needed.  MULTIPLY a bare number by a unit.
//
// local units = import "drifiers/units.jsonnet";
// local my_pitch = 5.0*units.mm;
//
{
    mm: 1.0,
    cm : 10*self.mm,
    ns: 1.0,
    us: 1e3*self.ns,
    eplus: 1.0,
    MeV: 1.0,
    megavolt: self.MeV/self.eplus,
    volt: 1e-6*self.megavolt, 
    K: 1.0,
}
