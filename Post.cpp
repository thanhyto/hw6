#include "Post.h"

Post::Post
(void)
{
  this->author = NULL;
  this->msg = NULL;

  // very critical ==> portable to different platforms
  this->receivers = NULL;
  this->links = NULL;
  this->actions = NULL;
  this->reactions = NULL;
  this->comments = NULL;
  this->created = NULL;
  this->updated = NULL;
  this->profile_id = "";
  this->post_id = "";
  this->is_published = false;
}

Post::Post
(Person * arg_author, Message * arg_msg)
{
  this->author = arg_author;
  this->msg = arg_msg;

  // very critical ==> portable to different platforms
  this->receivers = NULL;
  this->links = NULL;
  this->actions = NULL;
  this->reactions = NULL;
  this->comments = NULL;
  this->created = NULL;
  this->updated = NULL;
  this->profile_id = "";
  this->post_id = "";
  this->is_published = false;
}

Json::Value * Post::dumpJ
(void)
{
  int i;

  Json::Value * result_ptr = new Json::Value();
  Json::Value * dumpjv_ptr;

  // "actions"
  if ((this->actions != NULL) &&
      ((this->actions)->size() > 0))
    {
      Json::Value json_actions;
      Json::Value json_actions_array;

      for (i = 0; i < (this->actions)->size(); i++)
	{
	  dumpjv_ptr = ((*(this->actions))[i])->dumpJ();
	  json_actions[i] = (*dumpjv_ptr);
	  delete dumpjv_ptr;
	}
      json_actions_array["data"] = json_actions;
      json_actions_array["count"] = ((int) (*(this->actions)).size());
      (*result_ptr)["actions"] = json_actions_array;
    }

  // "comments"
  if ((this->comments != NULL) &&
      ((this->comments)->size() > 0))
    {
      Json::Value json_comments;
      Json::Value json_comments_array;
      for (i = 0; i < (this->comments)->size(); i++)
	{
	  dumpjv_ptr = ((*(this->comments))[i])->dumpJ();
	  if (dumpjv_ptr != NULL)
	    json_comments[i] = (*dumpjv_ptr);
	  else
	    {
	      std::cout << "dumpjv pointer NULL error" << std::endl;
	      fflush(stdout);
	    }	    
	  delete dumpjv_ptr;
	}
      json_comments_array["data"] = json_comments;
      json_comments_array["count"] = ((int) (*(this->comments)).size());
      (*result_ptr)["comments"] = json_comments_array;
    }

  // "links"
  if ((this->links != NULL) &&
      ((this->links)->size() > 0))
    {
      Json::Value json_links;
      Json::Value json_links_array;
      for (i = 0; i < (this->links)->size(); i++)
	{
	  dumpjv_ptr = ((*(this->links))[i])->dumpJ();
	  json_links[i] = (*dumpjv_ptr);
	  delete dumpjv_ptr;
	}
      json_links_array["data"] = json_links;
      json_links_array["count"] = ((int) (*(this->links)).size());
      (*result_ptr)["links"] = json_links_array;
    }

  // "message"
  if (this->msg != NULL)
    {
      (*result_ptr)["message"] = (this->msg)->content;
    }

  // "reactions"
  // **************************************************************************************
  // hint (for ecs36b_f2020): this part is similar to the reactions handling in Comment.cpp
  // **************************************************************************************
  if ((this->reactions != NULL) &&
      ((this->reactions)->size() > 0))
    {
      Json::Value json_reactions;
      Json::Value json_reactions_array;
      for (i = 0; i < (this->reactions)->size(); i++)
	{
	  dumpjv_ptr = ((*(this->reactions))[i])->dumpJ();
	  json_reactions[i] = (*dumpjv_ptr);
	  delete dumpjv_ptr;
	}
      json_reactions_array["data"] = json_reactions;
      json_reactions_array["count"] = ((int) (*(this->reactions)).size());
      (*result_ptr)["reactions"] = json_reactions_array;
    }

  // "id"
  (*result_ptr)["id"] = (this->profile_id + "_" + this->post_id);

  // "from"
  if (this->author != NULL)
    {
      dumpjv_ptr = (this->author)->dumpJ();
      (*result_ptr)["from"] = (*dumpjv_ptr);
      delete dumpjv_ptr;
    }

  // "to"
  if ((this->receivers != NULL) &&
      ((this->receivers)->size() > 0))
    {
      Json::Value json_tos;
      Json::Value json_tos_array;
      for (i = 0; i < (this->receivers)->size(); i++)
	{
	  dumpjv_ptr = ((*(this->receivers))[i])->dumpJ();
	  json_tos[i] = (*dumpjv_ptr);
	  delete dumpjv_ptr;
	}
      json_tos_array["data"] = json_tos;
      json_tos_array["count"] = ((int) (*(this->receivers)).size());
      (*result_ptr)["to"] = json_tos_array;
    }
  
  // "created_time"
  if (this->created != NULL)
    {
      std::string * str_ptr =  (this->created)->getTimeString();
      (*result_ptr)["created_time"] = *str_ptr;
      delete str_ptr;
    }

  // "updated_time"
  if (this->updated != NULL)
    {
      std::string * str_ptr =  (this->updated)->getTimeString();
      (*result_ptr)["updated_time"] = *str_ptr;
      delete str_ptr;
    }

  // "is_published"
  (*result_ptr)["is_published"] = this->is_published;

#ifdef _ECS36B_DEBUG_
  std::cout << result_ptr->toStyledString() << std::endl;
#endif /* _ECS36B_DEBUG_ */
  return result_ptr;
}

