
#ifndef _MESSAGE_H_
#define _MESSAGE_H_

// Message.h

class Message
{
 private:
 protected:
 public:
  std::string content;
  Message(std::string arg_s) { this->content = arg_s; }
  void setContent(std::string arg_s) { this->content = arg_s; }
  std::string getContent(void) { return this->content; }
};

#endif /* _MESSAGE_H_ */
