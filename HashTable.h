#ifndef HASHTABLE_H
#define HASHTABLE_H
#include <vector>
#include <string>
#include <cctype>
#include "Facility.h"
using namespace std;

class HashTable{
    private:
        struct HashNode{
            string key;
            Facility facility; // faciliity record
        };

        vector<vector<HashNode>> buckets;
        size_t capacity;
        size_t totalEntries;
        size_t totalCollisions;

        // hash function
        size_t hashFunction(const string& key) const{
            size_t hashValue = 0;
            for (char c : key){
                hashValue = hashValue * 31 + static_cast<size_t>(c);
            }
            return hashValue % capacity;
        }
    public:
        explicit HashTable(size_t initialCapacity = 11)
            : capacity(initialCapacity), totalEntries(0), totalCollisions(0){
                buckets.resize(capacity);
            }

            bool insert(const string& rawId, const Facility& facility, string& normalizedIdOut){
                normalizedIdOut = "";
                for(char c : rawId){
                    normalizedIdOut += static_cast<char>(toupper(static_cast<unsigned char>(c)));             
                }
                size_t index = hashFunction(normalizedIdOut);
                // check for duplicates
                for(const auto& node : buckets[index]){
                    if (node.key == normalizedIdOut){
                        return false;
                    }
                }
                // collisions for summary reporting
                if(!buckets[index].empty()){
                    totalCollisions++;
                }
                buckets[index].push_back({normalizedIdOut, facility});
                totalEntries++;
                return true;
            }
            const Facility* find(const string& rawId) const{
                string normalizedId = "";
                for (char c : rawId) {
                    normalizedId += static_cast<char>(toupper(static_cast<unsigned char>(c)));
                }
                size_t index = hashFunction(normalizedId);
                for (const auto& node : buckets[index]){
                    if(node.key == normalizedId){
                        return &(node.facility);
                    }
                }
                return nullptr;
            }
            size_t getCapacity() const { return capacity; }
            size_t getEntries() const { return totalEntries; }
            size_t getCollisions() const { return totalCollisions; }
};
#endif