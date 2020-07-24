#include "Garfield/Medium.hh"

namespace drifires {

    class MediumLar : public Garfield::Medium {
      public:
        MediumLar() ;
        virtual ~MediumLar();

        bool IsGas() const override { return false; }

        // given E-field in volts per cm, provide v in cm/ns
        bool ElectronVelocity(
            const double ex, const double ey, const double ez,
            const double /*bx*/, const double /*by*/, const double /*bz*/,
            double& vx, double& vy, double& vz);
        bool ElectronTownsend(const double, const double, const double,
                              const double, const double, const double,
                              double& alpha);
        bool ElectronAttachment(const double, const double, const double,
                                const double, const double, const double,
                                double& eta);
    };

}