int
checkPostID
(std::string id_string)
{
  // check if an id string is all numerical
  
  char idstr[1024 * 128];
  bzero(idstr, 1024 * 128);
  sprintf(idstr, "%s", id_string.c_str());
  if (idstr[0] == '\0') return -1;

  char c_prof_buf[256 * 128];
  char c_post_buf[256 * 128];
  bzero(c_prof_buf, 256 * 128);
  bzero(c_post_buf, 256 * 128);
  sscanf(idstr, "%[^_]_%s", c_prof_buf, c_post_buf);

  int i;
  if ((strlen(c_prof_buf) == 0) ||
      (strlen(c_post_buf) == 0) ||
      (strlen(c_prof_buf) > 64) ||
      (strlen(c_post_buf) > 64))
    return -1;
  
  for(i = 0; i < strlen(c_prof_buf); i++)
    {
      if ((c_prof_buf[i] < '0') || (c_prof_buf[i] > '9'))
	return -1;
    }

  for(i = 0; i < strlen(c_post_buf); i++)
    {
      if ((c_post_buf[i] < '0') || (c_post_buf[i] > '9'))
	return -1;
    }
  return 0;
}

bool 
Post::Jdump
(Json::Value *input_json_ptr)
{
  if ((input_json_ptr == NULL) ||
      ((*input_json_ptr).isNull() == true) ||
      ((*input_json_ptr).isObject() != true))
    {
      return false;
    }

  if ((*input_json_ptr).isArray() == true)
    {
      // wrong one
      // HW5_ERROR_JSON_PARSING;
      return false;
    }
  
  // "id"
  if ((((*input_json_ptr)["id"]).isNull() != true) &&
      (((*input_json_ptr)["id"]).isString() == true) &&
      ((((*input_json_ptr)["id"]).asString()).size() > 0) &&
      ((((*input_json_ptr)["id"]).asString()).size() < 256))
    {
      if (checkPostID(((*input_json_ptr)["id"]).asString()) != 0)
	{
	  // HW5_ERROR_JSON_POST_NO_ID;
	  return false;
	}

      char idstr[1024];
      bzero(idstr, 1024);
      sprintf(idstr, "%s", (((*input_json_ptr)["id"]).asString()).c_str());
      if (idstr[0] != '\0')
	{
	  char c_prof_buf[256];
	  char c_post_buf[256];
	  bzero(c_prof_buf, 256);
	  bzero(c_post_buf, 256);
	  sscanf(idstr, "%[^_]_%s", c_prof_buf, c_post_buf);

	  std::string profile_id = { c_prof_buf };
	  std::string post_id = { c_post_buf };
	  
	  this->profile_id = profile_id;
	  this->post_id = post_id;
	}
      else
	{
	  std::cerr << "No Post ID presented" << std::endl;
	  // HW5_ERROR_JSON_POST_NO_ID;
	  return false;
	}
    }
  else
    {
      std::cerr << "No Post ID presented" << std::endl;
      // HW5_ERROR_JSON_POST_NO_ID;
      return false;
    }

  int i, j;

  // "actions"
  if ((((*input_json_ptr)["actions"]).isNull() != true) &&
      (((*input_json_ptr)["actions"]).isObject() == true) &&
      (((*input_json_ptr)["actions"]["data"]).isNull() != true) &&
      (((*input_json_ptr)["actions"]["data"]).isArray() == true))
    {
      if (this->actions == NULL)
	this->actions = new vector<Action *>();

      for (i = 0; i < ((*input_json_ptr)["actions"]["data"]).size(); i++)
	{
	  Json::Value l_jv = (*input_json_ptr)["actions"]["data"][i];
	  Action *l_act_ptr = new Action();
	  bool rc = l_act_ptr->Jdump(&(l_jv));
	  if (rc == true)
	    {
	      // check if this is a new Action or not
	      vector<Action *>::iterator my_it_ac;
	      int flag_ac = 0;
      	      for (my_it_ac = (*(this->actions)).begin();
		   my_it_ac < (*(this->actions)).end(); my_it_ac++)
		{
		  if (((*my_it_ac)->host_url == l_act_ptr->host_url) &&
		      ((*my_it_ac)->owner_vsID == l_act_ptr->owner_vsID) &&
		      ((*my_it_ac)->class_id == l_act_ptr->class_id) &&
		      ((*my_it_ac)->object_id == l_act_ptr->object_id) &&
		      ((*my_it_ac)->name == l_act_ptr->name))
		    {
		      flag_ac = 1;
		    }
		}
 
	      if (flag_ac == 0)
		{
		  // new one
		  (*(this->actions)).push_back(l_act_ptr);
		}
	      else
		{
		  // already exist
		  delete l_act_ptr;
		}
	    }
	}
    }

  // "comments"
  if ((((*input_json_ptr)["comments"]).isNull() != true) &&
      (((*input_json_ptr)["comments"]).isObject() == true) &&
      (((*input_json_ptr)["comments"]["data"]).isNull() != true) &&
      (((*input_json_ptr)["comments"]["data"]).isArray() == true))
    {
      if (this->comments == NULL)
	this->comments = new vector<Comment *>();

      for (i = 0; i < ((*input_json_ptr)["comments"]["data"]).size(); i++)
	{
	  Json::Value l_jv = (*input_json_ptr)["comments"]["data"][i];
	  Comment *l_cmt_ptr = new Comment();
	  bool rc = l_cmt_ptr->Jdump(&(l_jv));
	  if (rc == true)
	    {
	      // check if this is a new Comment or not
	      vector<Comment *>::iterator my_it_cm;
	      int flag_cm = 0;
      	      for (my_it_cm = (*(this->comments)).begin();
		   my_it_cm < (*(this->comments)).end(); my_it_cm++)
		{
		  if (((*my_it_cm)->profile_id == l_cmt_ptr->profile_id) &&
		      ((*my_it_cm)->post_id == l_cmt_ptr->post_id) &&
		      ((*my_it_cm)->comment_id == l_cmt_ptr->comment_id))
		    {
		      flag_cm = 1;
		    }
		}
 
	      if (flag_cm == 0)
		{
		  // new one
		  (*(this->comments)).push_back(l_cmt_ptr);
		}
	      else
		{
		  // already exist
		  delete l_cmt_ptr;
		}
	    }
	}
    }

  // "links"
  if ((((*input_json_ptr)["links"]).isNull() != true) &&
      (((*input_json_ptr)["links"]).isObject() == true) &&
      (((*input_json_ptr)["links"]["data"]).isNull() != true) &&
      (((*input_json_ptr)["links"]["data"]).isArray() == true))
    {
      if (this->links == NULL)
	this->links = new vector<Link *>();

      for (i = 0; i < ((*input_json_ptr)["links"]["data"]).size(); i++)
	{
	  Json::Value l_jv = (*input_json_ptr)["links"]["data"][i];
	  Link *l_lnk_ptr = new Link();
	  bool rc = l_lnk_ptr->Jdump(&(l_jv));
	  if (rc == true)
	    {
	      vector<Link *>::iterator my_it_lk;
	      int flag_lk = 0;
      	      for (my_it_lk = (*(this->links)).begin();
		   my_it_lk < (*(this->links)).end(); my_it_lk++)
		{
		  if (((*my_it_lk)->url == l_lnk_ptr->url) &&
		      ((*my_it_lk)->class_id == l_lnk_ptr->class_id) &&
		      ((*my_it_lk)->object_id == l_lnk_ptr->object_id))
		    {
		      flag_lk = 1;
		    }
		}
 
	      if (flag_lk == 0)
		{
		  // new one
		  (*(this->links)).push_back(l_lnk_ptr);
		}
	      else
		{
		  delete l_lnk_ptr;
		}
	    }
	}
    }

  // "message"
  if ((((*input_json_ptr)["message"]).isNull() != true) &&
      (((*input_json_ptr)["message"]).isString() == true) &&
      ((((*input_json_ptr)["message"]).asString()).size() > 0) &&
      ((((*input_json_ptr)["message"]).asString()).size() < 512))
    {
      Message *l_msg_ptr = new Message((*input_json_ptr)["message"].asString());
      if (this->msg == NULL)
	this->msg = l_msg_ptr;
      else
	delete l_msg_ptr;
    }

  // "reactions"
  // **************************************************************************************
  // hint (for ecs36b_f2020): this part is similar to the reactions handling in Comment.cpp
  // **************************************************************************************
  if ((((*input_json_ptr)["reactions"]).isNull() != true) &&
      (((*input_json_ptr)["reactions"]).isObject() == true) &&
      (((*input_json_ptr)["reactions"]["data"]).isNull() != true) &&
      (((*input_json_ptr)["reactions"]["data"]).isArray() == true))
    {
      if (this->reactions == NULL)
	this->reactions = new vector<Reaction *>();

      for (i = 0; i < ((*input_json_ptr)["reactions"]["data"]).size(); i++)
	{
	  Json::Value l_jv = (*input_json_ptr)["reactions"]["data"][i];
	  Reaction *l_rct_ptr = new Reaction();
	  bool rc = l_rct_ptr->Jdump(&(l_jv));
	  if (rc == true)
	    {
	      // check if the reaction already exist
	      vector<Reaction *>::iterator my_it_rt;
	      int flag_rt = 0;
	      for (my_it_rt = (*(this->reactions)).begin();
		   my_it_rt < (*(this->reactions)).end(); my_it_rt++)
		{
		  if ((*((*my_it_rt)->author)) == (*(l_rct_ptr->author)))
		    {
		      // we assume one person can only react once
		      flag_rt = 1;
		    }
		}

	      if (flag_rt == 0)
		{
		  (*(this->reactions)).push_back(l_rct_ptr);
		}
	      else
		{
		  // existing reaction
		  delete l_rct_ptr;
		}
	    }
	}
    }

  // "from"
  if ((((*input_json_ptr)["from"]).isNull() != true) &&
      (((*input_json_ptr)["from"]).isObject() == true))
    {
      if (this->author == NULL)
	{
	  // we assume that you can NOT change the author of a Post
	  this->author = new Person();
	  bool rc = (this->author)->Jdump(&((*input_json_ptr)["from"]));
	  if (rc == false)
	    {
	      delete this->author;
	      this->author = NULL;
	      return false;
	    }
	}
    }

  // "to"
  if ((((*input_json_ptr)["to"]).isNull() != true) &&
      (((*input_json_ptr)["to"]).isObject() == true) &&
      (((*input_json_ptr)["to"]["data"]).isNull() != true) &&
      (((*input_json_ptr)["to"]["data"]).isArray() == true))
    {
      if (this->receivers == NULL)
	this->receivers = new vector<Person *>();

      for (i = 0; i < ((*input_json_ptr)["to"]["data"]).size(); i++)
	{
	  Json::Value l_jv = (*input_json_ptr)["to"]["data"][i];
	  Person *l_psn_ptr = new Person();
	  bool rc = l_psn_ptr->Jdump(&(l_jv));
	  if (rc == true)
	    {
	      vector<Person *>::iterator my_it_to;
	      int flag_to = 0;
      	      for (my_it_to = (*(this->receivers)).begin();
		   my_it_to < (*(this->receivers)).end(); my_it_to++)
		{
		  if ((*(*my_it_to)) == (*l_psn_ptr))
		    {
		      flag_to = 1;
		    }
		}
 
	      if (flag_to == 0)
		{
		  // new one
		  (*(this->receivers)).push_back(l_psn_ptr);
		}
	      else
		{
		  delete l_psn_ptr;
		}
	    }
	}
    }

  // "created_time"
  if ((((*input_json_ptr)["created_time"]).isNull() != true) &&
      (((*input_json_ptr)["created_time"]).isString() == true))
    {
      if (this->created == NULL)
	{
	  this->created = new JvTime((((*input_json_ptr)["created_time"]).asString()).c_str());
	}
    }

  // "updated_time"
  if ((((*input_json_ptr)["updated_time"]).isNull() != true) &&
      (((*input_json_ptr)["updated_time"]).isString() == true))
    {
      this->updated = new JvTime((((*input_json_ptr)["updated_time"]).asString()).c_str());
    }

  // "is_published"
  this->is_published = ((*input_json_ptr)["is_published"]).asBool();

#ifdef _ECS36B_DEBUG_
  std::cout << (this->dumpJ())->toStyledString() << std::endl;
#endif /* _ECS36B_DEBUG_ */

  return true; // if successful
}
