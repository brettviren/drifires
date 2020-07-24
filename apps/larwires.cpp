#include "Garfield/ComponentAnalyticField.hh"
#include "Garfield/MediumMagboltz.hh"
#include "Garfield/Sensor.hh"
#include "Garfield/ViewMedium.hh"
#include "Garfield/ViewField.hh"
#include "Garfield/ViewCell.hh"
#include "Garfield/Plotting.hh"
#include "Garfield/DriftLineRKF.hh"
#include "Garfield/ViewSignal.hh"

#include <TCanvas.h>
#include <TROOT.h>
#include <TApplication.h>
#include <TSystem.h>

#include <iostream>
#include <vector>

using namespace Garfield;

//using MediumLar = class MediumMagboltz;
class MediumLar : public Medium {
  public:
    MediumLar() : Medium() {
        m_className = "MediumLar";
        m_name = "NobelLiquid";
        SetTemperature(89.0);   // K
        SetAtomicWeight(39.95);
        SetAtomicNumber(18);
        SetMassDensity(1.3954); // g/cm3
        EnableDrift();
        EnablePrimaryIonisation();
        // SetW(...);
        // SetFanoFActor(...);
    }
    virtual ~MediumLar() {}


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
                          double& alpha) {alpha=0.0; return true;}
    bool ElectronAttachment(const double, const double, const double,
                            const double, const double, const double,
                            double& eta) {eta=0.0; return true;}
};

void wires(ComponentAnalyticField* cmp,
           double& xmin, double& xmax, double& ymin, double& ymax)
{
    const double gap = 0.5;
    const double pitch = 0.5;
    const double wdia = 0.015;
    const double vpcm = 500;
    const double respy = 10;
    
    const int nwires = 21;
    const double wire0 = -pitch * (nwires/2);

    xmin = wire0;
    xmax = -wire0;
    ymax = 10;
    ymin = -2*gap;

    struct PlaneInfo {
        std::string name;
        double potential;
        double location;
        bool iswire{true};
    };
    const std::vector<PlaneInfo> planeinfo {
        {"c", -vpcm*respy, respy, false},// cathode, 10cm
        {"g", -665,  2*gap},            // grid, 1.0cm
        {"u", -370,  1*gap},            // u induction, 0.5cm
        {"v",    0,  0*gap},            // v induction, 0.0cm
        {"w",  820, -1*gap},            // w collection, -0.5cm
        {"m",    0, -2*gap, false}      // mesh, -1.0cm
    };

    cmp->SetPeriodicityX(nwires*pitch);

    const std::string letters = "0abcdefghijklmnopqrstuvwxyz";

    for (const auto& pi : planeinfo) {
        if (pi.iswire) {
            for (int iwire=0; iwire < nwires; ++iwire) {
                const int irel = -nwires/2 + iwire;
                const double wx = wire0 + iwire * pitch;

                std::string wname = pi.name;
                if (irel < 0) {
                    wname += "-";
                }
                else if (irel > 0) {
                    wname += "+";
                }
                else {
                    wname += "0";
                }
                wname += letters[std::abs(irel)];
                cmp->AddWire(wx, pi.location, wdia, pi.potential, wname);
                std::cout << "add wire " << wname << std::endl;
                cmp->AddReadout(wname);
            }
        }
        else {
            cmp->AddPlaneY(pi.location, pi.potential, pi.name);
        }
    }

}

