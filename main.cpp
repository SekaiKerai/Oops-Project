// NOT FINAL
// Must Make changes to the enter to continue part. Press enter only once not twice
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <stdexcept>
#include <limits>
#include <algorithm>
#include <cctype>
#include <climits>
#include <cfloat>
#include <iomanip>  
#include <limits>   
using namespace std;

// Custom exception classes
class RatingException : public exception {
private:
    string message;
public:
    RatingException(const string& msg) : message(msg) {}
    const char* what() const noexcept override {
        return message.c_str();
    }
};

class InputException : public exception {
private:
    string message;
public:
    InputException(const string& msg) : message(msg) {}
    const char* what() const noexcept override {
        return message.c_str();
    }
};

// Data structures
struct Location {
    int id;
    string name;
    double lat;
    double lon;
};

struct RentalVehicle {
    string type;
    string model;
    string color;
    string licensePlate;
    string ownerName;
    string contact;
    double pricePerDay;
};

struct Driver {
    string name;
    string contact;
    double rating;
    double lat;
    double lon;
    string vehicleType; 
};

struct User {
    string name;
    string phone;
    string aadhaar;
    string email;
};

// Helper functions for input validation
template<typename T>
T getValidInput(const string& prompt, T minVal, T maxVal) {
    T value;
    while (true) {
        cout << prompt;
        if (cin >> value && value >= minVal && value <= maxVal) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        cout << "Invalid input. Please enter a value between " << minVal << " and " << maxVal << ".\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

string getValidString(const string& prompt, bool (*validator)(const string&) = nullptr, bool allowBlank = false) {
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);
        
        if (input.empty() && !allowBlank) {
            cout << "Input cannot be empty. Please try again.\n";
            continue;
        }
        
        if (validator && !input.empty() && !validator(input)) {
            continue;
        }
        
        return input;
    }
}

bool validatePhone(const string& phone) {
    if (phone.length() != 10 || phone.find_first_not_of("0123456789") != string::npos) {
        cout << "Invalid phone number. Please enter exactly 10 digits.\n";
        return false;
    }
    return true;
}

bool validateAadhaar(const string& aadhaar) {
    if (aadhaar.length() != 12 || aadhaar.find_first_not_of("0123456789") != string::npos) {
        cout << "Invalid Aadhaar number. Please enter exactly 12 digits.\n";
        return false;
    }
    return true;
}

bool validateEmail(const string& email) {
    if (email.find('@') == string::npos || email.find('.') == string::npos) {
        cout << "Invalid email format. Please include '@' and '.' in your email.\n";
        return false;
    }
    return true;
}

bool getConfirmation(const string& prompt) {
    char response;
    while (true) {
        cout << prompt << " (y/n): ";
        cin >> response;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        response = tolower(response);
        
        if (response == 'y') return true;
        if (response == 'n') return false;
        
        cout << "Please enter 'y' for yes or 'n' for no.\n";
    }
}

// Core functions
double calculateDistance(double lat1, double lon1, double lat2, double lon2) {
    const double R = 6371.0;
    double dLat = (lat2 - lat1) * M_PI / 180.0;
    double dLon = (lon2 - lon1) * M_PI / 180.0;
    double a = sin(dLat / 2) * sin(dLat / 2) + cos(lat1 * M_PI / 180.0) * cos(lat2 * M_PI / 180.0) * sin(dLon / 2) * sin(dLon / 2);
    return R * 2 * atan2(sqrt(a), sqrt(1 - a));
}

vector<Location> loadLocations() {
    vector<Location> locations;
    ifstream file("Locations.csv");
    if (!file.is_open()) {
        throw runtime_error("Could not open Locations.csv");
    }

    string line;
    while (getline(file, line)) {
        try {
            stringstream ss(line);
            Location loc;
            string token;
            
            getline(ss, token, ',');
            loc.id = stoi(token);
            
            getline(ss, token, ',');
            loc.name = token;
            
            getline(ss, token, ',');
            loc.lat = stod(token);
            
            getline(ss, token, ',');
            loc.lon = stod(token);
            
            locations.push_back(loc);
        } catch (...) {
            cerr << "Warning: Skipping malformed location entry\n";
        }
    }
    return locations;
}

