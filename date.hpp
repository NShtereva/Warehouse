#ifndef __DATE_HPP
#define __DATE_HPP

#include <iostream>
#include <fstream>

class Date
{
    private:
        unsigned int day;
        unsigned int month;
        unsigned int year;

        bool isLeapYear(unsigned int year) const;

    public:
        Date(unsigned int day = 1, unsigned int month = 1, unsigned int year = 1900);

        unsigned int getDay() const;
        unsigned int getMonth() const;
        unsigned int getYear() const;

        bool operator == (const Date& other) const;
        bool operator < (const Date& other) const;

        friend std::ostream& operator << (std::ostream& out, const Date& date);
        friend std::istream& operator >> (std::istream& in, Date& date);
};

#endif
