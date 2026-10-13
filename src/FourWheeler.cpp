#include "FourWheeler.h"

FourWheeler::FourWheeler(std::string id, std::string brand, std::string model,
                         double dailyRate, double lateFinePerDay,
                         int seatingCapacity, std::string transmission, std::string fuelType)
    : Vehicle(id, brand, model, VehicleType::FOUR_WHEELER, dailyRate, lateFinePerDay),
      seatingCapacity(seatingCapacity), transmission(transmission), fuelType(fuelType) {}

std::string FourWheeler::getCategoryName() const {
    return "4-Wheeler";
}

std::string FourWheeler::getSpecificDetails() const {
    return std::to_string(seatingCapacity) + " Seater, " + transmission + ", " + fuelType;
}
