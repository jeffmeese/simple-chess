#ifndef ARRAY2D_H
#define ARRAY2D_H

template <typename T>
class Array2D
{
public:
  // constructor
  Array2D()
    : pAr(0)
  {
    resize(3, 3);
  }

  Array2D(unsigned int wd, unsigned int ht)
    : pAr(0)
  {
    resize(wd, ht);
  }

  void resize(unsigned int wd, unsigned int ht)
  {
    if (pAr)
      delete [] pAr;

    pAr = 0;
    nWd = wd;
    nHt = ht;
    if(wd > 0 && ht > 0)
      pAr = new T[wd * ht];
  }

  // destructor
  ~Array2D()
  {
    if (pAr)
      delete[] pAr;
  }

  // indexing (parenthesis operator)
  //  two of them (for const correctness)

  const T& operator () (unsigned x,unsigned y) const
  {  return pAr[ y*nWd + x ];   }

  T& operator () (unsigned x,unsigned y)
  {  return pAr[ y*nWd + x ];   }

  // get dims
  unsigned GetWd() const { return nWd; }
  unsigned GetHt() const { return nHt; }


  // private data members
private:
  unsigned nWd;
  unsigned nHt;
  T*       pAr;

  // to prevent unwanted copying:
  Array2D(const Array2D<T>&);
  Array2D& operator = (const Array2D<T>&);
};

#endif // ARRAY2D_H
