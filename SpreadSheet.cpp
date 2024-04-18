#include "SpreadSheet.h"



SpreadSheet::SpreadSheet() : SpreadSheet(2, 2) {}

SpreadSheet::SpreadSheet(const SpreadSheet& rhv) : SpreadSheet(rhv.rowcnt, rhv.colcnt)
{
   for(int i = 0; i < rowcnt; ++i)
   {
      for(int j = 0; j < colcnt; ++j)
      {
         board[i][j] = rhv.board[i][j];
      }
   }
}

SpreadSheet::SpreadSheet(SpreadSheet&& rhv) :
   colcnt(rhv.colcnt),
   rowcnt(rhv.rowcnt),
   board(rhv.board)
   {rhv.board = nullptr;}

SpreadSheet::SpreadSheet(size_t size) : SpreadSheet(size, size) {}

SpreadSheet::SpreadSheet(size_t row, size_t col) :
   rowcnt(row),
   colcnt(col)
{
   board = new Cell*[row];
   for(int i = 0; i < row; ++i) {
      board[i] = new Cell[col];
   }
}

SpreadSheet::~SpreadSheet() {
   clear();
}

void SpreadSheet::clear() noexcept {
   for(int i = 0; i < rowcnt; ++i) {
      delete[] board[i];
   }
   delete[] board;
}

const SpreadSheet& SpreadSheet::operator=(const SpreadSheet& rhv) {
   if(this != &rhv)
   {
      clear();
      rowcnt = rhv.rowcnt;
      colcnt = rhv.colcnt;
      board = new Cell*[rhv.rowcnt];
      for(int i = 0; i < rhv.rowcnt; ++i) 
      {
         board[i] = new Cell[rhv.colcnt];
      }

      for(int i = 0; i < rowcnt; ++i)
      {
         for(int j = 0; j < colcnt; ++j)
         {
            board[i][j] = rhv.board[i][j];
         }
      }
   }
   return *this;
}

size_t SpreadSheet::row() const
{
   return rowcnt;
}

size_t SpreadSheet::col() const
{
   return colcnt;
}

const SpreadSheet& SpreadSheet::operator=(SpreadSheet&& rhv) 
{
   board = rhv.board;
   rowcnt = rhv.rowcnt;
   colcnt = rhv.colcnt;

   rhv.board = nullptr;
   return *this;
}

SpreadSheet::Column SpreadSheet::operator[](size_t pos) {
   return Column(board[pos]);
}

const SpreadSheet::Column SpreadSheet::operator[](size_t pos) const {
   return Column(board[pos]);
}

void SpreadSheet::swap(Cell& a, Cell& b) {
   Cell tmp = b;
   b = a;
   a = tmp;
}

void SpreadSheet::mirrorH()
{
   for(int i = 0; i < rowcnt; ++i) {
      for(int j = 0; j < colcnt / 2; ++j) {
         swap(board[i][j], board[i][colcnt - 1 - j]);
      }
   }
}
void SpreadSheet::mirrorV()
{
   for(int i = 0; i < rowcnt / 2; ++i) {
      for(int j = 0; j < colcnt; ++j) {
         swap(board[i][j], board[rowcnt - 1 - i][j]);
      }
   }
}
void SpreadSheet::mirrorD()
{
   rotate(1);
   mirrorV();
}
void SpreadSheet::mirrorSD()
{
   rotate(1);
   mirrorH();

   /*
         
      1 2 3 4 5 6
      3 2 1 6 5 4  
      4 6 7 8 9 4

      4 4 6
      9 5 5 
      8 6 4
      7 1 3
      6 2 2
      4 3 1

    */

}


void SpreadSheet::rotate1 ()
{
         Cell** arr = new Cell*[colcnt];
         for(int i = 0 ; i < colcnt; ++i)
         {
            arr[i] = new Cell[rowcnt];   
         }
         for(int i = 0; i < colcnt; ++i)
            for(int j = 0; j < rowcnt; ++j)
            { 
               arr[i][j] = board[j][i];
            }
         clear();
         board = arr;
         arr = nullptr;
         size_t tmp = rowcnt;
         rowcnt = colcnt; 
         colcnt = tmp;
         mirrorV();
         
}


void SpreadSheet::rotate(int cnt) 
{
   switch(cnt % 4)
   {
      case 0:
         return;
         break;
      case 1:
         rotate1();
         break;

      case 2:
         mirrorV();
         mirrorH();
         break;

      case 3:
         mirrorV();
         mirrorH();
         rotate1();
   }

}

void SpreadSheet::removeRows(std::initializer_list<size_t> rows) 
{
   std::vector<size_t> vec(rows);
   std::sort(vec.begin(),vec.end(),std::greater<size_t>());

   auto it = std::unique(vec.begin(), vec.end());
   vec.erase(it, vec.end());

  for(size_t val : vec)
  {
   removeRow(val);
   
  }
}

