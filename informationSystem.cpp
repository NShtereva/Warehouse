#include "informationSystem.hpp"

#include <cstring>
#include <cmath>

const size_t MAX_NAME_LEN = 255;
const size_t MAX_BUFFER_LEN = 10000;
const size_t MAX_OPERATION_LEN = 20;

InformationSystem::InformationSystem() : warehouse()
{ }

void InformationSystem::deallocateLocations(int**& locations, const int size)
{
    for(int i = 0; i < size; i++)
    {
        delete[] locations[i];
    }
    delete[] locations;
    locations = nullptr;
}

char* InformationSystem::processFileName(int day, int month, int year)
{
    char name[] = "cleanup-YYYY-MM-DD.txt";

    for(int i = 11; i >= 8; i--)
    {
        name[i] = (year % 10) + '0';
        year /= 10;
    }

    if(month < 10)
    {
        name[13] = '0';
        name[14] = (month % 10) + '0'; 
    }
    else
    {
        name[14] = (month % 10) + '0';
        month /= 10;
        name[13] = (month % 10) + '0'; 
    }

    if(day < 10)
    {
        name[16] = '0';
        name[17] = (day % 10) + '0'; 
    }
    else
    {
        name[17] = (day % 10) + '0';
        day /= 10;
        name[16] = (day % 10) + '0'; 
    }

    char* fileName = new(std::nothrow) char[strlen(name) + 1];
    if(!fileName)
    {
        std::cout << "Memory not allocated successfully!\n";
        return nullptr;
    }
    strcpy(fileName, name);

    return fileName;
}

void InformationSystem::availability() const
{
    if(this->warehouse.getSize() == 0)
    {
        std::cout << "The warehouse is empty!" << std::endl;
        return;
    }

    std::ofstream oFile("temp.txt");
    if(!oFile)
    {
        std::cout << "Problem while opening the file!" << std::endl;
        return;
    }

    Warehouse copy(this->warehouse);
    Product temp;

    int numberOfSections = this->warehouse.getSize();
    for(int i = 0; i < numberOfSections; i++)
    {
        int currNumberOfShelves = this->warehouse[i].getSize();
        for(int j = 0; j < currNumberOfShelves; j++)
        {
            int currNumberOfProducts = this->warehouse[i][j].getSize();
            while(currNumberOfProducts > 0)
            {
                temp = copy[i][j][0];

                int quantity = copy.totalQuantityOfProduct(temp);

                std::cout << "name: " << temp.getName() << ", maker: " << temp.getMaker()
                          << ", quantity: " << quantity << "\n";

                int size = 0, **locations = nullptr;
                bool removed = copy.removeProduct(temp.getName(), quantity, locations, size, oFile);
                if(!removed && size != 0 && locations != nullptr)
                {
                    deallocateLocations(locations, size);
                }
                else if(removed && size == 0 && locations == nullptr)
                {
                    int numberOfDivisions = ceil((double) quantity / MAX_QUANTITY_IN_ONE_SHELF_DIVISION);
                    currNumberOfProducts -=  numberOfDivisions;
                }
            }  
        }
    }

    oFile.close();
}

void InformationSystem::addProduct()
{
    Product p;
    std::cin >> p;

    bool added = this->warehouse.addProduct(p);

    if(!added)
    {
        std::cout << "\nError! The product has not been added to the warehouse!" << std::endl;
    }
    else
    {
        std::cout << "\nThe product is added to the warehouse!" << std::endl;    
        
        std::ofstream oFile("changes.txt", std::ios::app);
        if(!oFile)
        {
            std::cout << "Problem while opening the file!" << std::endl;
            return;
        }

        oFile << p.getDateOfEntry() << " " << "Added product -> name: " << p.getName() 
              << ", maker: " << p.getMaker() << ", quantity: " << p.getQuantity() 
              << std::endl;

        oFile.close();
    }
}

