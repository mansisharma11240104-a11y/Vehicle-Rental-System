#include "Vehicle.h"

Vehicle::Vehicle(std::string id, std::string brand, std::string model, VehicleType type,
                 double dailyRate, double lateFinePerDay)
    : id(id), brand(brand), model(model), type(type),
      status(VehicleStatus::AVAILABLE), dailyRate(dailyRate),
      lateFinePerDay(lateFinePerDay), currentCustomerName(""),
      currentCustomerPhone(""), rentedForDays(0) {}

std::string Vehicle::getId() const { return id; }
std::string Vehicle::getBrand() const { return brand; }
std::string Vehicle::getModel() const { return model; }
std::string Vehicle::getFullName() const { return brand + " " + model; }
VehicleType Vehicle::getType() const { return type; }
VehicleStatus Vehicle::getStatus() const { return status; }

std::string Vehicle::getStatusString() const {
    switch (status) {
        case VehicleStatus::AVAILABLE: return "AVAILABLE";
        case VehicleStatus::RENTED: return "RENTED";
        case VehicleStatus::IN_CLEANING: return "IN CLEANING";
        default: return "UNKNOWN";
    }
}

double Vehicle::getDailyRate() const { return dailyRate; }
double Vehicle::getLateFinePerDay() const { return lateFinePerDay; }
std::string Vehicle::getCustomerName() const { return currentCustomerName; }
std::string Vehicle::getCustomerPhone() const { return currentCustomerPhone; }
int Vehicle::getRentedForDays() const { return rentedForDays; }

void Vehicle::rentOut(const std::string& customerName, const std::string& phone, int days) {
    status = VehicleStatus::RENTED;
    currentCustomerName = customerName;
    currentCustomerPhone = phone;
    rentedForDays = days;
}

void Vehicle::markAsCleaning() {
    status = VehicleStatus::IN_CLEANING;
    currentCustomerName = "";
    currentCustomerPhone = "";
    rentedForDays = 0;
}

void Vehicle::markAsAvailable() {
    status = VehicleStatus::AVAILABLE;
    currentCustomerName = "";
    currentCustomerPhone = "";
    rentedForDays = 0;
}

void Vehicle::printCard() const {
    std::cout << "--------------------------------------------------\n";
    std::cout << " Vehicle ID   : " << id << "\n";
    std::cout << " Name         : " << getFullName() << "\n";
    std::cout << " Category     : " << getCategoryName() << "\n";
    std::cout << " Specifications: " << getSpecificDetails() << "\n";
    std::cout << " Daily Rate   : Rs. " << std::fixed << std::setprecision(2) << dailyRate << "/day\n";
    std::cout << " Late Fine    : Rs. " << lateFinePerDay << "/day\n";
    std::cout << " Status       : " << getStatusString() << "\n";
    if (status == VehicleStatus::RENTED) {
        std::cout << " Rented To    : " << currentCustomerName << " (" << currentCustomerPhone << ")\n";
        std::cout << " Rented For   : " << rentedForDays << " day(s)\n";
    }
    std::cout << "--------------------------------------------------\n";
}
