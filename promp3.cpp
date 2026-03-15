#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cmath>  // for ceil()
#include <iomanip>  // for formatting output (setw, left, etc.)
#include <limits> //for numeric_limits

using namespace std;

// constants for the panel types
const double areaperpanel = 54;  // area of one panel in square feet
const double efficiencyntype = 0.2267;  // n-type bifacial panel efficiency (22.67%)
const double efficiencyagrade = 0.195;  // a-grade solar panel efficiency (19.5%)
const double efficiencymonofacial = 0.21;  // mono facial panel efficiency (21%)
const double priceperpanelntype = 18135;  // price per n-type bifacial panel in rs
const double priceperpanelagrade = 13570;  // price per a-grade solar panel in rs
const double priceperpanelmonofacial = 15540;  // price per mono facial panel in rs
const double ratedoutputntype = 585;  // rated output of n-type bifacial panel in watts
const double ratedoutputagrade = 550;  // rated output of a-grade panel in watts
const double ratedoutputmonofacial = 560;  // rated output of mono facial panel in watts

// structure to store profile information
struct profile {
    string name;
    int password;
    int housesizevalue;
    string houseunit;
    int numrooms;
    int numappliances;
    string appliancenames[100];
    int appliancequantities[100];
    int applianceconsumption[100];
    int peakhours;
    int consumptionpercentage;
    double totalpowerconsumption;
    double energycovered;
    int panelchoice;
    int numpanels;
    double totalcost;
};

profile profiles[100];  // array to hold profiles
int profilecount = 0;  // track the number of profiles

// function to check if a string represents an integer
bool isinteger(const string& str) {
    for (char c : str) {
        if (!isdigit(c)) {
            return false;
        }
    }
    return true;
}

// function to validate integer inputs and ensure they're non-negative
int getvalidintinput(const string& prompt) {
    int input;
    string inputstr;
    while (true) {
        cout << prompt;
        cin >> inputstr;
        if (!isinteger(inputstr) || stoi(inputstr) < 0) {
            cout << "oops! please enter a valid positive integer.\n";
        }
        else {
            input = stoi(inputstr);
            return input;
        }
    }
}

// function to convert house size to square feet with input validation
int converttosquarefeet(int value, string unit) {
    transform(unit.begin(), unit.end(), unit.begin(), ::tolower);
    if (unit == "marla") {
        return value * 272.25;
    }
    else if (unit == "kanal") {
        return value * 6050;
    }
    else if (unit == "sqfeet") {
        return value;
    }
    else {
        while (unit != "marla" && unit != "kanal" && unit != "sqfeet") {
            cout << "invalid unit! please enter a valid unit (marla, kanal, sqfeet): ";
            cin >> unit;
            transform(unit.begin(), unit.end(), unit.begin(), ::tolower);
        }
        return converttosquarefeet(value, unit);
    }
}

// function to generate a unique numeric password (5 or 6 digits)
int generatepassword() {
    srand(time(0));  // seed for random number generation
    int password = rand() % 90000 + 10000;  // generate a 5-digit password (between 10000 and 99999)
    return password;
}

// function to get appliance details
void getappliancedetails(int numappliances, string appliancenames[], int appliancequantities[], int applianceconsumption[]) {
    cout << "enter the name and quantity for your appliances:\n";
    for (int i = 0; i < numappliances; ++i) {
        cout << "enter the name of appliance #" << i + 1 << ": ";
        cin.ignore();  // clear input buffer
        getline(cin, appliancenames[i]);
        appliancequantities[i] = getvalidintinput("enter how many number of " + appliancenames[i] + " do you have: ");
        applianceconsumption[i] = getvalidintinput("enter how many watts each " + appliancenames[i] + " consumes: ");
    }
}
void getprofiledetails() {
    cout << "\n🎉 welcome to the solar panel profile generator! 🎉\n";
    cout << "we're excited to help you find the perfect solar panel system for your home! 🌞 let's get started! 🚀\n";

    string profilename;
    cout << "enter a profile name: ";
    cin.ignore();  // clear input buffer
    getline(cin, profilename);

    bool profileexists = false;
    for (int i = 0; i < profilecount; ++i) {
        if (profiles[i].name == profilename) {
            cout << "profile name already exists! please choose another name.\n";
            profileexists = true;
            break;
        }
    }

    if (profileexists) return;

    profiles[profilecount].name = profilename;
    int password = generatepassword();
    profiles[profilecount].password = password;  // store the password
    cout << "generated password for this profile: " << password << endl;

    // gather house details
    profiles[profilecount].housesizevalue = getvalidintinput("enter the value of house size (number only): ");
    cout << "enter the unit (marla, kanal, sqfeet): ";
    cin >> profiles[profilecount].houseunit;

    profiles[profilecount].numrooms = getvalidintinput("how many rooms do you have in your house? ");
    profiles[profilecount].numappliances = getvalidintinput("how many appliances do you have? ");

    // get appliance details
    getappliancedetails(profiles[profilecount].numappliances, profiles[profilecount].appliancenames, profiles[profilecount].appliancequantities, profiles[profilecount].applianceconsumption);

    profiles[profilecount].peakhours = getvalidintinput("enter the number of peak sunlight hours per day: ");
    profiles[profilecount].consumptionpercentage = getvalidintinput("enter the percentage of energy consumption you want to cover with solar power: ");
}