void InformationSystem::removeProduct()
{
    if(this->warehouse.getSize() == 0)
    {
        std::cout << "The warehouse is empty!" << std::endl;
        return;
    }

    std::ofstream oFile("changes.txt", std::ios::app);
    if(!oFile)
    {
        std::cout << "Problem while opening the file!" << std::endl;
        return;
    }

    Date date;
    std::cout << "Enter date: ";
    std::cin >> date;

    std::cin.get();

    char name[MAX_NAME_LEN];
    std::cout << "name: ";
    std::cin.getline(name, MAX_NAME_LEN);

    int quantity;
    do
    {
        std::cout << "quantity: ";
        std::cin >> quantity;
    } while(quantity <= 0); 

    int size = 0, **locations = nullptr;
    
    bool removed = this->warehouse.removeProduct(name, quantity, locations, size);

    if(removed && size == 0 && locations == nullptr)
    {
        oFile << date << " " << "Removed product -> name: " << name 
              << ", quantity: " << quantity << std::endl;
    }
    else if(!removed && size != 0 && locations != nullptr)
    {
        std::cout << "\nInsufficient quantity! Current availability:" << std::endl;

        for(int i = 0; i < size; i++)
        {
            int section = locations[i][0], 
                shelf = locations[i][1], 
                number = locations[i][2];
            
            this->warehouse[section][shelf][number].print();
        }

        char operation[MAX_OPERATION_LEN];
        do
        {
            std::cout << "\nDo you want to remove the rest of this product? (yes/no): ";
            std::cin >> operation;
        } while(strcmp(operation, "yes") != 0 && strcmp(operation, "no") != 0);

        if(strcmp(operation, "yes") == 0)
        {
            Product currProduct;

            for(int i = 0; i < size; i++)
            {
                int section = locations[i][0], 
                    shelf = locations[i][1], 
                    number = locations[i][2];

                currProduct = this->warehouse[section][shelf][number];

                int currSize = 0, **currLocations = nullptr;

                removed = this->warehouse.removeProduct(currProduct.getName(), 
                                                        currProduct.getQuantity(), 
                                                        currLocations, currSize);

                if(!removed && currSize != 0 && currLocations != nullptr)
                {
                    std::cout << "Error! The product has not been removed from warehouse!" << std::endl;
                    deallocateLocations(currLocations, currSize);
                }
                else if(removed && currSize == 0 && currLocations == nullptr)
                {
                    oFile << date << " " << "Removed product -> name: " << currProduct.getName() 
                          << ", quantity: " << currProduct.getQuantity() << std::endl;
                }
            }
        }

        deallocateLocations(locations, size);
    }

    oFile.close();
}

void InformationSystem::referenceForChanges() const
{
    Date startDate, endDate;

    std::cout << "start date: ";
    std::cin >> startDate;

    std::cout << "end date: ";
    std::cin >> endDate;

    std::cout << "\nChanges in availability from " << startDate << " to " << endDate << ":" << std::endl;

    std::ifstream iFile("changes.txt");
    if(!iFile)
    {
        std::cout << "Problem while opening the file!" << std::endl;
        return;
    }

    Date temp; char buffer[MAX_BUFFER_LEN];

    bool end = false;

    while(!end)
    {
        iFile >> temp;
        iFile.get();
        iFile.getline(buffer, MAX_BUFFER_LEN);

        if((startDate < temp || startDate == temp) && (temp < endDate || temp == endDate))
        {
            std::cout << temp << ": " << buffer << std::endl;

            char symbol = iFile.get();
            if(symbol >= '1' && symbol <= '9') iFile.putback(symbol);
            else end = true;
        }
    }

    iFile.close();
}

