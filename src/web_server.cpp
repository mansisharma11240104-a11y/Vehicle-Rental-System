#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <memory>
#include <iomanip>
#include <algorithm>
#include <winsock2.h>
#include <ws2tcpip.h>

#include "Vehicle.h"
#include "TwoWheeler.h"
#include "FourWheeler.h"
#include "Transaction.h"

#pragma comment(lib, "ws2_32.lib")

// Global Data Structures matching the assignment requirements
std::unordered_map<std::string, std::shared_ptr<Vehicle>> g_vehicleRegistry;
std::vector<std::string> g_vehicleKeys;
std::queue<std::string> g_cleaningQueue;
std::vector<Transaction> g_transactionHistory;
int g_txnSequence = 1001;

void initSampleData() {
    auto tw1 = std::make_shared<TwoWheeler>("TW101", "Honda", "Activa 6G", 350.0, 100.0, 110, false, true);
    auto tw2 = std::make_shared<TwoWheeler>("TW102", "Royal Enfield", "Classic 350", 900.0, 300.0, 350, true, true);
    auto tw3 = std::make_shared<TwoWheeler>("TW103", "Yamaha", "R15 V4", 750.0, 250.0, 155, true, true);
    auto tw4 = std::make_shared<TwoWheeler>("TW104", "TVS", "Jupiter 125", 380.0, 120.0, 125, false, true);

    auto fw1 = std::make_shared<FourWheeler>("FW201", "Maruti", "Swift Dzire", 1600.0, 500.0, 5, "Manual", "Petrol");
    auto fw2 = std::make_shared<FourWheeler>("FW202", "Hyundai", "Creta SX", 2500.0, 800.0, 5, "Automatic", "Diesel");
    auto fw3 = std::make_shared<FourWheeler>("FW203", "Mahindra", "Thar 4x4", 3200.0, 1000.0, 4, "Manual", "Diesel");
    auto fw4 = std::make_shared<FourWheeler>("FW204", "Toyota", "Innova Crysta", 3800.0, 1200.0, 7, "Automatic", "Diesel");

    tw2->rentOut("Rahul Sharma", "9876543210", 3);
    fw3->rentOut("Priya Patel", "9123456789", 2);
    tw4->markAsCleaning();

    auto addV = [](const std::shared_ptr<Vehicle>& v) {
        g_vehicleRegistry[v->getId()] = v;
        g_vehicleKeys.push_back(v->getId());
    };

    addV(tw1); addV(tw2); addV(tw3); addV(tw4);
    addV(fw1); addV(fw2); addV(fw3); addV(fw4);

    g_cleaningQueue.push(tw4->getId());

    Transaction t0;
    t0.transactionId = "TXN-1000";
    t0.vehicleId = "FW201";
    t0.vehicleName = "Maruti Swift Dzire";
    t0.category = "4-Wheeler";
    t0.customerName = "Amit Kumar";
    t0.customerPhone = "9988776655";
    t0.bookedDays = 2;
    t0.actualDays = 3;
    t0.overdueDays = 1;
    t0.dailyRate = 1600.0;
    t0.baseFare = 3200.0;
    t0.lateFineRate = 500.0;
    t0.totalFine = 500.0;
    t0.totalBill = 3700.0;
    g_transactionHistory.push_back(t0);
}

// JSON Extraction Helpers
std::string extractJsonString(const std::string& json, const std::string& key) {
    std::string pattern = "\"" + key + "\"";
    size_t pos = json.find(pattern);
    if (pos == std::string::npos) return "";
    pos = json.find(':', pos);
    if (pos == std::string::npos) return "";
    pos = json.find('"', pos);
    if (pos == std::string::npos) return "";
    size_t end = json.find('"', pos + 1);
    if (end == std::string::npos) return "";
    return json.substr(pos + 1, end - pos - 1);
}

int extractJsonInt(const std::string& json, const std::string& key) {
    std::string pattern = "\"" + key + "\"";
    size_t pos = json.find(pattern);
    if (pos == std::string::npos) return 0;
    pos = json.find(':', pos);
    if (pos == std::string::npos) return 0;
    size_t start = json.find_first_of("0123456789-", pos);
    if (start == std::string::npos) return 0;
    size_t end = json.find_first_not_of("0123456789-", start);
    std::string valStr = (end == std::string::npos) ? json.substr(start) : json.substr(start, end - start);
    return std::stoi(valStr);
}