vector<RentalVehicle> loadRentalVehicles() {
    vector<RentalVehicle> vehicles;
    ifstream file("Vehicles.csv");
    if (!file.is_open()) {
        throw runtime_error("Could not open Vehicles.csv");
    }

    // Skip header line
    string line;
    getline(file, line);

    while (getline(file, line)) {
        try {
            stringstream ss(line);
            RentalVehicle vehicle;
            string token;
            
            getline(ss, token, ',');
            vehicle.type = token;
            
            getline(ss, token, ',');
            vehicle.model = token;
            
            getline(ss, token, ',');
            vehicle.licensePlate = token;
            
            getline(ss, token, ',');
            vehicle.ownerName = token;
            
            getline(ss, token, ',');
            vehicle.contact = token;
            
            getline(ss, token, ',');
            vehicle.pricePerDay = stod(token);
            
            vehicles.push_back(vehicle);
        } catch (...) {
            cerr << "Warning: Skipping malformed vehicle entry\n";
        }
    }
    return vehicles;
}

vector<Driver> loadDrivers() {
    vector<Driver> drivers;
    ifstream file("Drivers.csv");
    if (!file.is_open()) {
        throw runtime_error("Could not open Drivers.csv");
    }

    string line;
    while (getline(file, line)) {
        try {
            stringstream ss(line);
            Driver drv;
            string token;
            
            getline(ss, token, ',');
            drv.name = token;
            
            getline(ss, token, ',');
            drv.contact = token;
            
            getline(ss, token, ',');
            drv.rating = stod(token);
            if (drv.rating < 0 || drv.rating > 5) {
                drv.rating = 4.0; // Default if invalid
            }
            
            getline(ss, token, ',');
            drv.lat = stod(token);
            
            getline(ss, token, ',');
            drv.lon = stod(token);
            
            getline(ss, token, ',');
            drv.vehicleType = token;
            
            drivers.push_back(drv);
        } catch (...) {
            cerr << "Warning: Skipping malformed driver entry\n";
        }
    }
    return drivers;
}

User loadUserProfile() {
    User user;
    ifstream file("Users.csv");
    if (!file.is_open()) {
        return user;
    }

    string line;
    if (getline(file, line)) {
        try {
            stringstream ss(line);
            string token;
            
            getline(ss, token, ',');
            user.name = token;
            
            getline(ss, token, ',');
            user.phone = token;
            
            getline(ss, token, ',');
            user.aadhaar = token;
            
            getline(ss, token, ',');
            user.email = token;
        } catch (...) {
            cerr << "Warning: User profile data may be incomplete\n";
        }
    }
    return user;
}

void saveUserProfile(const User& user) {
    ofstream file("Users.csv");
    if (!file.is_open()) {
        throw runtime_error("Could not save user profile");
    }
    file << user.name << "," << user.phone << "," << user.aadhaar << "," << user.email;
}

void saveHistory(const string& type, const string& driverName, const string& vehicleType, 
                double distanceOrDays, double price, const string& startLocation, const string& endLocation) {
    string filename = (type == "BOOKING") ? "BookingHistory.csv" : "RentalHistory.csv";
    ofstream file(filename, ios::app);
    if (!file.is_open()) {
        throw runtime_error("Could not save history");
    }
    
    time_t now = time(0);
    tm *ltm = localtime(&now);
    file << ltm->tm_year + 1900 << "-" << ltm->tm_mon + 1 << "-" << ltm->tm_mday << " ";
    file << ltm->tm_hour << ":" << ltm->tm_min << ":" << ltm->tm_sec << ","; 
    file << driverName << "," << vehicleType << "," << fixed << setprecision(2) 
         << distanceOrDays << "," << price << "," << startLocation << "," << endLocation << endl;
}

