#include "TextBuffer.hpp"
#include "unit_test_framework.hpp"

using namespace std;

// Add your test cases here

TEST(test_insert) {
    TextBuffer buffer;
    buffer.insert('K');
    buffer.insert('I');
    buffer.insert('T');
    buffer.insert('H');
    buffer.insert('\n');
    buffer.insert('S');
    buffer.insert('A');
    buffer.insert('I');
    buffer.insert('\n');

    ASSERT_EQUAL(buffer.get_row() , 3);
    buffer.backward();
    ASSERT_EQUAL(buffer.data_at_cursor() , '\n');
    buffer.backward();
    ASSERT_EQUAL(buffer.data_at_cursor() , 'I');
    ASSERT_EQUAL(buffer.get_row() , 2);
    ASSERT_EQUAL(buffer.get_column(), 2);
    ASSERT_EQUAL(buffer.get_index(), 7);
}

TEST(test_forward) {
    TextBuffer buffer;
    buffer.insert('H');
    buffer.insert('O');
    buffer.insert('T');
    buffer.insert('\n');
    buffer.insert('D');
    buffer.insert('O');
    buffer.insert('G');
    buffer.insert('\n');

    ASSERT_EQUAL(buffer.get_row() , 3);
    ASSERT_EQUAL(buffer.forward() , false);
    buffer.backward();
    buffer.backward();
    buffer.backward();
    buffer.forward();
    ASSERT_EQUAL(buffer.data_at_cursor() , 'G');
}



TEST_MAIN()
