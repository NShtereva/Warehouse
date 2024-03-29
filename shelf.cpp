#include "shelf.hpp"

#include <cassert>
#include <cstring>
#include <cmath>

Shelf::Shelf()
{
    this->allocate(Constants::INITIAL_CAPACITY);
    this->setCapacity(Constants::INITIAL_CAPACITY);
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
    assert(size >= Constants::INITIAL_CAPACITY && size <= Constants::MAX_SHELF_CAPACITY);

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
    assert(capacity >= Constants::INITIAL_CAPACITY && capacity <= Constants::MAX_SHELF_CAPACITY);
    this->capacity = capacity;
}

void Shelf::resize()
{
    Product** newArr = new(std::nothrow) Product*[this->capacity * Constants::INCREASE_STEP];
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

    this->setCapacity(this->capacity * Constants::INCREASE_STEP);
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
    int numberOfNewDivisions = ceil((double) product.getQuantity() / Constants::MAX_QUANTITY_IN_ONE_SHELF_DIVISION);

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

                if(this->products[i]->getQuantity() < Constants::MAX_QUANTITY_IN_ONE_SHELF_DIVISION)
                {
                    int diff = Constants::MAX_QUANTITY_IN_ONE_SHELF_DIVISION - this->products[i]->getQuantity();
                    this->products[i]->setQuantity(Constants::MAX_QUANTITY_IN_ONE_SHELF_DIVISION);
                    newQuantity = product.getQuantity() - diff;
                }
                else // this->products[i]->getQuantity() == MAX_QUANTITY_IN_ONE_SHELF_DIVISION
                {
                    newQuantity = product.getQuantity();
                }

                index = i + 1;
                while(newQuantity > Constants::MAX_QUANTITY_IN_ONE_SHELF_DIVISION)
                {
                    this->relocationInDescOrderOfIndices(this->size, index);

                    bool added = this->addProduct(product, index);
                    if(!added) return *this;

                    this->products[index]->setQuantity(Constants::MAX_QUANTITY_IN_ONE_SHELF_DIVISION);
                    newQuantity -= Constants::MAX_QUANTITY_IN_ONE_SHELF_DIVISION;
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

                if(product.getQuantity() <= Constants::MAX_QUANTITY_IN_ONE_SHELF_DIVISION)
                {
                    index = i + 1;
                    this->relocationInDescOrderOfIndices(this->size, index);
                }
                else
                {
                    int currQuantity = product.getQuantity();

                    while(currQuantity > Constants::MAX_QUANTITY_IN_ONE_SHELF_DIVISION)
                    {
                        index = i + 1;
                        this->relocationInAscOrderOfIndices(index + 1, this->size);

                        bool added = this->addProduct(product, index);
                        if(!added) return *this;

                        this->products[index]->setQuantity(Constants::MAX_QUANTITY_IN_ONE_SHELF_DIVISION);
                        currQuantity -= Constants::MAX_QUANTITY_IN_ONE_SHELF_DIVISION;
                        i++;
                    }

                    index = i + 1;
                    this->relocationInAscOrderOfIndices(index + 1, this->size);

                    newQuantity = currQuantity;
                }
            }
            else // product.getExpiryDate() < this->products[i]->getExpiryDate()
            {
                if(product.getQuantity() <= Constants::MAX_QUANTITY_IN_ONE_SHELF_DIVISION)
                {
                    index = i;
                    this->relocationInDescOrderOfIndices(this->size, index);
                }
                else
                {
                    int currQuantity = product.getQuantity();

                    while(currQuantity > Constants::MAX_QUANTITY_IN_ONE_SHELF_DIVISION)
                    {
                        index = i;
                        this->relocationInDescOrderOfIndices(this->size, index);

                        bool added = this->addProduct(product, index);
                        if(!added) return *this;

                        this->products[index]->setQuantity(Constants::MAX_QUANTITY_IN_ONE_SHELF_DIVISION);
                        currQuantity -= Constants::MAX_QUANTITY_IN_ONE_SHELF_DIVISION;
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
        if(product.getQuantity() > Constants::MAX_QUANTITY_IN_ONE_SHELF_DIVISION)
        {
            int currQuantity = product.getQuantity();

            while(currQuantity > Constants::MAX_QUANTITY_IN_ONE_SHELF_DIVISION)
            {
                bool added = this->addProduct(product, this->size);
                if(!added) return *this;

                this->products[this->size - 1]->setQuantity(Constants::MAX_QUANTITY_IN_ONE_SHELF_DIVISION);
                currQuantity -= Constants::MAX_QUANTITY_IN_ONE_SHELF_DIVISION;
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

// In the variable numbers will be kept the indices on which the product is found on this shelf. Memory 
// for numbers will be allocated only in case of insufficient quantity of the product, otherwise 
// numbers = nullptr and size = 0.
bool Shelf::removeProduct(const char* productName, const int quantity, int*& numbers, int& size, std::ostream& out)
{
    for(int i = 0; i < this->size; i++)
    {
        if(strcmp(this->products[i]->getName(), productName) == 0)
        {
            int currQuantity = this->products[i]->getQuantity();

            if(currQuantity > quantity)
            {
                this->products[i]->print(out);
                this->products[i]->setQuantity(currQuantity - quantity);

                numbers = nullptr;
                size = 0;

                return true;
            }
            else if(currQuantity < quantity && i + 1 >= this->size)
            {
                // Insufficient quantity
                size = 1;

                numbers = new(std::nothrow) int[size];
                if(!numbers)
                {
                    std::cout << "Memory not allocated successfully!\n";
                    size = 0;
                    return false;
                }

                numbers[0] = i;
                return false;
            }
            else if(currQuantity < quantity && i + 1 < this->size)
            {
                if(strcmp(this->products[i + 1]->getName(), productName) != 0)
                {
                    // Insufficient quantity
                    size = 1;

                    numbers = new(std::nothrow) int[size];
                    if(!numbers)
                    {
                        std::cout << "Memory not allocated successfully!\n";
                        size = 0;
                        return false;
                    }

                    numbers[0] = i;
                    return false;
                }

                size = 1;
                int quantityOnThisShelf = currQuantity, index = i + 1;

                while(index < this->size && strcmp(this->products[index]->getName(), productName) == 0)
                {
                    quantityOnThisShelf += this->products[index]->getQuantity();
                    index++; size++;
                }

                if(quantityOnThisShelf < quantity)
                {
                    // Insufficient quantity
                    numbers = new(std::nothrow) int[size];
                    if(!numbers)
                    {
                        std::cout << "Memory not allocated successfully!\n";
                        size = 0;
                        return false;
                    }

                    int p = 0;
                    for(int j = i; j < index && p < size; j++)
                    {
                        numbers[p] = j;
                        p++;
                    }

                    return false;
                }

                this->products[i]->print(out);
                this->deleteProduct(i);

                quantityOnThisShelf = currQuantity, index = i;
                
                while(strcmp(this->products[index]->getName(), productName) == 0)
                {
                    this->products[index]->print(out);
                    quantityOnThisShelf += this->products[index]->getQuantity();

                    if(quantityOnThisShelf > quantity)
                    {
                        this->products[index]->setQuantity(quantityOnThisShelf - quantity);

                        numbers = nullptr;
                        size = 0;

                        return true;
                    }
                    else if(quantityOnThisShelf == quantity)
                    {
                        this->deleteProduct(i);

                        numbers = nullptr;
                        size = 0;

                        return true;     
                    }
                    else
                    {
                        this->deleteProduct(index);
                    }
                }
            }
            else // currQuantity == quantity
            {
                this->products[i]->print(out);
                this->deleteProduct(i);

                numbers = nullptr;
                size = 0;

                return true;              
            }
        }
    }
    
    numbers = nullptr;
    size = 0;
    return false;
}

size_t Shelf::totalQuantityOfProduct(const Product& product) const
{
    int counter = 0;
    bool flag = false;

    for(int i = 0; i < this->size && !flag; i++)
    {
        if(*this->products[i] == product)
        {
            flag = true;
            counter += this->products[i]->getQuantity();
            i++;

            while(i < this->size && *this->products[i - 1] == *this->products[i])
            {
                counter += this->products[i]->getQuantity();
                i++;
            }
        }
    }

    return counter;
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
    out << shelf.size << std::endl;

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
