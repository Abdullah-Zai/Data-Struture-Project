#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <limits>

using namespace std;

void clearScreen() {
    system("CLS"); // Clear screen for Windows
}

// Room Types Enum
enum RoomType {
    SINGLE_BED,
    DOUBLE_BED,
    DELUXE_ROOM
};

// Utility Functions
string getRoomTypeString(RoomType type) {
    switch (type) {
        case SINGLE_BED: return "Single Bed";
        case DOUBLE_BED: return "Double Bed";
        case DELUXE_ROOM: return "Deluxe Room";
        default: return "Unknown";
    }
}

// Room Class
class Room {
public:
    int roomNumber;
    RoomType type;
    double pricePerNight;
    bool isAvailable;
    bool isUnderMaintenance;

    Room(int number, RoomType t, double price) 
        : roomNumber(number), type(t), pricePerNight(price), isAvailable(true), isUnderMaintenance(false) {}

    void displayDetails() const {
        cout << "Room " << roomNumber 
             << " - " << getRoomTypeString(type) 
             << " - Rs. " << pricePerNight << " per night - " 
             << (isAvailable ? "Available" : "Not Available") 
             << (isUnderMaintenance ? " (Under Maintenance)" : "") << endl;
    }
};

// Meal Class
class Meal {
public:
    string name;
    double price;
    string description;

    Meal(string n, double p, string desc = "") : name(n), price(p), description(desc) {}

    void displayDetails() const {
        cout << name << " - Rs. " << price;
        if (!description.empty()) {
            cout << " (" << description << ")";
        }
        cout << endl;
    }
};

// Customer Class
class Customer {
public:
    string name;
    string phoneNumber;
    string email;
    int roomNumber;

    Customer(string n, string phone, string em, int r) 
        : name(n), phoneNumber(phone), email(em), roomNumber(r) {}

    void displayDetails() const {
        cout << "Name: " << name << ", Phone: " << phoneNumber 
             << ", Email: " << email << ", Room: " << roomNumber << endl;
    }
};

// Reservation Class
class Reservation {
public:
    Customer customer;
    Room* room;
    vector<Meal> orderedMeals;
    double totalBill;

    Reservation(Customer c, Room* r) : customer(c), room(r), totalBill(0) {}

    void addMeal(const Meal& meal) {
        orderedMeals.push_back(meal);
        totalBill += meal.price;
    }

    void calculateTotalBill(int days) {
        totalBill += room->pricePerNight * days;
    }

    void displayReservation() const {
        customer.displayDetails();
        room->displayDetails();
        cout << "Meals Ordered: " << orderedMeals.size() << endl;
        cout << "Total Bill: Rs. " << totalBill << endl;
    }
};

// Hotel Management System
class HotelManagementSystem {
private:
    vector<Room*> rooms;
    vector<Meal> meals;
    vector<Customer> customers;
    vector<Reservation> reservations;

public:
    HotelManagementSystem() {
        // Initialize Rooms
        rooms.push_back(new Room(101, SINGLE_BED, 2500));
        rooms.push_back(new Room(102, SINGLE_BED, 2500));
        rooms.push_back(new Room(201, DOUBLE_BED, 4000));
        rooms.push_back(new Room(202, DOUBLE_BED, 4000));
        rooms.push_back(new Room(301, DELUXE_ROOM, 6000));
        rooms.push_back(new Room(302, DELUXE_ROOM, 6000));

        // Initialize Meals
        meals.push_back(Meal("Breakfast", 1200, "Continental breakfast"));
        meals.push_back(Meal("Lunch", 1500, "Full course meal"));
        meals.push_back(Meal("Dinner", 2000, "Chef's special dinner"));
        meals.push_back(Meal("Breakfast & Dinner Combo", 550, "Special discounted package"));
    }

    ~HotelManagementSystem() {
        for (auto room : rooms) delete room;
    }

    void addRoom() {
        int roomNumber;
        int typeChoice;
        double price;

        cout << "Enter Room Number: ";
        cin >> roomNumber;
        cout << "Select Room Type (1: Single Bed, 2: Double Bed, 3: Deluxe Room): ";
        cin >> typeChoice;
        if (typeChoice < 1 || typeChoice > 3) {
            cout << "Invalid room type selected!\n";
            return;
        }
        cout << "Enter Price Per Night: ";
        cin >> price;

        RoomType type = static_cast<RoomType>(typeChoice - 1);
        rooms.push_back(new Room(roomNumber, type, price));
        cout << "Room added successfully!\n";
        cin.ignore();
        cin.get();
    }

    void viewAvailableRooms() const {
        cout << "\nAvailable Rooms:\n";
        for (const auto& room : rooms) {
            if (room->isAvailable && !room->isUnderMaintenance) {
                room->displayDetails();
            }
        }
        cin.ignore();
        cin.get();
    }