// function to perform all calculations and set calculated values in the profile
void performcalculations() {
    int housesizeinsqfeet = converttosquarefeet(profiles[profilecount].housesizevalue, profiles[profilecount].houseunit);
    cout << "your house size in square feet is: " << housesizeinsqfeet << " sq. feet.\n";

    // estimate total power consumption
    double totalpowerconsumption = (profiles[profilecount].numrooms * 200);  // approximation: each room = 200 watts
    for (int i = 0; i < profiles[profilecount].numappliances; ++i) {
        totalpowerconsumption += profiles[profilecount].appliancequantities[i] * profiles[profilecount].applianceconsumption[i];  // total power consumption of appliances
    }
    profiles[profilecount].totalpowerconsumption = totalpowerconsumption;
    cout << "total power consumption is estimated to be: " << totalpowerconsumption << " watts.\n";

    // energy covered by solar panels
    double energycovered = totalpowerconsumption * (profiles[profilecount].consumptionpercentage / 100.0);
    profiles[profilecount].energycovered = energycovered;
    cout << "you need solar panels to cover " << energycovered << " watts of energy.\n";

    // apply additional cost based on the energy covered
    double additionalcost = 0.0;

    if (energycovered < 0) {
        std::cout << "error: energy covered cannot be negative." << std::endl;
    }
    else if (energycovered <= 1200) { // 0 to 1.2 kw
        additionalcost = 60000;
    }
    else if (energycovered <= 3000) { // 1.2 to 3 kw
        additionalcost = 90000;
    }
    else if (energycovered <= 4000) { // 3 to 4 kw
        additionalcost = 150000;
    }
    else if (energycovered <= 6000) { // 4 to 6 kw
        additionalcost = 240000;
    }
    else if (energycovered <= 9000) { // 6 to 9 kw
        additionalcost = 375000;
    }
    else if (energycovered <= 12000) { // 9 to 12 kw
        additionalcost = 800000;
    }
    else { // above 12 kw
        std::cout << "energy covered exceeds supported range." << std::endl;
    }

    // choose solar panel type
    cout << "choose your solar panel type:\n";
    cout << "1. n-type bifacial (585w, 22.67% efficiency) - rs 18135\n";
    cout << "2. a-grade panel (550w, 19.5% efficiency) - rs 13570\n";
    cout << "3. mono facial (560w, 21% efficiency) - rs 15540\n";
    int panelchoice = getvalidintinput("enter your choice (1-3): ");
    profiles[profilecount].panelchoice = panelchoice;

    double paneloutput;
    switch (panelchoice) {
    case 1:
        paneloutput = ratedoutputntype;
        break;
    case 2:
        paneloutput = ratedoutputagrade;
        break;
    case 3:
        paneloutput = ratedoutputmonofacial;
        break;
    default:
        cout << "invalid choice! defaulting to n-type bifacial.\n";
        paneloutput = ratedoutputntype;
        break;
    }

    // calculate number of panels required
    int numpanels = ceil(energycovered / paneloutput);
    profiles[profilecount].numpanels = numpanels;
    cout << "you need " << numpanels << " panels to cover your energy needs.\n";

    // calculate total cost including additional cost based on energy covered
    double totalcost;
    switch (panelchoice) {
    case 1:
        totalcost = numpanels * priceperpanelntype + additionalcost;
        break;
    case 2:
        totalcost = numpanels * priceperpanelagrade + additionalcost;
        break;
    case 3:
        totalcost = numpanels * priceperpanelmonofacial + additionalcost;
        break;
    }

    profiles[profilecount].totalcost = totalcost;
    cout << "the total cost for your solar panel system is: rs " << totalcost << endl;

    profilecount++;  // increment the profile count
    cout << "profile created successfully!\n";
}


