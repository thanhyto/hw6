
#include "Action.h"

Action::Action(void)
{
  this->name = "";
}

Action::Action(std::string core_arg_host_url, std::string core_arg_owner_vsID,
	       std::string core_arg_class_id, std::string core_arg_object_id,
	       std::string arg_name)
  : Core { core_arg_host_url, core_arg_owner_vsID, core_arg_class_id, core_arg_object_id }
{
  this->name = arg_name;
}

Json::Value * 
Action::dumpJ
(void)
{
  Json::Value *result_ptr = new Json::Value();

  if (this->name != "")
    {
      (*result_ptr)["name"] = this->name;
    }

  if (this->owner_vsID != "")
    {
      (*result_ptr)["owner_vsID"] = this->owner_vsID;
    }

  if (this->host_url != "")
    {
      (*result_ptr)["host_url"] = this->host_url;
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
Action::Jdump
(Json::Value *input_json_ptr)
{
  if ((input_json_ptr == NULL) ||
      ((*input_json_ptr).isNull() == true) ||
      ((*input_json_ptr).isObject() != true))
    {
      return false;
    }

  if ((((*input_json_ptr)["host_url"]).isNull() == true) ||
      (((*input_json_ptr)["owner_vsID"]).isNull() == true) ||
      (((*input_json_ptr)["class_id"]).isNull() == true) ||
      (((*input_json_ptr)["object_id"]).isNull() == true) ||
      (((*input_json_ptr)["name"]).isNull() == true) ||
      (((*input_json_ptr)["host_url"]).isString() != true) ||
      (((*input_json_ptr)["owner_vsID"]).isString() != true) ||
      (((*input_json_ptr)["class_id"]).isString() != true) ||
      (((*input_json_ptr)["object_id"]).isString() != true) ||
      (((*input_json_ptr)["name"]).isString() != true))
    {
      return false;
    }

  if (this->name == "")
    {
      this->name = ((*input_json_ptr)["name"]).asString();
    }

  // so we allow Jdump to overwrite everything else but this->name
  this->owner_vsID = ((*input_json_ptr)["owner_vsID"]).asString();
  this->host_url = ((*input_json_ptr)["host_url"]).asString();
  this->class_id = ((*input_json_ptr)["class_id"]).asString();
  this->object_id = ((*input_json_ptr)["object_id"]).asString();

#ifdef _ECS36B_DEBUG_
  std::cout << (this->dumpJ())->toStyledString() << std::endl;
#endif /* _ECS36B_DEBUG_ */

  return true; // if successful
}
