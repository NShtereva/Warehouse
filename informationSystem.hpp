#ifndef __INFORMATIONSYSTEM_HPP
#define __INFORMATIONSYSTEM_HPP

#include "warehouse.hpp"
#include "constants.hpp"

class InformationSystem
{
    private:
        Warehouse warehouse;

        InformationSystem() = default;

        static void deallocateLocations(int**& locations, const int size);
        static char* processFileName(int day, int month, int year);
        static size_t getFileSize(const char* fileName);
        static void clearFileWithChanges();

        void availability() const;
        void addProduct();
        void removeProduct();
        static void referenceForChanges();
        void clear();

    public:
        InformationSystem(const InformationSystem& other) = delete;
        InformationSystem& operator = (const InformationSystem& other) = delete;

        static InformationSystem& getInstance();

        void useInformationSystem();
};

#endif
