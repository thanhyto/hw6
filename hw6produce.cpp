
// #define _ECS36B_DEBUG_

#include "Core.h"
#include "Link.h"
#include "Post.h"
#include "JvTime.h"

int
main(int argc, char *argv[])
{
  if (argc != 2) return -1;

  Json::Value * dumpjv_ptr;

  FILE *jf = fopen(argv[1], "r");
  if (jf == NULL) return -1;

  int i, j;
  long lSize;
  size_t lresult;

  // obtain file size:
  fseek(jf, 0 , SEEK_END);
  lSize = ftell(jf);
  rewind(jf);

  // allocate memory to contain the whole file:
  char *jf_ptr = (char *) malloc(sizeof(char)*lSize);
  // copy the file into the buffer:
  lresult = fread(jf_ptr, 1, lSize, jf);    
  if (lresult != lSize)
    {
      fputs("Reading error", stderr);
      exit (-3);
    }

  Json::Value myv_message;
  Json::CharReaderBuilder builder;
  Json::CharReader* reader = builder.newCharReader();
  std::string errors;
  bool parsingSuccessful = reader->parse
    (jf_ptr, jf_ptr + lSize, &myv_message, &errors);
  delete reader;

  if (!parsingSuccessful) {
    std::cout << "Failed to parse the content JSON, errors:" << std::endl;
    std::cout << errors << std::endl;
  }

  // new
  Post my_post { (Person *) NULL, (Message *) NULL };
  // bzero(&my_post, sizeof(my_post)); // ALERT ==> what is the problem here?

  bool result_flag = my_post.Jdump(&myv_message);

  if (result_flag == true)
    {
      // now let us print the JSON from C++ objects we just built.
      std::cout << (*(my_post.dumpJ())).toStyledString() << std::endl;
    }

  return 0;
}
