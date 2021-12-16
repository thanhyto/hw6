
#ifndef _ACTION_H_
#define _ACTION_H_

#include "Core.h"

class Action : public Core
{
 private:
 protected:
 public:
  std::string name;
  Action();
  Action(std::string, std::string, std::string, std::string, std::string);
  virtual Json::Value * dumpJ();
  virtual bool Jdump(Json::Value *);
};

#endif /* _ACTION_H_ */
