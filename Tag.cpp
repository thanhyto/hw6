
#include "Tag.h"

Tag::Tag
()
{
  this->index = 0;
  this->tagged = NULL;
  this->comment = "";
}

Tag::Tag
(int arg_index, Person * arg_tagged, std::string arg_comment)
{
  this->index = arg_index;
  this->tagged = arg_tagged;
  this->comment = arg_comment;
}

Json::Value *
Tag::dumpJ()
{
  if (this->index == 0)
    {
      return (Json::Value *) NULL;
    }

  Json::Value *result_ptr = new Json::Value();

  (*result_ptr)["index"] = this->index;

  if (this->tagged != NULL)
    {
      (*result_ptr)["tagged"] = (*((this->tagged)->dumpJ()));
    }
      
  if ((this->comment).size() == 0)
    {
      (*result_ptr)["comment"] = this->comment;
    }

#ifdef _ECS36B_DEBUG_
  std::cout << result_ptr->toStyledString() << std::endl;
#endif /* _ECS36B_DEBUG_ */
  return result_ptr;
}

bool 
Tag::Jdump
(Json::Value *input_json_ptr)
{
  if ((input_json_ptr == NULL) ||
      ((*input_json_ptr).isNull() == true) ||
      ((*input_json_ptr).isObject() != true))
    {
      return false;
    }

  if ((((*input_json_ptr)["index"]).isNull() == true) ||
      ((((*input_json_ptr)["tagged"]).isNull() == true) &&
       (((*input_json_ptr)["comment"]).isNull() == true)))
    {
      return false;
    }

  if ((((*input_json_ptr)["index"].isInt()) != true) ||
      (((*input_json_ptr)["index"].asInt()) == 0))
    {
      return false;
    }

  if ((((*input_json_ptr)["tagged"]).isNull() != true) &&
      (((*input_json_ptr)["tagged"]).isObject() != true))
    {
      return false;
    }

  if ((((*input_json_ptr)["comment"]).isNull() != true) &&
      (((*input_json_ptr)["comment"]).isString() != true))
    {
      return false;
    }

  if ((((*input_json_ptr)["tagged"]).isNull() != true) &&
      (this->tagged == NULL))
    {
      this->tagged = new Person();
      bool rc = (this->tagged)->Jdump(&((*input_json_ptr)["tagged"]));
      if (rc == false)
	{
	  delete this->tagged;
	  this->tagged = NULL;
	  return false;
	}
    }

  if ((((*input_json_ptr)["comment"]).isNull() != true) &&
      (this->comment == ""))
    {
      this->comment = ((*input_json_ptr)["comment"]).asString();
    }

  if (this->index == 0)
    {
      this->index = ((*input_json_ptr)["index"]).asInt();
    }

#ifdef _ECS36B_DEBUG_
  std::cout << (this->dumpJ())->toStyledString() << std::endl;
#endif /* _ECS36B_DEBUG_ */

  return true; // if successful
}