void InformationSystem::clear()
{
    if(this->warehouse.getSize() == 0)
    {
        std::cout << "The warehouse is empty!" << std::endl;
        return;
    }

    Date date;
    std::cout << "Enter date: ";
    std::cin >> date;

    char* fileName = processFileName(date.getDay(), date.getMonth(), date.getYear());

    std::ofstream oFile(fileName, std::ios::app);
    if(!oFile)
    {
        std::cout << "Problem while opening the file!" << std::endl;
        return;
    }

    std::ofstream changes("changes.txt", std::ios::app);
    if(!changes)
    {
        std::cout << "Problem while opening the file!" << std::endl;
        return;
    }

    Date minDate(date);
    minDate += 3;

    Product temp;

    int numberOfSections = this->warehouse.getSize();
    for(int i = 0; i < numberOfSections; i++)
    {
        int currNumberOfShelves = this->warehouse[i].getSize();
        for(int j = 0; j < currNumberOfShelves; j++)
        {
            int currNumberOfProducts = this->warehouse[i][j].getSize();
            int p = 0;
            while(currNumberOfProducts > 0 && p < currNumberOfProducts)
            {
                if(this->warehouse[i][j][p].getExpiryDate() < minDate)
                {
                    temp = this->warehouse[i][j][p];

                    int quantity = temp.getQuantity();

                    int size = 0, **locations = nullptr;
                    bool removed = this->warehouse.removeProduct(temp.getName(), quantity, locations, size, oFile);

                    if(!removed && size != 0 && locations != nullptr)
                    {
                        std::cout << "Error! The product has not been removed from warehouse!" << std::endl;
                        deallocateLocations(locations, size);
                    }
                    else if(removed && size == 0 && locations == nullptr)
                    {
                        currNumberOfProducts--;
                        changes << date << " " << "Removed product -> name: " << temp.getName() 
                                << ", quantity: " << temp.getQuantity() << std::endl;
                    }
                }
                else p++;
            }
        }
    }

    changes.close();

    delete[] fileName;
    fileName = nullptr;

    oFile.close();
}

void InformationSystem::useInformationSystem()
{
    char fileName[MAX_NAME_LEN];
    std::cout << "Enter the name of the file in which the warehouse will be stored:\n";
    std::cin >> fileName;

    std::ofstream file(fileName);
    if(!file)
    {
        std::cout << "Problem while opening the file!" << std::endl;
        return;
    }

    char operation[MAX_OPERATION_LEN];
    bool endOfProgram = false;

    bool flag = true;

    do
    {
        std::cout << "\nOperations:\n";
        std::cout << "  > availability\n";
        std::cout << "  > add\n";
        std::cout << "  > remove\n";
        std::cout << "  > changes\n";
        std::cout << "  > clear\n";
        std::cout << "  > exit\n";

        if(flag)
        {
            std::cin.get();
            flag = false;
        }
        
        do
	    {
	    	std::cout << "\nEnter an operation: ";
	    	std::cin.getline(operation, MAX_OPERATION_LEN);
	    	std::cout << "\n";

	    } while(strcmp(operation, "availability") != 0 && strcmp(operation, "add") != 0 && 
                strcmp(operation, "remove") != 0 && strcmp(operation, "changes") != 0 &&
				strcmp(operation, "clear") != 0 && strcmp(operation, "exit") != 0);
        
        if(strcmp(operation, "availability") == 0)
        {
            availability();
            flag = false;
        }
        else if(strcmp(operation, "add") == 0)
        {
            addProduct();
            file << this->warehouse;
            flag = false;
        }
        else if(strcmp(operation, "remove") == 0)
        {
            removeProduct();
            file << this->warehouse;
            flag = true;
        }
        else if(strcmp(operation, "changes") == 0)
        {
            referenceForChanges();
            flag = true;
        }
        else if(strcmp(operation, "clear") == 0)
        {
            clear();
            file << this->warehouse;
            flag = true;
        }
        else if(strcmp(operation, "exit") == 0)
        {
            endOfProgram = true;
        }

    } while(!endOfProgram);

    file.close();
}
