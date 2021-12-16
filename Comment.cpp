
#include "Comment.h"

Comment::Comment
(void)
{
  this->profile_id = "";
  this->post_id = "";
  this->comment_id = "";
  this->author = (Person *) NULL;
  this->msg = (Message *) NULL;
  this->created = (JvTime *) NULL;

  // very critical ==> portable to different platforms
  this->reactions = NULL;
  this->tags = NULL;
}

Comment::Comment
(std::string arg_profile_id, std::string arg_post_id, std::string arg_comment_id,
 Person * arg_author, Message * arg_msg, JvTime * arg_created)
{
  this->profile_id = arg_profile_id;
  this->post_id = arg_post_id;
  this->comment_id = arg_comment_id;
  this->author = arg_author;
  this->msg = arg_msg;
  this->created = arg_created;

  // very critical ==> portable to different platforms
  this->reactions = NULL;
  this->tags = NULL;
}

Json::Value * Comment::dumpJ
(void)
{
  int i;

  if ((this->profile_id == "") ||
      (this->post_id == "") ||
      (this->comment_id == "") ||
      (this->author == NULL) ||
      (this->msg == NULL) ||
      (this->created == NULL))
    return NULL;

  Json::Value * result_ptr = new Json::Value();
  std::string l_id = this->profile_id + "_" + this->post_id + "_" + this->comment_id;

  (*result_ptr)["id"] = l_id;
  (*result_ptr)["from"] = (*(this->author)->dumpJ());
  (*result_ptr)["message"] = (this->msg)->content;

  std::string * str_ptr =  (this->created)->getTimeString();
  (*result_ptr)["created_time"] = *str_ptr;
  delete str_ptr;

  // adding the reaction part
  Json::Value json_c_reactions;
  Json::Value json_c_reactions_array;

  if ((this->reactions != NULL) &&
      ((this->reactions)->size() > 0))
    {
      for (i = 0; i < (this->reactions)->size(); i++)
	{
	  Json::Value * dumpjv_ptr = ((*(this->reactions))[i])->dumpJ();
	  json_c_reactions[i] = (*dumpjv_ptr);
	  delete dumpjv_ptr;
	}
      json_c_reactions_array["data"] = json_c_reactions;
      json_c_reactions_array["count"] = ((int) (*(this->reactions)).size());
      (*result_ptr)["reactions"] = json_c_reactions_array;
    }

  // adding the tag part
  Json::Value json_c_tags;
  Json::Value json_c_tags_array;

  if ((this->tags != NULL) &&
      ((this->tags)->size() > 0))
    {
      for (i = 0; i < (this->tags)->size(); i++)
	{
	  Json::Value * dumpjv_ptr = ((*(this->tags))[i])->dumpJ();
	  json_c_tags[i] = (*dumpjv_ptr);
	  delete dumpjv_ptr;
	}
      json_c_tags_array["data"] = json_c_tags;
      json_c_tags_array["count"] = ((int) (*(this->tags)).size());
      (*result_ptr)["tags"] = json_c_tags_array;
    }

#ifdef _ECS36B_DEBUG_
  std::cout << result_ptr->toStyledString() << std::endl;
#endif /* _ECS36B_DEBUG_ */

  return result_ptr;
}

bool 
Comment::Jdump
(Json::Value *input_json_ptr)
{
  if ((input_json_ptr == NULL) ||
      ((*input_json_ptr).isNull() == true) ||
      ((*input_json_ptr).isObject() != true))
    {
      return false;
    }

  if ((((*input_json_ptr)["id"]).isNull() == true) ||
      (((*input_json_ptr)["from"]).isNull() == true) ||
      (((*input_json_ptr)["message"]).isNull() == true) ||
      (((*input_json_ptr)["created_time"]).isNull() == true) ||
      (((*input_json_ptr)["id"]).isString() != true) ||
      (((*input_json_ptr)["from"]).isObject() != true) ||
      (((*input_json_ptr)["message"]).isString() != true) ||
      (((*input_json_ptr)["created_time"]).isString() != true))
    {
      return false;
    }

  if ((this->profile_id == "") &&
      (this->post_id == "") &&
      (this->comment_id == ""))
    {
      char buf_profile[1024];
      bzero(buf_profile, 1024);
      char buf_post[1024];
      bzero(buf_post, 1024);
      char buf_comment[1024];
      bzero(buf_comment, 1024);

      sscanf(((*input_json_ptr)["id"].asString()).c_str(), "%[^_]_%[^_]_%s",
	     buf_profile, buf_post, buf_comment);
      this->profile_id = buf_profile;
      this->post_id = buf_post;
      this->comment_id = buf_comment;
    }

  if (this->author == NULL)
    {
      // ************************************************
      // to be completed by ecs36b_f2021 students for hw6
      // ************************************************





    }

  if (this->created == NULL)
    {
      // ************************************************
      // to be completed by ecs36b_f2021 students for hw6
      // ************************************************






    }

  if (this->msg == NULL)
    {
      this->msg = new Message(((*input_json_ptr)["message"]).asString());
    }

  // now handling the reaction and tag parts
  // (1) reactions
  if ((((*input_json_ptr)["reactions"]).isNull() != true) &&
      (((*input_json_ptr)["reactions"]).isObject() == true) &&
      (((*input_json_ptr)["reactions"]["data"]).isNull() != true) &&
      (((*input_json_ptr)["reactions"]["data"]).isArray() == true))
    {
      int j;
      for (j = 0; j < ((*input_json_ptr)["reactions"]["data"]).size(); j++)
	{
	  // ************************************************
	  // to be completed by ecs36b_f2021 students for hw6
	  // ************************************************




	}
    }

  // (2) tags
  if ((((*input_json_ptr)["tags"]).isNull() != true) &&
      (((*input_json_ptr)["tags"]).isObject() == true) &&
      (((*input_json_ptr)["tags"]["data"]).isNull() != true) &&
      (((*input_json_ptr)["tags"]["data"]).isArray() == true))
    {
      int j;
      for (j = 0; j < ((*input_json_ptr)["tags"]["data"]).size(); j++)
	{
	  Json::Value l_jv_tag = (*input_json_ptr)["tags"]["data"][j];
	  Tag *l_tag_ptr = new Tag();
	  bool rc = l_tag_ptr->Jdump(&(l_jv_tag));
	  if (rc == true)
	    {
	      // now check if the tag already exist
	      int flag_tg = 0;
	      if (this->tags != NULL)
		{
		  vector<Tag *>::iterator my_it_tg;
		  for (my_it_tg = (*(this->tags)).begin();
		       my_it_tg < (*(this->tags)).end(); my_it_tg++)
		    {
		      // within a given comment, each tag index value must be different
		      if (((*my_it_tg)->index) == (l_tag_ptr->index))
			{
			  // this is a duplicated tag as the index is the same
			  flag_tg = 1;
			}
		    }
		}
	      else
		{
		  // create this->tags
		  this->tags = new vector<Tag *>();
		}

	      if (flag_tg == 0)
		{
		  (*(this->tags)).push_back(l_tag_ptr);
		}
	      else
		{
		  delete l_tag_ptr;
		}
	    }
	}
    }

#ifdef _ECS36B_DEBUG_
  std::cout << (this->dumpJ())->toStyledString() << std::endl;
#endif /* _ECS36B_DEBUG_ */

  return true; // if successful
}
