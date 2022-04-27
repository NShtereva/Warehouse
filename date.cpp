#include "date.hpp"

#include <cassert>

Date::Date(unsigned int day, unsigned int month, unsigned int year)
{
    this->setDate(day, month, year);
}

bool Date::isLeapYear(unsigned int year) const
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

unsigned int Date::getDay() const
{
    return this->day;
}

unsigned int Date::getMonth() const
{
    return this->month;
}

unsigned int Date::getYear() const
{
    return this->year;
}

void Date::setDate(const int day, const int month, const int year)
{
    assert(month >= 1 && month <= 12);

    if(this->isLeapYear(year) && month == 2)
    {
        assert(day >= 1 && day <= 29);
    }
    else if(!this->isLeapYear(year) && month == 2)
    {
        assert(day >= 1 && day <= 28);
    }

    if(month == 1 || month == 3 || month == 5 || month == 7 || 
            month == 8 || month == 10 || month == 12)
    {
        assert(day >= 1 && day <= 31);
    }
    else if(month == 4 || month == 6 || month == 9 || month == 11)
    {
        assert(day >= 1 && day <= 30);
    }
    
    this->day = day;
    this->month = month;
    this->year = year;
}

bool Date::operator == (const Date& other) const
{
    return this->day == other.day && 
           this->month == other.month && 
           this->year == other.year;
}

bool Date::operator < (const Date& other) const
{
    return this->year < other.year ||
           (this->year == other.year && this->month < other.month) ||
           (this->year == other.year && this->month == other.month && this->day < other.day);
}

std::ostream& operator << (std::ostream& out, const Date& date)
{
   out << date.day << "." << date.month << "." << date.year;
   return out;
}

std::istream& operator >> (std::istream& in, Date& date)
{
    in >> date.day;
    in.get();
    in >> date.month;
    in.get();
    in >> date.year;

    return in;
}
