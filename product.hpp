#ifndef __PRODUCT_HPP
#define __PRODUCT_HPP

#include "date.hpp"

#include <iostream>
#include <fstream>

class Product
{
    private:
        char* name;
        Date expiryDate;
        Date dateOfEntry;
        char* maker;
        size_t quantity;

        // location:
        unsigned int section;
        unsigned int shelf;
        unsigned int number;

        char* comment;

        void setName(const char* name);
        void setExpiryDate(const Date& expiryDate);
        void setDateOfEntry(const Date& dateOfEntry);
        void setMaker(const char* maker);
        void setComment(const char* comment);
    
    public:
        Product();
        Product(const char* name, const Date& expiryDate, const Date& dateOfEntry, const char* maker, const int quantity, 
                                                    const int section, const int shelf, const int number, const char* comment);
        Product(const Product& other);
        ~Product();

        Product& operator = (const Product& other);

        bool operator == (const Product& other) const;

        const char* getName() const;
        Date getExpiryDate() const;
        const char* getMaker() const;
        size_t getQuantity() const;

        void setQuantity(const int quantity);

        void setSection(const int section);
        void setShelf(const int shelf);
        void setNumber(const int number);

        void print() const;

        friend std::ostream& operator << (std::ostream& out, const Product& p);
        friend std::istream& operator >> (std::istream& in, Product& p);
};

#endif
