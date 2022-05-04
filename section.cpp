#include "section.hpp"

#include <cassert>
#include <cmath>
#include <cstring>

const size_t INITIAL_CAPACITY = 2;
const size_t INCREASE_STEP = 2;

Section::Section()
{
    this->allocate(INITIAL_CAPACITY);
    this->setCapacity(INITIAL_CAPACITY);
    this->setSize(0);
}

Section::Section(const Section& other)
{
    this->copy(other);
}

Section::~Section()
{
    this->deallocate();
}

Section& Section::operator = (const Section& other)
{
    if(this != &other)
    {
        this->deallocate();
        this->copy(other);
    }

    return *this;
}

void Section::allocate(const size_t size)
{
    assert(size >= INITIAL_CAPACITY && size <= MAX_SECTION_CAPACITY);

    this->shelves = new(std::nothrow) Shelf*[size];
    if(!this->shelves)
    {
        std::cout << "Memory not allocated successfully!\n";
        return;
    }
}

void Section::copy(const Section& other)
{
    this->allocate(other.capacity);

    for(int i = 0; i < other.size; i++)
    {
        this->shelves[i] = new(std::nothrow) Shelf(*other.shelves[i]);
        if(!this->shelves[i])
        {
            std::cout << "Memory not allocated successfully!\n";
            return;
        }
    }

    this->setCapacity(other.capacity);
    this->setSize(other.size);
}

void Section::deallocate()
{
    for(int i = 0; i < this->size; i++)
    {
        delete this->shelves[i];
    }
    delete[] this->shelves;
    this->shelves = nullptr;
}

void Section::setSize(const int size)
{
    assert(size >= 0 && size <= this->capacity);
    this->size = size;
}

void Section::setCapacity(const int capacity)
{
    assert(capacity >= INITIAL_CAPACITY && capacity <= MAX_SECTION_CAPACITY);
    this->capacity = capacity;
}

void Section::resize()
{
    Shelf** newArr = new(std::nothrow) Shelf*[this->capacity * INCREASE_STEP];
    if(!newArr)
    {
        std::cout << "Memory not allocated successfully!\n";
        return;
    }

    for(int i = 0; i < this->size; i++)
    {
        newArr[i] = this->shelves[i];
    }

    delete[] this->shelves;

    this->shelves = newArr;
    newArr = nullptr;

    this->setCapacity(this->capacity * INCREASE_STEP);
}

bool Section::hasTheSameProduct(const Product& product, const int shelfNumber)
{
    int shelfSize = this->shelves[shelfNumber]->getSize();

    for(int i = 0; i < shelfSize; i++)
    {
        if((*this->shelves[shelfNumber])[i] == product && 
                    (*this->shelves[shelfNumber])[i].getExpiryDate() == product.getExpiryDate())
        {
            return true;
        }
    }

    return false;
}

size_t Section::getSize() const
{
    return this->size;
}

Section& Section::operator += (const Shelf& shelf)
{
    if(this->size + 1 > this->capacity)
    {
        resize();
    }

    this->shelves[this->size] = new(std::nothrow) Shelf(shelf);
    if(!this->shelves[this->size])
    {
        std::cout << "Memory not allocated successfully!\n";
        return *this;
    }

    this->setSize(this->size + 1);

    int currSize = (*this->shelves[this->size - 1]).getSize();
    for(int i = 0; i < currSize; i++)
    {
        (*this->shelves[this->size - 1])[i].setShelf(this->size);
    }

    return *this;
}

void Section::addProduct(Product& product)
{
    bool added = false;

    for(int i = 0; i < this->size && !added; i++)
    {
        int numberOfNewDivisions = ceil((double) product.getQuantity() / MAX_QUANTITY_IN_ONE_SHELF_DIVISION);

        if(this->shelves[i]->getSize() + numberOfNewDivisions <= MAX_SHELF_CAPACITY && hasTheSameProduct(product, i))
        {
            product.setShelf(i + 1);
            *this->shelves[i] += product;
            added = true;
        }
    }

    if(!added && this->size == 0)
    {
        Shelf newShelf;
        *this += newShelf;
        
        product.setShelf(1);
        *this->shelves[0] += product;

    }
    else if(!added && this->size > 0)
    {
        for(int i = 0; i < this->size; i++)
        {
            if(this->shelves[i]->getSize() <= MAX_SHELF_CAPACITY)
            {
                product.setShelf(i + 1);
                *this->shelves[i] += product;
            }
        }
    }
}

void Section::removeProduct(const char* productName, const int quantity)
{
    int numberOfDivisions = ceil((double) quantity / MAX_QUANTITY_IN_ONE_SHELF_DIVISION);

    int counter = 0;

    for(int i = 0; i < this->size; i++)
    {
        int currShelfSize = this->shelves[i]->getSize();

        for(int j = 0; j < currShelfSize; j++) 
        {
            if(strcmp((*this->shelves[i])[j].getName(), productName) == 0)
            {
                counter += (*this->shelves[i])[j].getQuantity();
            }
        }       
    }

    if(counter >= quantity)
    {
        counter = quantity;

        for(int i = 0; i < this->size && counter > 0; i++)
        {
            int currShelfSize = this->shelves[i]->getSize();

            int j = 0;
            bool atLeastOneRemoved = false, removed = false;

            while(j < currShelfSize && counter > 0 && !removed) 
            {
                if(strcmp((*this->shelves[i])[j].getName(), productName) == 0)
                {
                    int currQuantity = (*this->shelves[i])[j].getQuantity();
                    
                    if(currQuantity > counter)
                    {
                        (*this->shelves[i])[j].print();
                        (*this->shelves[i])[j].setQuantity(currQuantity - counter);
                        counter = 0;
                    }
                    else if(currQuantity == counter)
                    {
                        (*this->shelves[i]).removeProduct(productName, counter);
                        currShelfSize--;
                        counter = 0;
                    }
                    else // currQuantity < counter
                    {
                        (*this->shelves[i]).removeProduct(productName, currQuantity);
                        currShelfSize--;
                        counter -= currQuantity;
                    }
                    atLeastOneRemoved = true;
                }
                else 
                {
                    if(atLeastOneRemoved) removed = true;
                    j++;
                }
            }       
        }
    }
    else
    {
        // Insufficient quantity
        return;
    }

    if(counter > 0)
    {
        // Insufficient quantity
    }
}

Shelf& Section::operator [] (int index)
{
    assert(index >= 0 && index < this->size);
    return *this->shelves[index];
}

const Shelf Section::operator [] (int index) const
{
    assert(index >= 0 && index < this->size);
    return *this->shelves[index];
}

std::ostream& operator << (std::ostream& out, const Section& section)
{
    out << section.size << "\n";

    for(int i = 0; i < section.size; i++)
    {
        out << *section.shelves[i];
    }

    return out;
}

std::istream& operator >> (std::istream& in, Section& section)
{
    unsigned int size;
    in >> size;

    in.get();

    for(int i = 0; i < size; i++)
    {
        Shelf temp;
        in >> temp;
        section += temp;
    }

    return in;
}
