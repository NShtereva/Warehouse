#include "product.hpp"

#include <cassert>
#include <cstring>

const size_t MAX_NAME_LEN = 255;
const size_t MAX_MAKER_LEN = 255;
const size_t MAX_COMMENT_LEN = 10000;

Product::Product()
    : name(nullptr), expiryDate(Date()), dateOfEntry(Date()), maker(nullptr), comment(nullptr)
{
    this->setName("-");
    this->setMaker("-");
    this->setQuantity(0);
    this->setLocation(0, 0, 0);
    this->setComment("-");
}

Product::Product(const char* name, const Date& expiryDate, const Date& dateOfEntry, const char* maker, const int quantity, 
                                        const int section, const int shelf, const int number, const char* comment)
    : name(nullptr), expiryDate(expiryDate), dateOfEntry(dateOfEntry), maker(nullptr), comment(nullptr)
{
    this->setName(name);
    this->setMaker(maker);
    this->setQuantity(quantity);
    this->setLocation(section, shelf, number);
    this->setComment(comment);
}

Product::Product(const Product& other)
    : name(nullptr), expiryDate(other.expiryDate), dateOfEntry(other.dateOfEntry), maker(nullptr), comment(nullptr)
{
    this->setName(other.name);
    this->setMaker(other.maker);
    this->setQuantity(other.quantity);
    this->setLocation(other.location[0], other.location[1], other.location[2]);
    this->setComment(other.comment);
}

Product::~Product()
{
    delete[] this->name;
    this->name = nullptr;

    delete[] this->maker;
    this->maker = nullptr;

    delete[] this->comment;
    this->comment = nullptr;
}

Product& Product::operator = (const Product& other)
{
    if(this != &other)
    {
        this->setName(other.name);
        this->setExpiryDate(other.expiryDate);
        this->setDateOfEntry(other.dateOfEntry);
        this->setMaker(other.maker);
        this->setQuantity(other.quantity);
        this->setLocation(other.location[0], other.location[1], other.location[2]);
        this->setComment(other.comment);
    }

    return *this;
}

void Product::setName(const char* name)
{
    if(!name || strlen(name) > MAX_NAME_LEN) name = "";

    delete[] this->name;

    this->name = new(std::nothrow) char[strlen(name) + 1];
    if(!this->name)
    {
        std::cout << "Memory not allocated successfully!\n";
        return;
    }

    strcpy(this->name, name);
}

void Product::setExpiryDate(const Date& expiryDate)
{
    this->expiryDate.setDate(expiryDate.getDay(), expiryDate.getMonth(), expiryDate.getYear());
}

void Product::setDateOfEntry(const Date& dateOfEntry)
{
    this->dateOfEntry.setDate(dateOfEntry.getDay(), dateOfEntry.getMonth(), dateOfEntry.getYear());
}

void Product::setMaker(const char* maker)
{
    if(!maker || strlen(maker) > MAX_MAKER_LEN) maker = "";

    delete[] this->maker;

    this->maker = new(std::nothrow) char[strlen(maker) + 1];
    if(!this->maker)
    {
        std::cout << "Memory not allocated successfully!\n";
        return;
    }

    strcpy(this->maker, maker);
}

void Product::setQuantity(const int quantity)
{
    assert(quantity >= 0);
    this->quantity = quantity;
}

void Product::setLocation(const int section, const int shelf, const int number)
{
    assert(section >= 0);
    this->location[0] = section;

    assert(shelf >= 0);
    this->location[1] = shelf;

    assert(number >= 0);
    this->location[2] = number;
}

void Product::setComment(const char* comment)
{
    if(!comment || strlen(comment) > MAX_COMMENT_LEN) comment = "";

    delete[] this->comment;

    this->comment = new(std::nothrow) char[strlen(comment) + 1];
    if(!this->comment)
    {
        std::cout << "Memory not allocated successfully!\n";
        return;
    }

    strcpy(this->comment, comment);
}

std::ostream& operator << (std::ostream& out, const Product& p)
{
    out << p.name        << "\n" << p.expiryDate    << "\n" << p.dateOfEntry << "\n" 
        << p.maker       << "\n" << p.quantity      << "\n" 
        << p.location[0] << " "  << p.location[1]   << " "  << p.location[2] << "\n"
        << p.comment     << "\n";

    return out;
}

std::istream& operator >> (std::istream& in, Product& p)
{
    char buffer[MAX_COMMENT_LEN];

    in.getline(buffer, MAX_NAME_LEN);
    strncpy(p.name, buffer, strlen(buffer));
    p.name[strlen(buffer)] = '\0';

    in >> p.expiryDate >> p.dateOfEntry;

    in.get();

    in.getline(buffer, MAX_MAKER_LEN);
    strncpy(p.maker, buffer, strlen(buffer));
    p.maker[strlen(buffer)] = '\0';

    in >> p.quantity;
    in >> p.location[0] >> p.location[1] >> p.location[2];

    in.get();

    in.getline(buffer, MAX_COMMENT_LEN);
    strncpy(p.comment, buffer, strlen(buffer));
    p.comment[strlen(buffer)] = '\0';

    return in;
}
