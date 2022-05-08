#ifndef __INFORMATIONSYSTEM_HPP
#define __INFORMATIONSYSTEM_HPP

#include "warehouse.hpp"

class InformationSystem
{
    private:
        Warehouse warehouse;

        static void deallocateLocations(int**& locations, const int size);
        static char* processFileName(int day, int month, int year);

        void availability() const;
        void addProduct();
        void removeProduct();
        void referenceForChanges() const;
        void clear();

    public:
        InformationSystem();
        void useInformationSystem();
};

#endif
