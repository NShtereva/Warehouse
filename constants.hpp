#ifndef __CONSTANTS_HPP
#define __CONSTANTS_HPP

#include <iostream>

class Constants
{
    public:
        Constants() = delete;
        
        static const size_t MAX_NAME_LEN = 255;
        static const size_t MAX_MAKER_LEN = 255;
        static const size_t MAX_COMMENT_LEN = 10000;

        static const size_t INITIAL_CAPACITY = 2;
        static const size_t INCREASE_STEP = 2;

        static const size_t MAX_SHELF_CAPACITY = 16;
        static const size_t MAX_QUANTITY_IN_ONE_SHELF_DIVISION = 9;

        static const size_t MAX_SECTION_CAPACITY = 16;

        static const size_t MAX_BUFFER_LEN = 10000;
        static const size_t MAX_OPERATION_LEN = 20;
};

#endif
