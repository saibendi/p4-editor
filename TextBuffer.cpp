//
//  TextBuffer.cpp
//  p4-editor
//
//  Created by Bends on 7/7/24.
//

#include "TextBuffer.hpp"

//EFFECTS: Creates an empty text buffer. Its cursor is at the past-the-end
//         position, with row 1, column 0, and index 0.
TextBuffer::TextBuffer() {
    data.clear();   // data should already be empty, but sanity check
    cursor = data.end();
    row = 1;
    column = index = 0;
}


//MODIFIES: *this
//EFFECTS:  Moves the cursor one position forward and returns true,
//          unless the cursor is already at the past-the-end position,
//          in which case this does nothing and returns false.
//NOTE:     Your implementation must update the row, column, and index
//          if appropriate to maintain all invariants.
bool TextBuffer::forward() {
    if (cursor == data.end()) {     // if cursor is on the last iterator / node
        return false;
    }
    if (data_at_cursor() == '\n') { // if currently on newline char, when you increment, ++row, column = 0 and ++cursor;
        ++row;
        column = 0;
        ++cursor;
    }
    else {                          // if not newline char, row unchanged, ++column, and ++cursor;
        ++column;
        ++cursor;
    }
    ++index;
    return true;
}

//MODIFIES: *this
//EFFECTS:  Moves the cursor one position backward and returns true,
//          unless the cursor is already at the first character in
//          the buffer, in which case this does nothing and returns false.
//HINT:     Implement and use the private compute_column() member
//          function to compute the column when moving left from the
//          beginning of a line to the end of the previous one.
//NOTE:     Your implementation must update the row, column, and index
//          if appropriate to maintain all invariants.
bool TextBuffer::backward() {
    if (cursor == data.begin()) {   // if cursor is on the first iterator / node
        return false;
    }
    --cursor;
    if (data_at_cursor() == '\n') {
        --row;
        column = compute_column();
    }
    else {
        --column;
    }
    --index;
    return true;
}

//MODIFIES: *this
//EFFECTS:  Removes the character from the buffer that is at the cursor and
//          returns true, unless the cursor is at the past-the-end position,
//          in which case this does nothing and returns false.
//          The cursor will now point to the character that was after the
//          deleted character, or the past-the-end position if the deleted
//          character was the last one in the buffer.
//NOTE:     Your implementation must update the row, column, and index
//          if appropriate to maintain all invariants.
bool TextBuffer::remove() {
    if (cursor == data.end()) {     // if cursor is on the last iterator / node
        return false;
    }
    
    --index;            // index decreases either way
    data.erase(cursor);    // TODO: do i need to do cursor =?
    // column doesn't change either way
    if (data_at_cursor() == '\n') {    // if erasing newline, gets rid of a row, so row decrements and column doesn't change
        --row;
    }
    return true;
}

//MODIFIES: *this
//EFFECTS:  Inserts a character in the buffer before the cursor position.
//          If the cursor is at the past-the-end position, this means the
//          inserted character is the last character in the buffer.
//          The cursor remains in the same place as before the insertion.
//NOTE:     Your implementation must update the row, column, and index
//          if appropriate to maintain all invariants.
void TextBuffer::insert(char c) {
    data.insert(cursor, c); // inserting char 'c' right before cursor location - func takes care of edge cases
    if (c == '\n') {    // if newline, new row and column resets
        ++row;
        column = 0;
    }
    else {              // if not newline, same row, column increases by 1
        ++column;
    }
    ++index;            // index increases either way
}

//MODIFIES: *this
//EFFECTS:  Moves the cursor to the start of the current row (column 0).
//NOTE:     Your implementation must update the row, column, and index
//          if appropriate to maintain all invariants.
void TextBuffer::move_to_row_start() {
    do {
        --cursor;
        --index;
    }
    while(data_at_cursor() != '\n' && cursor != data.begin()); // if cursor value isn't newline AND if cursor isn't first node
    
    if (data_at_cursor() == '\n') { // cursor get's reset to first char in row
        ++cursor;
        ++index;
    }
    // column reset to 0, row get's unchanged;
    column = 0;
  }

//MODIFIES: *this
//EFFECTS:  Moves the cursor to the end of the current row (the
//          newline character that ends the row, or the past-the-end
//          position if the row is the last one in the buffer).
//NOTE:     Your implementation must update the row, column, and index
//          if appropriate to maintain all invariants.
void TextBuffer::move_to_row_end() {
    // order of while loop conditions important here bc you can't do a data_at_cursor at end() iterator
    //std::cout << "Entering move_to_row_end()" << std::endl;
    
    //std::cout << "Index 1: " << index << std::endl;

    while (cursor != data.end() && data_at_cursor() != '\n') {  // cursor only increments if not at end of row already
        ++cursor;
        ++index;
    }
    //std::cout << "Index 2: " << index << std::endl;
    // column value changes, row stays the same
    column = compute_column();
}