double extractJsonDouble(const std::string& json, const std::string& key) {
    std::string pattern = "\"" + key + "\"";
    size_t pos = json.find(pattern);
    if (pos == std::string::npos) return 0.0;
    pos = json.find(':', pos);
    if (pos == std::string::npos) return 0.0;
    size_t start = json.find_first_of("0123456789-.", pos);
    if (start == std::string::npos) return 0.0;
    size_t end = json.find_first_not_of("0123456789-.", start);
    std::string valStr = (end == std::string::npos) ? json.substr(start) : json.substr(start, end - start);
    return std::stod(valStr);
}

std::string readFileContent(const std::string& filepath) {
    std::ifstream file(filepath.c_str(), std::ios::in | std::ios::binary);
    if (!file.is_open()) return "";
    std::ostringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

void sendHttpResponse(SOCKET client, int statusCode, const std::string& contentType, const std::string& body) {
    std::ostringstream response;
    std::string statusText = (statusCode == 200) ? "OK" :
                             (statusCode == 404) ? "Not Found" : "Internal Server Error";

    response << "HTTP/1.1 " << statusCode << " " << statusText << "\r\n";
    response << "Content-Type: " << contentType << "\r\n";
    response << "Content-Length: " << body.length() << "\r\n";
    response << "Access-Control-Allow-Origin: *\r\n";
    response << "Access-Control-Allow-Methods: GET, POST, OPTIONS\r\n";
    response << "Access-Control-Allow-Headers: Content-Type\r\n";
    response << "Connection: close\r\n\r\n";
    response << body;

    std::string resStr = response.str();
    send(client, resStr.c_str(), (int)resStr.length(), 0);
}

int main() {
    std::cout << "========================================================\n";
    std::cout << "   Wheels & Ways - Vehicle Rental Web Server (C++)\n";
    std::cout << "========================================================\n";

    initSampleData();

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed.\n";
        return 1;
    }

    SOCKET serverSock = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSock == INVALID_SOCKET) {
        std::cerr << "Socket creation failed.\n";
        WSACleanup();
        return 1;
    }

    int opt = 1;
    setsockopt(serverSock, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));

    int port = 8080;
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(serverSock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed on port " << port << "!\n";
        closesocket(serverSock);
        WSACleanup();
        return 1;
    }

    if (listen(serverSock, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listen failed.\n";
        closesocket(serverSock);
        WSACleanup();
        return 1;
    }

    std::cout << " [*] Server successfully listening at http://localhost:" << port << "/\n";
    std::cout << " [*] Serving Web Dashboard and REST APIs on localhost\n";
    std::cout << " [*] Press Ctrl+C in this terminal window to stop.\n\n";

    while (true) {
        SOCKET clientSock = accept(serverSock, NULL, NULL);
        if (clientSock == INVALID_SOCKET) continue;

        char buffer[8192];
        int bytesRead = recv(clientSock, buffer, sizeof(buffer) - 1, 0);
        if (bytesRead <= 0) {
            closesocket(clientSock);
            continue;
        }
        buffer[bytesRead] = '\0';
        std::string request(buffer, bytesRead);

        std::istringstream reqStream(request);
        std::string method, path, httpVersion;
        reqStream >> method >> path >> httpVersion;

        // Extract body if POST
        std::string body = "";
        size_t headerEnd = request.find("\r\n\r\n");
        if (headerEnd != std::string::npos) {
            body = request.substr(headerEnd + 4);
        }

        // Check if Content-Length requires reading more data
        size_t clPos = request.find("Content-Length: ");
        if (clPos == std::string::npos) clPos = request.find("content-length: ");
        if (clPos != std::string::npos) {
            size_t clStart = request.find_first_of("0123456789", clPos);
            if (clStart != std::string::npos) {
                size_t clEnd = request.find("\r\n", clStart);
                int expectedLen = std::stoi(request.substr(clStart, clEnd - clStart));
                while ((int)body.length() < expectedLen) {
                    int extra = recv(clientSock, buffer, sizeof(buffer) - 1, 0);
                    if (extra <= 0) break;
                    body.append(buffer, extra);
                }
            }
        }

        if (method == "OPTIONS") {
            sendHttpResponse(clientSock, 200, "text/plain", "OK");
        }
        else if (method == "GET" && (path == "/" || path == "/index.html")) {
            std::string html = readFileContent("web/index.html");
            if (html.empty()) {
                html = readFileContent("C:\\Users\\mansi\\.gemini\\antigravity\\scratch\\vehicle_rental_system\\web\\index.html");
            }
            if (!html.empty()) {
                sendHttpResponse(clientSock, 200, "text/html; charset=utf-8", html);
            } else {
                sendHttpResponse(clientSock, 404, "text/plain", "web/index.html not found");
            }
        }
        else if (method == "GET" && path == "/api/vehicles") {
            std::ostringstream json;
            json << "[\n";
            bool first = true;
            for (const auto& key : g_vehicleKeys) {
                auto it = g_vehicleRegistry.find(key);
                if (it == g_vehicleRegistry.end()) continue;
                auto v = it->second;

                if (!first) json << ",\n";
                first = false;

                json << "  {\n";
                json << "    \"id\": \"" << v->getId() << "\",\n";
                json << "    \"brand\": \"" << v->getBrand() << "\",\n";
                json << "    \"model\": \"" << v->getModel() << "\",\n";
                json << "    \"type\": \"" << (v->getType() == VehicleType::TWO_WHEELER ? "TWO_WHEELER" : "FOUR_WHEELER") << "\",\n";
                json << "    \"category\": \"" << v->getCategoryName() << "\",\n";
                json << "    \"specs\": \"" << v->getSpecificDetails() << "\",\n";
                json << "    \"dailyRate\": " << v->getDailyRate() << ",\n";
                json << "    \"lateFine\": " << v->getLateFinePerDay() << ",\n";
                json << "    \"status\": \"" << v->getStatusString() << "\",\n";
                json << "    \"customerName\": \"" << v->getCustomerName() << "\",\n";
                json << "    \"customerPhone\": \"" << v->getCustomerPhone() << "\",\n";
                json << "    \"rentedForDays\": " << v->getRentedForDays() << "\n";
                json << "  }";
            }
            json << "\n]";
            sendHttpResponse(clientSock, 200, "application/json", json.str());
        }
        else if (method == "GET" && path == "/api/stats") {
            int total = (int)g_vehicleKeys.size();
            int available = 0, rented = 0, cleaning = 0;
            for (const auto& key : g_vehicleKeys) {
                auto v = g_vehicleRegistry[key];
                if (v->getStatus() == VehicleStatus::AVAILABLE) available++;
                else if (v->getStatus() == VehicleStatus::RENTED) rented++;
                else if (v->getStatus() == VehicleStatus::IN_CLEANING) cleaning++;
            }

            double totalRevenue = 0.0;
            double totalFines = 0.0;
            for (const auto& t : g_transactionHistory) {
                totalRevenue += t.totalBill;
                totalFines += t.totalFine;
            }

            std::ostringstream json;
            json << "{\n";
            json << "  \"total\": " << total << ",\n";
            json << "  \"available\": " << available << ",\n";
            json << "  \"rented\": " << rented << ",\n";
            json << "  \"cleaning\": " << cleaning << ",\n";
            json << "  \"revenue\": " << std::fixed << std::setprecision(2) << totalRevenue << ",\n";
            json << "  \"fines\": " << totalFines << "\n";
            json << "}";
            sendHttpResponse(clientSock, 200, "application/json", json.str());
        }
        else if (method == "GET" && path == "/api/cleaning-bay") {
            std::ostringstream json;
            json << "[\n";
            std::queue<std::string> tempQ = g_cleaningQueue;
            bool first = true;
            while (!tempQ.empty()) {
                std::string vid = tempQ.front();
                tempQ.pop();
                auto it = g_vehicleRegistry.find(vid);
                if (it == g_vehicleRegistry.end()) continue;

                if (!first) json << ",\n";
                first = false;

                json << "  {\n";
                json << "    \"id\": \"" << vid << "\",\n";
                json << "    \"name\": \"" << it->second->getFullName() << "\",\n";
                json << "    \"category\": \"" << it->second->getCategoryName() << "\",\n";
                json << "    \"specs\": \"" << it->second->getSpecificDetails() << "\"\n";
                json << "  }";
            }
            json << "\n]";
            sendHttpResponse(clientSock, 200, "application/json", json.str());
        }
        else if (method == "GET" && path == "/api/history") {
            std::ostringstream json;
            json << "[\n";
            bool first = true;
            for (auto it = g_transactionHistory.rbegin(); it != g_transactionHistory.rend(); ++it) {
                if (!first) json << ",\n";
                first = false;

                json << "  {\n";
                json << "    \"transactionId\": \"" << it->transactionId << "\",\n";
                json << "    \"vehicleId\": \"" << it->vehicleId << "\",\n";
                json << "    \"vehicleName\": \"" << it->vehicleName << "\",\n";
                json << "    \"category\": \"" << it->category << "\",\n";
                json << "    \"customerName\": \"" << it->customerName << "\",\n";
                json << "    \"customerPhone\": \"" << it->customerPhone << "\",\n";
                json << "    \"bookedDays\": " << it->bookedDays << ",\n";
                json << "    \"actualDays\": " << it->actualDays << ",\n";
                json << "    \"overdueDays\": " << it->overdueDays << ",\n";
                json << "    \"dailyRate\": " << it->dailyRate << ",\n";
                json << "    \"baseFare\": " << it->baseFare << ",\n";
                json << "    \"lateFineRate\": " << it->lateFineRate << ",\n";
                json << "    \"totalFine\": " << it->totalFine << ",\n";
                json << "    \"totalBill\": " << it->totalBill << "\n";
                json << "  }";
            }
            json << "\n]";
            sendHttpResponse(clientSock, 200, "application/json", json.str());
        }
        else if (method == "POST" && path == "/api/rent") {
            std::string vid = extractJsonString(body, "vehicleId");
            std::string name = extractJsonString(body, "customerName");
            std::string phone = extractJsonString(body, "customerPhone");
            int days = extractJsonInt(body, "days");

            auto it = g_vehicleRegistry.find(vid);
            if (it != g_vehicleRegistry.end() && it->second->getStatus() == VehicleStatus::AVAILABLE && days > 0) {
                it->second->rentOut(name, phone, days);
                sendHttpResponse(clientSock, 200, "application/json", "{\"success\":true,\"message\":\"Vehicle rented successfully\"}");
            } else {
                sendHttpResponse(clientSock, 400, "application/json", "{\"success\":false,\"message\":\"Vehicle not available or invalid days\"}");
            }
        }
        else if (method == "POST" && path == "/api/return") {
            std::string vid = extractJsonString(body, "vehicleId");
            int actualDays = extractJsonInt(body, "actualDays");
            if (actualDays <= 0) actualDays = 1;

            auto it = g_vehicleRegistry.find(vid);
            if (it != g_vehicleRegistry.end() && it->second->getStatus() == VehicleStatus::RENTED) {
                auto v = it->second;
                int bookedDays = v->getRentedForDays();
                int overdueDays = std::max(0, actualDays - bookedDays);
                double baseFare = bookedDays * v->getDailyRate();
                double fine = overdueDays * v->getLateFinePerDay();
                double totalBill = baseFare + fine;

                Transaction txn;
                txn.transactionId = "TXN-" + std::to_string(g_txnSequence++);
                txn.vehicleId = v->getId();
                txn.vehicleName = v->getFullName();
                txn.category = v->getCategoryName();
                txn.customerName = v->getCustomerName();
                txn.customerPhone = v->getCustomerPhone();
                txn.bookedDays = bookedDays;
                txn.actualDays = actualDays;
                txn.overdueDays = overdueDays;
                txn.dailyRate = v->getDailyRate();
                txn.baseFare = baseFare;
                txn.lateFineRate = v->getLateFinePerDay();
                txn.totalFine = fine;
                txn.totalBill = totalBill;
                g_transactionHistory.push_back(txn);

                // Enqueue to cleaning bay (Queue DS)
                v->markAsCleaning();
                g_cleaningQueue.push(v->getId());

                std::ostringstream resJson;
                resJson << "{\n";
                resJson << "  \"success\": true,\n";
                resJson << "  \"invoice\": {\n";
                resJson << "    \"transactionId\": \"" << txn.transactionId << "\",\n";
                resJson << "    \"vehicleId\": \"" << txn.vehicleId << "\",\n";
                resJson << "    \"vehicleName\": \"" << txn.vehicleName << "\",\n";
                resJson << "    \"category\": \"" << txn.category << "\",\n";
                resJson << "    \"customerName\": \"" << txn.customerName << "\",\n";
                resJson << "    \"customerPhone\": \"" << txn.customerPhone << "\",\n";
                resJson << "    \"bookedDays\": " << txn.bookedDays << ",\n";
                resJson << "    \"actualDays\": " << txn.actualDays << ",\n";
                resJson << "    \"overdueDays\": " << txn.overdueDays << ",\n";
                resJson << "    \"dailyRate\": " << txn.dailyRate << ",\n";
                resJson << "    \"baseFare\": " << txn.baseFare << ",\n";
                resJson << "    \"totalFine\": " << txn.totalFine << ",\n";
                resJson << "    \"totalBill\": " << txn.totalBill << "\n";
                resJson << "  }\n";
                resJson << "}";
                sendHttpResponse(clientSock, 200, "application/json", resJson.str());
            } else {
                sendHttpResponse(clientSock, 400, "application/json", "{\"success\":false,\"message\":\"Vehicle is not currently rented\"}");
            }
        }
        else if (method == "POST" && path == "/api/clean-next") {
            if (!g_cleaningQueue.empty()) {
                std::string vid = g_cleaningQueue.front();
                g_cleaningQueue.pop();
                auto it = g_vehicleRegistry.find(vid);
                if (it != g_vehicleRegistry.end()) {
                    it->second->markAsAvailable();
                    std::string msg = "Vehicle " + vid + " (" + it->second->getFullName() + ") cleaned and marked AVAILABLE!";
                    sendHttpResponse(clientSock, 200, "application/json", "{\"success\":true,\"message\":\"" + msg + "\"}");
                }
            } else {
                sendHttpResponse(clientSock, 200, "application/json", "{\"success\":false,\"message\":\"Cleaning queue is already empty\"}");
            }
        }
        else if (method == "POST" && path == "/api/clean-all") {
            int count = 0;
            while (!g_cleaningQueue.empty()) {
                std::string vid = g_cleaningQueue.front();
                g_cleaningQueue.pop();
                auto it = g_vehicleRegistry.find(vid);
                if (it != g_vehicleRegistry.end()) {
                    it->second->markAsAvailable();
                    count++;
                }
            }
            std::string msg = "All " + std::to_string(count) + " vehicle(s) cleaned and restored to AVAILABLE!";
            sendHttpResponse(clientSock, 200, "application/json", "{\"success\":true,\"message\":\"" + msg + "\"}");
        }
        else if (method == "POST" && path == "/api/add-vehicle") {
            std::string cat = extractJsonString(body, "category");
            std::string id = extractJsonString(body, "id");
            std::string brand = extractJsonString(body, "brand");
            std::string model = extractJsonString(body, "model");
            double dailyRate = extractJsonDouble(body, "dailyRate");
            double lateFine = extractJsonDouble(body, "lateFine");

            if (g_vehicleRegistry.find(id) != g_vehicleRegistry.end()) {
                sendHttpResponse(clientSock, 400, "application/json", "{\"success\":false,\"message\":\"Vehicle ID already exists\"}");
            } else {
                if (cat == "TWO_WHEELER") {
                    int cc = extractJsonInt(body, "cc");
                    bool geared = (body.find("\"geared\":true") != std::string::npos || extractJsonInt(body, "geared") == 1);
                    auto v = std::make_shared<TwoWheeler>(id, brand, model, dailyRate, lateFine, cc, geared, true);
                    g_vehicleRegistry[id] = v;
                    g_vehicleKeys.push_back(id);
                } else {
                    int seats = extractJsonInt(body, "seats");
                    std::string transmission = extractJsonString(body, "transmission");
                    std::string fuel = extractJsonString(body, "fuelType");
                    auto v = std::make_shared<FourWheeler>(id, brand, model, dailyRate, lateFine, seats, transmission, fuel);
                    g_vehicleRegistry[id] = v;
                    g_vehicleKeys.push_back(id);
                }
                sendHttpResponse(clientSock, 200, "application/json", "{\"success\":true,\"message\":\"Vehicle added to fleet successfully!\"}");
            }
        }
        else {
            sendHttpResponse(clientSock, 404, "text/plain", "Not Found");
        }

        closesocket(clientSock);
    }

    closesocket(serverSock);
    WSACleanup();
    return 0;
}
