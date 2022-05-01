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
    this->setSection(0);
    this->setShelf(0);
    this->setNumber(0);
    this->setComment("-");
}

Product::Product(const char* name, const Date& expiryDate, const Date& dateOfEntry, const char* maker, const int quantity, 
                                        const int section, const int shelf, const int number, const char* comment)
    : name(nullptr), expiryDate(expiryDate), dateOfEntry(dateOfEntry), maker(nullptr), comment(nullptr)
{
    this->setName(name);
    this->setMaker(maker);
    this->setQuantity(quantity);
    this->setSection(section);
    this->setShelf(shelf);
    this->setNumber(number);
    this->setComment(comment);
}

Product::Product(const Product& other)
    : name(nullptr), expiryDate(other.expiryDate), dateOfEntry(other.dateOfEntry), maker(nullptr), comment(nullptr)
{
    this->setName(other.name);
    this->setMaker(other.maker);
    this->setQuantity(other.quantity);
    this->setSection(other.section);
    this->setShelf(other.shelf);
    this->setNumber(other.number);
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
        this->setSection(other.section);
        this->setShelf(other.shelf);
        this->setNumber(other.number);
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

bool Product::operator == (const Product& other) const
{
    return strcmp(this->name, other.name) == 0 && strcmp(this->maker, other.maker) == 0;
}

const char* Product::getName() const
{
    return this->name;
}

Date Product::getExpiryDate() const
{
    return this->expiryDate;
}

size_t Product::getQuantity() const
{
    return this->quantity;
}

void Product::setQuantity(const int quantity)
{
    assert(quantity >= 0);
    this->quantity = quantity;
}

void Product::setSection(const int section)
{
    assert(section >= 0);
    this->section = section;
}

void Product::setShelf(const int shelf)
{
    assert(shelf >= 0);
    this->shelf = shelf;
}

void Product::setNumber(const int number)
{
    assert(number >= 0);
    this->number = number;
}

void Product::print() const
{
    std::cout << "name: "            << this->name        << ", expiry date: " << this->expiryDate 
              << ", date Of Entry: " << this->dateOfEntry << ", maker: "       << this->maker
              << ", quantity: "      << this->quantity    << ", section: "     << this->section
              << ", shelf: "         << this->shelf       << ", number: "      << this->number 
              << ", comment: "       << this->comment     << std::endl;
}

std::ostream& operator << (std::ostream& out, const Product& p)
{
    out << p.name    << "| " << p.expiryDate << " " << p.dateOfEntry << " " 
        << p.maker   << "| " << p.quantity   << " " 
        << p.section << " "  << p.shelf      << " " << p.number      << " "
        << p.comment << "|\n";

    return out;
}

std::istream& operator >> (std::istream& in, Product& p)
{
    char buffer[MAX_COMMENT_LEN];

    in.getline(buffer, MAX_NAME_LEN, '|');
    strncpy(p.name, buffer, strlen(buffer));
    p.name[strlen(buffer)] = '\0';

    in >> p.expiryDate >> p.dateOfEntry;

    in.getline(buffer, MAX_MAKER_LEN, '|');
    strncpy(p.maker, buffer, strlen(buffer));
    p.maker[strlen(buffer)] = '\0';

    in >> p.quantity;
    in >> p.section >> p.shelf >> p.number;

    in.getline(buffer, MAX_COMMENT_LEN, '|');
    strncpy(p.comment, buffer, strlen(buffer));
    p.comment[strlen(buffer)] = '\0';

    return in;
}