    void viewMealMenu() const {
    cout << "\nMeal Menu:\n";

    cout << "\nBreakfast Options:\n";
    cout << "1. Juice, Coffee, Tea, Cake, Pancakes, Fruits\n";
    
    cout << "\nLunch Options:\n";
    cout << "1. Rice, Curry, Salad, Bread, Chicken, Fish\n";
    
    cout << "\nDinner Options:\n";
    cout << "1. Soup, Grilled Meat, Vegetables, Pasta, Dessert\n";
    
    cout << "\nBreakfast & Dinner Combo:\n";
    cout << "1. Includes all options from breakfast and dinner\n";

    cin.ignore();
    cin.get();
}

    void reserveRoom() {
    viewAvailableRooms();
    int roomNumber, days;
    string name, phone, email;

    cout << "Enter Room Number to Reserve: ";
    cin >> roomNumber;

    Room* room = nullptr;
    for (auto& r : rooms) {
        if (r->roomNumber == roomNumber && r->isAvailable && !r->isUnderMaintenance) {
            room = r;
            break;
        }
    }

    if (!room) {
        cout << "Room not available!\n";
        cin.ignore();
        cin.get();
        return;
    }

    cout << "Enter Customer Details\n";
    cin.ignore();
    cout << "Name: ";
    getline(cin, name);
    cout << "Phone: ";
    getline(cin, phone);
    cout << "Email: ";
    getline(cin, email);
    cout << "Number of Days: ";
    cin >> days;

    Customer customer(name, phone, email, roomNumber);
    customers.push_back(customer);
    Reservation reservation(customer, room);

    // Calculate room charges
    reservation.calculateTotalBill(days);

    // Meal charges for the entire stay
    double mealChargePerDay = 550; // Single combo meal charge
    double totalMealCharge = mealChargePerDay * days;
    reservation.totalBill += totalMealCharge;

    reservations.push_back(reservation);
    room->isAvailable = false;

    // Display detailed reservation summary
    cout << "\n--- Reservation Details ---\n";
    cout << "Name: " << reservation.customer.name << endl;
    cout << "Phone: " << reservation.customer.phoneNumber << endl;
    cout << "Email: " << reservation.customer.email << endl;
    cout << "Room Number: " << reservation.room->roomNumber 
         << " (" << getRoomTypeString(reservation.room->type) << ")" << endl;
    cout << "Room Charges for " << days << " days: Rs. " << (room->pricePerNight * days) << endl;
    cout << "Meal Charges for " << days << " days: Rs. " << totalMealCharge << endl;
    cout << "Total Bill: Rs. " << reservation.totalBill << endl;
    cout << "-----------------------------\n";

    cin.ignore();
    cin.get();
}



    void checkOutCustomer() {
        int roomNumber;
        cout << "Enter Room Number for Checkout: ";
        cin >> roomNumber;

        auto it = find_if(reservations.begin(), reservations.end(), [roomNumber](const Reservation& r) {
            return r.room->roomNumber == roomNumber;
        });

        if (it != reservations.end()) {
            cout << "Final Bill: Rs. " << it->totalBill << endl;
            it->room->isAvailable = true;
            reservations.erase(it);
            cout << "Checkout successful!\n";
        } else {
            cout << "No reservation found for this room.\n";
        }
        cin.ignore();
        cin.get();
    }

    void adminMenu() {
        int choice;
        do {
            clearScreen();
            cout << "\nAdmin Menu:\n";
            cout << "1. Add Room\n";
            cout << "2. Check Out Customer\n";
            cout << "3. View Available Rooms\n";
            cout << "4. Exit\n";
            cout << "Enter choice: ";
            cin >> choice;

            clearScreen();
            switch (choice) {
                case 1: addRoom(); break;
                case 2: checkOutCustomer(); break;
                case 3: viewAvailableRooms(); break;
                case 4: break;
                default: 
                    cout << "Invalid choice! Try again.\n";
                    cin.ignore();
                    cin.get();
            }
        } while (choice != 4);
    }

    void customerMenu() {
        int choice;
        do {
            clearScreen();
            cout << "\nCustomer Menu:\n";
            cout << "1. Reserve Room\n";
            cout << "2. View Meal Menu\n";
            cout << "3. View Available Rooms\n";
            cout << "4. Exit\n";
            cout << "Enter choice: ";
            cin >> choice;

            clearScreen();
            switch (choice) {
                case 1: reserveRoom(); break;
                case 2: viewMealMenu(); break;
                case 3: viewAvailableRooms(); break;
                case 4: break;
                default: 
                    cout << "Invalid choice! Try again.\n";
                    cin.ignore();
                    cin.get();
            }
        } while (choice != 4);
    }

    void mainMenu() {
        int choice;
        do {
            clearScreen();
            cout << "\nHotel Management System:\n";
            cout << "1. Admin Menu\n";
            cout << "2. Customer Menu\n";
            cout << "3. Exit\n";
            cout << "Enter choice: ";
            cin >> choice;

            clearScreen();
            switch (choice) {
                case 1: adminMenu(); break;
                case 2: customerMenu(); break;
                case 3: break;
                default: 
                    cout << "Invalid choice! Try again.\n";
                    cin.ignore();
                    cin.get();
            }
        } while (choice != 3);
    }
};

int main() {
    HotelManagementSystem hms;
    hms.mainMenu();
    return 0;
}
