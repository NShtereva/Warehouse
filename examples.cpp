#include "examples.hpp"

#include "date.hpp"
#include "product.hpp"
#include "shelf.hpp"
#include "section.hpp"
#include "warehouse.hpp"

#include <iostream>

void Examples::testDate()
{
    Date date1(28, 02, 2020);
    Date date2(29, 02, 2020);

    std::cout << std::boolalpha << !(date1 == date2) << std::endl;
    std::cout << std::boolalpha << (date1 < date2) << std::endl;

    Date date3(01, 05, 2022);
    Date date4(20, 04, 2022);
    Date date5(31, 03, 2022);

    std::cout << std::boolalpha << (date4 < date3) << std::endl;
    std::cout << std::boolalpha << (date5 < date4) << std::endl;

    date5 += 3;

    Date date6(03, 04, 2022);
    std::cout << std::boolalpha << (date5 == date6) << std::endl;

    std::cout << std::endl;

    std::ofstream oFile("testDate.txt");
    if(!oFile)
    {
        std::cout << "Problem while opening the file!" << std::endl;
        return;
    }

    oFile << date1 << std::endl;
    oFile << date3 << std::endl;
    oFile << date4 << std::endl;
    oFile << date5 << std::endl;

    oFile.close();

    std::ifstream iFile("testDate.txt");
    if(!iFile)
    {
        std::cout << "Problem while opening the file!" << std::endl;
        return;
    }

    for(int i = 0; i < 4; i++)
    {
        Date date;
        iFile >> date;
        std::cout << date << "\n";
    }
    
    iFile.close();
}

void Examples::testProduct()
{
    Product p1("Product1", Date(9, 5, 2022), Date(28, 4, 2022), "maker1", 3, 0, 0, 0, "comment1");
    Product p2("Product2", Date(12, 5, 2022), Date(29, 4, 2022), "maker2", 9, 0, 0, 0, "comment2");
    Product p3("Product3", Date(20, 5, 2022), Date(27, 4, 2022), "maker3", 5, 0, 0, 0, "comment3");
    Product p4;

    std::cout << "Enter product:"<< std::endl;
    std::cin >> p4;

    std::cout << std::endl;

    p4.print();

    std::cout << std::endl;

    std::cout << std::boolalpha << (p1 == p4) << std::endl;
    std::cout << std::boolalpha << (p2 == p4) << std::endl;  
    std::cout << std::boolalpha << (p3 == p4) << std::endl; 
    
    std::cout << std::endl;

    std::ofstream oFile("testProduct.txt");
    if(!oFile)
    {
        std::cout << "Problem while opening the file!" << std::endl;
        return;
    }

    oFile << p1;
    oFile << p2;
    oFile << p3;
    oFile << p4;

    oFile.close();

    std::ifstream iFile("testProduct.txt");
    if(!iFile)
    {
        std::cout << "Problem while opening the file!" << std::endl;
        return;
    }

    for(int i = 0; i < 4; i++)
    {
        Product p;
        iFile >> p;
        std::cout << p;
    }

    iFile.close();
}

void Examples::testShelf()
{
    Product p1("Product1", Date(12, 5, 2022), Date(29, 4, 2022), "maker1", 12, 0, 0, 0, "comment1");
    Product p2("Product2", Date(5, 5, 2022), Date(28, 4, 2022), "maker2", 3, 0, 0, 0, "comment2");
    Product p3("Product2", Date(5, 5, 2022), Date(28, 4, 2022), "maker2", 16, 0, 0, 0, "comment2");
    Product p4("Product2", Date(6, 5, 2022), Date(29, 4, 2022), "maker2", 13, 0, 0, 0, "comment4");
    Product p5("Product2", Date(4, 5, 2022), Date(27, 4, 2022), "maker2", 15, 0, 0, 0, "comment5");
    Product p6("Product3", Date(18, 5, 2022), Date(17, 4, 2022), "maker3", 3, 0, 0, 0, "comment3");

    Shelf sh1;
    sh1 += p1;
    sh1 += p2;
    sh1 += p3;
    sh1 += p4;
    sh1 += p5;
    sh1 += p6;

    std::cout << "Shelf1:\n" << sh1 << std::endl;

    Shelf sh2(sh1);

    int size = 0, *numbers = nullptr;

    std::cout << std::boolalpha << sh1.removeProduct("Product1", 12, numbers, size) << " " << size << std::endl;
    std::cout << std::boolalpha << sh1.removeProduct("Product2", 3, numbers, size) << " " << size << std::endl;
    std::cout << std::boolalpha << sh1.removeProduct("Product2", 16, numbers, size) << " " << size << std::endl;

    std::cout << "\nNew shelf1:\n" << sh1 << std::endl;

    Shelf sh3;
    sh3 = sh1;

    std::cout << std::boolalpha << sh1.removeProduct("Product2", 13, numbers, size) << " " << size << std::endl;
    std::cout << std::boolalpha << sh1.removeProduct("Product2", 15, numbers, size) << " " << size << std::endl;
    std::cout << std::boolalpha << sh1.removeProduct("Product3", 3, numbers, size) << " " << size << std::endl;

    std::cout << "\nNew shelf1:\n" << sh1 << std::endl;
    std::cout << "Shelf2:\n" << sh2 << std::endl;
    std::cout << "Shelf3:\n" << sh3 << std::endl;

    std::cout << "Example with insufficient quantity:" << std::endl;
    std::cout << std::boolalpha << sh2.removeProduct("Product2", 100, numbers, size) << " " << size << std::endl;
    for(int i = 0; i < size; i++)
    {
        std::cout << numbers[i] << " ";
    }
    std::cout << std::endl;

    delete[] numbers;
    numbers = nullptr;

    std::ofstream oFile("testShelf.txt");
    if(!oFile)
    {
        std::cout << "Problem while opening the file!" << std::endl;
        return;
    }

    oFile << sh2;

    oFile.close();
    
    std::ifstream iFile("testShelf.txt");
    if(!iFile)
    {
        std::cout << "Problem while opening the file!" << std::endl;
        return;
    }

    iFile >> sh1;

    iFile.close();

    std::cout << std::endl;
    std::cout << sh1;
}

