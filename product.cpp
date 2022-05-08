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

Date Product::getDateOfEntry() const
{
    return this->dateOfEntry;
}

const char* Product::getMaker() const
{
    return this->maker;
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

void Product::print(std::ostream& out) const
{
    out << "name: "            << this->name        << ", expiry date: " << this->expiryDate 
        << ", date Of entry: " << this->dateOfEntry << ", maker: "       << this->maker
        << ", quantity: "      << this->quantity    << ", section: "     << this->section
        << ", shelf: "         << this->shelf       << ", number: "      << this->number 
        << ", comment: "       << this->comment     << std::endl;
}

std::ostream& operator << (std::ostream& out, const Product& p)
{
    char ch = (&out == &std::cout) ? ' ' : '|'; 

    out << p.name    << ch   << p.expiryDate << " " << p.dateOfEntry << " " 
        << p.maker   << ch   << p.quantity   << " " 
        << p.section << " "  << p.shelf      << " " << p.number      << " "
        << p.comment << ch   << "\n";

    return out;
}

std::istream& operator >> (std::istream& in, Product& p)
{
    char buffer[MAX_COMMENT_LEN];

    char delim = (&in == &std::cin) ? '\n' : '|'; 
    
    if(&in == &std::cin) 
        std::cout << "name: ";
    in.getline(buffer, MAX_NAME_LEN, delim);
    p.setName(buffer);

    if(&in == &std::cin) 
        std::cout << "expiry date: ";
    Date expiryDate; in >> expiryDate;
    p.setExpiryDate(expiryDate);

    if(&in == &std::cin) 
        std::cout << "date Of entry: ";
    Date dateOfEntry; in >> dateOfEntry;
    p.setDateOfEntry(dateOfEntry);

    in.get();

    if(&in == &std::cin) 
        std::cout << "maker: ";
    in.getline(buffer, MAX_MAKER_LEN, delim);
    p.setMaker(buffer);

    if(&in == &std::cin) 
        std::cout << "quantity: ";
    int quantity; in >> quantity;
    p.setQuantity(quantity);

    if(&in != &std::cin) 
        in >> p.section >> p.shelf >> p.number;

    in.get();

    if(&in == &std::cin) 
        std::cout << "comment: ";
    in.getline(buffer, MAX_COMMENT_LEN, delim);
    p.setComment(buffer);

    if(&in != &std::cin) 
        in.get();

    return in;
}