// function to create a profile by calling the two helper functions
void createprofile() {
    // step 1: get all the profile details from the user
    getprofiledetails();

    // step 2: perform all calculations and finalize the profile details
    performcalculations();
}

void loadprofilesfromfile() {
    ifstream file("profiles.txt");
    if (!file.is_open()) {
        cout << "no profiles file found. starting fresh...\n";
        return;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        profile profile;
        getline(ss, profile.name, ',');
        ss >> profile.password;
        ss.ignore();
        ss >> profile.housesizevalue;
        ss.ignore();
        getline(ss, profile.houseunit, ',');
        ss >> profile.numrooms;
        ss.ignore();
        ss >> profile.numappliances;
        ss.ignore();

        for (int i = 0; i < profile.numappliances; ++i) {
            getline(ss, profile.appliancenames[i], ',');
            ss >> profile.appliancequantities[i];
            ss.ignore();
            ss >> profile.applianceconsumption[i];
            ss.ignore();
        }

        ss >> profile.peakhours;
        ss.ignore();
        ss >> profile.consumptionpercentage;
        ss.ignore();
        ss >> profile.totalpowerconsumption;
        ss.ignore();
        ss >> profile.energycovered;
        ss.ignore();
        ss >> profile.panelchoice;
        ss.ignore();
        ss >> profile.numpanels;
        ss.ignore();
        ss >> profile.totalcost;

        profiles[profilecount++] = profile;
    }

    cout << "profiles loaded successfully!\n";
}

void saveprofilestofile() {
    ofstream file("profiles.txt");
    if (!file.is_open()) {
        cerr << "error opening file for saving profiles.\n";
        return;
    }
    for (int i = 0; i < profilecount; ++i) {
        profile& profile = profiles[i];
        file << profile.name << ","
            << profile.password << ","
            << profile.housesizevalue << ","
            << profile.houseunit << ","
            << profile.numrooms << ","
            << profile.numappliances;

        for (int j = 0; j < profile.numappliances; ++j) {
            file << "," << profile.appliancenames[j] << ","
                << profile.appliancequantities[j] << ","
                << profile.applianceconsumption[j];
        }

        file << "," << profile.peakhours << ","
            << profile.consumptionpercentage << ","
            << profile.totalpowerconsumption << ","
            << profile.energycovered << ","
            << profile.panelchoice << ","
            << profile.numpanels << ","
            << profile.totalcost << endl;
    }
    file.close();
    cout << "profiles saved successfully!\n";
}

void viewprofile() {
    string name;
    cout << "enter the profile name to view: ";
    cin.ignore();  // clear input buffer
    getline(cin, name);

    bool found = false;
    for (int i = 0; i < profilecount; ++i) {
        if (profiles[i].name == name) {
            found = true;
            cout << "\n---- profile details ----\n";
            cout << "name: " << profiles[i].name << endl;
            cout << "house size: " << profiles[i].housesizevalue << " " << profiles[i].houseunit << endl;
            cout << "number of rooms: " << profiles[i].numrooms << endl;
            cout << "number of appliances: " << profiles[i].numappliances << endl;

            for (int j = 0; j < profiles[i].numappliances; ++j) {
                cout << "appliance #" << j + 1 << ": " << profiles[i].appliancenames[j]
                    << " (quantity: " << profiles[i].appliancequantities[j]
                    << ", consumption: " << profiles[i].applianceconsumption[j] << " watts)" << endl;
            }

            cout << "peak sunlight hours: " << profiles[i].peakhours << endl;
            cout << "energy coverage: " << profiles[i].energycovered << " watts\n";
            cout << "panel choice: " << profiles[i].panelchoice << endl;
            cout << "number of panels: " << profiles[i].numpanels << endl;
            cout << "total cost: rs " << profiles[i].totalcost << endl;
            break;
        }
    }

    if (!found) {
        cout << "profile not found.\n";
    }
}

