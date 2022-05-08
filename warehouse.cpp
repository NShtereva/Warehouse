#include "warehouse.hpp"

#include <cassert>
#include <cstring>
#include <cmath>

const size_t INITIAL_CAPACITY = 2;
const size_t INCREASE_STEP = 2;

Warehouse::Warehouse()
{
    this->allocate(INITIAL_CAPACITY);
    this->setCapacity(INITIAL_CAPACITY);
    this->setSize(0);
}

Warehouse::Warehouse(const Warehouse& other)
{
    this->copy(other);
}

Warehouse::~Warehouse()
{
    this->deallocate();
}

Warehouse& Warehouse::operator = (const Warehouse& other)
{
    if(this != &other)
    {
        this->deallocate();
        this->copy(other);
    }

    return *this;
}

void Warehouse::allocate(const size_t size)
{
    assert(size >= INITIAL_CAPACITY);

    this->sections = new(std::nothrow) Section*[size];
    if(!this->sections)
    {
        std::cout << "Memory not allocated successfully!\n";
        return;
    }
}

void Warehouse::copy(const Warehouse& other)
{
    this->allocate(other.capacity);

    for(int i = 0; i < other.size; i++)
    {
        this->sections[i] = new(std::nothrow) Section(*other.sections[i]);
        if(!this->sections[i])
        {
            std::cout << "Memory not allocated successfully!\n";
            return;
        }
    }

    this->setCapacity(other.capacity);
    this->setSize(other.size);
}

void Warehouse::deallocate()
{
    for(int i = 0; i < this->size; i++)
    {
        delete this->sections[i];
    }
    delete[] this->sections;
    this->sections = nullptr;
}

void Warehouse::setSize(const int size)
{
    assert(size >= 0 && size <= this->capacity);
    this->size = size;
}

void Warehouse::setCapacity(const int capacity)
{
    assert(capacity >= INITIAL_CAPACITY);
    this->capacity = capacity;
}

void Warehouse::resize()
{
    Section** newArr = new(std::nothrow) Section*[this->capacity * INCREASE_STEP];
    if(!newArr)
    {
        std::cout << "Memory not allocated successfully!\n";
        return;
    }

    for(int i = 0; i < this->size; i++)
    {
        newArr[i] = this->sections[i];
    }

    delete[] this->sections;

    this->sections = newArr;
    newArr = nullptr;

    this->setCapacity(this->capacity * INCREASE_STEP);
}

bool Warehouse::allocateLocations(int**& locations, const int size)
{
    locations = new(std::nothrow) int*[size];
    if(!locations)
    {
        std::cout << "Memory not allocated successfully!\n";
        return false;
    }

    for(int i = 0; i < size; i++)
    {
        locations[i] = new(std::nothrow) int[3]; // section index, shelf index, product index
        if(!locations[i])
        {
            std::cout << "Memory not allocated successfully!\n";

            delete[] locations;
            locations = nullptr;

            return false;
        }
    }

    return true;
}

void Warehouse::deallocateLocations(int**& locations, const int size)
{
    for(int i = 0; i < size; i++)
    {
        delete[] locations[i];
    }
    delete[] locations;
    locations = nullptr;
}

size_t Warehouse::getSize() const
{
    return this->size;
}

Warehouse& Warehouse::operator += (const Section& section)
{
    if(this->size + 1 > this->capacity)
    {
        resize();
    }

    this->sections[this->size] = new(std::nothrow) Section(section);
    if(!this->sections[this->size])
    {
        std::cout << "Memory not allocated successfully!\n";
        return *this;
    }

    this->setSize(this->size + 1);

    int currSectionSize = (*this->sections[this->size - 1]).getSize();
    for(int i = 0; i < currSectionSize; i++)
    {
        int currShelfSize = (*this->sections[this->size - 1])[i].getSize();
        for(int j = 0; j < currShelfSize; j++)
        {
            (*this->sections[this->size - 1])[i][j].setSection(this->size);
        }
    }

    return *this;
}

