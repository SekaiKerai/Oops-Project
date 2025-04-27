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
#include <memory>
#include <conio.h>
#include <chrono>
#include <sys/stat.h>
#include <unordered_map>
using namespace std;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

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

// Rental Vehicle Hierarchy
class RentalVehicle {
protected:
    string type;
    string modelAndColor;  // Combined field
    string licensePlate;
    string ownerName;
    string contact;
    double pricePerDay;
public:
    RentalVehicle(const string& t, const string& mc, const string& lp, 
                const string& on, const string& cnt, double ppd)
        : type(t), modelAndColor(mc), licensePlate(lp), ownerName(on), contact(cnt), pricePerDay(ppd) {}
    
    virtual ~RentalVehicle() = default;
    
    virtual void displayDetails() const {
        cout << "| " << setw(31) << left << modelAndColor << " | "
            << setw(9) << left << licensePlate << " | "
            << setw(14) << left << ownerName << " | "
            << setw(10) << left << contact << " | "
            << "Rs." << setw(8) << left << fixed << setprecision(2) << pricePerDay << " |\n";
    }
    
    string getType() const { return type; }
    string getModelAndColor() const { return modelAndColor; }
    string getLicensePlate() const { return licensePlate; }
    string getOwnerName() const { return ownerName; }
    string getContact() const { return contact; }
    double getPricePerDay() const { return pricePerDay; }
};

class Motorcycle : public RentalVehicle {
public:
    Motorcycle(const string& mc, const string& lp, 
              const string& on, const string& cnt, double ppd)
        : RentalVehicle("Motorcycle", mc, lp, on, cnt, ppd) {}
};

class Hatchback : public RentalVehicle {
public:
    Hatchback(const string& mc, const string& lp, 
             const string& on, const string& cnt, double ppd)
        : RentalVehicle("Hatchback", mc, lp, on, cnt, ppd) {}
};

class Sedan : public RentalVehicle {
public:
    Sedan(const string& mc, const string& lp, 
         const string& on, const string& cnt, double ppd)
        : RentalVehicle("Sedan", mc, lp, on, cnt, ppd) {}
};

class SUV : public RentalVehicle {
public:
    SUV(const string& mc, const string& lp, 
       const string& on, const string& cnt, double ppd)
        : RentalVehicle("SUV", mc, lp, on, cnt, ppd) {}
};

// Booking Hierarchy
class Booking {
protected:
    string username;
    string driverName;
    string driverPhone;
    string vehicleType;
    double distance;
    string startLocation;
    string endLocation;
public:
    double price;
    Booking(const string&usn,const string& dn,const string&dp, const string& vt, double dist, const string& sl, const string& el)
        : driverName(dn), driverPhone(dp), vehicleType(vt), distance(dist), price(0), startLocation(sl), endLocation(el) {}
    
    virtual ~Booking() = default;
    
    virtual void displaySummary() const {
        cout << "| From: " << setw(38) << left << startLocation << " |\n"
             << "| To: " << setw(40) << left << endLocation << " |\n"
             << "| Distance: " << setw(34) << left << string(to_string(distance) + " km") << " |\n"
             << "| Driver: " << setw(36) << left << (driverName + " (" + vehicleType + ")") << " |\n"
             << "| Estimated Price: " << setw(27) << left << string("Rs." + to_string(price)) << " |\n";
    }
    
    virtual double calculatePrice() const { return price; }
    
    double getPrice() const { return price; }
    double getDistance() const { return distance; }
    string getDriverName() const { return driverName; }
    string getPhone() const {return driverPhone;}
    string getVehicleType() const { return vehicleType; }
    string getStartLocation() const { return startLocation; }
    string getEndLocation() const { return endLocation; }
};

class TuktukBooking : public Booking {
public:
    TuktukBooking(const string& usn,const string& dn,const string& dp, double dist, const string& sl, const string& el)
        : Booking(usn, dn, dp, "Tuktuk", dist, sl, el) {
        price = 20.0 + max(0.0, distance - 0.5) * 20.0;
    }
    
    double calculatePrice() const override {
        return 20.0 + max(0.0, distance - 0.5) * 20.0;
    }
};

class TaxiBooking : public Booking {
public:
    TaxiBooking(const string&usn,const string& dn,const string& dp, double dist, const string& sl, const string& el)
        : Booking(usn,dn, dp, "Taxi", dist, sl, el) {
        price = 100.0 + max(0.0, distance - 1.0) * 70.0;
    }
    
    double calculatePrice() const override {
        return 100.0 + max(0.0, distance - 1.0) * 70.0;
    }
};

struct Driver {
    string name;
    string contact;
    double rating;
    double lat;
    double lon;
    string vehicleType; 

    void updateLocation(double newLat, double newLon) {
        lat = newLat;
        lon = newLon;
    }
};

// Add to your data structures section
struct LoginCredentials {
    string username;
    string password;
    string role; // "user" or "driver"
};

struct User {
    string username;
    string password;
    string name;
    string phone;
    string aadhaar;
    string email;
    string role; // "user" or "driver"
};


