#include "shelf.hpp"

#include <cassert>
#include <cstring>
#include <cmath>

const size_t INITIAL_CAPACITY = 2;
const size_t INCREASE_STEP = 2;

Shelf::Shelf()
{
    this->allocate(INITIAL_CAPACITY);
    this->setCapacity(INITIAL_CAPACITY);
    this->setSize(0);
}

Shelf::Shelf(const Shelf& other)
{
    this->copy(other);
}

Shelf::~Shelf()
{
    this->deallocate();
}

Shelf& Shelf::operator = (const Shelf& other)
{
    if(this != &other)
    {
        this->deallocate();
        this->copy(other);
    }

    return *this;
}

void Shelf::allocate(const size_t size)
{
    assert(size >= INITIAL_CAPACITY && size <= MAX_SHELF_CAPACITY);

    this->products = new(std::nothrow) Product*[size];
    if(!this->products)
    {
        std::cout << "Memory not allocated successfully!\n";
        return;
    }
}

void Shelf::copy(const Shelf& other)
{
    this->allocate(other.capacity);

    for(int i = 0; i < other.size; i++)
    {
        this->products[i] = new(std::nothrow) Product(*other.products[i]);
        if(!this->products[i])
        {
            std::cout << "Memory not allocated successfully!\n";
            return;
        }
    }

    this->setCapacity(other.capacity);
    this->setSize(other.size);
}

void Shelf::deallocate()
{
    for(int i = 0; i < this->size; i++)
    {
        delete this->products[i];
    }
    delete[] this->products;
    this->products = nullptr;
}

void Shelf::setSize(const int size)
{
    assert(size >= 0 && size <= this->capacity);
    this->size = size;
}

void Shelf::setCapacity(const int capacity)
{
    assert(capacity >= INITIAL_CAPACITY && capacity <= MAX_SHELF_CAPACITY);
    this->capacity = capacity;
}

void Shelf::resize()
{
    Product** newArr = new(std::nothrow) Product*[this->capacity * INCREASE_STEP];
    if(!newArr)
    {
        std::cout << "Memory not allocated successfully!\n";
        return;
    }

    for(int i = 0; i < this->size; i++)
    {
        newArr[i] = this->products[i];
    }

    delete[] this->products;

    this->products = newArr;
    newArr = nullptr;

    this->setCapacity(this->capacity * INCREASE_STEP);
}

bool Shelf::addProduct(const Product& product, const int index)
{
    this->products[index] = new(std::nothrow) Product(product);
    if(!this->products[index])
    {
        std::cout << "Memory not allocated successfully!\n";
        return false;
    }

    this->products[index]->setNumber(index + 1);
    this->setSize(this->size + 1);

    return true;
}

void Shelf::relocationInAscOrderOfIndices(const int from, const int to)
{
    for(int j = from; j < to; j++)
    {
        this->products[j] = this->products[j - 1];
        this->products[j]->setNumber(j + 1);
    }
}

void Shelf::relocationInDescOrderOfIndices(const int from, const int to)
{
    for(int j = from; j > to; j--)
    {
        this->products[j] = this->products[j - 1];
        this->products[j]->setNumber(j + 1);
    }
}

void Shelf::deleteProduct(const int index)
{
    Product* save = this->products[index];
    for(int j = index; j < this->size - 1; j++)
    {
        this->products[j] = this->products[j + 1];
        this->products[j]->setNumber(j + 1);
    }
    delete save;

    this->setSize(this->size - 1);  
}

size_t Shelf::getSize() const
{
    return this->size;
}