// function to delete a profile with password check and confirmation
void deleteprofile() {
    string name;
    cout << "enter the profile name to delete: ";
    cin.ignore();  // clear input buffer
    getline(cin, name);

    bool found = false;
    for (int i = 0; i < profilecount; ++i) {
        if (profiles[i].name == name) {
            found = true;

            // ask for password before deleting
            int enteredpassword;
            cout << "enter the password for the profile: ";
            cin >> enteredpassword;

            if (enteredpassword == profiles[i].password) {
                // ask for confirmation before deleting
                char confirmation;
                cout << "are you sure you want to delete the profile \"" << profiles[i].name << "\"? (y/n): ";
                cin >> confirmation;

                if (confirmation == 'y' || confirmation == 'y') {
                    // shift all profiles after the deleted profile to fill the gap
                    for (int j = i; j < profilecount - 1; ++j) {
                        profiles[j] = profiles[j + 1];
                    }
                    profilecount--;  // decrease profile count
                    cout << "profile deleted successfully.\n";
                }
                else {
                    cout << "profile deletion cancelled.\n";
                }
            }
            else {
                cout << "incorrect password. profile not deleted.\n";
            }
            break;
        }
    }

    if (!found) {
        cout << "profile not found.\n";
    }
}


// function to update the profile
void updateprofile() {
    string profilename;
    cout << "enter profile name to update: ";
    cin.ignore();  // clear input buffer
    getline(cin, profilename);

    bool found = false;
    for (int i = 0; i < profilecount; ++i) {
        if (profiles[i].name == profilename) {
            found = true;
            cout << "profile found! let's update the details.\n";

            // ask if the user wants to update the profile name
            char updatechoice;
            cout << "do you want to update the profile name? (y/n): ";
            cin >> updatechoice;
            if (updatechoice == 'y' || updatechoice == 'y') {
                cout << "enter new profile name: ";
                cin.ignore();  // clear input buffer
                getline(cin, profiles[i].name);
            }

            // ask if the user wants to update the house size value
            cout << "do you want to update the house size value? (y/n): ";
            cin >> updatechoice;
            if (updatechoice == 'y' || updatechoice == 'y') {
                profiles[i].housesizevalue = getvalidintinput("enter new house size value: ");
                cout << "enter the unit (marla, kanal, sqfeet): ";
                cin >> profiles[i].houseunit;
            }

            // ask if the user wants to update the number of rooms
            cout << "do you want to update the number of rooms? (y/n): ";
            cin >> updatechoice;
            if (updatechoice == 'y' || updatechoice == 'y') {
                profiles[i].numrooms = getvalidintinput("enter the new number of rooms: ");
            }

            // ask if the user wants to update appliances
            cout << "do you want to update the appliances? (y/n): ";
            cin >> updatechoice;
            if (updatechoice == 'y' || updatechoice == 'y') {
                profiles[i].numappliances = getvalidintinput("enter the new number of appliances: ");
                // update appliance details
                getappliancedetails(profiles[i].numappliances, profiles[i].appliancenames, profiles[i].appliancequantities, profiles[i].applianceconsumption);
            }

            // ask if the user wants to update peak sunlight hours
            cout << "do you want to update the peak sunlight hours? (y/n): ";
            cin >> updatechoice;
            if (updatechoice == 'y' || updatechoice == 'y') {
                profiles[i].peakhours = getvalidintinput("enter the new peak sunlight hours per day: ");
            }

            // ask if the user wants to update energy consumption percentage
            cout << "do you want to update the energy consumption percentage? (y/n): ";
            cin >> updatechoice;
            if (updatechoice == 'y' || updatechoice == 'y') {
                profiles[i].consumptionpercentage = getvalidintinput("enter the new percentage of energy consumption you want to cover with solar power: ");
            }

            // now, call the calculation function to update the number of panels and total cost
            performcalculations();  // pass profile index to perform calculations on this specific profile

            cout << "profile updated successfully!\n";

            // after updating the profile, write the changes back to the file
            saveprofilestofile();  // save all updated profiles to the file
            break;
        }
    }

    if (!found) {
        cout << "profile not found.\n";
    }
}


int main() {
    int choice;
    loadprofilesfromfile(); // load profiles from file on startup

    while (true) {
        cout << "\nsolar panel profile management system\n";
        cout << "1. create a profile\n";
        cout << "2. view a profile\n";
        cout << "3. delete a profile\n";
        cout << "4. update a profile\n";
        cout << "5. exit\n";
        cout << "enter your choice: ";

        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "invalid input! please enter a number between 1 and 5.\n";
            continue;
        }

        switch (choice) {
        case 1:
            createprofile();
            saveprofilestofile(); //save after creating a profile
            break;
        case 2:
            viewprofile();
            break;
        case 3:
            deleteprofile();
            saveprofilestofile(); //save after deleting a profile
            break;
        case 4:
            updateprofile();
            saveprofilestofile(); //save after updating a profile
            break;
        case 5:
            cout << "exiting... goodbye!\n";
            return 0;
        default:
            cout << "invalid choice! please enter a valid number (1-4).\n";
        }
    }

    return 0;
}