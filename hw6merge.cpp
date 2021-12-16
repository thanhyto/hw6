
// hw6merge.cpp
// #define _ECS36B_DEBUG_

#include "Post.h"

int
main(int argc, char *argv[])
{
  if (argc != 3) return -1;

  Json::Value * dumpjv_ptr;

  FILE *jf1 = fopen(argv[1], "r");
  if (jf1 == NULL) return -1;

  FILE *jf2 = fopen(argv[2], "r");
  if (jf2 == NULL) return -1;

  int i, j;
  Json::CharReaderBuilder builder;
  Json::CharReader* reader;
  std::string errors;
  bool parsingSuccessful;

  // *** First JSON File ***
  long lSize1;
  size_t lresult1;

  // obtain file size:
  fseek(jf1, 0 , SEEK_END);
  lSize1 = ftell(jf1);
  rewind(jf1);

  // allocate memory to contain the whole file:
  char *jf1_ptr = (char *) malloc(sizeof(char)*lSize1);
  // copy the file into the buffer:
  lresult1 = fread(jf1_ptr, 1, lSize1, jf1);    
  if (lresult1 != lSize1)
    {
      fputs("Reading error", stderr);
      exit (-3);
    }

  Json::Value myv_message;
  reader = builder.newCharReader();
  parsingSuccessful = reader->parse(jf1_ptr, jf1_ptr + lSize1, &myv_message, &errors);
  delete reader;

  if (!parsingSuccessful) {
    std::cout << "Failed to parse the content of the first JSON, errors:" << std::endl;
    std::cout << errors << std::endl;
  }
  fclose(jf1);

  Post my_post;
  bool result_flag = my_post.Jdump(&myv_message);

  if (result_flag == true)
    {
      // now let us print the JSON from C++ objects we just built.
      std::cout << (*(my_post.dumpJ())).toStyledString() << std::endl;
    }

  // first JSON done, Let us get the second one
  // *** Second JSON File ***
  long lSize2;
  size_t lresult2;

  // obtain file size:
  fseek(jf2, 0 , SEEK_END);
  lSize2 = ftell(jf2);
  rewind(jf2);

  // allocate memory to contain the whole file:
  char *jf2_ptr = (char *) malloc(sizeof(char)*lSize2);
  // copy the file into the buffer:
  lresult2 = fread(jf2_ptr, 1, lSize2, jf2);    
  if (lresult2 != lSize2)
    {
      fputs("Reading JF2 error", stderr);
      exit (-3);
    }

  // Okay let us start Parsing the Second JSON File
  reader = builder.newCharReader();
  parsingSuccessful = reader->parse(jf2_ptr, jf2_ptr + lSize2, &myv_message, &errors);
  delete reader;

  if (!parsingSuccessful) {
    std::cout << "Failed to parse the content of the second JSON, errors:" << std::endl;
    std::cout << errors << std::endl;
  }
  fclose(jf2);

  // "id" (Let us check first if they share the same ID)
  char idstr[1024];
  bzero(idstr, 1024);
  sprintf(idstr, "%s", ((myv_message["id"]).asString()).c_str());
  if (idstr[0] != '\0')
    {
      char c_prof_buf[256];
      char c_post_buf[256];
      bzero(c_prof_buf, 256);
      bzero(c_post_buf, 256);
      sscanf(idstr, "%[^_]_%s", c_prof_buf, c_post_buf);
      std::string profile_id = { c_prof_buf };
      std::string post_id = { c_post_buf };
      if ((my_post.profile_id != profile_id) ||
	  (my_post.post_id    != post_id))
	{	  
	  std::cerr << "Post IDs mismatched!!" << std::endl;
	  return -2;
	}
    }
  else
    {
      std::cerr << "No Post ID presented for the second JSON" << std::endl;
      return -1;
    }

  // now, they matched, let us proceed with the merge action
  // what should we do here? (to be implemented by ecs36b_f2020 students)

  result_flag = my_post.Jdump(&myv_message);
  if (result_flag == true)
    {
      // now let us print the JSON from C++ objects we just built.
      std::cout << (*(my_post.dumpJ())).toStyledString() << std::endl;
    }

  // final printing
  // std::cout << (*(my_post.dumpJ())).toStyledString() << std::endl;

  return 0;
}