// Helper functions
template<typename T>
T getValidInput(const string& prompt, T minVal, T maxVal) {
    T value;
    while (true) {
        cout << prompt;
        try {
            string input;
            getline(cin, input);
            stringstream ss(input);
            if (ss >> value && ss.eof()) {  // Check if entire input was consumed
                if (value >= minVal && value <= maxVal) {
                    return value;
                }
                cout << "Input must be between " << minVal << " and " << maxVal << ".\n";
            } else {
                throw invalid_argument("Invalid input");
            }
        } catch (...) {
            cout << "Invalid input. Please enter a valid number between " 
                 << minVal << " and " << maxVal << ".\n";
        }
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

string getValidUsername(const string& prompt) {
    string username;
    bool isValid;
    cout << prompt; 

    do {
        
        getline(cin, username);

        username.erase(0, username.find_first_not_of(" \t\n\r\f\v"));
        username.erase(username.find_last_not_of(" \t\n\r\f\v") + 1);

        isValid = true;

        for (char &c : username) {
            c = tolower(c); 
            if (!isalnum(c) && c != '.' && c != '_') {
                cout << "❌ Invalid character '" << c << "'. Only letters (a-z), numbers (0-9), '.', and '_' are allowed.\n";
                isValid = false;
                break;
            }
        }

        if (isValid && username.empty()) {
            cout << "Username cannot be empty.\n";
            isValid = false;
        }

    } while (!isValid);

    return username;
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

string getAndValidatePassword(const string& prompt = "Enter password (min 8 chars, with upper, lower, and number): ") {
    string password;
    char ch;

    while (true) {
        password.clear();
        bool hasUpper = false, hasLower = false, hasDigit = false;

        cout << prompt;

        while ((ch = _getch()) != '\r') {
            if (ch == '\b') {
                if (!password.empty()) {
                    password.pop_back();
                    cout << "\b \b";
                }
            }
            else {
                password.push_back(ch);
                cout << '*';

                if (isupper(ch)) hasUpper = true;
                if (islower(ch)) hasLower = true;
                if (isdigit(ch)) hasDigit = true;
            }
        }
        cout << endl;

        // Validation check
        bool isValid = true;

        if (password.length() < 8) {
            cout << "Error: Password must be at least 8 characters long\n";
            isValid = false;
        }

        if (!hasUpper) {
            cout << "Error: Password must contain at least one uppercase letter\n";
            isValid = false;
        }

        if (!hasLower) {
            cout << "Error: Password must contain at least one lowercase letter\n";
            isValid = false;
        }

        if (!hasDigit) {
            cout << "Error: Password must contain at least one digit (0-9)\n";
            isValid = false;
        }

        if (isValid) {
            return password;
        }

        cout << "Please try again.\n\n";
    }
}


string getPassword(const string& prompt = "Enter password: ") {
    string password;
    char ch;
    
    cout << prompt;
    
    while ((ch = _getch()) != '\r') { // '\r' is Enter key on Windows
        if (ch == '\b') { // Handle backspace
            if (!password.empty()) {
                password.pop_back();
                cout << "\b \b"; // Erase the asterisk
            }
        }
        else {
            password.push_back(ch);
            cout << '*';
        }
    }
    cout << endl;
    
    return password;
}

//function that resets the driver locations after 4 am
void resetDriverLocations() {
    // Check if it's 4 AM or later
    time_t now = time(0);
    tm* local = localtime(&now);
    if (local->tm_hour < 4) return;

    // Check if already reset today
    struct stat fileInfo;
    stat("Drivers.csv", &fileInfo);
    tm* modified = localtime(&fileInfo.st_mtime);
    if (modified->tm_mday == local->tm_mday && modified->tm_hour >= 4) return;

    // Read original locations
    std::ifstream orig("OriginalDrivers.csv");
    std::string line;
    std::unordered_map<std::string, std::pair<std::string, std::string>> originalLocs;
    
    while (getline(orig, line)) {
        std::istringstream ss(line);
        std::string name, phone, rating, lat, lon, type;
        getline(ss, name, ',');
        getline(ss, phone, ',');
        getline(ss, rating, ',');
        getline(ss, lat, ',');
        getline(ss, lon, ',');
        getline(ss, type);
        originalLocs[phone] = {lat, lon};
    }

    // Update current file
    std::ifstream in("Drivers.csv");
    std::ostringstream updated;
    
    while (getline(in, line)) {
        std::istringstream ss(line);
        std::string name, phone, rating, lat, lon, type;
        
        getline(ss, name, ',');
        getline(ss, phone, ',');
        getline(ss, rating, ',');
        getline(ss, lat, ',');
        getline(ss, lon, ',');
        getline(ss, type);

        if (originalLocs.count(phone)) {
            updated << name << "," << phone << "," << rating << ","
                   << originalLocs[phone].first << "," 
                   << originalLocs[phone].second << "," 
                   << type << "\n";
        } else {
            updated << line << "\n"; // Keep unchanged if no original found
        }
    }

    // Write back the updated data
    std::ofstream("Drivers.csv") << updated.str();
}

//update a driver by searching his phone number in file and update his lat,lon during updating profile of driver
bool updateDriverLocation(const std::string& phone, double newLat, double newLon) {
    // Read all drivers from file
    std::ifstream inFile("Drivers.csv");
    if (!inFile.is_open()) {
        return false;
    }

    std::vector<Driver> drivers;
    std::string line;
    bool driverFound = false;

    while (std::getline(inFile, line)) {
        std::stringstream ss(line);
        Driver drv;
        std::string token;

        // Parse CSV line
        std::getline(ss, drv.name, ',');
        std::getline(ss, drv.contact, ',');
        std::getline(ss, token, ',');
        drv.rating = std::stod(token);
        std::getline(ss, token, ',');
        drv.lat = std::stod(token);
        std::getline(ss, token, ',');
        drv.lon = std::stod(token);
        std::getline(ss, drv.vehicleType);

        // Update if phone matches
        if (drv.contact == phone) {
            drv.lat = newLat;
            drv.lon = newLon;
            driverFound = true;
        }
        drivers.push_back(drv);
    }
    inFile.close();

    if (!driverFound) {
        return false;
    }

    // Write updated data back to file
    std::ofstream outFile("Drivers.csv");
    for (const auto& drv : drivers) {
        outFile << drv.name << ","
                << drv.contact << ","
                << drv.rating << ","
                << drv.lat << ","
                << drv.lon << ","
                << drv.vehicleType << "\n";
    }

    return true;
}

//start of login
vector<LoginCredentials> loadAllCredentials() {
    vector<LoginCredentials> credentials;
    ifstream file("Users.csv");
    if (!file.is_open()) return credentials;

    string line;
    while (getline(file, line)) {
        try {
            stringstream ss(line);
            LoginCredentials cred;
            string token;
            
            getline(ss, cred.username, ',');
            getline(ss, cred.password, ',');
            // Skip name, phone, aadhaar, email
            for (int i = 0; i < 4; i++) getline(ss, token, ',');
            getline(ss, cred.role, ',');
            
            credentials.push_back(cred);
        } catch (...) {
            continue;
        }
    }
    return credentials;
}

User loginUser(const vector<LoginCredentials>& allCredentials,string& rrole) {
    while (true) {
        cout << "\n+-------------------------------------+\n";
        cout << "|               LOGIN                |\n";
        cout << "+-------------------------------------+\n";

        string username;
        bool usernameExists = false;
        while (true) {
            username = getValidUsername("Username: ");
            
            for (const auto& cred : allCredentials) {
                if (cred.username == username && cred.role == rrole) {
                    usernameExists = true;
                    break;
                }
            }
            
            if (usernameExists) break;
            cout << "Username not found. Please try again.\n";
        }

        // Password attempt tracking
        const int MAX_ATTEMPTS = 3;
        int attempts_remaining = MAX_ATTEMPTS;
        
        while (attempts_remaining > 0) {
            string password = getPassword();
            
            for (const auto& cred : allCredentials) {
                if (cred.username == username && cred.password == password) {
                    // Load full user details from Users.csv
                    ifstream file("Users.csv");
                    string line;
                    while (getline(file, line)) {
                        stringstream ss(line);
                        User user;
                        getline(ss, user.username, ',');
                        if (user.username == username) {
                            getline(ss, user.password, ',');
                            getline(ss, user.name, ',');
                            getline(ss, user.phone, ',');
                            getline(ss, user.aadhaar, ',');
                            getline(ss, user.email, ',');
                            getline(ss, user.role, ',');
                            return user;
                        }
                    }
                }
            }
            
            attempts_remaining--;
            cout << "Invalid password. Attempts remaining: " << attempts_remaining << "\n";
        }
        
        // If we get here, all attempts were used
        cout << "\n+-------------------------------------+\n";
        cout << "|        TOO MANY FAILED ATTEMPTS!      |\n";
        cout << "|     THANK YOU FOR USING OUR SERVICE!  |\n";
        cout << "|           HAVE A GREAT DAY!           |\n";
        cout << "+-------------------------------------+\n";
        exit(0);
    }
}

User registerNewUser(const string& role, const vector<LoginCredentials>& allCredentials) {
    User newUser;
    double lat,lon;
    newUser.role = role;
    
    cout << "\n+--------------------------------------------+\n";
    cout << "|          NEW " << setw(10) << left << (role == "user" ? "USER" : "DRIVER") << " REGISTRATION       |\n";
    cout << "+----------------------------------------------+\n";
    
    
    bool usernameExists;
    do {
        usernameExists = false;
        newUser.username = getValidUsername("Choose Username: ");
        
        // Check if username already exists
        for (const auto& cred : allCredentials) {
            if (cred.username == newUser.username && cred.role == role) {
                cout << "Username already exists. Please choose a different one.\n";
                usernameExists = true;
                break;
            }
        }
    } while (usernameExists);
    
    newUser.password = getAndValidatePassword();
    
    if (role == "user") {
        newUser.name = getValidString("Enter your full name: ");
        newUser.phone = getValidString("Enter your phone number: ", validatePhone);
        newUser.aadhaar = getValidString("Enter your Aadhaar number: ", validateAadhaar);
        newUser.email = getValidString("Enter your email address: ", validateEmail);
    } else { // driver
        newUser.name = getValidString("Enter your full name: ");
        newUser.phone = getValidString("Enter your phone number: ", validatePhone);
        newUser.aadhaar = getValidString("Enter your Aadhaar number: ", validateAadhaar);
        newUser.email = getValidString("Enter your email address: ", validateEmail);
        int vehicleTypeChoice = getValidInput("Enter your vehicle type (1. Tuktuk, 2. Taxi): ",1,2);
        if(vehicleTypeChoice==1){
            newUser.role = "Tuktuk";
        }
        else{
            newUser.role = "Taxi";
        }
        cout << "Enter Start location Latitude and Longitudes : "<<endl;
        cout << "NOTE: Read the Manual for more details about Latitude and Longitude "<<endl;
        cout << "Latitude: "; cin>>lat;
        cout<< "Longitude: "; cin>>lon;
    }
    
    // Save to CSV
    ofstream file("Users.csv", ios::app);
    if (file.is_open()) {
        file << newUser.username << "," << newUser.password << ","
             << newUser.name << "," << newUser.phone << ","
             << newUser.aadhaar << "," << newUser.email << ","
             << newUser.role << "\n";
    }
    //default rating of 4.0
    
    if(role == "driver"){
        ofstream file2("Drivers.csv" , ios::app);
        if(file2.is_open()){
            file2 << newUser.name << "," << newUser.phone << ","
                    << "4.0" << "," << lat << "," << lon << ","
                    << newUser.role << "\n";
        }
    }
    
    cout << "\nRegistration successful!\n";
    return newUser;
}

// New main menu for login/registration
User handleLogin(bool showWelcome = true) {
    // if (showWelcome) {
    //     cout << "\n+-------------------------------------+\n";
    //     cout << "|    WELCOME TO CAB BOOKING SYSTEM    |\n";
    //     cout << "+-------------------------------------+\n";
    // }
    
    while (true) {
        cout << "\n+-------------------------------------+\n";
        cout << "| 1. User Login                      |\n";
        cout << "| 2. Driver Login                    |\n";
        cout << "| 3. Exit                            |\n";
        cout << "+-------------------------------------+\n";
        
        int choice = getValidInput("Enter your choice (1-3): ", 1, 3);
        
        if (choice == 3) {
            cout << "\n+-------------------------------------+\n";
            cout << "|  THANK YOU FOR USING OUR SERVICE!  |\n";
            cout << "|        HAVE A GREAT DAY!           |\n";
            cout << "+-------------------------------------+\n";
            exit(0);
        }
        
        string role = (choice == 1) ? "user" : "driver";
        
        cout << "\n+-------------------------------------+\n";
        cout << "| 1. Login to existing account      |\n";
        cout << "| 2. Register new account           |\n";
        cout << "| 3. Back                           |\n";
        cout << "+-------------------------------------+\n";
        
        int authChoice = getValidInput("Enter your choice (1-3): ", 1, 3);
        
        if (authChoice == 3) continue;
        
        auto allCredentials = loadAllCredentials();
        
        if (authChoice == 1) {
            return loginUser(allCredentials, role);
        } else if (authChoice == 2) {
            return registerNewUser(role,allCredentials);
        }
    }
}


//end of login 
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

vector<unique_ptr<RentalVehicle>> loadRentalVehicles() {
    vector<unique_ptr<RentalVehicle>> vehicles;
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
            string type, modelAndColor, licensePlate, ownerName, contact, priceStr;
            
            if (!getline(ss, type, ',') || 
                !getline(ss, modelAndColor, ',') || 
                !getline(ss, licensePlate, ',') || 
                !getline(ss, ownerName, ',') || 
                !getline(ss, contact, ',') || 
                !getline(ss, priceStr)) {
                cerr << "Warning: Skipping malformed vehicle entry (missing fields): " << line << endl;
                continue;
            }

            double pricePerDay = stod(priceStr);
            
            if (type == "Motorcycle") {
                vehicles.emplace_back(make_unique<Motorcycle>(modelAndColor, licensePlate, ownerName, contact, pricePerDay));
            } else if (type == "Hatchback") {
                vehicles.emplace_back(make_unique<Hatchback>(modelAndColor, licensePlate, ownerName, contact, pricePerDay));
            } else if (type == "Sedan") {
                vehicles.emplace_back(make_unique<Sedan>(modelAndColor, licensePlate, ownerName, contact, pricePerDay));
            } else if (type == "SUV") {
                vehicles.emplace_back(make_unique<SUV>(modelAndColor, licensePlate, ownerName, contact, pricePerDay));
            } else {
                cerr << "Warning: Unknown vehicle type '" << type << "' in line: " << line << endl;
            }
        } catch (const exception& e) {
            cerr << "Warning: Skipping malformed vehicle entry (" << e.what() << "): " << line << endl;
        }
    }
    return vehicles;
}

// vector<Driver> loadDrivers() {
//     vector<Driver> drivers;
//     ifstream file("Drivers.csv");
//     if (!file.is_open()) {
//         throw runtime_error("Could not open Drivers.csv");
//     }

//     string line;
//     while (getline(file, line)) {
//         try {
//             stringstream ss(line);
//             Driver drv;
//             string token;

//             getline(ss, token, ',');
//             drv.name = token;
            
//             getline(ss, token, ',');
//             drv.contact = token;
            
//             getline(ss, token, ',');
//             drv.rating = stod(token);
//             if (drv.rating < 0 || drv.rating > 5) {
//                 drv.rating = 4.0; // Default if invalid
//             }
            
//             getline(ss, token, ',');
//             drv.lat = stod(token);
            
//             getline(ss, token, ',');
//             drv.lon = stod(token);
            
//             getline(ss, token, ',');
//             drv.vehicleType = token;
            
//             drivers.push_back(drv);
//         } catch (...) {
//             cerr << "Warning: Skipping malformed driver entry\n";
//         }
//     }
//     return drivers;
// }

vector<Driver> loadDrivers() {
    vector<Driver> drivers;
    ifstream file("Drivers.csv");
    
    if (!file.is_open()) {
        throw runtime_error("Could not open Drivers.csv");
    }

    string line;
    while (getline(file, line)) {
        // Skip empty lines or whitespace-only lines
        if (line.find_first_not_of(" \t\n\r\v\f") == string::npos) {
            cerr << "Skipping empty line" << endl;
            continue;
        }

        // Remove trailing whitespace
        line.erase(line.find_last_not_of(" \t\n\r\v\f") + 1);

        try {
            stringstream ss(line);
            Driver drv;
            string token;

            // Parse name
            if (!getline(ss, drv.name, ',')) throw runtime_error("Missing name");

            // Parse phone
            if (!getline(ss, drv.contact, ',')) throw runtime_error("Missing phone");

            // Parse rating
            if (!getline(ss, token, ',')) throw runtime_error("Missing rating");
            drv.rating = stod(token);
            drv.rating = max(0.0, min(5.0, drv.rating)); // Clamp to 0-5

            // Parse latitude
            if (!getline(ss, token, ',')) throw runtime_error("Missing latitude");
            drv.lat = stod(token);

            // Parse longitude
            if (!getline(ss, token, ',')) throw runtime_error("Missing longitude");
            drv.lon = stod(token);

            // Parse vehicle type
            if (!getline(ss, drv.vehicleType)) throw runtime_error("Missing vehicle type");

            // Check for extra data
            if (ss >> token) throw runtime_error("Extra data in record");

            drivers.push_back(drv);
        } catch (const exception& e) {
            cerr << "Warning: Skipping malformed driver entry - " << e.what() 
                 << " in line: " << line << endl;
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
            
            getline(ss,token,',');  // added username input
            user.username = token;

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
    try {
        // Read all users from the file
        ifstream inFile("Users.csv");
        vector<string> lines;
        string line;
        
        while (getline(inFile, line)) {
            lines.push_back(line);
        }
        inFile.close();

        // Find and update the current user's record
        bool userFound = false;
        for (auto& record : lines) {
            stringstream ss(record);
            string username;
            getline(ss, username, ',');
            
            if (username == user.username) {
                // Reconstruct the line with updated user data
                record = user.username + "," + 
                         user.password + "," + 
                         user.name + "," + 
                         user.phone + "," + 
                         user.aadhaar + "," + 
                         user.email + "," + 
                         user.role;
                userFound = true;
                break;
            }
        }

        if (!userFound) {
            throw runtime_error("User not found in database");
        }

        // Write all records back to the file
        ofstream outFile("Users.csv");
        for (const auto& record : lines) {
            outFile << record << "\n";
        }
        outFile.close();

    } catch (const exception& e) {
        throw runtime_error("Failed to update user profile: " + string(e.what()));
    }
}

void saveHistory(const string& username, const string& type, 
    const string& driverName, const string& driverPhone,
    const string& vehicleType, double distanceOrDays, 
    double price, const string& startLocation, 
    const string& endLocation) {
string filename = (type == "BOOKING") ? "BookingHistory.csv" : "RentalHistory.csv";
ofstream file(filename, ios::app);
if (!file.is_open()) {
throw runtime_error("Could not save history");
}

time_t now = time(0);
tm *ltm = localtime(&now);

// Write all fields including username
file << username << ","  // Added username as first field
<< ltm->tm_year + 1900 << "-" 
<< setw(2) << setfill('0') << ltm->tm_mon + 1 << "-"
<< setw(2) << setfill('0') << ltm->tm_mday << " "
<< setw(2) << setfill('0') << ltm->tm_hour << ":"
<< setw(2) << setfill('0') << ltm->tm_min << ":"
<< setw(2) << setfill('0') << ltm->tm_sec << ","
<< driverName << ","
<< driverPhone << ","
<< vehicleType << ","
<< fixed << setprecision(2) << distanceOrDays << ","
<< price << ","
<< startLocation << ","
<< endLocation << endl;
}// changed saveHistory

void viewDriverBookings(const string& driverName) { //viewing driver booking
    try {
        ifstream file("BookingHistory.csv");
        if (!file.is_open()) {
            cout << "\nNo bookings found.\n";
            return;
        }

        cout << "\n";
        cout << "+------------------------------------------------------------------------------------------------+\n";
        cout << "|                                  YOUR DRIVING HISTORY                                         |\n";
        cout << "+---------------------+---------------+-----------+---------------------+-----------------------+\n";
        cout << "| Date/Time          | User          | Price     | From                | To                   |\n";
        cout << "+---------------------+---------------+-----------+---------------------+-----------------------+\n";

        string line;
        while (getline(file, line)) {
            try {
                stringstream ss(line);
                string user, dateTime, driver, vehicle, from, to;
                double distance, price;
                char comma;
                
                getline(ss, user, ',');
                getline(ss, dateTime, ',');
                getline(ss, driver, ',');
                if (driver != driverName) continue;
                
                getline(ss, vehicle, ',');
                ss >> distance >> comma >> price >> comma;
                getline(ss, from, ',');
                getline(ss, to);
                
                cout << "| " << setw(19) << left << dateTime.substr(0, 16)
                     << "| " << setw(13) << left << user
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
}

//add driver in file - adds the updated driver location after each booking by user
void updateDriverInFile(const Driver& updatedDriver) {
    // Read all drivers from file
    ifstream inFile("Drivers.csv");
    vector<string> lines;
    string line;
    
    // Read all lines (assuming no header in file)
    while (getline(inFile, line)) {
        lines.push_back(line);
    }
    inFile.close();

    // Find and update the specific driver
    bool driverFound = false;
    for (auto& record : lines) {
        stringstream ss(record);
        string name, phone, rating, lat, lon, type;
        
        // Parse the existing record
        getline(ss, name, ',');
        getline(ss, phone, ',');
        getline(ss, rating, ',');
        getline(ss, lat, ',');
        getline(ss, lon, ',');
        getline(ss, type);

        if (phone == updatedDriver.contact) {
            // Rebuild the line with updated location
            stringstream newRecord;
            newRecord << name << ","
                      << phone << ","
                      << rating << ","
                      << updatedDriver.lat << ","
                      << updatedDriver.lon << ","
                      << type;
            record = newRecord.str();
            driverFound = true;
            break;
        }
    }

    if (!driverFound) {
        throw runtime_error("Driver with phone " + updatedDriver.contact + " not found in database");
    }

    // Write all records back to file
    ofstream outFile("Drivers.csv");
    for (const auto& record : lines) {
        outFile << record << "\n";
    }
}

// Feature functions
void bookCab(const vector<Location>& locations, const vector<Driver>& drivers,string& username) {
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


        //adding feature for booking solo travel or sharing travel
        cout << "\n+-------------------------------------+\n";
        cout << "|          SELECT RIDE TYPE          |\n";
        cout << "+-------------------------------------+\n";
        cout << "| 1. Solo Ride (Full Price)         |\n";
        cout << "| 2. Shared Ride (prices split equally) |\n";
        cout << "+-------------------------------------+\n";
        int rideType = getValidInput("Select ride type (1-2): ",1,2);
        
        unique_ptr<Booking> booking;
        if (selectedDriver.vehicleType == "Tuktuk") {
            booking = make_unique<TuktukBooking>(username,selectedDriver.name,selectedDriver.contact, distance, startLoc->name, endLoc->name);
            if(rideType == 2){
                booking->price /= 4;
            }
        } else {
            booking = make_unique<TaxiBooking>(username,selectedDriver.name,selectedDriver.contact, distance, startLoc->name, endLoc->name);
            if(rideType == 2){
                booking->price /= 4;
            }
        }

        cout << "\n";
        cout << "+-----------------------------------------------+\n";
        cout << "|              BOOKING SUMMARY                 |\n";
        cout << "+-----------------------------------------------+\n";
        booking->displaySummary();
        cout << "+-----------------------------------------------+\n";

        if (getConfirmation("\nConfirm booking?")) {
            
            saveHistory(username,"BOOKING", booking->getDriverName(),booking->getPhone(), booking->getVehicleType(), 
                       booking->getDistance(), booking->getPrice(), 
                       booking->getStartLocation(), booking->getEndLocation());

            // update driver location start = end
            const_cast<Driver&>(selectedDriver).lat = endLoc->lat;
            const_cast<Driver&>(selectedDriver).lon = endLoc->lon;
            //update this in driver.csv
            updateDriverInFile(selectedDriver);
            
            cout << "\n";
            cout << "+-----------------------------------------------+\n";
            cout << "|          BOOKING CONFIRMED!                 |\n";
            cout << "+-----------------------------------------------+\n";
            cout << "| Your " << setw(39) << left << selectedDriver.vehicleType + " will arrive shortly." << " |\n";
            cout << "| Driver contact: " << setw(28) << left << selectedDriver.contact << " |\n";
            cout << "+-----------------------------------------------+\n";

            double rating = getValidInput("Rate your driver (0-5): ", 0.0, 5.0);
            
            // Update driver rating (average with previous rating)
            const_cast<Driver&>(selectedDriver).rating = 
                min(5.0, max(0.0, (selectedDriver.rating + rating) / 2.0));

            cout << "\n Thank you for your feedback!\n";
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

void rentCab(const vector<unique_ptr<RentalVehicle>>& vehicles,string& username) {
    try {
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
        vector<const RentalVehicle*> availableVehicles;
        for (const auto& vehicle : vehicles) {
            if (vehicle->getType() == vehicleType) {
                availableVehicles.push_back(vehicle.get());
            }
        }
        
        if (availableVehicles.empty()) {
            cout << "\nNo vehicles available for this type. Please try another type.\n";
            return;
        }
        
        cout << "\n";
        cout << "+----------------------------------------------------------------------------------------------------------+\n";
        cout << "|                                     AVAILABLE " << setw(20) << left << vehicleType + "s" << "                                     |\n";
        cout << "+-----+-------------------------------------+-----------+------------------+------------+-----------------+\n";
        cout << "| ID  | Model (Color)                       | Plate     | Owner            | Contact    | Price/Day       |\n";
        cout << "+-----+-------------------------------------+-----------+------------------+------------+-----------------+\n";
        
        for (int i = 0; i < availableVehicles.size(); ++i) {
            const auto& vehicle = availableVehicles[i];
            cout << "| " << setw(3) << left << i+1 << " | " 
                 << setw(35) << left << vehicle->getModelAndColor() << " | "
                 << setw(9) << left << vehicle->getLicensePlate() << " | "
                 << setw(16) << left << vehicle->getOwnerName() << " | "
                 << setw(10) << left << vehicle->getContact() << " | "
                 << "Rs." << setw(12) << left << fixed << setprecision(2) << vehicle->getPricePerDay() << " |\n";
        }
        cout << "+-----+-------------------------------------+-----------+------------------+------------+-----------------+\n";
        
        int vehicleChoice = getValidInput<int>("\nSelect vehicle (1-" + to_string(availableVehicles.size()) + "): ", 
                                            1, 
                                            static_cast<int>(availableVehicles.size()));
        const RentalVehicle* selectedVehicle = availableVehicles[vehicleChoice-1];
        
        int days = getValidInput("Enter integer number of days (1-30): ", 1, 30);
        double totalPrice = selectedVehicle->getPricePerDay() * days;
        
        cout << "\n";
        cout << "+-----------------------------------------------------+\n";
        cout << "|                  RENTAL SUMMARY                    |\n";
        cout << "+-----------------------------------------------------+\n";
        cout << "| Vehicle Type: " << setw(38) << left << selectedVehicle->getType() << " |\n";
        cout << "| Model (Color): " << setw(37) << left << selectedVehicle->getModelAndColor() << " |\n";
        cout << "| License Plate: " << setw(37) << left << selectedVehicle->getLicensePlate() << " |\n";
        cout << "| Owner Name: " << setw(40) << left << selectedVehicle->getOwnerName() << " |\n";
        cout << "| Owner Contact: " << setw(37) << left << selectedVehicle->getContact() << " |\n";
        cout << "| Duration: " << setw(42) << left << string(to_string(days) + " days") << " |\n";
        cout << "| Price/Day: " << setw(41) << left << string("Rs." + to_string(selectedVehicle->getPricePerDay())) << " |\n";
        cout << "| Total Price: " << setw(39) << left << string("Rs." + to_string(totalPrice)) << " |\n";
        cout << "+-----------------------------------------------------+\n";
        
        if (getConfirmation("\nConfirm rental?")) {
            saveHistory(username, "RENTAL", selectedVehicle->getType() + " - " + selectedVehicle->getModelAndColor(),  selectedVehicle->getContact(),
                       "N/A",days, totalPrice, "N/A", "N/A");
            cout << "\n";
            cout << "+-----------------------------------------------------+\n";
            cout << "|               RENTAL CONFIRMED!                    |\n";
            cout << "+-----------------------------------------------------+\n";
            cout << "| Enjoy your " << setw(42) << left << selectedVehicle->getModelAndColor() << " |\n";
            cout << "| Owner Contact: " << setw(37) << left << selectedVehicle->getContact() << " |\n";
            cout << "+-----------------------------------------------------+\n";
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
        while (true) {
            cout << "\n";
            cout << "+-----------------------------------------------+\n";
            cout << "|                USER PROFILE                  |\n";
            cout << "+-----------------------------------------------+\n";
            cout << "| Username: " << setw(34) << left << user.username << " |\n";
            cout << "| Name: " << setw(37) << left << user.name << " |\n";
            cout << "| Phone: " << setw(36) << left << user.phone << " |\n";
            cout << "| Aadhaar: " << setw(34) << left << user.aadhaar << " |\n";
            cout << "| Email: " << setw(36) << left << user.email << " |\n";
            cout << "+-----------------------------------------------+\n";
            
            cout << "\n+-------------------------------------+\n";
            cout << "|        PROFILE OPTIONS             |\n";
            cout << "+-------------------------------------+\n";
            cout << "| 1. Edit Profile Details            |\n";
            cout << "| 2. Change Password                 |\n";
            cout << "| 3. Back to Main Menu               |\n";
            cout << "+-------------------------------------+\n";
            
            int choice = getValidInput("Enter your choice (1-3): ", 1, 3);
            
            switch (choice) {
                case 1: {
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

                    if(user.role!="user"){
                        double lat,lon;
                        if(getConfirmation("\nWould you like to change your start location?")){
                            cout << "NOTE: Read the Manual for more details about Latitude and Longitude "<<endl;
                            cout << "Latitude: "; cin>>lat;
                            cout<< "Longitude: "; cin>>lon;
                        }
                        updateDriverLocation(user.phone,lat,lon);
                    }
                    cout << "\nProfile updated successfully!\n";
                    break;
                }
                case 2: {
                    // Password Change - Current Password Verification
                    const int MAX_ATTEMPTS = 3;
                    int attempts_remaining = MAX_ATTEMPTS;

                    cout << "For security, please verify your current password." << endl;
                    
                    while (attempts_remaining > 0) {
                        string currentPassword = getPassword("Enter current password: ");
                        
                        if (currentPassword == user.password) {
                            break;
                        }
                        
                        attempts_remaining--;
                        cout << "Incorrect password. Attempts remaining: " << attempts_remaining << endl;
                        
                        if (attempts_remaining == 0) {
                            cout << "Security notice: Maximum attempts reached. Returning to main menu." << endl;
                            cout << "\nPress Enter to continue...";
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cin.get();
                            return;
                        }
                    }

                    // New Password Entry
                    cout << "\nPassword verified. You may now set a new password." << endl;
                    while (true) {
                        string newPassword = getAndValidatePassword("Enter new password (min 8 chars, with upper, lower, and number): ");
                        
                        if (newPassword == user.password) {
                            cout << "Security requirement: New password must differ from current password." << endl;
                            continue;
                        }
                        
                        string confirmPassword = getPassword("Confirm new password: ");
                        
                        if (newPassword != confirmPassword) {
                            cout << "Error: Password confirmation does not match." << endl;
                            continue;
                        }
                        
                        // Successful password change
                        user.password = newPassword;
                        saveUserProfile(user);
                        cout << "Success: Your password has been updated." << endl;
                        break;
                    }
                    break;
                }
                case 3:
                    return;
            }
            
            cout << "\nPress Enter to continue...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
        }
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        cout << "\nPress Enter to continue...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
    }
}


void viewBookingHistory(const string& username) {
    try {
        ifstream file("BookingHistory.csv");
        if (!file.is_open()) {
            cout << "\nNo booking history found.\n";
            return;
        }

        cout << "\n";
        cout << "+------------------------------------------------------------------------------------------------------------------------------------------+\n";
        cout << "|                                                           YOUR BOOKING HISTORY                                                          |\n";
        cout << "+---------------------+---------------+-----------+---------------------+------------------------------------------------------------------+\n";
        cout << "| Date/Time           | Driver Name        | Contact   |Price     | From                              |To                                  |\n";
        cout << "+---------------------+---------------+-----------+---------------------+------------------------------------------------------------------+\n";
        vector<Driver>drivers = loadDrivers();
        string line;
        while (getline(file, line)) {
            try {
                stringstream ss(line);
                string user, dateTime, driver, vehicle, from, to,driverContact;
                double distance, price;
                char comma;
                
                getline(ss, user, ',');
                if (user != username) continue;
                
                getline(ss, dateTime, ',');
                getline(ss, driver, ',');
                getline(ss, driverContact,',');
                getline(ss, vehicle, ',');
                ss >> distance >> comma >> price >> comma;
                getline(ss, from, ',');
                getline(ss, to);


                cout << "| " << setw(19) << left << dateTime.substr(0, 19)
                     << "| " << setw(20) << left << driver 
                     << "| " << setw(10) << left << driverContact.substr(0, 10)
                     << "| Rs." << setw(6) << left << fixed << setprecision(2) << price
                     << "| " << setw(35) << left << from.substr(0, 35)
                     << "| " << setw(35) << left << to.substr(0, 35) << "|\n";
            } catch (...) {
                continue;
            }
        }
        cout << "+---------------------+---------------+-----------+---------------------+------------------------------------------------------------------+\n";
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }
} //changed booking history

void viewRentalHistory(string username) {
    try {
        ifstream file("RentalHistory.csv");
        if (!file.is_open()) {
            cout << "\nNo rental history found.\n";
            return;
        }

        cout << "\n";
        cout << "+--------------------------------------------------------------------------------------------------------+\n";
        cout << "|                                  VEHICLE RENTAL HISTORY                                                |\n";
        cout << "+---------------------+---------------------------------+-----------+--------------------------------------+\n";
        cout << "| Date/Time           | Vehicle                                  | Days      | Total Price               |\n";
        cout << "+---------------------+---------------------------------+-----------+------------------------------------+\n";

        string line;
        while (getline(file, line)) {
            try {
                stringstream ss(line);
                string dateTime, typedetail, vehicle,contact, from, to, usern;
                double days, price;
                char comma;
                
                getline(ss, usern, ',');
                if(usern != username)continue;
                getline(ss, dateTime, ',');
                getline(ss, typedetail, ',');
                getline(ss, contact, ',');
                getline(ss, vehicle, ',');
                ss >> days >> comma >> price >> comma;
                getline(ss, from, ',');
                getline(ss, to);
                
                cout << "| " << setw(20) << left << dateTime.substr(0, 20)
                     << "| " << setw(41) << left << typedetail.substr(0, 41)
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

void viewHistoryMenu(string& username) {
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
                viewBookingHistory(username);
                break;
            case 2:
                viewRentalHistory(username);
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

void showLogoutMessage() {
    cout << "\n+-------------------------------------+\n";
    cout << "|         LOGGED OUT SUCCESSFULLY       |\n";
    cout << "|      Returning to login screen...     |\n";
    cout << "+---------------------------------------+\n";
    cout << "Press Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

int main() {
    resetDriverLocations();
    try {
        displayWelcome();
        
        // Handle login/registration
        User currentUser = handleLogin(true);
        
        cout << "\nWelcome, " << currentUser.name << "!\n";

        vector<Location> locations = loadLocations();
        vector<unique_ptr<RentalVehicle>> rentalVehicles = loadRentalVehicles();
        vector<Driver> drivers = loadDrivers();
        User user = loadUserProfile();

        while (true) {
            cout << "\n========================================\n";
            cout << "|       CAB BOOKING SYSTEM MENU       |\n";
            cout << "========================================\n";
            
            if (currentUser.role == "user") {
                cout << "| 1. Book a Cab                      |\n";
                cout << "| 2. Rent a Vehicle                  |\n";
                cout << "| 3. User Profile                    |\n";
                cout << "| 4. View History                    |\n";
                cout << "| 5. Logout                          |\n";
                cout << "| 6. Exit                            |\n";
            } else { // driver
                cout << "| 1. View My Bookings               |\n";
                cout << "| 2. View/Edit Profile              |\n";
                cout << "| 3. Logout                         |\n";
                cout << "| 4. Exit                           |\n";
            }
            
            cout << "========================================\n";
            
            int maxChoice = currentUser.role == "user" ? 6 : 4;
            int choice = getValidInput("Enter your choice (1-" + to_string(maxChoice) + "): ", 1, maxChoice);
            
            if (currentUser.role == "user") {
                switch (choice) {
                    case 1: bookCab(locations, drivers, currentUser.username); break;
                    case 2: rentCab(rentalVehicles, currentUser.username); break;
                    case 3: viewUserProfile(currentUser); break;
                    case 4: viewHistoryMenu(currentUser.username); break;
                    case 5: showLogoutMessage(); currentUser = handleLogin(false); break;
                    case 6: {
                        cout << "\n+-------------------------------------+\n";
                        cout << "|  THANK YOU FOR USING OUR SERVICE!  |\n";
                        cout << "|        HAVE A GREAT DAY!           |\n";
                        cout << "+---------------------------------------+\n";
                        exit(0);
                    }
                    
                }
            } else {
                switch (choice) {
                    case 1: viewDriverBookings(currentUser.name); break;
                    case 2: viewUserProfile(currentUser); break;
                    case 3: showLogoutMessage(); currentUser = handleLogin(false); break;
                    case 4: {
                        cout << "\n+-------------------------------------+\n";
                        cout << "|  THANK YOU FOR USING OUR SERVICE!  |\n";
                        cout << "|        HAVE A GREAT DAY!           |\n";
                        cout << "+---------------------------------------+\n";
                        exit(0);
                    }
                }
            }
        }
    } catch (const exception& e) {
        cerr << "\nFatal error: " << e.what() << endl;
        cerr << "The program will now exit. Please try again later.\n";
        return 1;
    }
    return 0;
}
