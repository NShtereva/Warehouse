#include "date.hpp"

#include <cassert>

Date::Date(const unsigned int day, const unsigned int month, const unsigned int year)
{
    this->setDate(day, month, year);
}

bool Date::isLeapYear(unsigned int year)
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

void Date::setDate(const unsigned int day, const unsigned int month, const unsigned int year)
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

Date& Date::operator += (const int days)
{
    assert(days >= 1 && days <= 28);

    this->day += days;

    if(this->day > 31 && (month == 1 || month == 3 || month == 5 || month == 7 || 
                                            month == 8 || month == 10 || month == 12))
    {
        this->day -= 31;
        if(this->month == 12)
        {
            this->month = 1;
            this->year++;
        }
        else this->month++;       
    }
    else if(this->day > 30 && (month == 4 || month == 6 || month == 9 || month == 11))
    {
        this->day -= 30;
        this->month++; 
    }
    else if(days > 29 && this->isLeapYear(year) && month == 2)
    {
        this->day -= 29;
        this->month++; 
    }
    else if(days > 28 && !this->isLeapYear(year) && month == 2)
    {
        this->day -= 28;
        this->month++; 
    }

    return *this;
}

std::ostream& operator << (std::ostream& out, const Date& date)
{
   out << date.day << "." << date.month << "." << date.year;
   return out;
}

std::istream& operator >> (std::istream& in, Date& date)
{
    unsigned int day, month, year;

    in >> day; in.get();
    in >> month; in.get();
    in >> year;

    date.setDate(day, month, year);

    return in;
}
