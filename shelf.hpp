#ifndef __SHELF__HPP
#define __SHELF__HPP

#include "product.hpp"

const size_t MAX_SHELF_CAPACITY = 64;
const size_t MAX_QUANTITY_IN_ONE_SHELF_DIVISION = 9;

class Shelf
{
    private:
        Product** products;
        size_t size;
        size_t capacity;

        void allocate(const size_t size);
        void copy(const Shelf& other);
        void deallocate();

        void setSize(const int size);
        void setCapacity(const int capacity);

        void resize();

        bool addProduct(const Product& product, const int index);
        
        void relocationInAscOrderOfIndices(const int from, const int to);        
        void relocationInDescOrderOfIndices(const int from, const int to);

        void deleteProduct(const int index);
        
    public:
        Shelf();
        Shelf(const Shelf& other);
        ~Shelf();

        Shelf& operator = (const Shelf& other);

        size_t getSize() const;

        Shelf& operator += (const Product& product);

        void removeProduct(const char* productName, const int quantity);

        Product& operator [] (int index);
        const Product operator [] (int index) const;

        friend std::ostream& operator << (std::ostream& out, const Shelf& shelf);
        friend std::istream& operator >> (std::istream& in, Shelf& shelf);
};

#endif