bool Warehouse::addProduct(const Product& product)
{
    Product copy(product);

    if(this->size == 0)
    {
        copy.setSection(this->size + 1);
        
        Section* newSection = new(std::nothrow) Section();
        if(!newSection)
        {
            std::cout << "Memory not allocated successfully!\n";
            return false;
        }

        newSection->addProduct(copy);
        *this += *newSection;

        delete newSection;
        return true;
    }

    int currSectionSize, currShelfSize, numberOfNewDivisions = ceil((double) copy.getQuantity() / MAX_QUANTITY_IN_ONE_SHELF_DIVISION);

    for(int i = 0; i < this->size; i++)
    {
        currSectionSize = (*this->sections[i]).getSize();
        for(int j = 0; j < currSectionSize; j++)
        {
            currShelfSize = (*this->sections[i])[j].getSize(); 
            if(currShelfSize + numberOfNewDivisions <= MAX_SHELF_CAPACITY) 
            {
                copy.setSection(i + 1);
                (*this->sections[i])[j] += copy;
                return true;
            }
        }
    }

    if(currShelfSize + numberOfNewDivisions > MAX_SHELF_CAPACITY && currSectionSize + 1 <= MAX_SECTION_CAPACITY)
    {
        copy.setSection(this->size);

        Shelf* newShelf = new(std::nothrow) Shelf();
        if(!newShelf)
        {
            std::cout << "Memory not allocated successfully!\n";
            return false;
        }

        *newShelf += copy;

        *this->sections[this->size - 1] += *newShelf;
        currSectionSize++;

        delete newShelf;
        return true;
    }
    else if(currShelfSize + numberOfNewDivisions > MAX_SHELF_CAPACITY && currSectionSize + 1 > MAX_SECTION_CAPACITY)
    {
        copy.setSection(this->size);
        
        Section* newSection = new(std::nothrow) Section();
        if(!newSection)
        {
            std::cout << "Memory not allocated successfully!\n";
            return false;
        }

        newSection->addProduct(copy);
        *this += *newSection;

        delete newSection;
        return true;
    }
    
    return false;
}

bool Warehouse::removeProduct(const char* productName, const int quantity, int**& locations, int& _size, std::ostream& out)
{
    if(this->size == 0)
    {
        std::cout << "The warehouse is empty!" << std::endl;
        return false;
    }

    int** tempLocations;
    bool allocated = allocateLocations(tempLocations, quantity);
    if(!allocated)
    {
        locations = nullptr;
        _size = 0;
        return false;
    }

    _size = 0;
    int currSize = 0, **currLocations = nullptr;
    bool removed = false;

    for(int i = 0; i < this->size && !removed; i++)
    {
        bool flag = this->sections[i]->removeProduct(productName, quantity, currLocations, currSize, out);

        if(flag && currSize == 0 && currLocations == nullptr)
        {
            removed = true;
        }
        else if(!flag && currSize != 0 && currLocations != nullptr)
        {
            for(int j = 0; j < currSize; j++)
            {
                tempLocations[_size][0] = i;                        // section
                tempLocations[_size][1] = currLocations[j][0];      // shelf
                tempLocations[_size][2] = currLocations[j][1];      // number
                _size++;
            }

            deallocateLocations(currLocations, currSize);
        }
    }

    if(removed)
    {
        deallocateLocations(tempLocations, quantity);

        locations = nullptr;
        _size = 0;

        return true;
    }
    else
    {
        allocated = allocateLocations(locations, _size);
        if(!allocated)
        {
            locations = nullptr;
            _size = 0;
            return false;
        }

        for(int i = 0; i < _size; i++)
        {
            for(int j = 0; j < 3; j++)
            {
                locations[i][j] = tempLocations[i][j];
            }
        }

        deallocateLocations(tempLocations, quantity);
        return false;
    }
    
    return false;
}

size_t Warehouse::totalQuantityOfProduct(const Product& product) const
{
    int counter = 0;

    for(int i = 0; i < this->size; i++)
    {
        counter += this->sections[i]->totalQuantityOfProduct(product);
    }

    return counter;
}

Section& Warehouse::operator [] (int index)
{
    assert(index >= 0 && index < this->size);
    return *this->sections[index];
}

const Section Warehouse::operator [] (int index) const
{
    assert(index >= 0 && index < this->size);
    return *this->sections[index];
}

std::ostream& operator << (std::ostream& out, const Warehouse& warehouse)
{
    out << warehouse.size << "\n";

    for(int i = 0; i < warehouse.size; i++)
    {
        out << *warehouse.sections[i];
    }

    return out;
}

std::istream& operator >> (std::istream& in, Warehouse& warehouse)
{
    unsigned int size;
    in >> size;

    in.get();

    for(int i = 0; i < size; i++)
    {
        Section temp;
        in >> temp;
        warehouse += temp;
    }

    return in;
}
