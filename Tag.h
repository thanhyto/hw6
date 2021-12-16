
#ifndef _TAG_H_
#define _TAG_H_

// Tag.h
#include "Person.h"

class Tag
{
 private:
 protected:
 public:
  int index;
  Person * tagged;
  std::string comment;

  Tag();
  Tag(int arg_index, Person * arg_tagged, std::string arg_comment);
  virtual Json::Value * dumpJ();
  virtual bool Jdump(Json::Value *);
};

#endif /* _TAG_H_ */
