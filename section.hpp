#ifndef __SECTION_HPP
#define __SECTION_HPP

#include "shelf.hpp"

const size_t MAX_SECTION_CAPACITY = 16;

class Section
{
    private:
        Shelf** shelves;
        size_t size;
        size_t capacity;

        void allocate(const size_t size);
        void copy(const Section& other);
        void deallocate();

        void setSize(const int size);
        void setCapacity(const int capacity);

        void resize();

        bool hasTheSameProduct(const Product& product, const int shelfNumber);

    public:
        Section();
        Section(const Section& other);
        ~Section();

        Section& operator = (const Section& other);

        size_t getSize() const;

        Section& operator += (const Shelf& shelf);

        void addProduct(const Product& product);
        bool removeProduct(const char* productName, const int quantity, int**& locations, int& size);

        Shelf& operator [] (int index);
        const Shelf operator [] (int index) const;

        friend std::ostream& operator << (std::ostream& out, const Section& section);
        friend std::istream& operator >> (std::istream& in, Section& section);
};

#endif
