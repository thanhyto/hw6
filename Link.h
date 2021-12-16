
#ifndef _LINK_H_
#define _LINK_H_

// Link.h
#include "Core.h"

class Link
{
 private:
 protected:
 public:
  std::string url;
  std::string class_id;
  std::string object_id;

  Link(void);
  Link(std::string arg_url, std::string arg_class_id, std::string arg_object_id);
  virtual Json::Value * dumpJ();
  virtual bool Jdump(Json::Value *);
};

#endif /* _LINK_H_ */
