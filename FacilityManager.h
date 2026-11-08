#ifndef FACILITY_MANAGER_H
#define FACILITY_MANAGER_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cctype>
#include "Facility.h"
#include "HashTable.h"
using namespace std;

class FacilityManager {
    private:
        HashTable facilityTable;
        vector<Facility> facilityList;

        static string trim(const string& str) {
            size_t first = str.find_first_not_of(" \t\r\n");
            if(first == string::npos) return "";
            size_t last = str.find_last_not_of(" \t\r\n");
            return str.substr(first, (last - first + 1));
        }
        
        static bool equalsIgnoreCase(const string&  a, const string& b) {
        if (a.length() != b.length()) return false;
        for (size_t i = 0; i < a.length(); ++i) {
            if (tolower(static_cast<unsigned char>(a[i])) != 
                tolower(static_cast<unsigned char>(b[i]))) {
                return false;
            }
        }
        return true;
    }

public:
    explicit FacilityManager(size_t hashCapacity = 11) : facilityTable(hashCapacity) {}

    // parses and validates facilities.txt against accepted buildings
    void loadFromFile(const string& filepath, 
                      const vector<string>& acceptedBuildings, 
                      int& acceptedOut, int& rejectedOut) {
        acceptedOut = 0;
        rejectedOut = 0;

        ifstream file(filepath);
        if (!file.is_open()) {
            cout << "Error: Missing file " << filepath << ". Treated as empty.\n";
            return;
        }

        string line;
        int lineNum = 0;
        while (getline(file, line)) {
            lineNum++;
            if (line.empty() || line[0] == '#') continue; // skips comments/blanks

            stringstream ss(line);
            string idStr, nameStr, bldgStr, catStr;
            
            if (getline(ss, idStr, '|') && 
                getline(ss, nameStr, '|') && 
                getline(ss, bldgStr, '|') && 
                getline(ss, catStr)) {
                
                idStr = trim(idStr);
                nameStr = trim(nameStr);
                bldgStr = trim(bldgStr);
                catStr = trim(catStr);

                // validate ID format: F + 3 digits
                bool validId = (idStr.length() == 4 && 
                                (idStr[0] == 'F' || idStr[0] == 'f') &&
                                isdigit(idStr[1]) && 
                                isdigit(idStr[2]) && 
                                isdigit(idStr[3]));

                // Validate building reference
                bool validBldg = false;
                string matchedBldg = "";
                for (const auto& b : acceptedBuildings) {
                    if (equalsIgnoreCase(b, bldgStr)) {
                        validBldg = true;
                        matchedBldg = b; 
                        break;
                    }
                }

                if (!validId) {
                    cout << "Warning [facilities.txt:" << lineNum << "]: Invalid ID format (must be F plus 3 digits).\n";
                    rejectedOut++;
                } else if (!validBldg) {
                    cout << "Warning [facilities.txt:" << lineNum << "]: Unknown building reference '" << bldgStr << "'.\n";
                    rejectedOut++;
                } else {
                    Facility fac{idStr, nameStr, matchedBldg, catStr};
                    string normId;
                    if (facilityTable.insert(idStr, fac, normId)) {
                        fac.id = normId;
                        facilityList.push_back(fac);
                        acceptedOut++;
                    } else {
                        cout << "Warning [facilities.txt:" << lineNum << "]: Duplicate facility ID.\n";
                        rejectedOut++;
                    }
                }
            } else {
                cout << "Warning [facilities.txt:" << lineNum << "]: Malformed line.\n";
                rejectedOut++;
            }
        }
        file.close();
    }

    // view all facilities in fileorder
    void viewAllFacilities() const {
        if (facilityList.empty()) {
            cout << "No facilities recorded.\n";
            return;
        }
        for (const auto& f : facilityList) {
            cout << f.id << " | " << f.name << " | " << f.building << " | " << f.category << "\n";
        }
        cout << "Total facilities: " << facilityList.size() << "\n";
    }

    // searches facility by ID using the custom hash table.
    void searchFacilityById(const string& rawInputId) const {
        string searchId = trim(rawInputId);

        bool validFormat = (searchId.length() == 4 && 
                            (searchId[0] == 'F' || searchId[0] == 'f') &&
                            isdigit(searchId[1]) && 
                            isdigit(searchId[2]) && 
                            isdigit(searchId[3]));

        if (!validFormat) {
            cout << "Invalid format: an ID is F followed by exactly three digits.\n";
            return;
        }

        string normId = "";
        for (char c : searchId) {
            normId += static_cast<char>(toupper(static_cast<unsigned char>(c)));
        }

        const Facility* found = facilityTable.find(normId);
        if (found) {
            cout << "Found: " << found->name << " | " << found->building << " | " << found->category << "\n";
        } else {
            cout << "Not found: no facility with ID " << normId << "\n";
        }
    }

    // searches facilities by names and if there is a duplicate, then it'll handle it
    void searchByName(const std::string& rawName) const {
        std::string target = trim(rawName);
        if (target.empty()) {
            cout << "Error: Blank name entered.\n";
            return;
        }

        int matchCount = 0;
        for (const auto& f : facilityList) {
            if (equalsIgnoreCase(f.name, target)) {
                cout << f.id << " | " << f.name << " | " << f.building << " | " << f.category << "\n";
                matchCount++;
            }
        }
        cout << "Matches: " << matchCount << "\n";
    }

    // searches facilities by categort 
    void searchByCategory(const string& rawCategory) const {
        string target = trim(rawCategory);
        if (target.empty()) {
            cout << "Error: Blank category entered.\n";
            return;
        }

        int matchCount = 0;
        for (const auto& f : facilityList) {
            if (equalsIgnoreCase(f.category, target)) {
                cout << f.id << " | " << f.name << " | " << f.building << " | " << f.category << "\n";
                matchCount++;
            }
        }
        cout << "Matches: " << matchCount << "\n";
    }

    // categor summary report showing counts per category.
    void generateCategoryReport() const {
        if (facilityList.empty()) {
            cout << "No facilities recorded.\n";
            return;
        }

        vector<string> categories;
        vector<int> counts;

        for (const auto& f : facilityList) {
            bool found = false;
            for (size_t i = 0; i < categories.size(); ++i) {
                if (equalsIgnoreCase(categories[i], f.category)) {
                    counts[i]++;
                    found = true;
                    break;
                }
            }
            if (!found) {
                categories.push_back(f.category);
                counts.push_back(1);
            }
        }

        cout << "\n    Facility Category Summary Report    \n";
        for (size_t i = 0; i < categories.size(); ++i) {
            cout << categories[i] << ": " << counts[i] << " facility(ies)\n";
        }
        cout << "Total categories: " << categories.size() << "\n";
    }

    //getters for load summary printout
    size_t getHashCapacity() const { return facilityTable.getCapacity(); }
    size_t getHashEntries() const { return facilityTable.getEntries(); }
    size_t getHashCollisions() const { return facilityTable.getCollisions(); }
};
#endif
