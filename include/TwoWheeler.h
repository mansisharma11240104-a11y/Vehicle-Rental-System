#ifndef TWO_WHEELER_H
#define TWO_WHEELER_H

#include "Vehicle.h"

class TwoWheeler : public Vehicle {
private:
    int engineCc;
    bool isGeared;
    bool helmetIncluded;

public:
    TwoWheeler(std::string id, std::string brand, std::string model,
               double dailyRate, double lateFinePerDay,
               int engineCc, bool isGeared, bool helmetIncluded = true);

    std::string getCategoryName() const override;
    std::string getSpecificDetails() const override;

    int getEngineCc() const { return engineCc; }
    bool getIsGeared() const { return isGeared; }
    bool getHelmetIncluded() const { return helmetIncluded; }
};

#endif // TWO_WHEELER_H
