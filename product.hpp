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
        unsigned int location[3];
        char* comment;
    
    public:
        Product();
        Product(const char* name, const Date& expiryDate, const Date& dateOfEntry, const char* maker, const int quantity, 
                                                    const int section, const int shelf, const int number, const char* comment);
        Product(const Product& other);
        ~Product();

        Product& operator = (const Product& other);

        void setName(const char* name);
        void setExpiryDate(const Date& expiryDate);
        void setDateOfEntry(const Date& dateOfEntry);
        void setMaker(const char* maker);
        void setQuantity(const int quantity);
        void setLocation(const int section, const int shelf, const int number);
        void setComment(const char* comment);

        friend std::ostream& operator << (std::ostream& out, const Product& p);
        friend std::istream& operator >> (std::istream& in, Product& p);
};

#endif