int main(int argc, char * argv[])
{

    TApplication app("app", &argc, argv);
    plottingEngine.SetDefaultStyle();

    //using MyMedium = class MediumMagboltz;
    using MyMedium = class MediumLar;

    MyMedium lar;
    //lar.SetComposition("ar", 100);

    ViewMedium mediumView;
    mediumView.SetMedium(&lar);
    mediumView.EnableAutoRangeX(true);
    mediumView.EnableAutoRangeY(true);
    ComponentAnalyticField cmp;
    cmp.SetMedium(&lar);
    double xmin = 0., xmax = 0.;
    double ymin = 0., ymax = 0.;
    wires(&cmp, xmin, xmax, ymin, ymax);

    Sensor sensor;
    sensor.AddComponent(&cmp);
    const std::string letters = "abcdefghij";
    for (std::string p : {"u","v","w"}) {
        sensor.AddElectrode(&cmp, p+"00");
        for (int ind = 0; ind<10; ++ind) {
            std::string l = " ";
            l[0] = letters[ind];
            sensor.AddElectrode(&cmp, p+"+"+l);
            sensor.AddElectrode(&cmp, p+"-"+l);
        }
    }
    sensor.SetTimeWindow(0, 100, 1000);
    sensor.SetArea(xmin, ymin, xmax, ymax, -50,50);
    sensor.EnableComponent(0, true);

    DriftLineRKF drift;
    drift.SetSensor(&sensor);
    drift.EnableSignalCalculation();
    drift.SetIntegrationAccuracy(0.000001); // cm
    drift.SetMaximumStepSize(0.1); // cm
    drift.RejectKinks(true);
    
    ViewDrift driftView;
    drift.EnablePlotting(&driftView);
    driftView.SetArea(xmin,ymin,xmax,ymax);

    const std::string drift_type = "one";
    if (drift_type == "one") {
        // x,y,z,t
        drift.DriftElectron(0.15, ymax, 0.0, 0.0);
        auto npts = drift.GetNumberOfDriftLinePoints();
        std::cout << "drifted " << npts << std::endl;
    }
    else if (drift_type == "many") {
        for (double xdrift = 0.0; xdrift <= 0.5; xdrift += 0.01) {
            const double ydrift = 2.0;
            drift.DriftElectron(xdrift, ydrift, 0.0, 0.0);
            auto npts = drift.GetNumberOfDriftLinePoints();
            drift.DriftElectron(-xdrift, ydrift, 0.0, 0.0);
            npts = drift.GetNumberOfDriftLinePoints();
            std::cout << "drifted " << npts << " from x=" << xdrift << std::endl;
        }
    }
        

    //double sig = sensor.GetElectronSignal(const std::string label, const int bin);

    TCanvas canvas("c", "", 600, 600);
    canvas.Print("larwires.pdf[","pdf");


    ViewCell cellView;
    cellView.SetCanvas(&canvas);
    cellView.SetComponent(&cmp);
    cellView.DisableWireMarkers();
    ViewField fieldView;
    fieldView.SetCanvas(&canvas);
    fieldView.SetSensor(&sensor);

    ViewSignal sigView;
    sigView.SetCanvas(&canvas);
    sigView.SetSensor(&sensor);

    canvas.Clear();
    mediumView.SetCanvas(&canvas);
    mediumView.PlotElectronVelocity('e');
    canvas.Print("larwires.pdf","pdf");    
    canvas.SetLogx(false);

    //cmp.PrintCell();

    for (std::string p : {"u","v","w"}) {
        canvas.Clear();
        std::string name = p+"00";
        sigView.PlotSignal(name);
        canvas.SetTitle(name.c_str());
        canvas.Print("larwires.pdf","pdf");

        for (int ind = 0; ind<10; ++ind) {
            std::string l = " ";
            l[0] = letters[ind];

            name = p+"+"+l;
            canvas.Clear();
            sigView.PlotSignal(name);
            canvas.SetTitle(name.c_str());
            canvas.Print("larwires.pdf","pdf");

            name = p+"-"+l;
            canvas.Clear();
            sigView.PlotSignal(name);
            canvas.SetTitle(name.c_str());
            canvas.Print("larwires.pdf","pdf");
        }
    }


    // canvas.Clear();
    // fieldView.SetArea(-0.6, -0.6, 0.6, 1.5);
    // fieldView.Plot("v", "COLZ");
    // canvas.Print("larwires.pdf","pdf");

    // canvas.Clear();
    // fieldView.SetArea(-0.6, -0.6, 0.6, 1.5);
    // fieldView.Plot("e", "COLZ");
    // canvas.Print("larwires.pdf","pdf");

    // fieldView.SetArea(xmin, ymin, xmax, ymax);
    // fieldView.Plot("v", "COLZ");
    // canvas.Print("larwires.pdf","pdf");
    // canvas.Clear();

    canvas.Clear();
    driftView.SetArea(-0.6, -0.6, 0.6, 1.5);
    cellView.SetArea(-0.6, -0.6, 0.6, 1.5);
    driftView.SetCanvas(&canvas);
    driftView.Plot(true);
    cellView.Plot2d();
    canvas.Print("larwires.pdf","pdf");    

    canvas.Clear();
    driftView.SetArea(xmin,ymin,xmax,ymax);
    cellView.SetArea(xmin,ymin,xmax,ymax);
    driftView.SetCanvas(&canvas);
    driftView.Plot(true);
    cellView.Plot2d();
    canvas.Print("larwires.pdf","pdf");    
    canvas.Clear();

    // fieldView.PlotContour();
    // cellView.Plot2d();
    // canvas.Print("larwires.pdf","pdf");
    // canvas.Clear();



    // cellView.SetArea(-1, ymin, 1.0, 2.0);
    // fieldView.SetArea(-1, ymin, 1.0, 2.0);

    // canvas.Clear();
    // fieldView.PlotContourWeightingField("u00","v");
    // cellView.Plot2d();
    // canvas.Print("larwires.pdf","pdf");

    // canvas.Clear();
    // fieldView.PlotContourWeightingField("v00","v");
    // cellView.Plot2d();
    // canvas.Print("larwires.pdf","pdf");

    // canvas.Clear();
    // fieldView.PlotContourWeightingField("w00","v");
    // cellView.Plot2d();
    // canvas.Print("larwires.pdf","pdf");


    canvas.Print("larwires.pdf]","pdf");

    //app.Run(kTRUE);

    return 0;
}
