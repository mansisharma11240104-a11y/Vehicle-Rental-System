#ifndef FOUR_WHEELER_H
#define FOUR_WHEELER_H

#include "Vehicle.h"

class FourWheeler : public Vehicle {
private:
    int seatingCapacity;
    std::string transmission; // e.g. "Manual" or "Automatic"
    std::string fuelType;     // e.g. "Petrol", "Diesel", "Electric"

public:
    FourWheeler(std::string id, std::string brand, std::string model,
                double dailyRate, double lateFinePerDay,
                int seatingCapacity, std::string transmission, std::string fuelType);

    std::string getCategoryName() const override;
    std::string getSpecificDetails() const override;

    int getSeatingCapacity() const { return seatingCapacity; }
    std::string getTransmission() const { return transmission; }
    std::string getFuelType() const { return fuelType; }
};

#endif // FOUR_WHEELER_H