Shelf& Shelf::operator += (const Product& product)
{
    int numberOfNewDivisions = ceil((double) product.getQuantity() / MAX_QUANTITY_IN_ONE_SHELF_DIVISION);

    while(this->size + numberOfNewDivisions > this->capacity)
    {
        resize();
    }

    int index = -1, newQuantity = -1;
    bool isUnaddedProduct = true;

    for(int i = 0; i < this->size && index == -1; i++)
    {
        if(*this->products[i] == product)
        {
            isUnaddedProduct = false;

            if(this->products[i]->getExpiryDate() == product.getExpiryDate())
            {
                while(i + 1 < this->size && *this->products[i + 1] == product && 
                            this->products[i + 1]->getExpiryDate() == product.getExpiryDate())
                {
                    i++;
                }

                if(this->products[i]->getQuantity() < MAX_QUANTITY_IN_ONE_SHELF_DIVISION)
                {
                    int diff = MAX_QUANTITY_IN_ONE_SHELF_DIVISION - this->products[i]->getQuantity();
                    this->products[i]->setQuantity(MAX_QUANTITY_IN_ONE_SHELF_DIVISION);
                    newQuantity = product.getQuantity() - diff;
                }
                else // this->products[i]->getQuantity() == MAX_QUANTITY_IN_ONE_SHELF_DIVISION
                {
                    newQuantity = product.getQuantity();
                }

                index = i + 1;
                while(newQuantity > MAX_QUANTITY_IN_ONE_SHELF_DIVISION)
                {
                    this->relocationInDescOrderOfIndices(this->size, index);

                    bool added = this->addProduct(product, index);
                    if(!added) return *this;

                    this->products[index]->setQuantity(MAX_QUANTITY_IN_ONE_SHELF_DIVISION);
                    newQuantity -= MAX_QUANTITY_IN_ONE_SHELF_DIVISION;
                    i++;
                    index++;
                }

                this->relocationInDescOrderOfIndices(this->size, index);               
            }
            else if(this->products[i]->getExpiryDate() < product.getExpiryDate())
            {
                while(i + 1 < this->size && *this->products[i + 1] == product && 
                      (this->products[i + 1]->getExpiryDate() < product.getExpiryDate() || 
                                product.getExpiryDate() == this->products[i + 1]->getExpiryDate()))
                {
                    i++;
                }

                if(product.getQuantity() <= MAX_QUANTITY_IN_ONE_SHELF_DIVISION)
                {
                    index = i + 1;
                    this->relocationInDescOrderOfIndices(this->size, index);
                }
                else
                {
                    int currQuantity = product.getQuantity();

                    while(currQuantity > MAX_QUANTITY_IN_ONE_SHELF_DIVISION)
                    {
                        index = i + 1;
                        this->relocationInAscOrderOfIndices(index + 1, this->size);

                        bool added = this->addProduct(product, index);
                        if(!added) return *this;

                        this->products[index]->setQuantity(MAX_QUANTITY_IN_ONE_SHELF_DIVISION);
                        currQuantity -= MAX_QUANTITY_IN_ONE_SHELF_DIVISION;
                        i++;
                    }

                    index = i + 1;
                    this->relocationInAscOrderOfIndices(index + 1, this->size);

                    newQuantity = currQuantity;
                }
            }
            else // product.getExpiryDate() < this->products[i]->getExpiryDate()
            {
                if(product.getQuantity() <= MAX_QUANTITY_IN_ONE_SHELF_DIVISION)
                {
                    index = i;
                    this->relocationInDescOrderOfIndices(this->size, index);
                }
                else
                {
                    int currQuantity = product.getQuantity();

                    while(currQuantity > MAX_QUANTITY_IN_ONE_SHELF_DIVISION)
                    {
                        index = i;
                        this->relocationInDescOrderOfIndices(this->size, index);

                        bool added = this->addProduct(product, index);
                        if(!added) return *this;

                        this->products[index]->setQuantity(MAX_QUANTITY_IN_ONE_SHELF_DIVISION);
                        currQuantity -= MAX_QUANTITY_IN_ONE_SHELF_DIVISION;
                        i++;
                    }

                    index = i;
                    this->relocationInDescOrderOfIndices(this->size, index);

                    newQuantity = currQuantity;
                }
            }
        }
    }

    if(this->size == 0 || isUnaddedProduct)
    {
        if(product.getQuantity() > MAX_QUANTITY_IN_ONE_SHELF_DIVISION)
        {
            int currQuantity = product.getQuantity();

            while(currQuantity > MAX_QUANTITY_IN_ONE_SHELF_DIVISION)
            {
                bool added = this->addProduct(product, this->size);
                if(!added) return *this;

                this->products[this->size - 1]->setQuantity(MAX_QUANTITY_IN_ONE_SHELF_DIVISION);
                currQuantity -= MAX_QUANTITY_IN_ONE_SHELF_DIVISION;
            }
            
            newQuantity = currQuantity;
        }
    }

    index = (index == -1) ? this->size : index;

    bool added = this->addProduct(product, index);
    if(!added) return *this;

    if(newQuantity != -1)  this->products[index]->setQuantity(newQuantity);

    return *this;
}

void Shelf::removeProduct(const char* productName, const int quantity)
{
    for(int i = 0; i < this->size; i++)
    {
        if(strcmp(this->products[i]->getName(), productName) == 0)
        {
            int currQuantity = this->products[i]->getQuantity();

            if(currQuantity > quantity)
            {
                this->products[i]->print();
                this->products[i]->setQuantity(currQuantity - quantity);
                return;
            }
            else if(currQuantity < quantity)
            {
                if(strcmp(this->products[i + 1]->getName(), productName) != 0)
                {
                    // Insufficient quantity
                    return;
                }

                int counter = currQuantity, index = i + 1;

                while(index < this->size && strcmp(this->products[index]->getName(), productName) == 0)
                {
                    counter += this->products[index]->getQuantity();
                    index++;
                }

                if(counter < quantity)
                {
                    // Insufficient quantity
                    return;
                }

                this->products[i]->print();
                this->deleteProduct(i);

                counter = currQuantity, index = i;
                
                while(strcmp(this->products[index]->getName(), productName) == 0)
                {
                    this->products[index]->print();
                    counter += this->products[index]->getQuantity();

                    if(counter > quantity)
                    {
                        this->products[index]->setQuantity(counter - quantity);
                        return;
                    }
                    else if(counter == quantity)
                    {
                        this->deleteProduct(i);
                        return;     
                    }
                    else
                    {
                        this->deleteProduct(index);
                    }
                }
            }
            else
            {
                this->products[i]->print();
                this->deleteProduct(i);
                return;              
            }
        }
    }
}

Product& Shelf::operator [] (int index)
{
    assert(index >= 0 && index < this->size);
    return *this->products[index];
}

const Product Shelf::operator [] (int index) const
{
    assert(index >= 0 && index < this->size);
    return *this->products[index];
}

std::ostream& operator << (std::ostream& out, const Shelf& shelf)
{
    out << shelf.size << "\n";

    for(int i = 0; i < shelf.size; i++)
    {
        out << *shelf.products[i];
    }

    return out;
}

std::istream& operator >> (std::istream& in, Shelf& shelf)
{
    unsigned int size;
    in >> size;

    in.get();

    for(int i = 0; i < size; i++)
    {
        Product temp;
        in >> temp;
        shelf += temp;
    }

    return in;
}
