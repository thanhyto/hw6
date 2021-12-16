
#ifndef _POST_H_
#define _POST_H_

// Post.h
#include "Core.h"
#include "Tag.h"
#include "Comment.h"
#include "Reaction.h"
#include "Message.h"
#include "Person.h"
#include "Link.h"
#include "Action.h"
#include "JvTime.h"

class Post
{
 private:
 protected:
 public:
  std::string                  profile_id;
  std::string                  post_id;

  Person                     * author;
  std::vector<Person      *> * receivers;

  Message                    * msg;

  std::vector<Link        *> * links;
  std::vector<Action      *> * actions;
  std::vector<Reaction    *> * reactions;
  std::vector<Comment     *> * comments;

  JvTime                     * created;
  JvTime                     * updated;
  bool is_published;

  Post(void);
  Post(Person* arg_author, Message* arg_msg);
  virtual Json::Value * dumpJ();
  virtual bool Jdump(Json::Value *);
};

// error code
#define HW5_ERROR_NORMAL                0
#define HW5_ERROR_VSID_HISTORY         -1
#define HW5_ERROR_VSID_POST_ID         -2
#define HW5_ERROR_JSON_POST_MERGING    -3
#define HW5_ERROR_LRESULT_LSIZE        -4
#define HW5_ERROR_JSON_PARSING         -5
#define HW5_ERROR_JSON_2_POST          -6
#define HW5_ERROR_JSON_POST_NO_ID      -7
#define HW5_ERROR_JSON_PTR_NULL        -8
#define HW5_ERROR_FILE_NOT_EXIST       -9
#define HW5_ERROR_FILE_READ            -10
#define HW5_ERROR_FILE_NAME_PTR_NULL   -11
#define HW5_ERROR_FILE_WRITE           -12
#define HW5_ERROR_LESS_THAN_20         -13
#define HW5_ERROR_NO_SEARCH_KEYWORD    -14
#define HW5_ERROR_NO_OKEY              -15
#define HW5_ERROR_NO_OKEY_STRANGE      -16
#define HW5_ERROR_SMACK_STACK_DETECTED -17
#define HW5_ERROR_BAD_KEYWORD_FORMAT   -18
#define HW5_ERROR_SET_STD_TM           -19
#define HW5_ERROR_NULL_POST_POINTER    -20
#define HW5_ERROR_POST_ID_MISMATCHED   -21
#define HW5_ERROR_SUSPICIOUS           -22

#endif /* _POST_H_ */
