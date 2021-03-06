#ifndef ST_H
#define ST_H

#include <string>
#include <hash_fun.h>
#include <ext/hash_map>
using namespace std;

class SymTabEntry;

struct string_eq {
  bool operator()(const string& s1, const string& s2) const {
    return s1==s2;
  }
};
struct string_hash {
  size_t operator() (const string& s1) const {
    __gnu_cxx::hash<const char*> hashFunc;
    return hashFunc(s1.c_str());
  }
};

enum ErrorST {OK, DUPLICATE_SYM, INVALID_SCOPE, SYM_NOT_PRESENT};

class SymTab {
 public:
  class const_iterator {
   private:
    const SymTabEntry* current;
   public:
    const_iterator(const SymTabEntry *c) { current = c; };
    const SymTabEntry* operator* () const { return current;};
    const_iterator & operator++ ();
    bool operator==(const const_iterator& i) const 
	  {return (current == i.current);};
    bool operator!=(const const_iterator& i) const 
      {return !(operator==(i));};
  };

  class iterator {
   private:
    SymTabEntry* current;
   public:
    iterator(SymTabEntry *c) { current = c; };
    SymTabEntry* operator* () { return current;};
    iterator & operator++ ();
    bool operator==(const iterator& i) const {return (current == i.current);};
    bool operator!=(const iterator& i) const {return !operator==(i);};
  };

 public:
  SymTab(int initSize=4);
  virtual ~SymTab() {};

  int size() const {return map_.size();};

  const SymTabEntry* lookUp(string name) const;
  SymTabEntry* lookUp(string name) {
    return (SymTabEntry*)(((const SymTab*)this)->lookUp(name));
  }
  
  ErrorST insert(SymTabEntry*); // will resize automatically
  void print(ostream& os,int indent=0) const {printST(os, indent);};
  virtual void printST(ostream& os,int ind=0,char ldelim='{',char rdelim='}',
					   bool linebreaks=true, int first=0, int last=0) const;
  virtual void typeCheckST(int first, int last) const;
  virtual void memAllocST(int first, int last);

  const_iterator begin() const { return const_iterator(first_); };
  iterator begin() { return iterator(first_); };
  // End of list is indicated when the iterator's current pointer
  // equals NULL, but not when it is equal to last_. 
  const_iterator end() const { return const_iterator(NULL); };
  iterator end() { return iterator(NULL); };

 private:
  __gnu_cxx::
	hash_map<const string, SymTabEntry *, string_hash, string_eq> map_;
  SymTabEntry* first_;
  SymTabEntry* last_;

 private: // Disable copy and assignment.
  SymTab(const SymTab&);
  SymTab operator=(const SymTab &);
};

#endif