void Examples::testSection()
{
    Product p1("Product1", Date(12, 5, 2022), Date(29, 4, 2022), "maker1", 12, 0, 0, 0, "comment1");
    Product p2("Product2", Date(5, 5, 2022), Date(28, 4, 2022), "maker2", 3, 0, 0, 0, "comment2");
    Product p3("Product2", Date(5, 5, 2022), Date(28, 4, 2022), "maker2", 16, 0, 0, 0, "comment2");
    Product p4("Product2", Date(6, 5, 2022), Date(29, 4, 2022), "maker2", 13, 0, 0, 0, "comment2");
    Product p5("Product2", Date(4, 5, 2022), Date(27, 4, 2022), "maker2", 15, 0, 0, 0, "comment2");
    Product p6("Product3", Date(18, 5, 2022), Date(17, 4, 2022), "maker3", 3, 0, 0, 0, "comment3");
    Product p7("Product1", Date(12, 5, 2022), Date(30, 4, 2022), "maker1", 9, 0, 0, 0, "comment1");
    Product p8("Product2", Date(4, 5, 2022), Date(27, 4, 2022), "maker2", 5, 0, 0, 0, "comment2");
    Product p9("Product5", Date(15, 5, 2022), Date(1, 5, 2022), "maker5", 18, 0, 0, 0, "comment5");

    Shelf sh1, sh2;
    sh1 += p1; 
    sh1 += p2;
    sh1 += p3;
    sh1 += p4;
    sh1 += p5;
    sh1 += p6;

    sh2 += p7;
    sh2 += p8;
    sh2 += p9;

    Section s1;
    s1 += sh1;
    s1 += sh2;
    std::cout << "Section1:\n" << s1 << std::endl; 

    Section s2;
    s2.addProduct(p1);
    s2.addProduct(p2);
    s2.addProduct(p3);
    s2.addProduct(p4);
    s2.addProduct(p5);
    s2.addProduct(p6);
    s2.addProduct(p7);
    s2.addProduct(p8);
    s2.addProduct(p9);
    std::cout << "Section2:\n" << s2 << std::endl;

    int size = 0, **locations = nullptr;

    Section s3(s1);
    std::cout << std::boolalpha << s3.removeProduct("Product1", 21, locations, size) << " " << size << std::endl;
    std::cout << std::boolalpha << s3.removeProduct("Product2", 52, locations, size) << " " << size << std::endl;
    std::cout << std::boolalpha << s3.removeProduct("Product3", 3, locations, size) << " " << size << std::endl;
    std::cout << std::boolalpha << s3.removeProduct("Product5", 18, locations, size) << " " << size << std::endl;
    std::cout << "\nSection3:\n" << s3 << std::endl;

    std::cout << "Example with insufficient quantity:" << std::endl;
    std::cout << std::boolalpha << s2.removeProduct("Product5", 100, locations, size) << " " << size << std::endl;
    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j < 2; j++)
        {
            std::cout << locations[i][j] << " ";
        }
        std::cout << std::endl;
    }

    for(int i = 0; i < size; i++)
    {
        delete[] locations[i];
    }
    delete[] locations;
    locations = nullptr;

    std::cout << "\n--------------------------------------------------------------\n";

    std::ofstream oFile1("testSection1.txt");
    if(!oFile1)
    {
        std::cout << "Problem while opening the file!" << std::endl;
        return;
    }

    oFile1 << s1;

    oFile1.close();

    Section s4;
    
    std::ifstream iFile1("testSection1.txt");
    if(!iFile1)
    {
        std::cout << "Problem while opening the file!" << std::endl;
        return;
    }

    iFile1 >> s4;

    iFile1.close();

    std::cout << s4 << std::endl;

    std::cout << "--------------------------------------------------------------\n";

    std::ofstream oFile2("testSection2.txt");
    if(!oFile2)
    {
        std::cout << "Problem while opening the file!" << std::endl;
        return;
    }

    oFile2 << s2;

    oFile2.close();

    Section s5;
    
    std::ifstream iFile2("testSection2.txt");
    if(!iFile2)
    {
        std::cout << "Problem while opening the file!" << std::endl;
        return;
    }

    iFile2 >> s5;

    iFile2.close();

    std::cout << s5 << std::endl;
}

