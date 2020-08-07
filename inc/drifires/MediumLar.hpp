#ifndef GARFIELDPP_MEDIUMLAR_HPP
#define GARFIELDPP_MEDIUMLAR_HPP

#include "Garfield/Medium.hh"

// this might be sent upstream to Garfield++
namespace Garfield {

    class MediumLar : public Garfield::Medium {
      public:
        MediumLar()
        {
            m_className = "MediumLar";
            m_name = "NobelLiquid";
            SetTemperature(89.0);   // K
            SetAtomicWeight(39.95);
            SetAtomicNumber(18);
            SetMassDensity(1.3954); // g/cm3
            EnableDrift();
            EnablePrimaryIonisation();
            // SetW(...);
            // SetFanoFActor(0.117);
        }            
        virtual ~MediumLar() { }

        bool IsGas() const override { return false; }

        // given E-field in volts per cm, provide v in cm/ns
        bool ElectronVelocity(
            const double ex, const double ey, const double ez,
            const double /*bx*/, const double /*by*/, const double /*bz*/,
            double& vx, double& vy, double& vz)
        {
            const double temperature = GetTemperature(); // K
            const double Trel = temperature / 89.0;
            const double Trel32 = sqrt(Trel*Trel*Trel);

            const double a0=551.6;   // cm2/sec(/V?)
            const double a1=7953.7;  // cm2/sec/kV
            const double a2=4440.43; // cm2/sec/kV^3/2
            const double a3=4.29;    // cm2/sec/kV^5/2
            const double a4=43.63;   // cm2/sec/kV^2
            const double a5=0.2053;  // cm2/sec/kV^3

            const double emag2 = (ex*ex + ey*ey + ez*ez) / 1e6; // put into kV^2/cm^2
            const double emag = sqrt(emag2);
            const double emag3 = emag*emag2;
            const double emag5 = emag2*emag3;
            const double emag52 = sqrt(emag5);
            const double emag32 = sqrt(emag3);
        
            const double num = (a0 + a1*emag + a2*emag32 + a3*emag52);
            const double den = (1 + (a1/a0)*emag + a4*emag2 + a5*emag3);
            const double mu = Trel32*num/den; // cm2/(V*sec)


            // https://lar.bnl.gov/properties/trans.html
            // const double evpcm = emag * 1e3; // put into V/cm
            // const double vmag = mu*evpcm;
            // vx = mu*evpcm * (ex/evpcm)
            vx = -mu * ex * 1e-9;
            vy = -mu * ey * 1e-9;
            vz = -mu * ez * 1e-9;

            // const double evpcm = emag * 1e3; // put into V/cm
            // const double cmnspmmus = 10000.0;
            // const double vmmus = sqrt(vx*vx + vy*vy + vz*vz)*cmnspmmus;
            // std::cerr << "v=" << vmmus << " mm/us, E=" << evpcm << " V/cm\n";
            return true;
        }            
        bool ElectronTownsend(const double, const double, const double,
                              const double, const double, const double,
                              double& alpha) { alpha=0.0; return true; }

        bool ElectronAttachment(const double, const double, const double,
                                const double, const double, const double,
                                double& eta) { eta=0.0; return true; }

    };

} // namespace Garfield

#endif