// Feature functions
void bookCab(const vector<Location>& locations, const vector<Driver>& drivers) {
    try {
        cout << "\n";
        cout << "+-------------------------------------+\n";
        cout << "|        AVAILABLE LOCATIONS         |\n";
        cout << "+-----+------------------------------+\n";
        cout << "| ID  | Location Name               |\n";
        cout << "+-----+------------------------------+\n";
        for (const auto& loc : locations) {
            cout << "| " << setw(3) << loc.id << " | " << setw(28) << left << loc.name << " |\n";
        }
        cout << "+-----+------------------------------+\n";

        int startId = getValidInput("\nEnter start location ID: ", 1, static_cast<int>(locations.size()));
        int endId = getValidInput("Enter end location ID: ", 1, static_cast<int>(locations.size()));

        const Location* startLoc = nullptr;
        const Location* endLoc = nullptr;
        
        for (const auto& loc : locations) {
            if (loc.id == startId) startLoc = &loc;
            if (loc.id == endId) endLoc = &loc;
        }

        if (!startLoc || !endLoc) {
            throw runtime_error("Invalid location selection");
        }

        double distance = calculateDistance(startLoc->lat, startLoc->lon, endLoc->lat, endLoc->lon);
        cout << "\nDistance: " << fixed << setprecision(2) << distance << " km\n";

        vector<int> availableTuktuks;
        vector<int> availableTaxis;
        
        for (int i = 0; i < drivers.size(); ++i) {
            double driverDistance = calculateDistance(startLoc->lat, startLoc->lon, 
                                                   drivers[i].lat, drivers[i].lon);
            if (driverDistance <= 1.0) {
                if (drivers[i].vehicleType == "Tuktuk") {
                    availableTuktuks.push_back(i);
                } else if (drivers[i].vehicleType == "Taxi") {
                    availableTaxis.push_back(i);
                }
            }
        }

        if (availableTuktuks.empty() && availableTaxis.empty()) {
            cout << "\nSorry, no drivers available near your location. Please try again later.\n";
            return;
        }

        if (!availableTuktuks.empty()) {
            cout << "\n";
            cout << "+-------------------------------------------------------------+\n";
            cout << "|                    AVAILABLE TUKTUKS                       |\n";
            cout << "+-----+---------------------+------------+--------+----------+\n";
            cout << "| ID  | Driver Name        | Contact    | Rating | Distance |\n";
            cout << "+-----+---------------------+------------+--------+----------+\n";
            for (int i = 0; i < availableTuktuks.size(); ++i) {
                int idx = availableTuktuks[i];
                double driverDistance = calculateDistance(startLoc->lat, startLoc->lon, 
                                                       drivers[idx].lat, drivers[idx].lon);
                cout << "| T" << setw(3) << left << i+1 << " | " << setw(19) << drivers[idx].name
                     << " | " << setw(10) << drivers[idx].contact
                     << " | " << setw(6) << fixed << setprecision(1) << drivers[idx].rating
                     << " | " << setw(8) << fixed << setprecision(2) << driverDistance << " |\n";
            }
            cout << "+-----+---------------------+------------+--------+----------+\n";
        }

        if (!availableTaxis.empty()) {
            cout << "\n";
            cout << "+-------------------------------------------------------------+\n";
            cout << "|                     AVAILABLE TAXIS                       |\n";
            cout << "+-----+---------------------+------------+--------+----------+\n";
            cout << "| ID  | Driver Name        | Contact    | Rating | Distance |\n";
            cout << "+-----+---------------------+------------+--------+----------+\n";
            for (int i = 0; i < availableTaxis.size(); ++i) {
                int idx = availableTaxis[i];
                double driverDistance = calculateDistance(startLoc->lat, startLoc->lon, 
                                                       drivers[idx].lat, drivers[idx].lon);
                cout << "| X" << setw(3) << left << i+1 << " | " << setw(19) << drivers[idx].name
                     << " | " << setw(10) << drivers[idx].contact
                     << " | " << setw(6) << fixed << setprecision(1) << drivers[idx].rating
                     << " | " << setw(8) << fixed << setprecision(2) << driverDistance << " |\n";
            }
            cout << "+-----+---------------------+------------+--------+----------+\n";
        }

        string choice;
        while (true) {
            cout << "\nEnter driver code (T1-T" << availableTuktuks.size() 
                 << " for Tuktuks, X1-X" << availableTaxis.size() << " for Taxis): ";
            cin >> choice;
            cin.ignore();

            if (choice.empty()) continue;

            char typeChar = toupper(choice[0]);
            if (typeChar != 'T' && typeChar != 'X') {
                cout << "Please enter T for Tuktuks or X for Taxis.\n";
                continue;
            }

            try {
                int num = stoi(choice.substr(1));
                if ((typeChar == 'T' && num >= 1 && num <= availableTuktuks.size()) ||
                    (typeChar == 'X' && num >= 1 && num <= availableTaxis.size())) {
                    break;
                }
                cout << "Invalid number. Please try again.\n";
            } catch (...) {
                cout << "Invalid format. Please try again.\n";
            }
        }

        char typeChar = toupper(choice[0]);
        int choiceNum = stoi(choice.substr(1)) - 1;
        int driverIdx = (typeChar == 'T') ? availableTuktuks[choiceNum] : availableTaxis[choiceNum];
        const Driver& selectedDriver = drivers[driverIdx];

        bool isTuktuk = (selectedDriver.vehicleType == "Tuktuk");
        double price = isTuktuk ? 
            (20.0 + max(0.0, distance - 0.5) * 20.0) : 
            (100.0 + max(0.0, distance - 1.0) * 70.0);

        cout << "\n";
        cout << "+-----------------------------------------------+\n";
        cout << "|              BOOKING SUMMARY                 |\n";
        cout << "+-----------------------------------------------+\n";
        cout << "| From: " << setw(38) << left << startLoc->name << " |\n";
        cout << "| To: " << setw(40) << left << endLoc->name << " |\n";
        cout << "| Distance: " << setw(34) << left << string(to_string(distance) + " km") << " |\n";
        cout << "| Driver: " << setw(36) << left << (selectedDriver.name + " (" + selectedDriver.vehicleType + ")") << " |\n";
        cout << "| Contact: " << setw(35) << left << selectedDriver.contact << " |\n";
        cout << "| Estimated Price: " << setw(27) << left << string("Rs." + to_string(price)) << " |\n";
        cout << "+-----------------------------------------------+\n";

        if (getConfirmation("\nConfirm booking?")) {
            double rating = getValidInput("Rate your experience (0-5): ", 0.0, 5.0);
            
            // Update driver rating (average with previous rating)
            const_cast<Driver&>(selectedDriver).rating = 
                min(5.0, max(0.0, (selectedDriver.rating + rating) / 2.0));

            saveHistory("BOOKING", selectedDriver.name, selectedDriver.vehicleType, 
                       distance, price, startLoc->name, endLoc->name);
            
            cout << "\n";
            cout << "+-----------------------------------------------+\n";
            cout << "|          BOOKING CONFIRMED!                 |\n";
            cout << "+-----------------------------------------------+\n";
            cout << "| Your " << setw(39) << left << selectedDriver.vehicleType + " will arrive shortly." << " |\n";
            cout << "| Driver contact: " << setw(28) << left << selectedDriver.contact << " |\n";
            cout << "+-----------------------------------------------+\n";
        } else {
            cout << "\nBooking cancelled.\n";
        }
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }
    
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void rentCab(const vector<RentalVehicle>& vehicles) {
    try {
        // Get vehicle type from user
        cout << "\n";
        cout << "+-------------------------------------+\n";
        cout << "|        VEHICLE TYPE OPTIONS       |\n";
        cout << "+-------------------------------------+\n";
        cout << "| 1. Motorcycle                     |\n";
        cout << "| 2. Hatchback                      |\n";
        cout << "| 3. Sedan                          |\n";
        cout << "| 4. SUV                            |\n";
        cout << "+-------------------------------------+\n";
        
        int typeChoice = getValidInput("Select vehicle type (1-4): ", 1, 4);
        
        string vehicleType;
        switch(typeChoice) {
            case 1: vehicleType = "Motorcycle"; break;
            case 2: vehicleType = "Hatchback"; break;
            case 3: vehicleType = "Sedan"; break;
            case 4: vehicleType = "SUV"; break;
        }
        
        // Filter vehicles by type
        vector<RentalVehicle> availableVehicles;
        for (const auto& vehicle : vehicles) {
            if (vehicle.type == vehicleType) {
                availableVehicles.push_back(vehicle);
            }
        }
        
        if (availableVehicles.empty()) {
            cout << "\nNo vehicles available for this type. Please try another type.\n";
            return;
        }
        
        // Display available vehicles
        cout << "\n";
        cout << "+------------------------------------------------------------------------------------------------+\n";
        cout << "|                        AVAILABLE " << setw(20) << left << vehicleType + "S" << "                       |\n";
        cout << "+-----+---------------------------------+-----------+----------------+------------+-------------+\n";
        cout << "| ID  | Model (Color)                  | Plate     | Owner          | Contact    | Price/Day   |\n";
        cout << "+-----+---------------------------------+-----------+----------------+------------+-------------+\n";
        
        for (int i = 0; i < availableVehicles.size(); ++i) {
            const auto& vehicle = availableVehicles[i];
            cout << "| " << setw(3) << left << i+1 << " | " 
                 << setw(31) << left << vehicle.model << " | "
                 << setw(9) << left << vehicle.licensePlate << " | "
                 << setw(14) << left << vehicle.ownerName << " | "
                 << setw(10) << left << vehicle.contact << " | "
                 << "Rs." << setw(8) << left << fixed << setprecision(2) << vehicle.pricePerDay << " |\n";
        }
        cout << "+-----+---------------------------------+-----------+----------------+------------+-------------+\n";
        
        // Get vehicle selection
        int vehicleChoice = getValidInput<int>("\nSelect vehicle (1-" + to_string(availableVehicles.size()) + "): ", 
                                            1, 
                                            static_cast<int>(availableVehicles.size()));
        const RentalVehicle& selectedVehicle = availableVehicles[vehicleChoice-1];
        
        // Get rental duration
        int days = getValidInput("Enter number of days (1-30): ", 1, 30);
        double totalPrice = selectedVehicle.pricePerDay * days;
        
        // Show rental summary
        cout << "\n";
        cout << "+-----------------------------------------------+\n";
        cout << "|              RENTAL SUMMARY                  |\n";
        cout << "+-----------------------------------------------+\n";
        cout << "| Vehicle Type: " << setw(30) << left << selectedVehicle.type << " |\n";
        cout << "| Model: " << setw(37) << left << selectedVehicle.model << " |\n";
        cout << "| License Plate: " << setw(29) << left << selectedVehicle.licensePlate << " |\n";
        cout << "| Owner Name: " << setw(32) << left << selectedVehicle.ownerName << " |\n";
        cout << "| Owner Contact: " << setw(29) << left << selectedVehicle.contact << " |\n";
        cout << "| Duration: " << setw(34) << left << string(to_string(days) + " days") << " |\n";
        cout << "| Price/Day: " << setw(33) << left << string("Rs." + to_string(selectedVehicle.pricePerDay)) << " |\n";
        cout << "| Total Price: " << setw(31) << left << string("Rs." + to_string(totalPrice)) << " |\n";
        cout << "+-----------------------------------------------+\n";
        
        if (getConfirmation("\nConfirm rental?")) {
            saveHistory("RENTAL", "RENTAL", selectedVehicle.type + " - " + selectedVehicle.model, 
                       days, totalPrice, "N/A", "N/A");
            cout << "\n";
            cout << "+-----------------------------------------------+\n";
            cout << "|          RENTAL CONFIRMED!                  |\n";
            cout << "+-----------------------------------------------+\n";
            cout << "| Enjoy your " << setw(34) << left << selectedVehicle.model << " |\n";
            cout << "| Owner Contact: " << setw(29) << left << selectedVehicle.contact << " |\n";
            cout << "+-----------------------------------------------+\n";
        } else {
            cout << "\nRental cancelled.\n";
        }
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }
    
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void viewUserProfile(User& user) {
    try {
        cout << "\n";
        cout << "+-----------------------------------------------+\n";
        cout << "|                USER PROFILE                  |\n";
        cout << "+-----------------------------------------------+\n";
        cout << "| 1. Name: " << setw(35) << left << user.name << " |\n";
        cout << "| 2. Phone: " << setw(34) << left << user.phone << " |\n";
        cout << "| 3. Aadhaar: " << setw(32) << left << user.aadhaar << " |\n";
        cout << "| 4. Email: " << setw(35) << left << user.email << " |\n";
        cout << "+-----------------------------------------------+\n";
        
        if (getConfirmation("\nDo you want to edit your profile?")) {
            cout << "\nLeave field blank to keep current value:\n";
            
            string newName = getValidString("Enter new name [" + user.name + "]: ", nullptr, true);
            if (!newName.empty()) user.name = newName;
            
            string newPhone = getValidString("Enter new phone [" + user.phone + "]: ", validatePhone, true);
            if (!newPhone.empty()) user.phone = newPhone;
            
            string newAadhaar = getValidString("Enter new Aadhaar [" + user.aadhaar + "]: ", validateAadhaar, true);
            if (!newAadhaar.empty()) user.aadhaar = newAadhaar;
            
            string newEmail = getValidString("Enter new email [" + user.email + "]: ", validateEmail, true);
            if (!newEmail.empty()) user.email = newEmail;
            
            saveUserProfile(user);
            cout << "\nProfile updated successfully!\n";
        }
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }
    
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void viewBookingHistory() {
    try {
        ifstream file("BookingHistory.csv");
        if (!file.is_open()) {
            cout << "\nNo booking history found.\n";
            return;
        }

        cout << "\n";
        cout << "+------------------------------------------------------------------------------------------------+\n";
        cout << "|                                  CAB BOOKING HISTORY                                         |\n";
        cout << "+---------------------+---------------+-----------+---------------------+-----------------------+\n";
        cout << "| Date/Time          | Vehicle       | Price     | From                | To                   |\n";
        cout << "+---------------------+---------------+-----------+---------------------+-----------------------+\n";

        string line;
        while (getline(file, line)) {
            try {
                stringstream ss(line);
                string dateTime, driver, vehicle, from, to;
                double distance, price;
                char comma;
                
                getline(ss, dateTime, ',');
                getline(ss, driver, ',');
                getline(ss, vehicle, ',');
                ss >> distance >> comma >> price >> comma;
                getline(ss, from, ',');
                getline(ss, to);
                
                cout << "| " << setw(19) << left << dateTime.substr(0, 16)
                     << "| " << setw(13) << left << vehicle
                     << "| Rs." << setw(6) << left << fixed << setprecision(2) << price
                     << "| " << setw(19) << left << from.substr(0, 19)
                     << "| " << setw(21) << left << to.substr(0, 21) << "|\n";
            } catch (...) {
                continue;
            }
        }
        cout << "+---------------------+---------------+-----------+---------------------+-----------------------+\n";
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }
    
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void viewRentalHistory() {
    try {
        ifstream file("RentalHistory.csv");
        if (!file.is_open()) {
            cout << "\nNo rental history found.\n";
            return;
        }

        cout << "\n";
        cout << "+------------------------------------------------------------------------------------------------+\n";
        cout << "|                                  VEHICLE RENTAL HISTORY                                       |\n";
        cout << "+---------------------+---------------------------------+-----------+-----------------------------+\n";
        cout << "| Date/Time          | Vehicle                        | Days      | Total Price                 |\n";
        cout << "+---------------------+---------------------------------+-----------+-----------------------------+\n";

        string line;
        while (getline(file, line)) {
            try {
                stringstream ss(line);
                string dateTime, driver, vehicle, from, to;
                double days, price;
                char comma;
                
                getline(ss, dateTime, ',');
                getline(ss, driver, ',');
                getline(ss, vehicle, ',');
                ss >> days >> comma >> price >> comma;
                getline(ss, from, ',');
                getline(ss, to);
                
                cout << "| " << setw(19) << left << dateTime.substr(0, 16)
                     << "| " << setw(31) << left << vehicle.substr(0, 31)
                     << "| " << setw(9) << left << fixed << setprecision(0) << days
                     << "| Rs." << setw(25) << left << fixed << setprecision(2) << price << "|\n";
            } catch (...) {
                continue;
            }
        }
        cout << "+---------------------+---------------------------------+-----------+-----------------------------+\n";
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }
    
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void viewHistoryMenu() {
    while (true) {
        cout << "\n";
        cout << "+-------------------------------------+\n";
        cout << "|          VIEW HISTORY MENU         |\n";
        cout << "+-------------------------------------+\n";
        cout << "| 1. View Cab Booking History       |\n";
        cout << "| 2. View Vehicle Rental History    |\n";
        cout << "| 3. Back to Main Menu             |\n";
        cout << "+-------------------------------------+\n";
        
        int choice = getValidInput("Enter your choice (1-3): ", 1, 3);
        
        switch (choice) {
            case 1:
                viewBookingHistory();
                break;
            case 2:
                viewRentalHistory();
                break;
            case 3:
                return;
        }
    }
}

void displayWelcome() {
    cout << "\n";
    cout << "========================================\n";
    cout << "|    WELCOME TO CAB BOOKING SYSTEM    |\n";
    cout << "========================================\n";
    cout << "| Fast, Reliable, and Affordable Rides |\n";
    cout << "========================================\n";
}

int main() {
    try {
        displayWelcome();
        
        vector<Location> locations = loadLocations();
        vector<RentalVehicle> rentalVehicles = loadRentalVehicles();
        vector<Driver> drivers = loadDrivers();
        User user = loadUserProfile();
        
        if (user.name.empty()) {
            cout << "\n";
            cout << "+-------------------------------------+\n";
            cout << "|        NEW USER REGISTRATION       |\n";
            cout << "+-------------------------------------+\n";
            user.name = getValidString("Enter your full name: ");
            user.phone = getValidString("Enter your phone number: ", validatePhone);
            user.aadhaar = getValidString("Enter your Aadhaar number: ", validateAadhaar);
            user.email = getValidString("Enter your email address: ", validateEmail);
            saveUserProfile(user);
            cout << "\n";
            cout << "+-------------------------------------+\n";
            cout << "|   REGISTRATION COMPLETE!          |\n";
            cout << "+-------------------------------------+\n";
            cout << "| Welcome, " << setw(26) << left << user.name << " |\n";
            cout << "+-------------------------------------+\n";
        } else {
            cout << "\nWelcome back, " << user.name << "!\n";
        }

        while (true) {
            cout << "\n";
            cout << "========================================\n";
            cout << "|       CAB BOOKING SYSTEM MENU       |\n";
            cout << "========================================\n";
            cout << "| 1. Book a Cab                      |\n";
            cout << "| 2. Rent a Vehicle                  |\n";
            cout << "| 3. View/Edit Profile              |\n";
            cout << "| 4. View History                   |\n";
            cout << "| 5. Exit                           |\n";
            cout << "========================================\n";
            
            int choice = getValidInput("Enter your choice (1-5): ", 1, 5);
            
            switch (choice) {
                case 1:
                    bookCab(locations, drivers);
                    break;
                case 2:
                    rentCab(rentalVehicles);
                    break;
                case 3:
                    viewUserProfile(user);
                    break;
                case 4:
                    viewHistoryMenu();
                    break;
                case 5:
                    cout << "\n";
                    cout << "+-------------------------------------+\n";
                    cout << "|  THANK YOU FOR USING OUR SERVICE!  |\n";
                    cout << "|        HAVE A GREAT DAY!           |\n";
                    cout << "+-------------------------------------+\n";
                    return 0;
            }
        }
    } catch (const exception& e) {
        cerr << "\nFatal error: " << e.what() << endl;
        cerr << "The program will now exit. Please try again later.\n";
        return 1;
    }
    return 0;
}