//REQUIRES: new_column >= 0
//MODIFIES: *this
//EFFECTS:  Moves the cursor to the given column in the current row,
//          if it exists. If the row does not have that many columns,
//          moves to the end of the row (the newline character that
//          ends the row, or the past-the-end position if the row is
//          the last one in the buffer).
//NOTE:     Your implementation must update the row, column, and index
//          if appropriate to maintain all invariants.
void TextBuffer::move_to_column(int new_column) {
    //std::cout << "Entering move_to_column()" << std::endl;

    // TODO: check to see if condition where new_column == column should be included
    //std::cout << "New Column: " << new_column << std::endl;
    //std::cout << "Column 1: " << column << std::endl;

    move_to_row_end();
    //std::cout << "Column 2: " << column << std::endl;
    //std::cout << "Index 3: " << index << std::endl;

    if (new_column < column) {
        while (column != new_column) {
            backward(); // decrements cursor backwards and all other invariants
        }
    }
}


//MODIFIES: *this
//EFFECTS:  Moves the cursor to the previous row, retaining the
//          current column if possible.

//          If the previous row is
//          shorter than the current column, moves to the end of the
//          previous row (the newline character that ends the row).

//          Does nothing if the cursor is already in the first row.

//          Returns true if the position changed, or false if it did
//          not (i.e. if the cursor was already in the first row).
//NOTE:     Your implementation must update the row, column, and index
//          if appropriate to maintain all invariants.
bool TextBuffer::up() {
    //std::cout << "Entering up()" << std::endl;
    if (row == 1) {
        return false;
    }
    else {    // if row not equal to 1
        int original_row_column = column;
        //std::cout << "Original Row Column: " << original_row_column << std::endl;
        move_to_row_start();    // go to beginning of row, to keep track of index
        backward();             // go back one to previous row's \n
        //std::cout << "Index 0: " << index << std::endl;
        move_to_column(original_row_column);             // move to row end
        //std::cout << "Entering up()" << std::endl;
        return true;
    }
}

//MODIFIES: *this
//EFFECTS:  Moves the cursor to the next row, retaining the current
//          column if possible. If the next row is shorter than the
//          current column, moves to the end of the next row (the
//          newline character that ends the row, or the past-the-end
//          position if the row is the last one in the buffer). Does
//          nothing if the cursor is already in the last row.
//          Returns true if the position changed, or false if it did
//          not (i.e. if the cursor was already in the last row).
//NOTE:     Your implementation must update the row, column, and index
//          if appropriate to maintain all invariants.
bool TextBuffer::down() {
    int original_row_column = column;
    move_to_row_end();
    if (is_at_end()) {                               // if we're at the end of the buffer, there's no more rows
        move_to_column(original_row_column);         // move back to original column of that same row
        return false;                                // And return false
    }
    forward();                              // step into the new row
    move_to_column(original_row_column);    // func defaults to end of row and walks backwards to original_row_column
    return true;
}

//EFFECTS:  Returns whether the cursor is at the past-the-end position.
  bool TextBuffer::is_at_end() const {
      if (cursor == data.end()) {
          return true;
      }
      return false;
  }

//REQUIRES: the cursor is not at the past-the-end position
//EFFECTS:  Returns the character at the current cursor
  char TextBuffer::data_at_cursor() const {
      assert(!is_at_end());
      return *cursor;
  }

//EFFECTS:  Returns the row of the character at the current cursor.
  int TextBuffer::get_row() const {
      return row;
  }

//EFFECTS:  Returns the column of the character at the current cursor.
  int TextBuffer::get_column() const {
      return column;
  }

//EFFECTS:  Returns the index of the character at the current cursor
//          with respect to the entire contents. If the cursor is at
//          the past-the-end position, returns size() as the index.
  int TextBuffer::get_index() const {
      if (is_at_end()) {
          return size();
      }
      return index;
  }

//EFFECTS:  Returns the number of characters in the buffer.
  int TextBuffer::size() const {
      return data.size();
  }

//EFFECTS:  Returns the contents of the text buffer as a string.
//HINT: Implement this using the string constructor that takes a
//      begin and end iterator. You may use this implementation:
//        return std::string(data.begin(), data.end());
std::string TextBuffer::stringify() const {
      if (data.empty()) {   // if empty buffer
          return "";
      }
      return std::string(data.begin(), data.end());
}