void Examples::testWarehouse()
{
    Product p1("Product1", Date(12, 5, 2022), Date(29, 4, 2022), "maker1", 12, 0, 0, 0, "comment1");
    Product p2("Product2", Date(5, 5, 2022), Date(28, 4, 2022), "maker2", 3, 0, 0, 0, "comment2");
    Product p3("Product2", Date(5, 5, 2022), Date(28, 4, 2022), "maker2", 16, 0, 0, 0, "comment2");
    Product p4("Product2", Date(6, 5, 2022), Date(29, 4, 2022), "maker2", 13, 0, 0, 0, "comment2");
    Product p5("Product2", Date(4, 5, 2022), Date(27, 4, 2022), "maker2", 15, 0, 0, 0, "comment2");
    Product p6("Product3", Date(18, 5, 2022), Date(17, 4, 2022), "maker3", 3, 0, 0, 0, "comment3");
    Product p7("Product1", Date(12, 5, 2022), Date(30, 4, 2022), "maker1", 9, 0, 0, 0, "comment1");
    Product p8("Product2", Date(4, 5, 2022), Date(27, 4, 2022), "maker2", 5, 0, 0, 0, "comment2");
    Product p9("Product5", Date(15, 5, 2022), Date(1, 5, 2022), "maker5", 18, 0, 0, 0, "comment5");
    Product p10("Product4", Date(17, 6, 2022), Date(30, 4, 2022), "maker4", 30, 0, 0, 0, "comment4");
    Product p11("Product6", Date(10, 6, 2022), Date(1, 5, 2022), "maker6", 18, 0, 0, 0, "comment6");
    Product p12("Product7", Date(13, 6, 2022), Date(8, 5, 2022), "maker7", 30, 0, 0, 0, "comment7");

    Warehouse w1;
    w1.addProduct(p1);
    w1.addProduct(p2);
    w1.addProduct(p3); 
    w1.addProduct(p4); 
    w1.addProduct(p5);
    w1.addProduct(p6); 
    w1.addProduct(p7);
    w1.addProduct(p8); 
    w1.addProduct(p9); 
    w1.addProduct(p10);
    w1.addProduct(p11);
    w1.addProduct(p12);
    std::cout << "Warehouse1:\n" << w1 << std::endl;

    Warehouse w2(w1);

    int size = 0, **locations = nullptr;

    std::cout << std::boolalpha << w1.removeProduct("Product1", 21, locations, size) << " " << size << std::endl;
    std::cout << std::boolalpha << w1.removeProduct("Product2", 52, locations, size) << " " << size << std::endl;
    std::cout << std::boolalpha << w1.removeProduct("Product3", 3, locations, size) << " " << size << std::endl;
    std::cout << std::boolalpha << w1.removeProduct("Product4", 30, locations, size) << " " << size << std::endl;
    std::cout << std::boolalpha << w1.removeProduct("Product5", 18, locations, size) << " " << size << std::endl;
    std::cout << std::boolalpha << w1.removeProduct("Product6", 18, locations, size) << " " << size << std::endl;
    std::cout << std::boolalpha << w1.removeProduct("Product7", 30, locations, size) << " " << size << std::endl;
    std::cout << "\nNew warehouse1:\n" << w1 << std::endl;

    std::cout << "Example with insufficient quantity:" << std::endl;
    std::cout << std::boolalpha << w2.removeProduct("Product5", 50, locations, size) << " " << size << std::endl;
    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            std::cout << locations[i][j] << " ";
        }
        std::cout << std::endl;
    }

    for(int i = 0; i < size; i++)
    {
        delete[] locations[i];
    }
    delete[] locations;
    locations = nullptr;

    std::cout << "\n--------------------------------------------------------------\n";
    
    std::ofstream oFile("testWarehouse.txt");
    if(!oFile)
    {
        std::cout << "Problem while opening the file!" << std::endl;
        return;
    }

    oFile << w2;

    oFile.close();

    Warehouse w3;
    
    std::ifstream iFile("testWarehouse.txt");
    if(!iFile)
    {
        std::cout << "Problem while opening the file!" << std::endl;
        return;
    }

    iFile >> w3;

    iFile.close();

    std::cout << w3;
}
