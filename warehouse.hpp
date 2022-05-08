#ifndef __WAREHOUSE_HPP
#define __WAREHOUSE_HPP

#include "section.hpp"

class Warehouse
{
    private:
        Section** sections;
        size_t size;
        size_t capacity;

        void allocate(const size_t size);
        void copy(const Warehouse& other);
        void deallocate();

        void setSize(const int size);
        void setCapacity(const int capacity);

        void resize();

        static bool allocateLocations(int**& locations, const int size);
        static void deallocateLocations(int**& locations, const int size);

    public:
        Warehouse();
        Warehouse(const Warehouse& other);
        ~Warehouse();

        Warehouse& operator = (const Warehouse& other);

        size_t getSize() const;

        Warehouse& operator += (const Section& section);

        bool addProduct(const Product& product);
        bool removeProduct(const char* productName, const int quantity, int**& locations, 
                                                        int& _size, std::ostream& out = std::cout);

        size_t totalQuantityOfProduct(const Product& product) const;

        Section& operator [] (int index);
        const Section operator [] (int index) const;

        friend std::ostream& operator << (std::ostream& out, const Warehouse& warehouse);
        friend std::istream& operator >> (std::istream& in, Warehouse& warehouse);
};

#endif
