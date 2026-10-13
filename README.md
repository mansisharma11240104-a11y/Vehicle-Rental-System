# Wheels & Ways - Vehicle Rental System (C++)

A comprehensive terminal and localhost web-based Vehicle Rental System written in modern C++ (C++14) implementing Object-Oriented Programming (OOP) and 3 fundamental Data Structures.

---

## 🌐 Localhost Web Application

The system includes a native C++ HTTP web server (`web_server.exe`) serving a modern, interactive dashboard UI directly on **`http://localhost:8080/`**.

### Features on Localhost:
- 📊 **Real-time KPI Cards**: Total Fleet, Available vehicles, Rented vehicles, Cleaning queue count, and Gross Revenue with fines.
- 🛵 **2-Wheelers & 🚗 4-Wheelers Tabs**: Filter fleet by category.
- 🔑 **Interactive Renting**: Select vehicle, input customer info, view live daily cost preview.
- 🔄 **Smart Return & Late Fine Calculator**: Select rented vehicle, enter actual days used, and automatically calculate overdue days, base fare, late fine, and print a formatted invoice.
- 🧼 **Live FIFO Cleaning Bay Queue**: Visualizes vehicles waiting in line for washing and sanitization with "Clean Next" and "Clean All" buttons.
- 📜 **Historical Rental Ledger**: Shows all completed bookings, dates, invoices, and collected fines.
- ➕ **Fleet Expansion Modal**: Add new 2-wheelers and 4-wheelers on the fly.

---

## 🧠 Data Structures Used

| Data Structure | Implementation | Purpose & Time Complexity |
| :--- | :--- | :--- |
| **1. Hash Map** | `std::unordered_map<string, shared_ptr<Vehicle>>` | **$O(1)$ Average Lookup**: Fast retrieval, status update, and searching of vehicles using their unique ID (e.g., `TW101`, `FW201`). |
| **2. FIFO Queue** | `std::queue<string>` | **$O(1)$ First-In-First-Out Processing**: Models the realistic Cleaning & Inspection Bay line. Vehicles returned from rental enter the back of the queue (`push`) and are washed/restocked in arrival order (`pop`). |
| **3. Dynamic Array** | `std::vector<Transaction>` | **$O(1)$ Amortized Append**: Acts as the immutable audit log and historical rental transaction ledger. Used to calculate total earnings, fines, and print invoice history. |

---

## 📁 Project Structure

```
vehicle_rental_system/
├── include/
│   ├── Vehicle.h         # Base Vehicle class and enums
│   ├── TwoWheeler.h      # Derived 2-Wheeler class
│   ├── FourWheeler.h     # Derived 4-Wheeler class
│   ├── Transaction.h     # Receipt & invoice data structure
│   └── RentalSystem.h    # Core system controller & DS management
├── src/
│   ├── Vehicle.cpp       # Base vehicle implementation
│   ├── TwoWheeler.cpp    # 2-Wheeler specifics
│   ├── FourWheeler.cpp   # 4-Wheeler specifics
│   ├── RentalSystem.cpp  # System operations, rates, and queue pipeline
│   ├── web_server.cpp    # Native C++ HTTP Web Server & REST API
│   └── main.cpp          # Interactive terminal UI
├── web/
│   └── index.html        # Responsive web dashboard UI
├── build.bat             # 1-Click build script for both CLI and Web Server
├── run.bat               # 1-Click runner for terminal CLI
├── start_web.bat         # 1-Click runner for localhost web server (opens browser)
├── standalone_vehicle_rental.cpp # Single-file version for lab/online submissions
└── README.md             # Documentation
```

---

## 🛠️ How to Run

### 1. Run on Localhost Web Browser
- Double-click [`start_web.bat`](file:///C:/Users/mansi/.gemini/antigravity/scratch/vehicle_rental_system/start_web.bat) or run:
  ```powershell
  .\web_server.exe
  ```
- Open your browser at: **`http://localhost:8080/`**

### 2. Run the Terminal Version
- Double-click [`run.bat`](file:///C:/Users/mansi/.gemini/antigravity/scratch/vehicle_rental_system/run.bat) or run:
  ```powershell
  .\vehicle_rental.exe
  ```
