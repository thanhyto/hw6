
#include "Link.h"

Link::Link
(void)
{
  this->url = "";
  this->class_id = "";
  this->object_id = "";
}

Link::Link
(std::string arg_url, std::string arg_class_id, std::string arg_object_id)
{
  this->url = arg_url;
  this->class_id = arg_class_id;
  this->object_id = arg_object_id;
}

Json::Value * Link::dumpJ
(void)
{
  Json::Value *result_ptr = new Json::Value();

  if (this->url != "")
    {
      (*result_ptr)["url"] = this->url;
    }

  if (this->class_id != "")
    {
      (*result_ptr)["class_id"] = this->class_id;
    }

  if (this->object_id != "")
    {
      (*result_ptr)["object_id"] = this->object_id;
    }

#ifdef _ECS36B_DEBUG_
  std::cout << result->toStyledString() << std::endl;
#endif /* _ECS36B_DEBUG_ */
  return result_ptr;
}

bool 
Link::Jdump
(Json::Value *input_json_ptr)
{
  if ((input_json_ptr == NULL) ||
      ((*input_json_ptr).isNull() == true) ||
      ((*input_json_ptr).isObject() != true))
    {
      return false;
    }

  if ((((*input_json_ptr)["url"]).isNull() == true) ||
      (((*input_json_ptr)["class_id"]).isNull() == true) ||
      (((*input_json_ptr)["object_id"]).isNull() == true) ||
      (((*input_json_ptr)["url"]).isString() != true) ||
      (((*input_json_ptr)["class_id"]).isString() != true) ||
      (((*input_json_ptr)["object_id"]).isString() != true))
    {
      return false;
    }

  if (this->url == "")
    {
      this->url = ((*input_json_ptr)["url"]).asString();
    }

  // so we allow Jdump to overwrite everything else but this->url
  this->class_id = ((*input_json_ptr)["class_id"]).asString();
  this->object_id = ((*input_json_ptr)["object_id"]).asString();

#ifdef _ECS36B_DEBUG_
  std::cout << (this->dumpJ())->toStyledString() << std::endl;
#endif /* _ECS36B_DEBUG_ */

  return true; // if successful
}