void SpreadSheet::swap_row(Cell** a, Cell** b)
{
   Cell* tmp = *a;
   *a = *b;
   *b = tmp;
   tmp = nullptr;
}

void SpreadSheet::removeRow(size_t row) 
{
   for(int i = 0; i < rowcnt - 1 - row; ++i)
   {
      swap_row(&board[row + i] , &board[row + 1 + i]);
   }
   delete[] board[rowcnt - 1];
   --rowcnt;
}

void SpreadSheet::removeCol(size_t col) 
{
  rotate(3);
  removeRow(col);
  rotate(1);
}

void SpreadSheet::removeCols(std::initializer_list<size_t> cols) 
{
   
   std::vector<size_t> vec(cols);
   std::sort(vec.begin(),vec.end(),std::greater<size_t>());

   auto it = std::unique(vec.begin(), vec.end());
   vec.erase(it, vec.end());

  for(size_t val : vec)
  {
   removeCol(val);
   
  }
}

void SpreadSheet::resizeRow(size_t r) 
{
  if(r == rowcnt)
  {
     return;
  }
  else if(r < rowcnt)
  {
     for(int i = rowcnt - 1; i != r - 1; --i)
     {
         removeRow(i);
     }
  }
  else
  {
      Cell** arr = new Cell*[r];
      for(int i = 0; i < r; ++i)
      {
         arr[i] = new Cell[colcnt];
      }

      for(int i = 0; i < rowcnt; ++i)
         for(int j = 0; j < colcnt; ++j)
         {
            arr[i][j] = board[i][j];
         }
   
      for(int i = rowcnt; i < r; ++i)
         for(int j = 0; j < colcnt; ++j)
         {
            arr[i][j] = '0';
         }
      clear();
      board = arr;
      rowcnt = r;
      arr = nullptr;
  }
}

void SpreadSheet::resizeCol(size_t c) 
{
  if(c == colcnt)
  {
     return;
  }
  else if(c < colcnt)
  {
     for(int i = colcnt - 1; i != c - 1; --i)
     {
         removeCol(i);
     }
  }
  else
  {
      Cell** arr = new Cell*[rowcnt];
      for(int i = 0; i < rowcnt; ++i)
      {
         arr[i] = new Cell[c];
      }

      for(int i = 0; i < rowcnt; ++i)
         for(int j = 0; j < colcnt; ++j)
         {
            arr[i][j] = board[i][j];
         }
   
      for(int i = 0; i < rowcnt; ++i)
         for(int j = colcnt; j < c; ++j)
         {
            arr[i][j] = '0';
         }
      clear();
      board = arr;
      colcnt = c;
      arr = nullptr;
  }
   
}

void SpreadSheet::resize(size_t r, size_t c) 
{
   resizeRow(r);
   resizeCol(c);
}

SpreadSheet SpreadSheet::slice(std::initializer_list<size_t> rows, std::initializer_list<size_t> cols) 
{
   SpreadSheet arr(rows.size(), cols.size());

   size_t i = 0;
   size_t j = 0;
   for (auto row : rows)
   {
      j = 0;
      for (auto col : cols)
      {
         if (row < rowcnt && col < colcnt)
         {
            arr.board[i][j] = board[row][col];
         }
         ++j;
      }
       ++i;
   }
   return arr;
}






//Proxy class
SpreadSheet::Column::Column(Cell* col) : col(col) {}

Cell& SpreadSheet::Column::operator[](size_t pos) {
   return col[pos];
}

const Cell& SpreadSheet::Column::operator[](size_t pos) const{
   return col[pos];
}


bool operator==(const SpreadSheet& lhv, const SpreadSheet& rhv)
{
   if(lhv.col() != rhv.col() || lhv.row() != rhv.row())
   {
      return false;
   }
   for(int i = 0; i < lhv.row(); ++i)
      for(int j = 0; j < lhv.col(); ++j)
      {
         if(lhv[i][j] != rhv[i][j])
            return false;
      }
   return true;
}

bool operator!=(const SpreadSheet& lhv, const SpreadSheet& rhv)
{
   return !(lhv == rhv);

}

std::ostream& operator<<(std::ostream& out,const SpreadSheet& ob) {


    for (int i = 0; i < ob.row(); ++i) {
        std::cout << '\n'; 
        for (int j = 0; j < ob.col(); ++j) {
            out << std::setw(5) << std::right << ob[i][j];
        }
        std::cout << std::endl;
    }
   std::cout << '\n';
return out;
}



/*
int main() {
   SpreadSheet ob(5,4);
   for(int i = 0; i < 5; ++i) {
      for(int j = 0; j < 4; ++j) {
         std::cin >> ob[i][j];
      }
   }
   ob.mirrorSD();
   std::cout << ob;
   
}

*/

