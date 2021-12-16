
#ifndef _REACTION_H_
#define _REACTION_H_

// Reaction.h
#include "Core.h"
#include "Person.h"
#include "JvTime.h"

class Reaction
{
 private:
 protected:
 public:
  std::string type;
  Person * author;
  JvTime * created;
  Reaction();
  Reaction(std::string arg_type, Person * arg_author, JvTime * arg_created);
  virtual Json::Value * dumpJ();
  virtual bool Jdump(Json::Value *);
};

#endif /* _REACTION_H_ */
