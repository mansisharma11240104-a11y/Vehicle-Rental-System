#include "TwoWheeler.h"

TwoWheeler::TwoWheeler(std::string id, std::string brand, std::string model,
                       double dailyRate, double lateFinePerDay,
                       int engineCc, bool isGeared, bool helmetIncluded)
    : Vehicle(id, brand, model, VehicleType::TWO_WHEELER, dailyRate, lateFinePerDay),
      engineCc(engineCc), isGeared(isGeared), helmetIncluded(helmetIncluded) {}

std::string TwoWheeler::getCategoryName() const {
    return "2-Wheeler";
}

std::string TwoWheeler::getSpecificDetails() const {
    std::string details = std::to_string(engineCc) + "cc, ";
    details += (isGeared ? "Geared" : "Automatic / Scooter");
    details += (helmetIncluded ? ", Helmet Incl." : "");
    return details;
}
